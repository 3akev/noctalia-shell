#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <sys/inotify.h>
#include <unordered_map>

class Inotify {
public:
  using WatchMask = std::uint32_t;
  using Callback = std::function<void(const inotify_event*)>;

  Inotify();
  ~Inotify();

  Inotify(const Inotify&) = delete;
  Inotify& operator=(const Inotify&) = delete;

  struct WatchEntry {
    int wd;
    std::filesystem::path path;
    WatchMask mask;
  };

  std::optional<int>
  watch(const std::filesystem::path& path, WatchMask mask, std::optional<Callback> callback = std::nullopt) noexcept;

  [[nodiscard]] int fd() const noexcept { return m_inotifyFd; }

  void unwatch(int wd);

  void drain(std::optional<Callback> callback = std::nullopt) noexcept;

private:
  int m_inotifyFd = -1;
  std::unordered_map<int, std::optional<Callback>> m_wdToCallback;
};
