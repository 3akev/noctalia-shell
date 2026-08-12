#include "system/app_identity.h"
#include "system/internal_app_metadata.h"
#include "tests/test_check.h"
#include "util/string_utils.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace internal_apps {

  std::optional<AppMetadata> metadataForAppId(std::string_view /*appId*/) { return std::nullopt; }

  void applyMetadataToDesktopEntry(DesktopEntry& /*entry*/) {}

} // namespace internal_apps

namespace {

  DesktopEntry sampleChatEntry() {
    DesktopEntry entry;
    entry.id = "sample-chat-desktop";
    entry.name = "Sample Chat";
    entry.nameLower = "sample chat";
    entry.startupWmClass = "SampleChat";
    entry.startupWmClassLower = "samplechat";
    entry.exec = "sample-chat-desktop";
    entry.icon = "sample-chat-desktop";
    return entry;
  }

  DesktopEntry sampleMailEntry() {
    DesktopEntry entry;
    entry.id = "sample-mail";
    entry.name = "Sample Mail";
    entry.nameLower = "sample mail";
    entry.startupWmClass = "SampleMail";
    entry.startupWmClassLower = "samplemail";
    entry.exec = "sample-mail";
    entry.icon = "sample-mail";
    return entry;
  }

  DesktopEntry displayNameOnlyEntry() {
    DesktopEntry entry;
    entry.id = "other-app";
    entry.name = "Risky Match";
    entry.nameLower = "risky match";
    entry.startupWmClass = "OtherApp";
    entry.startupWmClassLower = "otherapp";
    entry.exec = "other-app";
    entry.icon = "other-app";
    return entry;
  }

  void expectMatch(const DesktopEntry& entry, std::string_view token) {
    TEST_CHECK(app_identity::desktopEntryMatchesLower(entry, token));
  }

  void expectNoMatch(const DesktopEntry& entry, std::string_view token) {
    TEST_CHECK(!app_identity::desktopEntryMatchesLower(entry, token));
  }

  DesktopEntry easyEffectsEntry() {
    DesktopEntry entry;
    entry.id = "com.github.wwmm.easyeffects";
    entry.name = "Easy Effects";
    entry.nameLower = "easy effects";
    entry.startupWmClass = "easyeffects";
    entry.startupWmClassLower = "easyeffects";
    entry.exec = "easyeffects";
    entry.icon = "easyeffects";
    return entry;
  }

  DesktopEntry duplicateTailEntry(std::string_view id, std::string_view startupWmClass) {
    DesktopEntry entry;
    entry.id = std::string(id);
    entry.name = std::string(id);
    entry.nameLower = StringUtils::toLower(entry.name);
    entry.startupWmClass = std::string(startupWmClass);
    entry.startupWmClassLower = StringUtils::toLower(entry.startupWmClass);
    entry.exec = entry.id;
    entry.icon = entry.id;
    return entry;
  }

} // namespace

int main() {
  const DesktopEntry chat = sampleChatEntry();

  expectMatch(chat, "sample-chat-desktop");
  expectMatch(chat, "samplechat");
  expectMatch(chat, "sample chat");
  expectMatch(chat, "sample.chat.desktop");
  expectMatch(chat, "sample_chat_desktop");
  expectMatch(chat, "sample chat desktop");
  expectMatch(chat, "Sample.ChatDesktop");
  expectNoMatch(chat, "");
  expectNoMatch(chat, "sample-calendar");

  const DesktopEntry displayNameOnly = displayNameOnlyEntry();
  expectMatch(displayNameOnly, "risky match");
  expectNoMatch(displayNameOnly, "risky.match");

  const std::vector<DesktopEntry> entries = {chat};
  const DesktopEntry resolved = app_identity::resolveRunningDesktopEntry("Sample.ChatDesktop", entries);

  TEST_CHECK(resolved.id == "sample-chat-desktop");
  TEST_CHECK(resolved.exec == "sample-chat-desktop");
  TEST_CHECK(resolved.icon == "sample-chat-desktop");

  const DesktopEntry fallback = app_identity::resolveRunningDesktopEntry("Unknown.App", entries);
  TEST_CHECK(fallback.id == "Unknown.App");
  TEST_CHECK(fallback.name == "Unknown.App");
  TEST_CHECK(fallback.nameLower == "unknown.app");
  TEST_CHECK(fallback.exec.empty());
  TEST_CHECK(fallback.icon.empty());

  // Hidden/NoDisplay entries are excluded at parse time, so the resolver never receives one in
  // production and does not re-filter them. If one is present it resolves like any other entry.
  DesktopEntry hidden = sampleChatEntry();
  hidden.hidden = true;
  TEST_CHECK(app_identity::resolveRunningDesktopEntry("Sample.ChatDesktop", {hidden}).id == "sample-chat-desktop");

  DesktopEntry noDisplay = sampleChatEntry();
  noDisplay.noDisplay = true;
  TEST_CHECK(app_identity::resolveRunningDesktopEntry("Sample.ChatDesktop", {noDisplay}).id == "sample-chat-desktop");

  const std::vector<DesktopEntry> multipleEntries = {sampleChatEntry(), sampleMailEntry()};
  const auto resolvedApps =
      app_identity::resolveRunningApps({"Sample.ChatDesktop", "sample-chat-desktop", "SampleMail"}, multipleEntries);
  TEST_CHECK(resolvedApps.size() == 2);
  TEST_CHECK(resolvedApps[0].entry.id == "sample-chat-desktop");
  TEST_CHECK(resolvedApps[1].entry.id == "sample-mail");

  const auto unknownApps = app_identity::resolveRunningApps({"Unknown.App", "unknown-app"}, multipleEntries);
  TEST_CHECK(unknownApps.size() == 2);
  TEST_CHECK(unknownApps[0].entry.id == "Unknown.App");
  TEST_CHECK(unknownApps[1].entry.id == "unknown-app");

  const DesktopEntry easyEffects = easyEffectsEntry();
  const DesktopEntry kdeResolved = app_identity::resolveRunningDesktopEntry("org.kde.easyeffects", {easyEffects});
  TEST_CHECK(kdeResolved.id == "com.github.wwmm.easyeffects");
  TEST_CHECK(kdeResolved.name == "Easy Effects");
  TEST_CHECK(kdeResolved.icon == "easyeffects");

  const std::vector<DesktopEntry> ambiguousTail = {
      duplicateTailEntry("com.foo.easyeffects", "foo-easyeffects"),
      duplicateTailEntry("com.bar.easyeffects", "bar-easyeffects"),
  };
  const DesktopEntry ambiguousResolved = app_identity::resolveRunningDesktopEntry("org.kde.easyeffects", ambiguousTail);
  TEST_CHECK(ambiguousResolved.id == "org.kde.easyeffects");

  return 0;
}
