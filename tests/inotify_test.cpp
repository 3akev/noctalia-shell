#include "core/inotify/inotify.h"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <unistd.h>

namespace {

  bool expect(bool condition, const char* message) {
    if (!condition) {
      std::println(stderr, "inotify_test: FAIL: {}", message);
    }
    return condition;
  }

  std::filesystem::path uniqueTempDir() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = std::filesystem::temp_directory_path()
        / ("noctalia-inotify-test-" + std::to_string(::getpid()) + "-" + std::to_string(now));
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    return dir;
  }

  void cleanup(const std::filesystem::path& path) {
    std::error_code ec;
    std::filesystem::remove_all(path, ec);
  }

  bool watchOnValidDirectoryReturnsId() {
    Inotify in;
    const auto dir = uniqueTempDir();
    auto wd = in.watch(dir, IN_CREATE);
    const bool ok = expect(wd.has_value(), "watch() on a valid directory should return a WatchId");
    cleanup(dir);
    return ok;
  }

  bool watchOnMissingPathReturnsNullopt() {
    Inotify in;
    const auto dir = uniqueTempDir();
    const auto missing = dir / "does-not-exist";
    auto wd = in.watch(missing, IN_CREATE);
    const bool ok = expect(!wd.has_value(), "watch() on a missing path should return std::nullopt");
    cleanup(dir);
    return ok;
  }

  bool watchOnEmptyPathReturnsNullopt() {
    Inotify in;
    auto wd = in.watch(std::filesystem::path{}, IN_CREATE);
    return expect(!wd.has_value(), "watch() on an empty path should return std::nullopt");
  }

  bool drainInvokesCallbackOnCreateInDirectory() {
    Inotify in;
    const auto dir = uniqueTempDir();

    bool fired = false;
    uint32_t mask = 0;
    std::string name;
    in.watch(dir, IN_CREATE, [&](const inotify_event* e) {
      fired = true;
      mask = e->mask;
      if (e->len > 0) {
        name = std::string(e->name);
      }
    });

    const auto file = dir / "child.txt";
    std::ofstream{file} << "hello";

    in.drain();

    const bool ok = expect(fired, "callback should fire when a file is created in a watched directory")
        && expect((mask & IN_CREATE) != 0, "event mask should contain IN_CREATE")
        && expect(name == "child.txt", "event name should carry the created file name");
    cleanup(dir);
    return ok;
  }

  bool drainInvokesCallbackOnFileModify() {
    Inotify in;
    const auto dir = uniqueTempDir();
    const auto file = dir / "monitored.txt";
    std::ofstream{file} << "initial";

    bool fired = false;
    uint32_t mask = 0;
    in.watch(file, IN_MODIFY, [&](const inotify_event* e) {
      fired = true;
      mask = e->mask;
    });

    std::ofstream{file} << "changed";

    in.drain();

    const bool ok = expect(fired, "callback should fire when a watched file is modified")
        && expect((mask & IN_MODIFY) != 0, "event mask should contain IN_MODIFY");
    cleanup(dir);
    return ok;
  }

  bool drainDeliversIndependentCallbacksForMultipleWatches() {
    Inotify in;
    const auto dir = uniqueTempDir();
    const auto fileA = dir / "a.txt";
    const auto fileB = dir / "b.txt";

    int hitsA = 0;
    int hitsB = 0;
    std::ofstream{fileA} << "seed";
    std::ofstream{fileB} << "seed";
    auto wdA = in.watch(fileA, IN_MODIFY, [&](const inotify_event*) { ++hitsA; });
    auto wdB = in.watch(fileB, IN_MODIFY, [&](const inotify_event*) { ++hitsB; });
    if (!expect(wdA.has_value(), "watch A should return a WatchId")
        || !expect(wdB.has_value(), "watch B should return a WatchId")) {
      cleanup(dir);
      return false;
    }
    if (*wdA == *wdB) {
      cleanup(dir);
      return expect(false, "distinct watches should receive distinct WatchIds");
    }

    std::ofstream{fileA} << "1";
    in.drain();

    // After modifying only file A, only callback A should have fired.
    const bool ok = expect(hitsA == 1, "callback A should fire exactly once for its own file")
        && expect(hitsB == 0, "callback B should not fire for an event on file A");
    if (!ok) {
      cleanup(dir);
      return false;
    }

    std::ofstream{fileB} << "2";
    in.drain();

    const bool ok2 = expect(hitsA == 1, "callback A should remain at one hit after file B changes")
        && expect(hitsB == 1, "callback B should fire exactly once after its own file changes");
    cleanup(dir);
    return ok2;
  }

  bool unwatchStopsCallbacks() {
    Inotify in;
    const auto dir = uniqueTempDir();

    int callCount = 0;
    auto wd = in.watch(dir, IN_CREATE, [&](const inotify_event*) { ++callCount; });
    if (!expect(wd.has_value(), "watch should return a WatchId before unwatch")) {
      cleanup(dir);
      return false;
    }

    in.unwatch(*wd);

    std::ofstream{dir / "after.txt"} << "x";
    in.drain();

    const bool ok = expect(callCount == 0, "no callback should fire after unwatch");
    cleanup(dir);
    return ok;
  }

  bool unwatchUnknownIdIsHarmless() {
    Inotify in;
    const auto dir = uniqueTempDir();
    in.watch(dir, IN_CREATE, [](const inotify_event*) {});
    in.unwatch(-1);
    cleanup(dir);
    return true;
  }

  bool drainHandlesIgnoredEventWithoutLosingState() {
    Inotify in;
    const auto dir = uniqueTempDir();
    const auto file = dir / "watched.txt";
    std::ofstream{file} << "x";

    // drain()'s IN_IGNORED branch erases the watch entry without invoking the
    // callback, so a deleted watched file must NOT deliver any callback.
    int callCount = 0;
    in.watch(file, IN_MODIFY | IN_IGNORED, [&](const inotify_event*) { ++callCount; });

    std::error_code ec;
    std::filesystem::remove(file, ec);
    in.drain();

    if (!expect(callCount == 0, "deleting a watched file should not invoke its callback (IN_IGNORED branch)")) {
      cleanup(dir);
      return false;
    }

    // Object must remain usable after the auto-removed watch.
    const auto newFile = dir / "watched2.txt";
    std::ofstream{newFile} << "y";
    bool fired = false;
    in.watch(newFile, IN_MODIFY, [&](const inotify_event*) { fired = true; });
    std::ofstream{newFile} << "z";
    in.drain();

    const bool ok = expect(fired, "a fresh watch should keep working after an IN_IGNORED was drained");
    expect(in.fd() >= 0, "fd() should remain valid after handling IN_IGNORED");
    cleanup(dir);
    return ok;
  }

  bool drainWithGlobalCallbackFiresOnEvent() {
    Inotify in;
    const auto dir = uniqueTempDir();

    bool fired = false;
    uint32_t mask = 0;
    std::string name;
    in.watch(dir, IN_CREATE);

    const auto file = dir / "child.txt";
    std::ofstream{file} << "hello";

    in.drain([&](const inotify_event* e) {
      fired = true;
      mask = e->mask;
      if (e->len > 0) {
        name = std::string(e->name);
      }
    });

    const bool ok = expect(fired, "global drain callback should fire when an event occurs in a watched directory")
        && expect((mask & IN_CREATE) != 0, "global callback event mask should contain IN_CREATE")
        && expect(name == "child.txt", "global callback event name should carry the created file name");
    cleanup(dir);
    return ok;
  }

  bool drainWithGlobalCallbackFiresForAllWatches() {
    Inotify in;
    const auto dir = uniqueTempDir();
    const auto dirA = dir / "a";
    const auto dirB = dir / "b";
    std::error_code ec;
    std::filesystem::create_directories(dirA, ec);
    std::filesystem::create_directories(dirB, ec);

    in.watch(dirA, IN_CREATE);
    in.watch(dirB, IN_CREATE);

    int globalHits = 0;
    in.drain([&](const inotify_event*) { ++globalHits; });

    // No events yet — nothing should fire.
    if (!expect(globalHits == 0, "global callback should not fire before any events are generated")) {
      cleanup(dir);
      return false;
    }

    std::ofstream{dirA / "file_a.txt"} << "x";
    in.drain([&](const inotify_event*) { ++globalHits; });

    if (!expect(globalHits == 1, "global callback should fire once for an event in watch A")) {
      cleanup(dir);
      return false;
    }

    std::ofstream{dirB / "file_b.txt"} << "y";
    in.drain([&](const inotify_event*) { ++globalHits; });

    const bool ok = expect(globalHits == 2, "global callback should receive events from all watches");
    cleanup(dir);
    return ok;
  }

  bool drainWithGlobalAndPerWatchCallbacksBothFire() {
    Inotify in;
    const auto dir = uniqueTempDir();

    int perWatchHits = 0;
    int globalHits = 0;
    in.watch(dir, IN_CREATE, [&](const inotify_event*) { ++perWatchHits; });

    std::ofstream{dir / "child.txt"} << "hello";

    in.drain([&](const inotify_event*) { ++globalHits; });

    const bool ok = expect(perWatchHits == 1, "per-watch callback should fire for its own watch")
        && expect(globalHits == 1, "global callback should also fire for the same event");
    cleanup(dir);
    return ok;
  }

} // namespace

int main() {
  bool ok = true;
  ok = watchOnValidDirectoryReturnsId() && ok;
  ok = watchOnMissingPathReturnsNullopt() && ok;
  ok = watchOnEmptyPathReturnsNullopt() && ok;
  ok = drainInvokesCallbackOnCreateInDirectory() && ok;
  ok = drainInvokesCallbackOnFileModify() && ok;
  ok = drainDeliversIndependentCallbacksForMultipleWatches() && ok;
  ok = unwatchStopsCallbacks() && ok;
  ok = unwatchUnknownIdIsHarmless() && ok;
  ok = drainHandlesIgnoredEventWithoutLosingState() && ok;
  ok = drainWithGlobalCallbackFiresOnEvent() && ok;
  ok = drainWithGlobalCallbackFiresForAllWatches() && ok;
  ok = drainWithGlobalAndPerWatchCallbacksBothFire() && ok;
  return ok ? 0 : 1;
}
