#pragma once

#include "cli/schema.h"
#include "theme/scheme.h"

#include <array>
#include <string_view>

namespace noctalia::cli {

  inline constexpr std::array<std::string_view, 10> kBuiltinPaletteNames{
      "Ayu", "Catppuccin", "Dracula", "Eldritch", "Gruvbox", "Kanagawa", "Noctalia", "Nord", "Rosé Pine", "Tokyo-Night",
  };

  inline constexpr std::array kMsgColorSchemeSetBuiltinPositionals{
      Positional{"name", {}, kBuiltinPaletteNames, true, false, false},
  };
  inline constexpr std::array kMsgColorSchemeSetWallpaperPositionals{
      Positional{"name", {}, theme::kSchemeNames, true, false, false},
  };
  inline constexpr std::array kMsgColorSchemeSetCommunityPositionals{
      Positional{"name", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgColorSchemeSetCustomPositionals{
      Positional{"name", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgColorSchemeSetSubcommands{
      Command{"builtin", "Use a built-in palette", {}, {}, {}, kMsgColorSchemeSetBuiltinPositionals, {}, false},
      Command{
          "wallpaper", "Use a wallpaper generator scheme", {}, {}, {}, kMsgColorSchemeSetWallpaperPositionals, {}, false
      },
      Command{"community", "Use a community palette", {}, {}, {}, kMsgColorSchemeSetCommunityPositionals, {}, false},
      Command{"custom", "Use a custom palette", {}, {}, {}, kMsgColorSchemeSetCustomPositionals, {}, false},
  };

  inline constexpr std::array kMsgPluginsPluginPositionals{
      Positional{"author/plugin", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgPluginsUpdatePositionals{
      Positional{"source-name", {}, {}, true, false, false},
  };
  inline constexpr std::array<std::string_view, 2> kMsgPluginSourceKindChoices{"git", "path"};
  inline constexpr std::array kMsgPluginsSourceAddPositionals{
      Positional{"name", {}, {}, true, false, false},
      Positional{"kind", {}, kMsgPluginSourceKindChoices, true, false, false},
      Positional{"location", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgPluginsSourceRemovePositionals{
      Positional{"name", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgPluginsSourceSubcommands{
      Command{"list", "List plugin sources", {}, {}, {}, {}, {}, false},
      Command{"add", "Add a plugin source", {}, {}, {}, kMsgPluginsSourceAddPositionals, {}, false},
      Command{"remove", "Remove a plugin source", {}, {}, {}, kMsgPluginsSourceRemovePositionals, {}, false},
  };
  inline constexpr std::array kMsgPluginsSubcommands{
      Command{"list", "List installed plugins", {}, {}, {}, {}, {}, false},
      Command{"enable", "Enable a plugin", {}, {}, {}, kMsgPluginsPluginPositionals, {}, false},
      Command{"disable", "Disable a plugin", {}, {}, {}, kMsgPluginsPluginPositionals, {}, false},
      Command{"update", "Update a plugin source", {}, {}, {}, kMsgPluginsUpdatePositionals, {}, false},
      Command{"source", "Manage plugin sources", {}, {}, {}, {}, kMsgPluginsSourceSubcommands, false},
  };

  inline constexpr std::array<std::string_view, 7> kMsgBarAutoHideSetStateChoices{"on",    "off", "smart", "true",
                                                                                  "false", "1",   "0"};
  inline constexpr std::array<std::string_view, 2> kMsgBarLayerSetLayerChoices{"top", "overlay"};
  inline constexpr std::array<std::string_view, 2> kMsgEffectsProfileSetKindChoices{"output", "input"};
  inline constexpr std::array<std::string_view, 4> kMsgLogLevelSetLevelChoices{"debug", "info", "warn", "error"};
  inline constexpr std::array<std::string_view, 8> kMsgMediaActionChoices{"next",        "previous",       "toggle",
                                                                          "play",        "pause",          "stop",
                                                                          "next-player", "previous-player"};
  inline constexpr std::array<std::string_view, 6> kMsgNotificationDndSetStateChoices{"on",    "off", "true",
                                                                                      "false", "1",   "0"};
  inline constexpr std::array<std::string_view, 2> kMsgPowerCycleDirectionChoices{"next", "prev"};
  inline constexpr std::array<std::string_view, 3> kMsgScreenshotFullscreenModeChoices{"pick", "monitor", "all"};
  inline constexpr std::array<std::string_view, 6> kMsgSessionActionChoices{"lock",   "suspend", "lock-and-suspend",
                                                                            "logout", "reboot",  "shutdown"};
  inline constexpr std::array<std::string_view, 2> kMsgTaskbarCycleDirectionChoices{"next", "prev"};
  inline constexpr std::array<std::string_view, 3> kMsgThemeModeSetModeChoices{"dark", "light", "auto"};
  inline constexpr std::array<std::string_view, 2> kMsgWorkspaceSwitchDirectionChoices{"next", "prev"};

  inline constexpr std::array kMsgBarAutoHideSetPositionals{
      Positional{"state", {}, kMsgBarAutoHideSetStateChoices, true, false, false},
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"monitor-selector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBarHidePositionals{
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"monitor-selector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBarLayerSetPositionals{
      Positional{"layer", {}, kMsgBarLayerSetLayerChoices, true, false, false},
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"monitor-selector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBarReserveTogglePositionals{
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"monitor-selector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBarShowPositionals{
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"monitor-selector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBarTogglePositionals{
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"monitor-selector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBrightnessDownPositionals{
      Positional{"target", {}, {}, false, false, false},
      Positional{"step", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgBrightnessOsdPositionals{
      Positional{"value", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgBrightnessSetPositionals{
      Positional{"target", {}, {}, false, false, false},
      Positional{"value", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgBrightnessUpPositionals{
      Positional{"target", {}, {}, false, false, false},
      Positional{"step", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgClipboardCopyPositionals{
      Positional{"text", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgEffectsProfileSetPositionals{
      Positional{"kind", {}, kMsgEffectsProfileSetKindChoices, true, false, false},
      Positional{"profile", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgKeyboardBacklightOsdPositionals{
      Positional{"value", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgKeyboardBacklightSetPositionals{
      Positional{"value", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgLogLevelSetPositionals{
      Positional{"level", {}, kMsgLogLevelSetLevelChoices, true, false, false},
  };
  inline constexpr std::array kMsgMediaPositionals{
      Positional{"action", {}, kMsgMediaActionChoices, true, false, false},
  };
  inline constexpr std::array kMsgMicVolumeDownPositionals{
      Positional{"step", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgMicVolumeOsdPositionals{
      Positional{"value", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgMicVolumeSetPositionals{
      Positional{"value", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgMicVolumeUpPositionals{
      Positional{"step", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgNotificationDndSetPositionals{
      Positional{"state", {}, kMsgNotificationDndSetStateChoices, true, false, false},
  };
  inline constexpr std::array kMsgNotificationShowPositionals{
      Positional{"summary", {}, {}, true, false, false},
      Positional{"body", {}, {}, true, false, true},
  };
  inline constexpr std::array kMsgPanelClosePositionals{
      Positional{"id", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgPanelOpenPositionals{
      Positional{"id", {}, {}, true, false, false},
      Positional{"context", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgPanelTogglePositionals{
      Positional{"id", {}, {}, true, false, false},
      Positional{"context", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgPluginPositionals{
      Positional{"author/plugin:entry", {}, {}, true, false, false},
      Positional{"target[:bar-name]", {}, {}, true, false, false},
      Positional{"event", {}, {}, true, false, false},
      Positional{"payload", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgPowerCyclePositionals{
      Positional{"direction", {}, kMsgPowerCycleDirectionChoices, false, false, false},
  };
  inline constexpr std::array kMsgPowerSetPositionals{
      Positional{"profile", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgScreenshotFullscreenPositionals{
      Positional{"mode", {}, kMsgScreenshotFullscreenModeChoices, false, false, false},
  };
  inline constexpr std::array kMsgSessionPositionals{
      Positional{"action", {}, kMsgSessionActionChoices, true, false, false},
  };
  inline constexpr std::array kMsgSettingsOpenPositionals{
      Positional{"context", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgSettingsOpenPluginPositionals{
      Positional{"plugin-id", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgSettingsOpenWidgetPositionals{
      Positional{"bar-name", {}, {}, false, false, false},
      Positional{"widget-name", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgSettingsTogglePositionals{
      Positional{"context", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgTaskbarCyclePositionals{
      Positional{"direction", {}, kMsgTaskbarCycleDirectionChoices, true, false, false},
  };
  inline constexpr std::array kMsgThemeModeSetPositionals{
      Positional{"mode", {}, kMsgThemeModeSetModeChoices, true, false, false},
  };
  inline constexpr std::array kMsgVolumeDownPositionals{
      Positional{"step", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgVolumeOsdPositionals{
      Positional{"value", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgVolumeSetPositionals{
      Positional{"value", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgVolumeUpPositionals{
      Positional{"step", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgWallpaperGetPositionals{
      Positional{"connector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgWallpaperNextPositionals{
      Positional{"connector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgWallpaperPreviousPositionals{
      Positional{"connector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgWallpaperRandomPositionals{
      Positional{"connector", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgWallpaperSetPositionals{
      Positional{"connector", {}, {}, false, false, false},
      Positional{"path", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgWindowSwitcherPositionals{
      Positional{"action", {}, {}, false, false, false},
  };
  inline constexpr std::array kMsgWorkspaceAlertAddPositionals{
      Positional{"workspace", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgWorkspaceAlertAddWindowPositionals{
      Positional{"window-id", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgWorkspaceAlertClearPositionals{
      Positional{"workspace", {}, {}, true, false, false},
  };
  inline constexpr std::array kMsgWorkspaceSwitchPositionals{
      Positional{"direction", {}, kMsgWorkspaceSwitchDirectionChoices, true, false, false},
  };

  inline constexpr std::array kMsgSubcommands{
      Command{
          "bar-auto-hide-set", "Set auto-hide state for a bar", {}, {}, {}, kMsgBarAutoHideSetPositionals, {}, false
      },
      Command{
          "bar-hide",
          "Hide one or all bars and release their layout gaps",
          {},
          {},
          {},
          kMsgBarHidePositionals,
          {},
          false
      },
      Command{"bar-layer-set", "Set one or all bar layers", {}, {}, {}, kMsgBarLayerSetPositionals, {}, false},
      Command{
          "bar-reserve-toggle",
          "Toggle reserve space for one or all bars",
          {},
          {},
          {},
          kMsgBarReserveTogglePositionals,
          {},
          false
      },
      Command{"bar-show", "Show one or all bars", {}, {}, {}, kMsgBarShowPositionals, {}, false},
      Command{"bar-toggle", "Toggle visibility for one or all bars", {}, {}, {}, kMsgBarTogglePositionals, {}, false},
      Command{"bluetooth-disable", "Disable Bluetooth", {}, {}, {}, {}, {}, false},
      Command{"bluetooth-enable", "Enable Bluetooth", {}, {}, {}, {}, {}, false},
      Command{"bluetooth-status", "Print Bluetooth state", {}, {}, {}, {}, {}, false},
      Command{"bluetooth-toggle", "Toggle Bluetooth", {}, {}, {}, {}, {}, false},
      Command{
          "brightness-down",
          "Decrease brightness (defaults to current monitor)",
          {},
          {},
          {},
          kMsgBrightnessDownPositionals,
          {},
          false
      },
      Command{
          "brightness-list-backlight-devices", "List available sysfs backlight device names", {}, {}, {}, {}, {}, false
      },
      Command{
          "brightness-osd",
          "Show brightness OSD without changing brightness",
          {},
          {},
          {},
          kMsgBrightnessOsdPositionals,
          {},
          false
      },
      Command{
          "brightness-set",
          "Set brightness (defaults to current monitor)",
          {},
          {},
          {},
          kMsgBrightnessSetPositionals,
          {},
          false
      },
      Command{
          "brightness-up",
          "Increase brightness (defaults to current monitor)",
          {},
          {},
          {},
          kMsgBrightnessUpPositionals,
          {},
          false
      },
      Command{"caffeine-disable", "Disable caffeine (idle inhibitor)", {}, {}, {}, {}, {}, false},
      Command{"caffeine-enable", "Enable caffeine (idle inhibitor)", {}, {}, {}, {}, {}, false},
      Command{"caffeine-toggle", "Toggle caffeine (idle inhibitor)", {}, {}, {}, {}, {}, false},
      Command{"clipboard-clear", "Clear clipboard history", {}, {}, {}, {}, {}, false},
      Command{"clipboard-copy", "Copy text to the clipboard", {}, {}, {}, kMsgClipboardCopyPositionals, {}, false},
      Command{
          "clipboard-text",
          "Print the most recent clipboard text (empty when the selection holds no text)",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{
          "color-scheme-get",
          "Print active color scheme: <source> <name> (source is builtin, wallpaper, community, or custom)",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{
          "color-scheme-set",
          "Set palette source and selection in settings.toml (builtin name, wallpaper generator scheme, community id, "
          "or custom scheme folder name)",
          {},
          {},
          {},
          {},
          kMsgColorSchemeSetSubcommands,
          false
      },
      Command{"config-reload", "Reload the config file", {}, {}, {}, {}, {}, false},
      Command{"desktop-widgets-edit", "Open the desktop widgets editor", {}, {}, {}, {}, {}, false},
      Command{"desktop-widgets-exit", "Close the desktop widgets editor", {}, {}, {}, {}, {}, false},
      Command{
          "desktop-widgets-hide",
          "Hide desktop widgets now (runtime only; does not change the saved setting)",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{
          "desktop-widgets-show",
          "Show desktop widgets now (runtime only; does not change the saved setting)",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{
          "desktop-widgets-toggle",
          "Toggle desktop widgets visibility (runtime only; does not change the saved setting)",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{"desktop-widgets-toggle-edit", "Toggle desktop widgets edit mode", {}, {}, {}, {}, {}, false},
      Command{"dock-hide", "Hide the dock (persists override)", {}, {}, {}, {}, {}, false},
      Command{"dock-reload", "Reload dock configuration", {}, {}, {}, {}, {}, false},
      Command{"dock-show", "Show the dock (persists override)", {}, {}, {}, {}, {}, false},
      Command{"dock-toggle", "Toggle dock visibility (persists override)", {}, {}, {}, {}, {}, false},
      Command{"dpms-off", "Turn monitors off", {}, {}, {}, {}, {}, false},
      Command{"dpms-on", "Turn monitors on", {}, {}, {}, {}, {}, false},
      Command{
          "effects-profile-set",
          "Set the EasyEffects output or input profile",
          {},
          {},
          {},
          kMsgEffectsProfileSetPositionals,
          {},
          false
      },
      Command{
          "greeter-sync", "Sync wallpaper, colors, and monitor layout to Noctalia Greeter", {}, {}, {}, {}, {}, false
      },
      Command{"keyboard-backlight-down", "Decrease all keyboard backlights by one level", {}, {}, {}, {}, {}, false},
      Command{
          "keyboard-backlight-osd",
          "Show keyboard backlight OSD without changing brightness",
          {},
          {},
          {},
          kMsgKeyboardBacklightOsdPositionals,
          {},
          false
      },
      Command{
          "keyboard-backlight-set",
          "Set all keyboard backlights (0-100 percentage)",
          {},
          {},
          {},
          kMsgKeyboardBacklightSetPositionals,
          {},
          false
      },
      Command{"keyboard-backlight-toggle", "Toggle all keyboard backlights on/off", {}, {}, {}, {}, {}, false},
      Command{"keyboard-backlight-up", "Increase all keyboard backlights by one level", {}, {}, {}, {}, {}, false},
      Command{"keyboard-layout-cycle", "Switch to the next keyboard layout", {}, {}, {}, {}, {}, false},
      Command{"lockscreen-widgets-edit", "Open the lockscreen widgets editor", {}, {}, {}, {}, {}, false},
      Command{"lockscreen-widgets-exit", "Close the lockscreen widgets editor", {}, {}, {}, {}, {}, false},
      Command{"lockscreen-widgets-toggle-edit", "Toggle lockscreen widgets edit mode", {}, {}, {}, {}, {}, false},
      Command{"log-level-set", "Set the console log level", {}, {}, {}, kMsgLogLevelSetPositionals, {}, false},
      Command{"log-level-status", "Print the current console log level", {}, {}, {}, {}, {}, false},
      Command{"media", "Control active media playback", {}, {}, {}, kMsgMediaPositionals, {}, false},
      Command{"mic-mute", "Toggle microphone mute", {}, {}, {}, {}, {}, false},
      Command{"mic-volume-down", "Decrease microphone volume", {}, {}, {}, kMsgMicVolumeDownPositionals, {}, false},
      Command{
          "mic-volume-osd",
          "Show the microphone volume OSD without changing volume (defaults to the current volume)",
          {},
          {},
          {},
          kMsgMicVolumeOsdPositionals,
          {},
          false
      },
      Command{"mic-volume-set", "Set microphone volume", {}, {}, {}, kMsgMicVolumeSetPositionals, {}, false},
      Command{"mic-volume-up", "Increase microphone volume", {}, {}, {}, kMsgMicVolumeUpPositionals, {}, false},
      Command{
          "network-toggle",
          "Disconnect the active network, or reconnect when nothing is connected",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{"nightlight-disable", "Disable night light schedule", {}, {}, {}, {}, {}, false},
      Command{"nightlight-enable", "Enable night light schedule", {}, {}, {}, {}, {}, false},
      Command{"nightlight-force-toggle", "Toggle forced night light mode", {}, {}, {}, {}, {}, false},
      Command{"nightlight-toggle", "Toggle night light schedule", {}, {}, {}, {}, {}, false},
      Command{"notification-clear-active", "Dismiss all currently active notifications", {}, {}, {}, {}, {}, false},
      Command{"notification-clear-history", "Clear notification history", {}, {}, {}, {}, {}, false},
      Command{
          "notification-dnd-set",
          "Set notification Do Not Disturb state",
          {},
          {},
          {},
          kMsgNotificationDndSetPositionals,
          {},
          false
      },
      Command{"notification-dnd-status", "Print notification Do Not Disturb state", {}, {}, {}, {}, {}, false},
      Command{"notification-dnd-toggle", "Toggle notification Do Not Disturb state", {}, {}, {}, {}, {}, false},
      Command{
          "notification-invoke-latest",
          "Invoke the default action of the most recent active notification",
          {},
          {},
          {},
          {},
          {},
          false
      },
      Command{
          "notification-show",
          "Show an internal Noctalia notification",
          {},
          {},
          {},
          kMsgNotificationShowPositionals,
          {},
          false
      },
      Command{"osd-disable", "Disable OSD popups", {}, {}, {}, {}, {}, false},
      Command{"osd-enable", "Enable OSD popups", {}, {}, {}, {}, {}, false},
      Command{"osd-toggle", "Toggle OSD popups", {}, {}, {}, {}, {}, false},
      Command{
          "panel-close",
          "Close the active panel, or close the named panel if it is active",
          {},
          {},
          {},
          kMsgPanelClosePositionals,
          {},
          false
      },
      Command{
          "panel-open",
          "Open a panel by id, optionally with context (e.g. launcher /emo, control-center audio)",
          {},
          {},
          {},
          kMsgPanelOpenPositionals,
          {},
          false
      },
      Command{
          "panel-toggle",
          "Toggle a panel by id, optionally with context (e.g. launcher /emo, control-center audio)",
          {},
          {},
          {},
          kMsgPanelTogglePositionals,
          {},
          false
      },
      Command{"plugin", "Dispatch an event to a plugin entry", {}, {}, {}, kMsgPluginPositionals, {}, false},
      Command{
          "plugins",
          "Manage plugins and sources (list/enable/disable/update, source list/add/remove)",
          {},
          {},
          {},
          {},
          kMsgPluginsSubcommands,
          false
      },
      Command{
          "power-cycle",
          "Step through UPower's ordered profile list, forward by default (wraps)",
          {},
          {},
          {},
          kMsgPowerCyclePositionals,
          {},
          false
      },
      Command{
          "power-set",
          "Set the UPower power profile (e.g. performance, balanced, power-saver)",
          {},
          {},
          {},
          kMsgPowerSetPositionals,
          {},
          false
      },
      Command{
          "screenshot-fullscreen",
          "Capture the focused monitor by default, pick interactively with pick, or all outputs with all",
          {},
          {},
          {},
          kMsgScreenshotFullscreenPositionals,
          {},
          false
      },
      Command{"screenshot-region", "Start an interactive region screenshot", {}, {}, {}, {}, {}, false},
      Command{"session", "Run a built-in session action", {}, {}, {}, kMsgSessionPositionals, {}, false},
      Command{"settings-close", "Close the settings window", {}, {}, {}, {}, {}, false},
      Command{
          "settings-open",
          "Open the settings window, or focus it if already open, optionally at a specific section",
          {},
          {},
          {},
          kMsgSettingsOpenPositionals,
          {},
          false
      },
      Command{
          "settings-open-plugin",
          "Open the settings window at a plugin's settings (e.g. noctalia/notes)",
          {},
          {},
          {},
          kMsgSettingsOpenPluginPositionals,
          {},
          false
      },
      Command{
          "settings-open-widget",
          "Open the settings window at a bar widget; from a widget gesture, targets that widget",
          {},
          {},
          {},
          kMsgSettingsOpenWidgetPositionals,
          {},
          false
      },
      Command{
          "settings-toggle",
          "Toggle the settings window, optionally at a specific section",
          {},
          {},
          {},
          kMsgSettingsTogglePositionals,
          {},
          false
      },
      Command{"status", "Print current state as JSON", {}, {}, {}, {}, {}, false},
      Command{
          "taskbar-cycle",
          "Step to the adjacent task or workspace group in the invoking taskbar",
          {},
          {},
          {},
          kMsgTaskbarCyclePositionals,
          {},
          false
      },
      Command{"templates-apply", "Apply configured theme templates for the current palette", {}, {}, {}, {}, {}, false},
      Command{"theme-mode-get", "Print the current resolved theme mode", {}, {}, {}, {}, {}, false},
      Command{
          "theme-mode-set",
          "Set theme mode and persist to settings.toml",
          {},
          {},
          {},
          kMsgThemeModeSetPositionals,
          {},
          false
      },
      Command{"theme-mode-toggle", "Toggle theme mode between dark and light", {}, {}, {}, {}, {}, false},
      Command{"volume-down", "Decrease speaker volume", {}, {}, {}, kMsgVolumeDownPositionals, {}, false},
      Command{"volume-mute", "Toggle speaker mute", {}, {}, {}, {}, {}, false},
      Command{
          "volume-osd",
          "Show the volume OSD without changing volume (defaults to the current volume)",
          {},
          {},
          {},
          kMsgVolumeOsdPositionals,
          {},
          false
      },
      Command{"volume-set", "Set speaker volume", {}, {}, {}, kMsgVolumeSetPositionals, {}, false},
      Command{"volume-up", "Increase speaker volume", {}, {}, {}, kMsgVolumeUpPositionals, {}, false},
      Command{
          "wallpaper-get",
          "Print default wallpaper path, or effective path for an output",
          {},
          {},
          {},
          kMsgWallpaperGetPositionals,
          {},
          false
      },
      Command{
          "wallpaper-next",
          "Switch to the next wallpaper immediately",
          {},
          {},
          {},
          kMsgWallpaperNextPositionals,
          {},
          false
      },
      Command{
          "wallpaper-previous",
          "Switch to the previous wallpaper immediately",
          {},
          {},
          {},
          kMsgWallpaperPreviousPositionals,
          {},
          false
      },
      Command{
          "wallpaper-random",
          "Switch to a random wallpaper immediately",
          {},
          {},
          {},
          kMsgWallpaperRandomPositionals,
          {},
          false
      },
      Command{
          "wallpaper-set",
          "Set wallpaper for all or a specific output (persisted)",
          {},
          {},
          {},
          kMsgWallpaperSetPositionals,
          {},
          false
      },
      Command{"wifi-disable", "Disable Wi-Fi", {}, {}, {}, {}, {}, false},
      Command{"wifi-enable", "Enable Wi-Fi", {}, {}, {}, {}, {}, false},
      Command{"wifi-status", "Print Wi-Fi state", {}, {}, {}, {}, {}, false},
      Command{"wifi-toggle", "Toggle Wi-Fi", {}, {}, {}, {}, {}, false},
      Command{
          "window-switcher",
          "Open or close the window switcher overlay",
          {},
          {},
          {},
          kMsgWindowSwitcherPositionals,
          {},
          false
      },
      Command{
          "workspace-alert-add",
          "Add a workspace alert (by number, name, or id)",
          {},
          {},
          {},
          kMsgWorkspaceAlertAddPositionals,
          {},
          false
      },
      Command{
          "workspace-alert-add-window",
          "Add a workspace alert for a window",
          {},
          {},
          {},
          kMsgWorkspaceAlertAddWindowPositionals,
          {},
          false
      },
      Command{
          "workspace-alert-clear", "Clear a workspace alert", {}, {}, {}, kMsgWorkspaceAlertClearPositionals, {}, false
      },
      Command{"workspace-alert-clear-all", "Clear all workspace alerts", {}, {}, {}, {}, {}, false},
      Command{"workspace-alert-status", "Print workspace alerts", {}, {}, {}, {}, {}, false},
      Command{
          "workspace-switch",
          "Switch to the adjacent workspace on the target monitor (stops at both ends)",
          {},
          {},
          {},
          kMsgWorkspaceSwitchPositionals,
          {},
          false
      },
  };

  inline constexpr Command kMsgCmd{
      "msg", "Send a command to the running instance", {}, {}, {}, {}, kMsgSubcommands, false,
  };

  [[nodiscard]] constexpr const Command* findMsgCommand(std::string_view name) {
    for (const Command& command : kMsgCmd.subcommands) {
      if (command.name == name)
        return &command;
    }
    return nullptr;
  }

} // namespace noctalia::cli
