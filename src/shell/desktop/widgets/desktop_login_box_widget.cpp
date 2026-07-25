#include "shell/desktop/widgets/desktop_login_box_widget.h"

#include "render/core/renderer.h"
#include "render/scene/node.h"
#include "shell/lockscreen/lockscreen_login_box.h"
#include "ui/builders.h"
#include "ui/palette.h"
#include "ui/style.h"

#include <memory>
#include <string_view>

namespace {

  constexpr float kLoginGlyphSize = 16.0f;
  constexpr float kRegularInfoHeight = 56.0f;
  constexpr float kRegularSessionHeight = 58.0f;

  [[nodiscard]] bool isStyleSetting(std::string_view key) {
    return key == "background_color"
        || key == "background_opacity"
        || key == "background_radius"
        || key == lockscreen_login_box::kLayoutKey
        || key == lockscreen_login_box::kShowSessionButtonsKey
        || key == lockscreen_login_box::kShowLoginButtonKey
        || key == lockscreen_login_box::kInputOpacityKey
        || key == lockscreen_login_box::kInputRadiusKey;
  }

} // namespace

void DesktopLoginBoxWidget::create() {
  auto rootNode = std::make_unique<Node>();

  auto panel = ui::box({});
  m_panel = panel.get();
  rootNode->addChild(std::move(panel));

  auto infoGhost = ui::box({});
  m_infoGhost = infoGhost.get();
  rootNode->addChild(std::move(infoGhost));

  auto mediaGhost = ui::box({});
  m_mediaGhost = mediaGhost.get();
  rootNode->addChild(std::move(mediaGhost));

  auto weatherGhost = ui::box({});
  m_weatherGhost = weatherGhost.get();
  rootNode->addChild(std::move(weatherGhost));

  auto passwordGhost = ui::box({});
  m_passwordGhost = passwordGhost.get();
  rootNode->addChild(std::move(passwordGhost));

  auto loginButtonGhost = ui::box({
      .fill = colorSpecFromRole(ColorRole::Primary, 0.9f),
  });
  m_loginButtonGhost = loginButtonGhost.get();
  rootNode->addChild(std::move(loginButtonGhost));

  auto loginGlyph = ui::glyph({
      .out = &m_loginGlyph,
      .glyph = "check",
      .glyphSize = kLoginGlyphSize,
      .color = colorSpecFromRole(ColorRole::OnPrimary),
  });
  rootNode->addChild(std::move(loginGlyph));

  for (Box*& sessionSlot : m_sessionGhosts) {
    auto sessionGhost = ui::box({});
    sessionSlot = sessionGhost.get();
    rootNode->addChild(std::move(sessionGhost));
  }

  setRoot(std::move(rootNode));
}

void DesktopLoginBoxWidget::setSettings(const std::unordered_map<std::string, WidgetSettingValue>& settings) {
  m_settings = settings;
  lockscreen_login_box::normalizeSettings(m_settings);
}

bool DesktopLoginBoxWidget::applySetting(
    const std::string& key, const WidgetSettingValue& value,
    const std::unordered_map<std::string, WidgetSettingValue>& allSettings, Renderer& renderer
) {
  (void)value;
  m_settings = allSettings;
  lockscreen_login_box::normalizeSettings(m_settings);
  if (!isStyleSetting(key)) {
    return false;
  }
  doLayout(renderer);
  return true;
}

