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
    constexpr std::array<GestureBinding, 2> kBluetooth{
        {{Gesture::Left, "panel-toggle control-center bluetooth"}, {Gesture::Right, "bluetooth-toggle"}}
    };
    constexpr std::array<GestureBinding, 3> kBrightness{
        {{Gesture::Left, "panel-toggle control-center monitor"},
         {Gesture::ScrollUp, "brightness-up"},
         {Gesture::ScrollDown, "brightness-down"}}
    };
    constexpr std::array<GestureBinding, 1> kClipboard{{{Gesture::Left, "panel-toggle clipboard"}}};
    constexpr std::array<GestureBinding, 1> kClock{{{Gesture::Left, "panel-toggle control-center calendar"}}};
    constexpr std::array<GestureBinding, 1> kControlCenter{{{Gesture::Left, "panel-toggle control-center home"}}};
    constexpr std::array<GestureBinding, 1> kLauncher{{{Gesture::Left, "panel-toggle launcher"}}};
    constexpr std::array<GestureBinding, 2> kNetwork{
        {{Gesture::Left, "panel-toggle control-center network"}, {Gesture::Right, "network-toggle"}}
    };
    constexpr std::array<GestureBinding, 1> kCaffeine{{{Gesture::Left, "caffeine-toggle"}}};
    constexpr std::array<GestureBinding, 1> kKeyboardLayout{{{Gesture::Left, "keyboard-layout-cycle"}}};
    // Thumb buttons skip tracks; scroll up matches forward.
    constexpr std::array<GestureBinding, 6> kMedia{
        {{Gesture::Left, "panel-toggle control-center media"},
         {Gesture::Right, "media toggle"},
         {Gesture::Back, "media previous"},
         {Gesture::Forward, "media next"},
         {Gesture::ScrollUp, "media next"},
         {Gesture::ScrollDown, "media previous"}}
    };
    // Left is a plain on/off toggle, right flips the always-on override.
    constexpr std::array<GestureBinding, 2> kNightlight{
        {{Gesture::Left, "nightlight-toggle"}, {Gesture::Right, "nightlight-force-toggle"}}
    };
    constexpr std::array<GestureBinding, 2> kNotification{
        {{Gesture::Left, "panel-toggle control-center notifications"}, {Gesture::Right, "notification-dnd-toggle"}}
    };
    // Left steps toward performance, right back toward power-saver; scroll up matches left.
    constexpr std::array<GestureBinding, 4> kPowerProfile{
        {{Gesture::Left, "power-cycle next"},
         {Gesture::Right, "power-cycle prev"},
         {Gesture::ScrollUp, "power-cycle next"},
         {Gesture::ScrollDown, "power-cycle prev"}}
    };
    constexpr std::array<GestureBinding, 1> kScreenshot{{{Gesture::Left, "screenshot-region"}}};
    constexpr std::array<GestureBinding, 1> kSession{{{Gesture::Left, "panel-toggle session"}}};
    constexpr std::array<GestureBinding, 2> kTaskbar{
        {{Gesture::ScrollUp, "taskbar-cycle prev"}, {Gesture::ScrollDown, "taskbar-cycle next"}}
    };
    constexpr std::array<GestureBinding, 2> kWorkspaces{
        {{Gesture::ScrollUp, "workspace-switch prev"}, {Gesture::ScrollDown, "workspace-switch next"}}
    };
    constexpr std::array<GestureBinding, 1> kSettings{{{Gesture::Left, "settings-open"}}};
    constexpr std::array<GestureBinding, 1> kSysmon{{{Gesture::Left, "panel-toggle control-center system"}}};
    constexpr std::array<GestureBinding, 1> kWallpaper{{{Gesture::Left, "panel-toggle wallpaper"}}};
    constexpr std::array<GestureBinding, 1> kThemeMode{{{Gesture::Left, "theme-mode-toggle"}}};
    constexpr std::array<GestureBinding, 4> kVolumeOutput{
        {{Gesture::Left, "panel-toggle control-center audio"},
         {Gesture::Right, "volume-mute"},
         {Gesture::ScrollUp, "volume-up"},
         {Gesture::ScrollDown, "volume-down"}}
    };
    // A volume widget set to the microphone drives the source, not the sink.
    constexpr std::array<GestureBinding, 4> kVolumeInput{
        {{Gesture::Left, "panel-toggle control-center audio"},
         {Gesture::Right, "mic-mute"},
         {Gesture::ScrollUp, "mic-volume-up"},
         {Gesture::ScrollDown, "mic-volume-down"}}
    };
    constexpr std::array<GestureBinding, 1> kWeather{{{Gesture::Left, "panel-toggle control-center weather"}}};

    constexpr std::array<TypeDefaults, 23> kTypeDefaults{{
        {"battery", kBattery},
        {"bluetooth", kBluetooth},
        {"brightness", kBrightness},
        {"caffeine", kCaffeine},
        {"clipboard", kClipboard},
        {"clock", kClock},
        {"control-center", kControlCenter},
        {"keyboard_layout", kKeyboardLayout},
        {"launcher", kLauncher},
        {"media", kMedia},
        {"network", kNetwork},
        {"nightlight", kNightlight},
        {"notifications", kNotification},
        {"power_profile", kPowerProfile},
        {"screenshot", kScreenshot},
        {"session", kSession},
        {"settings", kSettings},
        {"sysmon", kSysmon},
        {"taskbar", kTaskbar},
        {"theme_mode", kThemeMode},
        {"wallpaper", kWallpaper},
        {"weather", kWeather},
        {"workspaces", kWorkspaces},
    }};

    struct TypeReserved {
      std::string_view type;
      GestureMask gestures;
    };

    const std::array<TypeReserved, 4> kTypeReserved{{
        // Left activates an individual workspace.
        {"workspaces", GestureMask{Gesture::Left}},
        // Left activates a task, middle closes it.
        {"taskbar", GestureMask{Gesture::Left, Gesture::Middle}},
        // Left activates a tray item, right opens its menu.
        {"tray", GestureMask{Gesture::Left, Gesture::Right}},
        // Right opens the capture menu, a popup anchored to this widget.
        {"screenshot", GestureMask{Gesture::Right}},
    }};

  } // namespace

  std::span<const GestureBinding> builtinGestureDefaults() noexcept { return kBuiltinDefaults; }

  std::span<const GestureBinding> gestureDefaultsForType(std::string_view type, const WidgetConfig* config) noexcept {
    if (type == "volume") {
      const std::string device = config != nullptr ? config->getString("device", "output") : std::string("output");
      return device == "input" ? std::span<const GestureBinding>(kVolumeInput)
                               : std::span<const GestureBinding>(kVolumeOutput);
    }
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

  std::unordered_map<std::string, std::string>
  defaultActionsForType(std::string_view type, const WidgetConfig* config) {
    std::unordered_map<std::string, std::string> actions;
    const auto apply = [&actions](std::span<const GestureBinding> bindings) {
      for (const auto& binding : bindings) {
        actions[std::string(gestureConfigKey(binding.gesture))] = std::string(binding.action);
      }
    };
    apply(builtinGestureDefaults());
    apply(gestureDefaultsForType(type, config));
    return actions;
  }

} // namespace noctalia::bar
