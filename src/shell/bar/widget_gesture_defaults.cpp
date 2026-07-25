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

    // Per-type defaults land here as widgets migrate their hardcoded click handlers.
    constexpr std::array<TypeDefaults, 0> kTypeDefaults{};

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
