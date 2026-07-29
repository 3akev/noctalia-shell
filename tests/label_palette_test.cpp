#include "render/scene/glyph_node.h"
#include "render/scene/text_node.h"
#include "ui/controls/glyph.h"
#include "ui/controls/label.h"
#include "ui/palette.h"

#include <print>

namespace {

  bool expect(bool condition, const char* message) {
    if (!condition) {
      std::println(stderr, "label_palette_test: {}", message);
      return false;
    }
    return true;
  }

} // namespace

int main() {
  const Palette originalPalette = palette;
  Palette firstPalette = originalPalette;
  firstPalette.shadow = rgba(0.1f, 0.2f, 0.3f, 0.8f);
  setPalette(firstPalette);

  Label label;
  label.setShadow(colorSpecFromRole(ColorRole::Shadow, 0.5f), 2.0f, 3.0f);
  const auto& labelChildren = label.children();
  auto* textNode = labelChildren.empty() ? nullptr : dynamic_cast<TextNode*>(labelChildren.front().get());

  Glyph glyph;
  glyph.setShadow(colorSpecFromRole(ColorRole::Shadow, 0.4f), 4.0f, 5.0f);
  const auto& glyphChildren = glyph.children();
  auto* glyphNode = glyphChildren.empty() ? nullptr : dynamic_cast<GlyphNode*>(glyphChildren.front().get());

  bool ok = true;
  ok = expect(textNode != nullptr, "label owns a text node") && ok;
  ok = expect(glyphNode != nullptr, "glyph owns a glyph node") && ok;
  if (textNode != nullptr) {
    ok = expect(textNode->hasShadow(), "role-based text shadow is enabled") && ok;
    ok = expect(
             textNode->shadowColor() == colorForRole(ColorRole::Shadow, 0.5f),
             "text shadow resolves from the current palette"
         )
        && ok;
  }
  if (glyphNode != nullptr) {
    ok = expect(glyphNode->hasShadow(), "role-based glyph shadow is enabled") && ok;
    ok = expect(
             glyphNode->shadowColor() == colorForRole(ColorRole::Shadow, 0.4f),
             "glyph shadow resolves from the current palette"
         )
        && ok;
  }

  Palette secondPalette = firstPalette;
  secondPalette.shadow = rgba(0.9f, 0.7f, 0.5f, 0.6f);
  setPalette(secondPalette);

  if (textNode != nullptr) {
    ok = expect(textNode->shadowColor() == colorForRole(ColorRole::Shadow, 0.5f), "text shadow follows palette changes")
        && ok;
    ok = expect(
             textNode->shadowOffsetX() == 2.0f && textNode->shadowOffsetY() == 3.0f,
             "text palette changes preserve shadow offsets"
         )
        && ok;
  }
  if (glyphNode != nullptr) {
    ok = expect(
             glyphNode->shadowColor() == colorForRole(ColorRole::Shadow, 0.4f), "glyph shadow follows palette changes"
         )
        && ok;
    ok = expect(
             glyphNode->shadowOffsetX() == 4.0f && glyphNode->shadowOffsetY() == 5.0f,
             "glyph palette changes preserve shadow offsets"
         )
        && ok;
  }

  setPalette(originalPalette);
  return ok ? 0 : 1;
}
