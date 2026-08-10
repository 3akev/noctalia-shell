#include "cli/schema_msg.h"
#include "theme/builtin_palettes.h"
#include "theme/scheme.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <string_view>

namespace {

  const noctalia::cli::Command& child(const noctalia::cli::Command& command, std::string_view name) {
    const auto it = std::ranges::find(command.subcommands, name, &noctalia::cli::Command::name);
    assert(it != command.subcommands.end());
    return *it;
  }

} // namespace

int main() {
  const noctalia::cli::Command* colorSchemeSet = noctalia::cli::findMsgCommand("color-scheme-set");
  assert(colorSchemeSet != nullptr);

  const auto& builtin = child(*colorSchemeSet, "builtin");
  assert(builtin.positionals.size() == 1);
  const auto palettes = noctalia::theme::builtinPalettes();
  assert(builtin.positionals.front().choices.size() == palettes.size());
  for (std::size_t i = 0; i < palettes.size(); ++i)
    assert(builtin.positionals.front().choices[i] == palettes[i].name);

  const auto& wallpaper = child(*colorSchemeSet, "wallpaper");
  assert(wallpaper.positionals.size() == 1);
  assert(wallpaper.positionals.front().choices.size() == noctalia::theme::kSchemeNames.size());
  for (std::size_t i = 0; i < noctalia::theme::kSchemeNames.size(); ++i) {
    const std::string_view name = noctalia::theme::kSchemeNames[i];
    assert(wallpaper.positionals.front().choices[i] == name);
    const auto parsed = noctalia::theme::schemeFromString(name);
    assert(parsed.has_value());
    assert(noctalia::theme::schemeToString(*parsed) == name);
  }

  return EXIT_SUCCESS;
}
