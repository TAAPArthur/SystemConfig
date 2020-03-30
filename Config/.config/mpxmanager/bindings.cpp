#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

#include <mpxmanager/Extensions/containers.h>
#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/session.h>
#include <mpxmanager/Extensions/window-clone.h>
#include <mpxmanager/Extensions/xmousecontrol.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/chain.h>
#include <mpxmanager/debug.h>
#include <mpxmanager/device-grab.h>
#include <mpxmanager/extra-rules.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/layouts.h>
#include <mpxmanager/logger.h>
#include <mpxmanager/settings.h>
#include <mpxmanager/system.h>
#include <mpxmanager/test-functions.h>
#include <mpxmanager/window-properties.h>
#include <mpxmanager/windows.h>
#include <mpxmanager/wm-rules.h>
#include <mpxmanager/wmfunctions.h>
#include "config.h"

Binding customBindings[] = {
    {Mod4Mask, XK_F6, {raiseOrRun, "arandr"} },
    {0, XF86XK_Display, {spawn, "autorandr -c"} },
    {ShiftMask, XF86XK_Display, {spawn, "xsane-xrandr -i configure"} },
    {Mod4Mask, XF86XK_Display, {raiseOrRun, "arandr"} },

    {0, XF86XK_MonBrightnessUp, {spawn, "brightnessctl s +1"} },
    {0, XF86XK_MonBrightnessDown, {spawn, "brightnessctl s 1-"} },
    {Mod4Mask, XF86XK_MonBrightnessUp, {spawn, "brightness +.1"} },
    {Mod4Mask | ShiftMask, XF86XK_MonBrightnessUp, {spawn, "brightness"} },
    {Mod4Mask, XF86XK_MonBrightnessDown, {spawn, "brightness -.1"} },
    {Mod4Mask | ShiftMask, XF86XK_MonBrightnessDown, {spawn, "brightness"} },
    {Mod4Mask | ControlMask, XF86XK_MonBrightnessDown, {spawn, "xrandr-invert-colors"} },
    {Mod4Mask | ControlMask, XF86XK_MonBrightnessUp, {spawn, "xrandr-invert-colors"} },
    {0, XF86XK_Tools, {spawn, "xrandr-invert-colors"} },

    {Mod4Mask, XK_v, {spawn, "clip-history select -w $_WIN_ID && xsel --clipboard | xvkbd -window $_WIN_ID -file - 2>/dev/null"} },

    {Mod4Mask, XK_p, {spawn, "dmenu_run"} },

    {0, XK_Print, {spawn, "maim -s | xclip -selection clipboard -t image/png"} },
    {ShiftMask, XK_Print, {spawn, "maim -s /tmp/$(date +%s).png"} },
    {Mod4Mask, XK_Print, {spawn, "maim -i $_WIN_ID| xclip -selection clipboard -t image/png"} },
    {Mod4Mask | ShiftMask, XK_Print, {spawn, "maim -i $_WIN_ID /tmp/$(date +%s).png"} },
    {ControlMask, XK_Print, {spawn, "maim -i | xclip -selection clipboard -t image/png"} },

    {0, XF86XK_Eject, {spawn, "dmenu-pycalc"} },
    {0, XF86XK_Calculator, {spawn, "dmenu-pycalc"} },
    {Mod4Mask, XK_Delete, +[] {raiseOrRun("gt5", "/usr/bin/$TERMINAL -r gt5 -e 'gt5 /' -T gt5", MATCHES_ROLE);} },
    {Mod4Mask| ShiftMask, XK_Delete, +[] {raiseOrRun("gt5", "/usr/bin/$TERMINAL -r gt5 -e 'gt5 ~' -T gt5", MATCHES_ROLE);} },

    {ControlMask | Mod1Mask, XK_Delete, {raiseOrRun, "notify-send -t 20000  CPU \"$(ps -Ao pid,pcpu,pmem,comm --sort=-pcpu --cols=27 | head -n 6)\" && notify-send -t 20000  MEM \"$(ps -Ao pid,pcpu,pmem,comm --sort=-pmem --cols=27 | head -n 6)\" "},{ .noKeyRepeat = 1}},

    {Mod4Mask, XK_Escape, {spawn, "xtrlock"}, {.noKeyRepeat = 1} },
    {ControlMask | Mod1Mask, XK_Escape, {spawn, SUSPEND_CMD}, {.noKeyRepeat = 1} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_Escape, {spawn, SHUTDOWN_CMD}, {.noKeyRepeat = 1} },


    {0, XF86XK_Bluetooth, {spawn, "rfkill unblock bluetooth"} },
    {ShiftMask, XF86XK_Bluetooth, {spawn, "rfkill block bluetooth"} },

    {0, XF86XK_AudioLowerVolume, {spawn, "pamixer -d 1"} },
    {0, XF86XK_AudioRaiseVolume, {spawn, "pamixer -i 1"} },
    {0, XF86XK_AudioMute, {spawn, "pamixer -t"} },
    {ShiftMask, XF86XK_AudioLowerVolume, {spawn, "pamixer --default-source -d 1"} },
    {ShiftMask, XF86XK_AudioRaiseVolume, {spawn, "pamixer --default-source -i 1"} },
    {ShiftMask, XF86XK_AudioMute, {spawn, "pamixer -t"} },
    {0, XF86XK_AudioMicMute, {spawn, "pamixer --default-source -t"} },
    {Mod3Mask, XF86XK_AudioLowerVolume, {spawn, "omnivolctrl clear"} },
    {Mod3Mask, XF86XK_AudioRaiseVolume, {spawn, "omnivolctrl clear"} },
    {Mod4Mask, XF86XK_AudioLowerVolume, {spawn, "omnivolctrl -1%"} },
    {Mod4Mask, XF86XK_AudioRaiseVolume, {spawn, "omnivolctrl +1%"} },
    {Mod4Mask, XF86XK_AudioMute, {spawn, "omnivolctrl sink-input-mute toggle"} },
    {Mod4Mask, XF86XK_AudioPrev, {spawn, "omnivolctrl sink-inputs"} },
    {0, XF86XK_AudioPrev, {spawn, "omnivolctrl sinks"} },

    {Mod4Mask, XK_Menu, {raiseOrRun, "pavucontrol"} },

    {Mod4Mask, XK_w, +[]{return raiseOrRun("browser", "$BROWSER", MATCHES_ROLE);} },
    {Mod4Mask | ShiftMask, XK_w, {spawn, "$BROWSER_ALT"} },
    {Mod4Mask | ControlMask | ShiftMask, XK_w, {spawn, "$BROWSER"} },

    {Mod4Mask | ControlMask, XK_v, {raiseOrRun, "vlc"} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_v, {raiseOrRun, "pitivi"} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_a, {raiseOrRun, "audacity"} },
    {Mod4Mask | ControlMask, XK_d, {raiseOrRun, "dolphin-emu"} },
    {Mod4Mask, XK_g, {spawn, "stream-video"} },
    {Mod4Mask | ShiftMask, XK_g, {spawn, "stream-video --save"} },
    {Mod4Mask, XK_s, {raiseOrRun, "steam"} },
    {Mod4Mask, XK_a, {raiseOrRun, "xreader"} },
    {ControlMask | Mod1Mask, XK_t, {raiseOrRun, "$TERMINAL"} },

    {0, XF86XK_AudioPlay, +[]() {toggleLayout(FULL);}},
    {DEFAULT_MOD_MASK, XF86XK_AudioPlay, {toggleMask,FULLSCREEN_MASK}},
    {DEFAULT_MOD_MASK | ShiftMask, XF86XK_AudioPlay, {toggleMask,ROOT_FULLSCREEN_MASK}},

    {Mod3Mask, XK_f, centerMouseInWindow},
    {Mod4Mask, XK_n, {cycleSlave, UP}},
    {Mod3Mask, XK_n, {cycleSlave, UP}},
    {Mod4Mask|ShiftMask, XK_n, {cycleSlave, DOWN}},
    {Mod3Mask|ShiftMask, XK_n, {cycleSlave, DOWN}},

    {Mod4Mask, XK_d, startMPX},
    {Mod4Mask | ControlMask, XK_d, saveMPXMasterInfo},
    {Mod4Mask | ControlMask | ShiftMask, XK_d, loadMPXMasterInfo},
    {Mod4Mask | ShiftMask, XK_d, stopMPX},

    {Mod4Mask, XK_x, +[](WindowInfo*winInfo) {cloneWindow(winInfo);} },
    {Mod4Mask | ShiftMask, XK_x, killAllClones},
    {Mod4Mask , XK_z, createContainer},

    {Mod4Mask , XK_grave, clean},

} ;
Chain* chainBindings[] = {
    new Chain{Mod4Mask | Mod1Mask, XK_d, {{startSplitMaster,"splitMaster",NO_PASSTHROUGH}},{
          {0,XK_Escape, endSplitMaster, {.noGrab=1}},
          {0,XK_Escape, endActiveChain, {.passThrough=NO_PASSTHROUGH, .noGrab=1}},
          {WILDCARD_MODIFIER,0, attachActiveSlaveToMarkedMaster, {.passThrough=NO_PASSTHROUGH, .noGrab=1}}
        },{.noKeyRepeat = 0},GLOBAL_CHAIN,"splitMasterChain"
    }
};

void addNormalBindings() {
    for(Binding& b : customBindings)
        getDeviceBindings().add(b);
    for(Chain* b : chainBindings)
        getDeviceBindings().add(b);
}
