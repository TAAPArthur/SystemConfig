

#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <xcb/xinput.h>

#include <mpxmanager/bindings.h>
#include <mpxmanager/events.h>
#include <mpxmanager/default-rules.h>
#include <mpxmanager/test-functions.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/mywm-util.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/logger.h>
#include <mpxmanager/settings.h>
#include <mpxmanager/spawn.h>
#include <mpxmanager/windows.h>
#include <mpxmanager/wmfunctions.h>
#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/window-clone.h>
#include <mpxmanager/Extensions/xmousecontrol.h>


Binding customBindings[]={

    {0,XK_F6, RUN_OR_RAISE(CLASS,"arandr")},
    {Mod4Mask,XK_F6, RUN_OR_RAISE(CLASS,"arandr")},

    {Mod4Mask, XF86XK_MonBrightnessUp, BIND(spawn,"~/Documents/Keyboard_Commands/brightness .1")},
    {Mod4Mask |ShiftMask, XF86XK_MonBrightnessUp, BIND(spawn,"~/Documents/Keyboard_Commands/brightness -a 1")},
    {Mod4Mask, XF86XK_MonBrightnessDown, BIND(spawn,"~/Documents/Keyboard_Commands/brightness -.1")},
    {Mod4Mask |ShiftMask, XF86XK_MonBrightnessDown, BIND(spawn,"~/Documents/Keyboard_Commands/brightness -a 1")},
    {Mod4Mask | ControlMask , XF86XK_MonBrightnessDown, BIND(spawn,"xrandr-invert-colors")},
    {Mod4Mask | ControlMask , XF86XK_MonBrightnessUp, BIND(spawn,"xrandr-invert-colors")},
    {0 , XF86XK_Tools, BIND(spawn,"xrandr-invert-colors")},

    {0, XF86XK_AudioLowerVolume, BIND(spawn,"pamixer -d 1")},
    {0, XF86XK_AudioRaiseVolume, BIND(spawn,"pamixer -i 1")},
    {0, XF86XK_AudioMute, BIND(spawn,"pamixer -t")},
    {Mod4Mask, XF86XK_AudioLowerVolume, BIND(spawn,"omnivolctrl -1%")},
    {Mod4Mask, XF86XK_AudioRaiseVolume, BIND(spawn,"omnivolctrl +1%")},
    {Mod4Mask, XF86XK_AudioMute, BIND(spawn,"omnivolctrl sink-input-mute toggle")},
    {Mod4Mask, XF86XK_AudioPrev, BIND(spawn,"omnivolctrl sink-inputs")},
    {0, XF86XK_AudioPrev, BIND(spawn,"omnivolctrl sinks")},

    {Mod4Mask, XK_Menu, RUN_OR_RAISE(CLASS,"pavucontrol")},


    {Mod4Mask,XK_w, RUN_OR_RAISE(CLASS|ENV_VAR,"$BROWSER")},
    {Mod4Mask | ShiftMask ,XK_w, BIND(spawn,"$BROWSER --new-window")},
    {Mod4Mask | ControlMask,XK_w, RUN_OR_RAISE(CLASS,"google-chrome","google-chrome-stable --new-window --disk-cache-size=1000")},
    {Mod4Mask | ControlMask| ShiftMask,XK_w, BIND(spawn,"google-chrome-stable --new-window")},
    {Mod4Mask,XK_f, RUN_OR_RAISE(CLASS,"firefox")},
    {Mod4Mask | ShiftMask ,XK_f, BIND(spawn,"firefox --new-window")},

    {Mod4Mask,XK_v, RUN_OR_RAISE(CLASS,"vlc")},
    {Mod4Mask | ControlMask ,XK_v, RUN_OR_RAISE(CLASS,"virtualbox")},
    {Mod4Mask | ControlMask | Mod1Mask ,XK_v, RUN_OR_RAISE(CLASS,"pitivi")},
    {Mod4Mask | ControlMask | Mod1Mask ,XK_a, RUN_OR_RAISE(CLASS,"audacity")},
    {Mod4Mask | ControlMask,XK_g, RUN_OR_RAISE(CLASS,"gummi")},
    //{Mod4Mask ,XK_n, RUN_OR_RAISE(CLASS,"write_stylus")},
    {Mod4Mask | ControlMask,XK_d, RUN_OR_RAISE(CLASS,"dolphin-emu")},
    {Mod4Mask ,XK_s, RUN_OR_RAISE(CLASS,"steam")},
    {Mod4Mask,XK_a, RUN_OR_RAISE(CLASS,"atril")},
    {Mod4Mask,XK_g, RUN_OR_RAISE(CLASS,"gedit")},
    {Mod4Mask | ShiftMask,XK_g, BIND(spawn,"gedit -s")},
    {ControlMask | Mod1Mask,XK_t, RUN_OR_RAISE(CLASS|ENV_VAR,"$TERMINAL")},
    {ControlMask | Mod1Mask | ShiftMask,XK_t, BIND(spawn,"$TERMINAL")},


    {Mod4Mask,XK_p, BIND(spawn,"dmenu_run_history")},
    {Mod4Mask | ControlMask,XK_p, BIND(spawn,"dmenu_extended_run")},
    {Mod4Mask | ControlMask, XK_Menu, RUN_OR_RAISE(CLASS,"blueman-manager")},
    {0, XK_Print, BIND(spawn,"xfce4-screenshooter -r")},
    {Mod4Mask, XK_Print, BIND(spawn,"xfce4-screenshooter -w")},
    {Mod4Mask|ShiftMask, XK_Print, BIND(spawn,"xfce4-screenshooter")},

    {Mod4Mask | ControlMask ,XK_Print, RUN_OR_RAISE(CLASS,"simplescreenrecorder")},
    {0, XF86XK_Eject, BIND(spawn,"dmenu-pycalc")},
    {0, XF86XK_Calculator, BIND(spawn,"dmenu-pycalc")},
    {Mod4Mask,XK_Delete, RUN_OR_RAISE(TITLE|ENV_VAR,"gt5","$TERMINAL -e 'gt5 /' -T gt5")},
    {Mod4Mask|ShiftMask,XK_Delete, RUN_OR_RAISE(TITLE|ENV_VAR,"gt5","$TERMINAL -e 'gt5 ~' -T gt5")},

    {ControlMask | Mod1Mask , XK_Delete, RUN_OR_RAISE(CLASS,"xfce4-taskmanager")},
    {Mod4Mask | ControlMask | Mod1Mask  , XK_l, BIND(spawn,"xtrlock"),.noKeyRepeat=1},
    {Mod4Mask, XK_Escape, BIND(spawn,"xtrlock"),.noKeyRepeat=1},
    {ControlMask | Mod1Mask, XK_Escape, BIND(spawn,"systemctl suspend"),.noKeyRepeat=1},
    {Mod4Mask | ControlMask | Mod1Mask, XK_Escape, BIND(spawn,"notify-send 'System will shutdown in 1 min'; shutdown 1"),.noKeyRepeat=1},

    {Mod4Mask ,XK_d, BIND(startMPX)},
    {Mod4Mask | ControlMask | Mod1Mask ,XK_d, BIND(saveMasterInfo)},
    {Mod4Mask | ControlMask | Mod1Mask|ShiftMask ,XK_d, BIND(loadMasterInfo)},
    {Mod4Mask | ControlMask | Mod1Mask ,XK_e, BIND(splitMaster)},
    {Mod4Mask | ShiftMask,XK_d, BIND(stopMPX)},

    {Mod4Mask ,XK_x, BIND((int(*)(WindowInfo*))cloneWindow)},
    {Mod4Mask |ShiftMask,XK_x, BIND(killAllClones)},


    {Mod4Mask ,XK_grave, BIND(resetUserMask)},
    {Mod4Mask|Mod3Mask, 	XK_Up, BIND(clickButton,	 SCROLL_UP)},
    {Mod4Mask|Mod3Mask, 	XK_Down, BIND(clickButton,	 SCROLL_DOWN)},
    {Mod4Mask|Mod3Mask, 	XK_Left, BIND(clickButton,	 SCROLL_LEFT)},
    {Mod4Mask|Mod3Mask, 	XK_Right, BIND(clickButton,	 SCROLL_RIGHT)},

};

