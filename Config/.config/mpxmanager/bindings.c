#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

#include <mpxmanager/Extensions/containers.h>
#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/session.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/boundfunction.h>
#include <mpxmanager/util/debug.h>
#include <mpxmanager/xutil/device-grab.h>
#include <mpxmanager/Extensions/extra-rules.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/layouts.h>
#include <mpxmanager/util/logger.h>
#include <mpxmanager/settings.h>
#include <mpxmanager/system.h>
#include <mpxmanager/xutil/window-properties.h>
#include <mpxmanager/windows.h>
#include <mpxmanager/wm-rules.h>
#include <mpxmanager/wmfunctions.h>
#include "config.h"


void containTablessWindows() {
    containWindows(getWorkspace(3), (WindowFunctionArg) {matchesClass,  .arg = {.str = "vimb"}}, "vimb");
    containWindows(getWorkspace(4), (WindowFunctionArg) {matchesClass,  .arg = {.str = "st"}}, "st");
}


void raiseOrRunBrowser(int dir) {
    raiseOrRunFunc("browser", "$BROWSER", dir, matchesRole);
}

void raiseOrRunTerminal(int dir) {
    raiseOrRunFunc("$TERMINAL", "$TERMINAL", dir, matchesClass);
}

Binding customBindings[] = {
    {Mod4Mask, XK_F6, {raiseOrRun,  .arg = {.str = "arandr"}} },
    {0, XF86XK_Display, {spawn,  .arg = {.str = "autorandr -c"}} },
    {ShiftMask, XF86XK_Display, {spawn,  .arg = {.str = "xsane-xrandr -i configure"}} },
    {Mod4Mask, XF86XK_Display, {raiseOrRun,  .arg = {.str = "arandr"}} },

    {0, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "brightnessctl s +1"}} },
    {0, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "brightnessctl s 1-"}} },
    {Mod4Mask, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "brightness +.1"}} },
    {Mod4Mask | ShiftMask, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "brightness"}} },
    {Mod4Mask, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "brightness -.1"}} },
    {Mod4Mask | ShiftMask, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "brightness"}} },
    {Mod4Mask | ControlMask, XF86XK_MonBrightnessDown, {spawn,  .arg = {.str = "xrandr-invert-colors"}} },
    {Mod4Mask | ControlMask, XF86XK_MonBrightnessUp, {spawn,  .arg = {.str = "xrandr-invert-colors"}} },
    {0, XF86XK_Tools, {spawn,  .arg = {.str = "xrandr-invert-colors"}} },

    {Mod4Mask, XK_v, {spawn,  .arg = {.str = "clip-history select -w $_WIN_ID"}} },
    {Mod4Mask | ShiftMask, XK_v, {spawn,  .arg = {.str = "clip-history select -w $_WIN_ID && xsel --clipboard | xvkbd -window $_WIN_ID -file - 2>/dev/null"}} },

    {Mod4Mask, XK_p, {spawn,  .arg = {.str = "dmenu_run"}} },
    {0, XF86XK_PowerOff, {spawn,  .arg = {.str = "nmenu"}} },

    {0, XK_Print, {spawn,  .arg = {.str = "screenshot maim -s"}} },
    {ShiftMask, XK_Print, {spawn,  .arg = {.str = "maim -i $PICTURES_DIR/$(date +%s).png"}} },
    {Mod4Mask, XK_Print, {spawn,  .arg = {.str = "maim -i $_WIN_ID| xclip -selection clipboard -t image/png"}} },
    {Mod4Mask | ShiftMask, XK_Print, {spawn,  .arg = {.str = "maim -i $_WIN_ID $PICTURES_DIR/$(date +%s).png"}} },
    {ControlMask, XK_Print, {spawn,  .arg = {.str = "screenshot maim -i $_WIN_ID"}} },

    {0, XF86XK_Eject, {spawn,  .arg = {.str = "dmenu-pycalc"}} },
    {0, XF86XK_Calculator, {spawn,  .arg = {.str = "dmenu-pycalc"}} },
    {Mod4Mask, XK_Delete, {raiseOrRun2,  .arg = {.str = "gt5"}, .arg2.str = "$TERMINAL -c gt5 -e gt5 /"} },
    {Mod4Mask | ShiftMask, XK_Delete, {raiseOrRun2,  .arg = {.str = "gt5"}, .arg2.str = "$TERMINAL -c gt5 -e gt5 ~"} },

    {ControlMask | Mod1Mask, XK_Delete, {spawn,  .arg = {.str = "notify-send -t 20000  CPU \"}$(ps -Ao pid,pcpu,pmem,comm --sort=-pcpu --cols=27 | head -n 6)\" && notify-send -t 20000  MEM \"$(ps -Ao pid,pcpu,pmem,comm --sort=-pmem --cols=27 | head -n 6)\" "}}, { .noKeyRepeat = 1}},

    {Mod4Mask, XK_Escape, {spawn,  .arg = {.str = "xtrlock"}}, {.noKeyRepeat = 1} },
    {ControlMask | Mod1Mask, XK_Escape, {spawn, .arg.str = SUSPEND_CMD}, {.noKeyRepeat = 1} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_Escape, {spawn, .arg.str = SHUTDOWN_CMD}, {.noKeyRepeat = 1} },


    {0, XF86XK_Bluetooth, {spawn,  .arg = {.str = "rfkill unblock bluetooth"}} },
    {ShiftMask, XF86XK_Bluetooth, {spawn,  .arg = {.str = "rfkill block bluetooth"}} },

    {0, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "vol -d 1"}} },
    {0, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "vol -i 1"}} },
    {0, XF86XK_AudioMute, {spawn,  .arg = {.str = "vol -t"}} },
    {ShiftMask, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "vol --default-source -d 1"}} },
    {ShiftMask, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "vol --default-source -i 1"}} },
    {ShiftMask, XF86XK_AudioMute, {spawn,  .arg = {.str = "vol --default-source -t"}} },
    {0, XF86XK_AudioMicMute, {spawn,  .arg = {.str = "vol --default-source -t"}} },
    {Mod3Mask, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "omnivolctrl clear"}} },
    {Mod3Mask, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "omnivolctrl clear"}} },
    {Mod4Mask, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "omnivolctrl -1%"}} },
    {Mod4Mask, XF86XK_AudioRaiseVolume, {spawn,  .arg = {.str = "omnivolctrl +1%"}} },
    {Mod4Mask, XF86XK_AudioMute, {spawn,  .arg = {.str = "omnivolctrl sink-input-mute toggle"}} },
    {Mod4Mask, XF86XK_AudioPrev, {spawn,  .arg = {.str = "omnivolctrl sink-inputs"}} },
    {0, XF86XK_AudioPrev, {spawn,  .arg = {.str = "omnivolctrl sinks"}} },

    {Mod4Mask, XK_Menu, {raiseOrRun,  .arg = {.str = "pavucontrol"}} },

    CYCLE_BINDINGS(Mod4Mask, ShiftMask, XK_s, raiseOrRunTerminal, XK_Super_L),
    {Mod4Mask | ControlMask, XK_s, {spawn,  .arg = {.str = "$TERMINAL"}} },

    CYCLE_BINDINGS(Mod4Mask, ShiftMask, XK_w, raiseOrRunBrowser, XK_Super_L),
    {Mod4Mask | Mod1Mask, XK_w, {spawn,  .arg = {.str = "$BROWSER_ALT"}} },
    {Mod4Mask | ControlMask | ShiftMask, XK_w, {spawn,  .arg = {.str = "$BROWSER"}} },
    {Mod4Mask | ControlMask, XK_w, {spawn,  .arg = {.str = "firefox"}} },

    {Mod4Mask | ControlMask, XK_d, {raiseOrRun,  .arg = {.str = "dolphin-emu"}} },


    {0, XF86XK_AudioPlay, {toggleActiveLayoutOrCycle, .arg = {.p = &FULL}}},
    {DEFAULT_MOD_MASK, XF86XK_AudioPlay, {toggleMask, {FULLSCREEN_MASK}}, .flags = {.windowToPass = FOCUSED_WINDOW}},
    {DEFAULT_MOD_MASK | ShiftMask, XF86XK_AudioPlay, {toggleMask, {ROOT_FULLSCREEN_MASK}}, .flags = {.windowToPass = FOCUSED_WINDOW}},

    {Mod3Mask, XK_f, {centerMouseInWindow}, .flags = {.windowToPass = FOCUSED_WINDOW}},

    /*
    {Mod4Mask, XK_x, {cloneFocusedWindow}},
    {Mod4Mask | ShiftMask, XK_x, {killAllClones}},
    */

    {Mod4Mask| ControlMask, XK_t, {containTablessWindows}, },
    {Mod4Mask , XK_z, {containWindowAndActivate},  {.windowToPass = FOCUSED_WINDOW}},
    {Mod4Mask | ShiftMask, XK_z, {releaseAllWindows}},
    {Mod3Mask, XK_t, {toggleContainer},  {.windowToPass = FOCUSED_WINDOW}},
    {Mod4Mask | Mod1Mask | ShiftMask, XK_z, {clearAllFakeMonitors}},


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
