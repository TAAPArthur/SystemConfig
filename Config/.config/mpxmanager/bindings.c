#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <stdlib.h>

#include <mpxmanager/Extensions/containers.h>
#include <mpxmanager/Extensions/extra-rules.h>
#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/session.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/boundfunction.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/layouts.h>
#include <mpxmanager/settings.h>
#include <mpxmanager/system.h>
#include <mpxmanager/util/debug.h>
#include <mpxmanager/util/logger.h>
#include <mpxmanager/windows.h>
#include <mpxmanager/wm-rules.h>
#include <mpxmanager/wmfunctions.h>
#include <mpxmanager/xutil/device-grab.h>
#include <mpxmanager/xutil/window-properties.h>

void raiseOrRunBrowser(int dir) {
    raiseOrRunFunc("browser", "$BROWSER", dir, matchesRole);
}

void raiseOrRunMediaView(int dir) {
    raiseOrRunFunc("media_player", "exit 0", dir, matchesRole);
}

void raiseOrRunTerminal(int dir) {
    raiseOrRunFunc("$TERMINAL", "$TERMINAL", dir, matchesClass);
}

Binding customBindings[] = {

    {0, 0, {replayKeyboardEvent}, .flags = {.mode=KEYBOARD_PASSTHROUGH_MODE, .mask=XCB_INPUT_XI_EVENT_MASK_KEY_PRESS}},

    {0, XF86XK_Display, {spawn,  .arg = {.str = "srandr load"}} },
    {ShiftMask, XF86XK_Display, {spawn,  .arg = {.str = "xsane-xrandr -i configure"}} },
    {Mod4Mask, XF86XK_Display, {raiseOrRun,  .arg = {.str = "xsane-xrandr dup"}} },

    {0, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "brightness 5%"}} },
    {Mod4Mask, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "brightness 5%"}} },
    {ShiftMask, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "brightness 10"}} },
    {0, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "brightness -5%"}} },
    {Mod4Mask, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "brightness -5%"}} },
    {ShiftMask, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "brightness -10"}} },
    {0, XF86XK_Tools, {spawn,  .arg = {.str = "xrandr-invert-colors"}} },

    {0, XF86XK_Bluetooth, {spawn,  .arg = {.str = "rfkill unblock bluetooth"}} },
    {ShiftMask, XF86XK_Bluetooth, {spawn,  .arg = {.str = "rfkill block bluetooth"}} },

    {0, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "vol -d 1"}} },
    {0, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "vol -i 1"}} },
    {0, XF86XK_AudioMute, {spawn,  .arg = {.str = "vol -t"}} },
    {Mod4Mask, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "vol -d 1"}} },
    {Mod4Mask, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "vol -i 1"}} },
    {Mod4Mask, XF86XK_AudioMute, {spawn,  .arg = {.str = "vol -t"}} },
    {ShiftMask, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "vol --default-source -d 1"}} },
    {ShiftMask, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "vol --default-source -i 1"}} },
    {ShiftMask, XF86XK_AudioMute, {spawn,  .arg = {.str = "vol --default-source -t"}} },
    {0, XF86XK_AudioMicMute, {spawn,  .arg = {.str = "vol --default-source -t"}} },


    {0, XF86XK_PowerOff, {spawn,  .arg = {.str = "nmenu"}} , {.noKeyRepeat = 1} },
    {Mod4Mask, XF86XK_PowerOff, {spawn,  .arg = {.str = "nmenu"}} , {.noKeyRepeat = 1} },

    {Mod4Mask, XK_p, {spawn,  .arg = {.str = "dmenu_run"}} },
    {Mod4Mask|ShiftMask, XK_p, {spawn,  .arg = {.str = "dmenu-calc"}} },


    {Mod4Mask, XK_v, {spawn,  .arg = {.str = "clip-history select -w $_WIN_ID"}} },
    {Mod4Mask | ShiftMask, XK_v, {spawn,  .arg = {.str = "clip-history select -w $_WIN_ID && xsel --clipboard | xvkbd -window $_WIN_ID -file - 2>/dev/null"}} },


    {0, XK_Print, {spawn,  .arg = {.str = "cd $PICTURES_DIR; screenshot -s ?"}} },
    {ShiftMask, XK_Print, {spawn,  .arg = {.str = "screenshot -a $_VIEW_X $_VIEW_Y $_VIEW_WIDTH $_VIEW_HEIGHT $PICTURES_DIR/$(date -u +%F_%H:%M:%S).png"}} },
    {Mod4Mask, XK_Print, {spawn,  .arg = {.str = "screenshot -w $_WIN_ID - | xclip -selection clipboard -t image/png"}} },
    {Mod4Mask | ShiftMask, XK_Print, {spawn,  .arg = {.str = "screenshot -w $_WIN_ID $PICTURES_DIR/$(date -u +%F_%H:%M:%S).png"}} },
    {ControlMask, XK_Print, {spawn,  .arg = {.str = "screenshot -w $_WIN_ID ?"}} },

    {Mod4Mask, XK_Delete, {raiseOrRun2,  .arg = {.str = "ncdu"}, .arg2.str = "$TERMINAL -c ncdu -e ncdu /"} },
    {Mod4Mask | ShiftMask, XK_Delete, {raiseOrRun2,  .arg = {.str = "ncdu"}, .arg2.str = "$TERMINAL -c ncdu -e ncdu ~"} },

    CYCLE_BINDINGS(Mod4Mask, ShiftMask, XK_s, raiseOrRunTerminal, XK_Super_L),
    {Mod4Mask | ControlMask, XK_s, {spawn,  .arg = {.str = "$TERMINAL"}} },

    CYCLE_BINDINGS(Mod4Mask, ShiftMask, XK_w, raiseOrRunBrowser, XK_Super_L),
    CYCLE_BINDINGS(Mod4Mask, ShiftMask, XK_e, raiseOrRunMediaView, XK_Super_L),
    {Mod4Mask | Mod1Mask, XK_w, {spawnSilent,  .arg = {.str = "$BROWSER_ALT"}} },
    {Mod4Mask | ControlMask | ShiftMask, XK_w, {spawnSilent,  .arg = {.str = "$BROWSER"}} },
    {Mod4Mask | ControlMask, XK_w, {spawnSilent,  .arg = {.str = "firefox"}} },

    {Mod4Mask | ControlMask, XK_d, {raiseOrRun,  .arg = {.str = "dolphin-emu"}} },


    {0, XF86XK_AudioPlay, {toggleActiveLayoutOrCycle, .arg = {.p = &FULL}}},
    {DEFAULT_MOD_MASK, XF86XK_AudioPlay, {toggleMask, {FULLSCREEN_MASK}}, .flags = {.windowToPass = FOCUSED_WINDOW}},
    {DEFAULT_MOD_MASK | ShiftMask, XF86XK_AudioPlay, {toggleMask, {ROOT_FULLSCREEN_MASK}}, .flags = {.windowToPass = FOCUSED_WINDOW}},

    {Mod3Mask, XK_f, {centerMouseInWindow}, .flags = {.windowToPass = FOCUSED_WINDOW}},

    /*
    {Mod4Mask, XK_x, {cloneFocusedWindow}},
    {Mod4Mask | ShiftMask, XK_x, {killAllClones}},
    */

    {Mod4Mask, XK_semicolon, {toggleContainer},  {.windowToPass = FOCUSED_WINDOW}},
    {Mod3Mask, XK_semicolon, {toggleContainer},  {.windowToPass = FOCUSED_WINDOW}},

    SPLIT_MASTER_BINDING(Mod4Mask | Mod1Mask, XK_d),

};

Binding masterBindings[] = {
    {Mod4Mask, XK_b, {splitMaster}},
    {Mod4Mask, XK_n, {cycleSlaves, .arg.i = UP}},
    {Mod4Mask | ShiftMask, XK_n, {cycleSlaves, .arg.i = DOWN}},
    {Mod4Mask, XK_m, {shiftSlaves, .arg.i = UP}},
    {Mod4Mask | ShiftMask, XK_m, {shiftSlaves, .arg.i = DOWN}},
    {Mod4Mask, XK_comma, {cycleActiveSlave, .arg.i = UP}},
    {Mod4Mask | ShiftMask, XK_comma, {cycleActiveSlave, .arg.i = DOWN}},
    {Mod4Mask, XK_d, {startMPX}},
    {Mod4Mask | ControlMask, XK_d, {(void(*)())saveMPXMasterInfo}},
    {Mod4Mask | ShiftMask, XK_d, {destroyAllNonDefaultMasters}},
};

void addNormalBindings() {
    addBindings(customBindings, LEN(customBindings));
    addBindings(masterBindings, LEN(masterBindings));
}
