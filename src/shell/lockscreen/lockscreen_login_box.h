#pragma once

#include "config/config_types.h"
#include "ui/palette.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class WaylandConnection;

namespace desktop_settings {
  enum class DesktopWidgetSettingsScope;
}

namespace lockscreen_login_box {

  constexpr std::string_view kWidgetType = "login_box";
  constexpr std::string_view kWidgetIdPrefix = "lockscreen-login-box@";

  constexpr std::string_view kLayoutKey = "layout";
  constexpr std::string_view kLayoutCompact = "compact";
  constexpr std::string_view kLayoutRegular = "regular";
  constexpr std::string_view kShowSessionButtonsKey = "show_session_buttons";
  constexpr std::string_view kInputOpacityKey = "input_opacity";
  constexpr std::string_view kInputRadiusKey = "input_radius";
  constexpr std::string_view kCenterPasswordTextKey = "center_password_text";
  constexpr std::string_view kShowLoginButtonKey = "show_login_button";
  constexpr std::string_view kShowPasswordHintKey = "show_password_hint";
  constexpr std::string_view kShowCapsLockKey = "show_caps_lock";
  constexpr std::string_view kShowKeyboardLayoutKey = "show_keyboard_layout";

  enum class LayoutMode : std::uint8_t {
    Compact,
    Regular,
  };

  struct LoginBoxStyle {
    LayoutMode layout = LayoutMode::Regular;
    ColorSpec panelFill = colorSpecFromRole(ColorRole::SurfaceVariant, 0.88f);
    float panelOpacity = 0.88f;
    float panelRadius = 12.0f;
    float inputOpacity = 1.0f;
    float inputRadius = 6.0f;
    bool centerPasswordText = false;
    bool showLoginButton = true;
    bool showPasswordHint = true;
    bool showCapsLock = true;
    bool showKeyboardLayout = true;
    bool showSessionButtons = true;
  };

  [[nodiscard]] bool isLoginBoxWidget(const DesktopWidgetState& state);
  [[nodiscard]] bool isLoginBoxWidgetType(std::string_view type);
  [[nodiscard]] bool isLoginBoxWidgetId(std::string_view id);
  [[nodiscard]] std::string widgetIdForOutput(std::string_view outputKey);

  [[nodiscard]] LayoutMode resolveLayout(const std::unordered_map<std::string, WidgetSettingValue>& settings);
  [[nodiscard]] LayoutMode resolveLayout(std::string_view layout);

  constexpr float kCompactDefaultWidthCap = 400.0f;
  constexpr float kRegularDefaultWidthCap = 810.0f;
  constexpr float kCompactMinPanelWidth = 240.0f;
  // Min width for media + weather; forecast needs more.
  constexpr float kRegularMinPanelWidth = 720.0f;
  constexpr float kCompactMinPanelHeight = 64.0f;
  // Ideal session row height plus the gap above it (used when toggling session buttons).
  constexpr float kRegularSessionBlockHeight = 66.0f;
  // Fits shrinkable info/session rows plus password/status with spaceSm padding/gaps.
  constexpr float kRegularMinPanelHeight = 190.0f;
  constexpr float kRegularMinPanelHeightNoSession = kRegularMinPanelHeight - kRegularSessionBlockHeight;
  constexpr float kCompactMaxPanelHeight = 120.0f;
  constexpr float kRegularMaxPanelHeight = 300.0f;

  struct PanelContentLayout {
    float contentLeft = 0.0f;
    float contentTop = 0.0f;
    float inputWidth = 0.0f;
    float buttonX = 0.0f;
    float controlHeight = 0.0f;
  };

  [[nodiscard]] float defaultPanelWidth(float screenWidth, LayoutMode layout);
  [[nodiscard]] float defaultPanelHeight(LayoutMode layout, bool showSessionButtons = true);
  [[nodiscard]] float minPanelWidth(LayoutMode layout);
  [[nodiscard]] float minPanelHeight(LayoutMode layout, bool showSessionButtons = true);
  [[nodiscard]] float maxPanelHeight(LayoutMode layout);
  [[nodiscard]] float resolvePanelWidth(float screenWidth, float boxWidth, LayoutMode layout);
  [[nodiscard]] float resolvePanelHeight(float boxHeight, LayoutMode layout, bool showSessionButtons = true);
  void defaultPanelSize(
      float screenWidth, float& boxWidth, float& boxHeight, LayoutMode layout, bool showSessionButtons = true
  );
  void clampPanelSize(
      float screenWidth, float& boxWidth, float& boxHeight, LayoutMode layout, bool showSessionButtons = true
  );
  [[nodiscard]] PanelContentLayout panelContentLayout(float panelWidth, float panelHeight, bool showLoginButton);
  void defaultPanelCenter(
      float screenWidth, float screenHeight, float& cx, float& cy, LayoutMode layout, bool showSessionButtons = true
  );
  void panelOriginFromCenter(
      float cx, float cy, float screenWidth, float boxWidth, float boxHeight, LayoutMode layout, float& panelX,
      float& panelY, float& panelWidthOut, float& panelHeightOut, bool showSessionButtons = true
  );

  [[nodiscard]] const DesktopWidgetState*
  findForOutput(const std::vector<DesktopWidgetState>& widgets, std::string_view outputKey);

  [[nodiscard]] LoginBoxStyle resolveStyle(const std::unordered_map<std::string, WidgetSettingValue>& settings);
  void applyDefaultSettings(
      std::unordered_map<std::string, WidgetSettingValue>& settings, desktop_settings::DesktopWidgetSettingsScope scope
  );
  void applyAllDefaultSettings(std::unordered_map<std::string, WidgetSettingValue>& settings);
  void normalizeSettings(std::unordered_map<std::string, WidgetSettingValue>& settings);

  void ensureWidgets(std::vector<DesktopWidgetState>& widgets, const WaylandConnection& wayland);

} // namespace lockscreen_login_box
