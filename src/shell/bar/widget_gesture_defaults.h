#pragma once

#include "shell/bar/widget_action.h"

#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

namespace noctalia::bar {

  // Default gesture bindings, keyed by widget type so both the runtime and the settings GUI read
  // the same source. The GUI only has the type string, never a Widget instance, which is why these
  // are not virtuals on Widget.

  // Layer 1: applies to every widget type.
  [[nodiscard]] std::span<const GestureBinding> builtinGestureDefaults() noexcept;

  // Layer 2: what a widget type declares for itself. Empty when it declares none.
  [[nodiscard]] std::span<const GestureBinding> gestureDefaultsForType(std::string_view type) noexcept;

  // Layers 1 and 2 merged, as config-shaped strings. Used to show defaults in the settings editor.
  [[nodiscard]] std::unordered_map<std::string, std::string> defaultActionsForType(std::string_view type);

} // namespace noctalia::bar
