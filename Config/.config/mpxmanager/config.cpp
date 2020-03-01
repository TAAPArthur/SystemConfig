#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

//#include <mpxmanager/Extensions/gestures.h>
//#include <mpxmanager/Extensions/gesture-functions.h>
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

void clean() {
    for(WindowInfo*winInfo:getAllWindows()) {
        if(winInfo->isDock())
            winInfo->removeFromWorkspace();
        if(!winInfo->getWorkspace()){
            if(winInfo->hasMask(MAPPABLE_MASK))
                mapWindow(winInfo->getID());
            removeBorder(*winInfo);
        }
    }
}

/*
GestureBinding gestureBindings[] = {
    {{GESTURE_TAP},[]{moveAndClick(1);}},
    {{GESTURE_TAP},[]{moveAndClick(1);}, {.count = 2}},
    {{GESTURE_NORTH},[]{moveAndClick(4);}, {.mask =TouchMotionMask}},
    {{GESTURE_SOUTH},[]{moveAndClick(5);}, {.mask =TouchMotionMask}},
    {{GESTURE_EAST},[]{moveAndClick(6);}, {.mask =TouchMotionMask}},
    {{GESTURE_WEST},[]{moveAndClick(7);}, {.mask =TouchMotionMask}},
    {{GESTURE_EAST},[]{shiftFocus(UP);}, {.fingers = 2}},
    {{GESTURE_WEST},[]{shiftFocus(DOWN);}, {.fingers = 2}},
};
*/

Binding phoneBindings[] = {
    {0, XF86XK_PowerOff, {spawn, "systemctl-suspend"} },
};

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
    {ControlMask | Mod1Mask, XK_Escape, {spawn, "systemctl suspend"}, {.noKeyRepeat = 1} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_Escape, {spawn, "notify-send 'System will shutdown in 1 min'; shutdown 1"}, {.noKeyRepeat = 1} },


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

    {Mod4Mask , XK_grave, clean},


} ;
Chain* chainBindings[] = {
    new Chain{Mod4Mask | Mod1Mask, XK_d, {startSplitMaster,"splitMaster",NO_PASSTHROUGH},{
          {0,XK_Escape, endSplitMaster, {.noGrab=1}},
          {0,XK_Escape, endActiveChain, {.passThrough=NO_PASSTHROUGH, .noGrab=1}},
          {WILDCARD_MODIFIER,0, attachActiveSlaveToMarkedMaster, {.passThrough=NO_PASSTHROUGH, .noGrab=1}}
        },{.noKeyRepeat = 0},GLOBAL_CHAIN,"splitMasterChain"
    }
};

void addFocusMask(WindowInfo*winInfo) {
    if(!winInfo->isInputOnly()&&!winInfo->hasPartOfMask(INPUT_MASK|WM_TAKE_FOCUS_MASK))
        winInfo->addMask(INPUT_MASK);
}

xcb_atom_t STEAM_GAME;
void loadCustomAtoms() {
    STEAM_GAME = getAtom("STEAM_GAME");
    LOG(LOG_LEVEL_INFO, "Loading steam atom %d\n", STEAM_GAME);
}
bool isSteamGame(WindowID win) {
    return getWindowPropertyValue(win, STEAM_GAME, XCB_ATOM_CARDINAL);
}
void handleSteamGame(WindowInfo*winInfo) { if(isSteamGame(winInfo->getID())) {
        winInfo->addMask(FULLSCREEN_MASK );
    }
}

void configureWindowsByName(WindowInfo*winInfo) {
    if(matchesClass(winInfo, "dzen2")){
        winInfo->addMask(PRIMARY_MONITOR_MASK|ALWAYS_ON_BOTTOM_MASK);
        winInfo->setTilingOverrideEnabled(3);
    }
    if(matchesClass(winInfo, "Onboard")){
        winInfo->addMask(ALWAYS_ON_TOP_MASK);
    }
}

void loadSettings() {
    LD_PRELOAD_INJECTION = 1;
    setLogLevel(LOG_LEVEL_DEBUG);
    SHELL = getenv("SHELL");
    spawnPipe("dzen2 -ta l -fg '#00FF00' -bg '#000000' -dock -fn 'xft:Bitstream Vera Sans Mono:size = 10:antialias = true' -h 20 -e ''");

    loadNormalSettings();
    for(Binding& b : customBindings)
        getDeviceBindings().add(b);
    for(Chain* b : chainBindings)
        getDeviceBindings().add(b);


    getWorkspace(8)->setName("7Gaming");
    getWorkspace(8)->addMask(FLOATING_MASK);

    getBatchEventRules(SCREEN_CHANGE).add( {spawn, "conky-install.sh;nitrogen --restore 2>/dev/null"} );

    getEventRules(X_CONNECTION).add( {loadCustomAtoms, "loadCustomAtoms"}, PREPEND_UNIQUE );
    getEventRules(PRE_REGISTER_WINDOW).add(USER_EVENT(addFocusMask));
    getEventRules(CLIENT_MAP_ALLOW).add( {
        +[](WindowInfo*winInfo){
            if(matchesTitle(winInfo,"Steam - News"))
                winInfo->moveToWorkspace(8);
    },"HideSteamNews"});
    getEventRules(CLIENT_MAP_ALLOW).add(USER_EVENT(handleSteamGame));
    getEventRules(CLIENT_MAP_ALLOW).add(USER_EVENT(configureWindowsByName));
    getEventRules(CLIENT_MAP_ALLOW).add({+[](WindowInfo*winInfo) {if(winInfo->getType()==ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION)winInfo->addMask(ALWAYS_ON_TOP_MASK);},"AOT notifications"} );


    addKeepTransientsOnTopRule();
    addResumeCustomStateRules();
    addNoDockFocusRule();
    addIgnoreSmallWindowRule();
    //addIgnoreOverrideRedirectWindowsRule(ADD_REMOVE);


    for(Binding& b : phoneBindings)
        getDeviceBindings().add(b);


    // Extensions
    addAutoMPXRules();
    addCloneRules();
    startAutoUpdatingClones();
    addDefaultXMouseControlBindings();
    addStartXMouseControlRule();
    /*
    for(GestureBinding& b : gestureBindings)
        getGestureBindings().add(new GestureBinding(b));
    getEventRules(X_CONNECTION).add([]{initGestureMaster();enableGestures();});
    */

    addAllDebugRules();
}
__attribute__((constructor)) static void _onStart() {
}
