#include "compositors/niri/niri_runtime.h"
#include "compositors/niri/niri_workspace_backend.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>

int main() {
  const std::string socketPath = "/tmp/noctalia-niri-workspace-test-" + std::to_string(getpid()) + ".sock";
  ::unlink(socketPath.c_str());

  const int listener = ::socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
  assert(listener >= 0);

  sockaddr_un address{};
  address.sun_family = AF_UNIX;
  assert(socketPath.size() < sizeof(address.sun_path));
  std::memcpy(address.sun_path, socketPath.c_str(), socketPath.size() + 1);
  assert(::bind(listener, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == 0);
  assert(::listen(listener, 1) == 0);

  // Exercise backend event handling without NIRI_SOCKET so construction does not
  // open the persistent event-stream connection (that path is covered elsewhere).
  compositors::niri::NiriRuntime runtime;
  NiriWorkspaceBackend backend(runtime);
  int changes = 0;
  backend.setChangeCallback([&]() { ++changes; });
  backend.handleEvent(
      "WorkspacesChanged",
      {{"workspaces",
        {
            {{"id", 1}, {"idx", 1}, {"output", "DP-1"}},
            {{"id", 2}, {"idx", 2}, {"output", "DP-1"}},
        }}}
  );
  changes = 0;
  backend.handleEvent(
      "WindowsChanged",
      {{"windows",
        {
            {{"id", 41},
             {"workspace_id", 1},
             {"app_id", "com.mitchellh.ghostty"},
             {"is_focused", true},
             {"layout", {{"pos_in_scrolling_layout", {1, 1}}}}},
            {{"id", 42},
             {"workspace_id", 2},
             {"app_id", "com.mitchellh.ghostty"},
             {"is_focused", false},
             {"layout", {{"pos_in_scrolling_layout", {1, 1}}}}},
        }}}
  );
  assert(backend.focusedWindowId() == std::optional<std::string>("41"));

  backend.handleEvent("WindowFocusChanged", {{"id", 42}});
  assert(backend.focusedWindowId() == std::optional<std::string>("42"));
  assert(changes == 2);

  // A new scrolling-layout position must invalidate taskbar ordering even
  // though workspace/app membership is unchanged.
  backend.handleEvent(
      "WindowOpenedOrChanged",
      {{"window",
        {{"id", 41},
         {"workspace_id", 1},
         {"app_id", "com.mitchellh.ghostty"},
         {"layout", {{"pos_in_scrolling_layout", {2, 1}}}}}}}
  );
  assert(changes == 3);
  auto windows = backend.workspaceWindows();
  const auto window41 = std::ranges::find(windows, "41", &WorkspaceWindow::windowId);
  assert(window41 != windows.end() && window41->x == 2 && window41->y == 1);

  std::string request;
  std::jthread server([&]() {
    const int client = ::accept4(listener, nullptr, nullptr, SOCK_CLOEXEC);
    assert(client >= 0);
    char buffer[512];
    const ssize_t size = ::read(client, buffer, sizeof(buffer));
    assert(size > 0);
    request.assign(buffer, static_cast<std::size_t>(size));
    constexpr std::string_view response = "{\"Ok\":null}\n";
    assert(::write(client, response.data(), response.size()) == static_cast<ssize_t>(response.size()));
    ::close(client);
  });

  assert(::setenv("NIRI_SOCKET", socketPath.c_str(), 1) == 0);
  runtime.refresh();

  assert(backend.closeWindowById("42"));
  server.join();
  const nlohmann::json expectedRequest{{"Action", {{"CloseWindow", {{"id", 42}}}}}};
  assert(nlohmann::json::parse(request) == expectedRequest);
  assert(!backend.closeWindowById("not-an-id"));

  ::close(listener);
  ::unlink(socketPath.c_str());
  return 0;
}
