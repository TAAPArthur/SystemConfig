#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/session.h>
#include <mpxmanager/Extensions/window-clone.h>
#include <mpxmanager/Extensions/xmousecontrol.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/debug.h>
#include <mpxmanager/device-grab.h>
#include <mpxmanager/extra-rules.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/logger.h>
#include <mpxmanager/settings.h>
#include <mpxmanager/system.h>
#include <mpxmanager/test-functions.h>
#include <mpxmanager/windows.h>
#include <mpxmanager/wm-rules.h>
#include <mpxmanager/chain.h>
#include <mpxmanager/wmfunctions.h>


Binding customBindings[] = {
    {0, XK_F6, {raiseOrRun, "arandr"} },
    {Mod4Mask, XK_F6, {raiseOrRun, "arandr"} },

    {Mod4Mask, XF86XK_MonBrightnessUp, {spawn, "~/Documents/Keyboard_Commands/brightness .1"} },
    {Mod4Mask | ShiftMask, XF86XK_MonBrightnessUp, {spawn, "~/Documents/Keyboard_Commands/brightness -a 1"} },
    {Mod4Mask, XF86XK_MonBrightnessDown, {spawn, "~/Documents/Keyboard_Commands/brightness -.1"} },
    {Mod4Mask | ShiftMask, XF86XK_MonBrightnessDown, {spawn, "~/Documents/Keyboard_Commands/brightness -a 1"} },
    {Mod4Mask | ControlMask, XF86XK_MonBrightnessDown, {spawn, "xrandr-invert-colors"} },
    {Mod4Mask | ControlMask, XF86XK_MonBrightnessUp, {spawn, "xrandr-invert-colors"} },
    {0, XF86XK_Tools, {spawn, "xrandr-invert-colors"} },

    {Mod4Mask, XK_v, {spawn, "clip-history select -w $_WIN_ID && xsel --clipboard | xvkbd -window $_WIN_ID -file - 2>/dev/null"} },

    {Mod4Mask, XK_p, {spawn, "dmenu_run_history"} },
    {Mod4Mask | ControlMask, XK_p, {spawn, "dmenu_extended_run"} },
    {Mod4Mask | ControlMask, XK_Menu, {raiseOrRun, "blueman-manager"} },

    {0, XK_Print, {spawn, "maim -s | xclip -selection clipboard -t image/png"} },
    {ShiftMask, XK_Print, {spawn, "maim -s /tmp/$(date +%s).png"} },
    {Mod4Mask, XK_Print, {spawn, "maim -i $_WIN_ID| xclip -selection clipboard -t image/png"} },
    {Mod4Mask | ShiftMask, XK_Print, {spawn, "maim -i | xclip -selection clipboard -t image/png"} },

    {0, XF86XK_Eject, {spawn, "dmenu-pycalc"} },
    {0, XF86XK_Calculator, {spawn, "dmenu-pycalc"} },
    {Mod4Mask, XK_Delete, +[] {raiseOrRun("gt5", "$TERMINAL -e 'gt5 /' -T gt5", 0);} },
    {Mod4Mask| ShiftMask, XK_Delete, +[] {raiseOrRun("gt5", "$TERMINAL -e 'gt5 ~' -T gt5", 0);} },

    {ControlMask | Mod1Mask, XK_Delete, {raiseOrRun, "notify-send -t 20000  CPU \"$(ps -Ao pid,pcpu,pmem,comm --sort=-pcpu --cols=27 | head -n 6)\" && notify-send -t 20000  MEM \"$(ps -Ao pid,pcpu,pmem,comm --sort=-pmem --cols=27 | head -n 6)\" "},{ .noKeyRepeat = 1}},

    {Mod4Mask, XK_Escape, {spawn, "xtrlock"}, {.noKeyRepeat = 1} },
    {ControlMask | Mod1Mask, XK_Escape, {spawn, "systemctl suspend"}, {.noKeyRepeat = 1} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_Escape, {spawn, "notify-send 'System will shutdown in 1 min'; shutdown 1"}, {.noKeyRepeat = 1} },

    {0, XF86XK_AudioLowerVolume, {spawn, "pamixer -d 1"} },
    {0, XF86XK_AudioRaiseVolume, {spawn, "pamixer -i 1"} },
    {0, XF86XK_AudioMute, {spawn, "pamixer -t"} },
    {Mod4Mask, XF86XK_AudioLowerVolume, {spawn, "omnivolctrl -1%"} },
    {Mod4Mask, XF86XK_AudioRaiseVolume, {spawn, "omnivolctrl +1%"} },
    {Mod4Mask, XF86XK_AudioMute, {spawn, "omnivolctrl sink-input-mute toggle"} },
    {Mod4Mask, XF86XK_AudioPrev, {spawn, "omnivolctrl sink-inputs"} },
    {0, XF86XK_AudioPrev, {spawn, "omnivolctrl sinks"} },

    {Mod4Mask, XK_Menu, {raiseOrRun, "pavucontrol"} },

    {Mod4Mask, XK_w, {raiseOrRun, "$BROWSER"} },
    {Mod4Mask | ShiftMask, XK_w, {spawn, "$BROWSER_ALT"} },
    {Mod4Mask | ControlMask, XK_w, +[] {raiseOrRun("google-chrome", "google-chrome-stable --new-window --disk-cache-size=1000");} },
    {Mod4Mask | ControlMask | ShiftMask, XK_w, {spawn, "google-chrome-stable --new-window"} },
    {Mod4Mask, XK_f, {raiseOrRun, "firefox"} },
    {Mod4Mask | ShiftMask, XK_f, {spawn, "firefox --new-window"} },

    {Mod4Mask | ControlMask, XK_v, {raiseOrRun, "vlc"} },
    {Mod4Mask, XK_m, {raiseOrRun, "virtualbox"} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_v, {raiseOrRun, "pitivi"} },
    {Mod4Mask | ControlMask | Mod1Mask, XK_a, {raiseOrRun, "audacity"} },
    {Mod4Mask | ControlMask, XK_g, {raiseOrRun, "gummi"} },
    {Mod4Mask | ControlMask, XK_d, {raiseOrRun, "dolphin-emu"} },
    {Mod4Mask, XK_s, {raiseOrRun, "steam"} },
    {Mod4Mask, XK_a, {raiseOrRun, "xreader"} },
    {ControlMask | Mod1Mask, XK_t, {raiseOrRun, "$TERMINAL"} },
    {ControlMask | Mod1Mask | ShiftMask, XK_t, {spawn, "$TERMINAL"} },

    {Mod4Mask, XK_d, startMPX},
    {Mod4Mask | ControlMask | Mod1Mask, XK_d, saveMPXMasterInfo},
    {Mod4Mask | ControlMask | Mod1Mask | ShiftMask, XK_d, loadMPXMasterInfo},
    {Mod4Mask | ShiftMask, XK_d, stopMPX},

    {Mod4Mask, XK_x, +[](WindowInfo*winInfo) {cloneWindow(winInfo);} },
    {Mod4Mask | ShiftMask, XK_x, killAllClones},


    {Mod4Mask | Mod3Mask, 	XK_Up, {clickButton, 	 SCROLL_UP} },
    {Mod4Mask | Mod3Mask, 	XK_Down, {clickButton, 	 SCROLL_DOWN} },
    {Mod4Mask | Mod3Mask, 	XK_Left, {clickButton, 	 SCROLL_LEFT} },
    {Mod4Mask | Mod3Mask, 	XK_Right, {clickButton,	 SCROLL_RIGHT} },

} ;
Chain chainBindings[] = {
    {Mod4Mask | ControlMask | Mod1Mask, XK_e, startSplitMaster,{
          {0,XK_Escape, endSplitMaster, {.noGrab=1}},
          {0,XK_Escape, endActiveChain, {.passThrough=NO_PASSTHROUGH, .noGrab=1}},
          {WILDCARD_MODIFIER,0, attachActiveSlaveToMarkedMaster, {.passThrough=NO_PASSTHROUGH, .noGrab=1}}
        },{},0,"splitMaster"
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
    xcb_get_property_cookie_t cookie;
    xcb_get_property_reply_t* reply;
    bool result = 0;
    LOG(LOG_LEVEL_INFO, "Checking Steam %d\n", STEAM_GAME);
    dumpAtoms(&STEAM_GAME, 1);
    cookie = xcb_get_property(dis, 0, win, STEAM_GAME, XCB_ATOM_CARDINAL, 0, sizeof(int));
    if((reply = xcb_get_property_reply(dis, cookie, NULL)) && xcb_get_property_value_length(reply)) {
        result = 1;
        LOG(LOG_LEVEL_INFO, "Found steam game\n\n");
        free(reply);
    }
    return result;
}
void handleSteamGame(WindowInfo*winInfo) {
    if(isSteamGame(winInfo->getID())) {
        winInfo->addMask(FULLSCREEN_MASK );
        std::cout<<*winInfo<<"\n";
    }
}

void loadSettings() {
    setLogLevel(LOG_LEVEL_DEBUG);
    LD_PRELOAD_INJECTION = 1;
    SHELL = getenv("SHELL");
    spawnPipe("dzen2 -ta l -fg '#00FF00' -bg '#000000' -dock -fn 'xft:Bitstream Vera Sans Mono:size = 10:antialias = true' -h 20 -e ''");

    // static Rule steamRule = {"Steam", MATCH_ANY_LITERAL, BIND(returnTrue), .passThrough = NO_PASSTHROUGH, .filterMatch = NBIND(hasMask, INPUT_MASK)} ;
    // addToList(getEventRules(ProcessingWindow), &steamRule);
    /*
    static Rule steamRule = {"Steam", MATCH_ANY_LITERAL, BIND(floatWindow), .filterMatch = NBIND(hasMask, INPUT_MASK)} ;
    addToList(getEventRules(PropertyLoad), &steamRule);
    static Rule steamNewsRule = {"Steam - News.*", TITLE, BIND(moveWindowToWorkspace, -1)} ;
    addToList(getEventRules(PropertyLoad), &steamNewsRule);
    */

    //static Rule floatRule = {".*(Preferences|Properties).*", MATCH_ANY_REGEX, BIND(floatWindow)} ;
    //addToList(getEventRules(ClientMapAllow), );


    getWorkspace(7)->setName("7Gaming");
    getWorkspace(7)->addMask(FLOATING_MASK);

    getBatchEventRules(onScreenChange).add( {spawn, "conky-install.sh;nitrogen --restore 2>/dev/null"} );

    getEventRules(ClientMapAllow).add( USER_EVENT(handleSteamGame));
    getEventRules(PreRegisterWindow).add(USER_EVENT(addFocusMask));
    // getEventRules(onWindowMove).add(USER_EVENT(raiseTrayer));
    getEventRules(ClientMapAllow).add({+[](WindowInfo*winInfo) {if(matchesClass(winInfo, "dzen2"))winInfo->addMask(ALWAYS_ON_BOTTOM);},"AOB dzen2"});
    getEventRules(ClientMapAllow).add({+[](WindowInfo*winInfo) {if(winInfo->getType()==ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION)winInfo->addMask(ALWAYS_ON_TOP);},"AOT notifications"} );

    loadNormalSettings();
    for(Binding&b:customBindings)
        getDeviceBindings().add(b);
    for(Binding&b:chainBindings)
        getDeviceBindings().add(b);
    addDefaultXMouseControlBindings();
    addKeepTransientsOnTopRule();
    addDieOnIntegrityCheckFailRule();
    addResumeCustomStateRules();
    addAutoMPXRules();
    addNoDockFocusRule();
    addIgnoreSmallWindowRule();
     addIgnoreOverrideRedirectWindowsRule(ADD_REMOVE);
    addCloneRules();
    startAutoUpdatingClones();
    addStartXMouseControlRule();
    getEventRules(onXConnection).add( {loadCustomAtoms, "loadCustomAtoms"}, PREPEND_UNIQUE );
    /*
    getEventRules(ClientMapAllow).add(+[](WindowInfo*winInfo) {
        if(matchesClass(winInfo, "google-chrome"))
            passiveGrab(winInfo->getID(), NON_ROOT_DEVICE_EVENT_MASKS|XCB_INPUT_XI_EVENT_MASK_BUTTON_PRESS);
    } );
    */
}
__attribute__((constructor)) static void _onStart() {
}