char *floatTitles[]={"Preferences"};
static Rule raiseTrayerRule={NULL,0,
    BOTH(
        BIND(findAndRaiseLazy,(&((Rule){"trayer",CLASS|RESOURCE,NULL}))),
        BIND(findAndLowerLazy,(&((Rule){"dzen2",CLASS|RESOURCE,NULL}))),
    )
    ,.filterMatch=BIND(isDock)
};

void loadSettings(){
    SHELL=getenv("SHELL");
    DEFAULT_WINDOW_MASKS|=INPUT_MASK;
    //spawnPipe("xmobar -d --top -F green --font='xft:Bitstream Vera Sans Mono:size=9:antialias=true' -t '%StdinReader%'");
    spawnPipe("dzen2 -ta l -fg '#00FF00' -bg '#000000' -p -dock -fn 'xft:Bitstream Vera Sans Mono:size=10:antialias=true' -h 20 ");

    //static Rule steamRule={"Steam",MATCH_ANY_LITERAL,BIND(returnTrue),.passThrough=NO_PASSTHROUGH,.filterMatch=NBIND(hasMask,INPUT_MASK)};
    //addToList(getEventRules(ProcessingWindow),&steamRule);
    static Rule steamRule={"Steam",MATCH_ANY_LITERAL,BIND(floatWindow),.filterMatch=NBIND(hasMask,INPUT_MASK)};
    addToList(getEventRules(PropertyLoad),&steamRule);

    static Rule steamNewsRule={"Steam - News.*",TITLE,BIND(moveWindowToWorkspace,-1)};
    addToList(getEventRules(PropertyLoad),&steamNewsRule);
    static Rule floatRule={".*(Preferences|Properties).*",MATCH_ANY_REGEX,BIND(floatWindow)};
    addToList(getEventRules(ProcessingWindow),&floatRule);
    static Rule monitorChangeRule={.onMatch=BIND(spawn,"conky-install.sh;nitrogen --restore 2>/dev/null")};
    addToList(getBatchEventRules(onScreenChange),&monitorChangeRule);
    addToList(getEventRules(onWindowMove),&raiseTrayerRule);

    loadNormalSettings();
    enableXMouseControl();

    addAutoMPXRules();
    addNoDockFocusRule();
    addCloneRules();
    addBindings(customBindings,LEN(customBindings));
    startAutoUpdatingClones();
    runInNewThread(runXMouseControl,NULL);
}