void DesktopLoginBoxWidget::doLayout(Renderer& renderer) {
  const float screenWidth = m_screenWidth > 0.0f ? m_screenWidth : 1920.0f;
  const lockscreen_login_box::LoginBoxStyle style = lockscreen_login_box::resolveStyle(m_settings);
  const float panelWidth = lockscreen_login_box::resolvePanelWidth(screenWidth, m_boxWidth, style.layout);
  const float panelHeight =
      lockscreen_login_box::resolvePanelHeight(m_boxHeight, style.layout, style.showSessionButtons);
  const bool regular = style.layout == lockscreen_login_box::LayoutMode::Regular;

  if (m_panel != nullptr) {
    m_panel->setPosition(0.0f, 0.0f);
    m_panel->setSize(panelWidth, panelHeight);
    m_panel->setStyle(
        RoundedRectStyle{
            .fill = resolveColorSpec(style.panelFill),
            .border = colorForRole(ColorRole::Outline, style.panelOpacity),
            .fillMode = FillMode::Solid,
            .radius = Style::scaledRadius(style.panelRadius),
            .softness = 1.0f,
            .borderWidth = Style::borderWidth,
        }
    );
  }

  const float pad = Style::spaceMd;
  float contentTop = pad;
  if (regular) {
    const float infoHeight = std::min(kRegularInfoHeight, panelHeight * 0.3f);
    if (m_infoGhost != nullptr) {
      m_infoGhost->setVisible(true);
      m_infoGhost->setPosition(pad, contentTop);
      m_infoGhost->setSize(panelWidth - pad * 2.0f, infoHeight);
      m_infoGhost->setStyle(
          RoundedRectStyle{
              .fill = colorForRole(ColorRole::Surface, 0.35f),
              .fillMode = FillMode::Solid,
              .radius = Style::scaledRadius(style.inputRadius),
          }
      );
    }
    const float halfGap = Style::spaceSm;
    const float halfWidth = std::max(40.0f, (panelWidth - pad * 2.0f - halfGap) * 0.5f);
    if (m_mediaGhost != nullptr) {
      m_mediaGhost->setVisible(true);
      m_mediaGhost->setPosition(pad + Style::spaceXs, contentTop + Style::spaceXs);
      m_mediaGhost->setSize(halfWidth - Style::spaceXs, infoHeight - Style::spaceXs * 2.0f);
      m_mediaGhost->setStyle(
          RoundedRectStyle{
              .fill = colorForRole(ColorRole::Primary, 0.18f),
              .fillMode = FillMode::Solid,
              .radius = Style::scaledRadius(style.inputRadius),
          }
      );
    }
    if (m_weatherGhost != nullptr) {
      m_weatherGhost->setVisible(true);
      m_weatherGhost->setPosition(pad + halfWidth + halfGap + Style::spaceXs, contentTop + Style::spaceXs);
      m_weatherGhost->setSize(halfWidth - Style::spaceXs, infoHeight - Style::spaceXs * 2.0f);
      m_weatherGhost->setStyle(
          RoundedRectStyle{
              .fill = colorForRole(ColorRole::Secondary, 0.18f),
              .fillMode = FillMode::Solid,
              .radius = Style::scaledRadius(style.inputRadius),
          }
      );
    }
    contentTop += infoHeight + Style::spaceSm;
  } else {
    if (m_infoGhost != nullptr) {
      m_infoGhost->setVisible(false);
    }
    if (m_mediaGhost != nullptr) {
      m_mediaGhost->setVisible(false);
    }
    if (m_weatherGhost != nullptr) {
      m_weatherGhost->setVisible(false);
    }
  }

  const float sessionReserve = regular && style.showSessionButtons ? kRegularSessionHeight + Style::spaceSm : 0.0f;
  const float passwordAreaHeight = std::max(Style::controlHeight, panelHeight - contentTop - sessionReserve - pad);
  const lockscreen_login_box::PanelContentLayout layout = lockscreen_login_box::panelContentLayout(
      panelWidth, passwordAreaHeight + Style::spaceSm * 2.0f, style.showLoginButton
  );
  const float passwordTop = contentTop + std::max(0.0f, (passwordAreaHeight - layout.controlHeight) * 0.5f);

  if (m_passwordGhost != nullptr) {
    m_passwordGhost->setPosition(layout.contentLeft, passwordTop);
    m_passwordGhost->setSize(layout.inputWidth, layout.controlHeight);
    m_passwordGhost->setStyle(
        RoundedRectStyle{
            .fill = colorForRole(ColorRole::Surface, style.inputOpacity),
            .border = colorForRole(ColorRole::Outline),
            .fillMode = FillMode::Solid,
            .radius = Style::scaledRadius(style.inputRadius),
            .borderWidth = Style::borderWidth,
        }
    );
  }

  if (m_loginButtonGhost != nullptr) {
    m_loginButtonGhost->setVisible(style.showLoginButton);
    if (style.showLoginButton) {
      m_loginButtonGhost->setPosition(layout.buttonX, passwordTop);
      m_loginButtonGhost->setSize(layout.controlHeight, layout.controlHeight);
      m_loginButtonGhost->setStyle(
          RoundedRectStyle{
              .fill = colorForRole(ColorRole::Primary, 0.9f),
              .fillMode = FillMode::Solid,
              .radius = Style::scaledRadius(style.inputRadius),
          }
      );
    }
  }

  if (m_loginGlyph != nullptr) {
    m_loginGlyph->setVisible(style.showLoginButton);
    if (style.showLoginButton) {
      m_loginGlyph->setPosition(
          layout.buttonX + (layout.controlHeight - kLoginGlyphSize) * 0.5f,
          passwordTop + (layout.controlHeight - kLoginGlyphSize) * 0.5f
      );
      m_loginGlyph->measure(renderer);
    }
  }

  const bool showSession = regular && style.showSessionButtons;
  const float sessionTop = panelHeight - pad - kRegularSessionHeight;
  const float sessionGap = Style::spaceSm;
  const float sessionWidth = (panelWidth - pad * 2.0f - sessionGap * static_cast<float>(m_sessionGhosts.size() - 1))
      / static_cast<float>(m_sessionGhosts.size());
  for (std::size_t i = 0; i < m_sessionGhosts.size(); ++i) {
    Box* ghost = m_sessionGhosts[i];
    if (ghost == nullptr) {
      continue;
    }
    ghost->setVisible(showSession);
    if (!showSession) {
      continue;
    }
    ghost->setPosition(pad + static_cast<float>(i) * (sessionWidth + sessionGap), sessionTop);
    ghost->setSize(sessionWidth, kRegularSessionHeight);
    ghost->setStyle(
        RoundedRectStyle{
            .fill = colorForRole(ColorRole::Surface, 0.55f),
            .fillMode = FillMode::Solid,
            .radius = Style::scaledRadius(style.inputRadius),
        }
    );
  }

  if (Node* rootNode = root()) {
    rootNode->setSize(panelWidth, panelHeight);
  }
}
