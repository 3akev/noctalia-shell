#include "shell/bar/widget_gesture_defaults.h"

#include <array>
#include <vector>

namespace noctalia::bar {

  namespace {

    constexpr std::array<GestureBinding, 1> kBuiltinDefaults{
        // Middle click opens the widget's own settings; `settings-open-widget` resolves the target
        // from the invoking widget.
        GestureBinding{Gesture::Middle, "settings-open-widget"},
    };

    struct TypeDefaults {
      std::string_view type;
      std::span<const GestureBinding> bindings;
    };

    // Widgets whose whole-widget gestures are declared here rather than wired by hand in create().
    constexpr std::array<GestureBinding, 1> kBattery{{{Gesture::Left, "panel-toggle control-center power"}}};
    constexpr std::array<GestureBinding, 1> kClipboard{{{Gesture::Left, "panel-toggle clipboard"}}};
    constexpr std::array<GestureBinding, 1> kClock{{{Gesture::Left, "panel-toggle control-center calendar"}}};
    constexpr std::array<GestureBinding, 1> kControlCenter{{{Gesture::Left, "panel-toggle control-center home"}}};
    constexpr std::array<GestureBinding, 1> kLauncher{{{Gesture::Left, "panel-toggle launcher"}}};
    constexpr std::array<GestureBinding, 1> kSession{{{Gesture::Left, "panel-toggle session"}}};
    constexpr std::array<GestureBinding, 1> kSettings{{{Gesture::Left, "settings-open"}}};
    constexpr std::array<GestureBinding, 1> kSysmon{{{Gesture::Left, "panel-toggle control-center system"}}};
    constexpr std::array<GestureBinding, 1> kWallpaper{{{Gesture::Left, "panel-toggle wallpaper"}}};
    constexpr std::array<GestureBinding, 1> kWeather{{{Gesture::Left, "panel-toggle control-center weather"}}};

    constexpr std::array<TypeDefaults, 10> kTypeDefaults{{
        {"battery", kBattery},
        {"clipboard", kClipboard},
        {"clock", kClock},
        {"control-center", kControlCenter},
        {"launcher", kLauncher},
        {"session", kSession},
        {"settings", kSettings},
        {"sysmon", kSysmon},
        {"wallpaper", kWallpaper},
        {"weather", kWeather},
    }};

    struct TypeReserved {
      std::string_view type;
      GestureMask gestures;
    };

    const std::array<TypeReserved, 3> kTypeReserved{{
        // Left activates an individual workspace.
        {"workspaces", GestureMask{Gesture::Left}},
        // Left activates a task, middle closes it.
        {"taskbar", GestureMask{Gesture::Left, Gesture::Middle}},
        // Left activates a tray item, right opens its menu.
        {"tray", GestureMask{Gesture::Left, Gesture::Right}},
    }};

  } // namespace

  std::span<const GestureBinding> builtinGestureDefaults() noexcept { return kBuiltinDefaults; }

  std::span<const GestureBinding> gestureDefaultsForType(std::string_view type) noexcept {
    for (const auto& entry : kTypeDefaults) {
      if (entry.type == type) {
        return entry.bindings;
      }
    }
    return {};
  }

  GestureMask reservedGesturesForType(std::string_view type) noexcept {
    for (const auto& entry : kTypeReserved) {
      if (entry.type == type) {
        return entry.gestures;
      }
    }
    return {};
  }

  std::unordered_map<std::string, std::string> defaultActionsForType(std::string_view type) {
    std::unordered_map<std::string, std::string> actions;
    const auto apply = [&actions](std::span<const GestureBinding> bindings) {
      for (const auto& binding : bindings) {
        actions[std::string(gestureConfigKey(binding.gesture))] = std::string(binding.action);
      }
    };
    apply(builtinGestureDefaults());
    apply(gestureDefaultsForType(type));
    return actions;
  }

} // namespace noctalia::bar
