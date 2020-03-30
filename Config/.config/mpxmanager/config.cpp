#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

#include <mpxmanager/Extensions/gestures.h>
#include <mpxmanager/Extensions/gesture-functions.h>
#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/session.h>
#include <mpxmanager/Extensions/window-clone.h>
#include <mpxmanager/Extensions/xmousecontrol.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/boundfunction.h>
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

bool TOUCH_SCREEN_AS_MOUSE_NATIVE;
bool NO_GESTURES;
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

xcb_atom_t STEAM_GAME;
void loadCustomAtoms() {
    STEAM_GAME = getAtom("STEAM_GAME");
    LOG(LOG_LEVEL_INFO, "Loading steam atom %d\n", STEAM_GAME);
}
bool isSteamGame(WindowID win) {
    return getWindowPropertyValue(win, STEAM_GAME, XCB_ATOM_CARDINAL);
}
void handleSteamGame(WindowInfo*winInfo) {
    if(isSteamGame(winInfo->getID())) {
        winInfo->addMask(FULLSCREEN_MASK );
    }
}

void configureWindowsByName(WindowInfo*winInfo) {
    if(matchesClass(winInfo, "dzen2")){
        winInfo->addMask(PRIMARY_MONITOR_MASK|BELOW_MASK);
        winInfo->removeMask(ABOVE_MASK);
        winInfo->setTilingOverrideEnabled(3);
        lowerWindow(winInfo);
    }
    if(matchesClass(winInfo, "remote.py")){
        winInfo->addMask(PRIMARY_MONITOR_MASK|ABOVE_MASK|Y_MAXIMIZED_MASK);
        winInfo->setTilingOverrideEnabled(3);
        winInfo->moveToWorkspace(getActiveWorkspaceIndex());
    }
    if(matchesClass(winInfo, "Onboard")){
        winInfo->addMask(ABOVE_MASK);
    }
    if(matchesClass(winInfo, "conky")){
        winInfo->removeMask(INPUT_MASK);
        winInfo->addMask(PRIMARY_MONITOR_MASK|X_CENTERED_MASK);
    }
    if(matchesClass(winInfo, "tabbed")){
        winInfo->addMask(INPUT_MASK);
    }
}

void onXConnectionStart() {
    loadCustomAtoms();
    //enableGestures();
}

void addNormalBindings();
void addGestureBindings();
void loadSettings() {

    LD_PRELOAD_INJECTION = 1;
    ALLOW_SETTING_UNSYNCED_MASKS = 1;
    setLogLevel(LOG_LEVEL_DEBUG);
    spawnPipe("dzen2 -ta l -fg '#00FF00' -bg '#000000' -dock -fn 'xft:Bitstream Vera Sans Mono:size = 10:antialias = true' -h 20 -e ''");

    loadNormalSettings();
    addNormalBindings();
    addGestureBindings();

    getWorkspace(8)->setName("7Gaming");
    getWorkspace(8)->addMask(FLOATING_MASK);

    //getBatchEventRules(SCREEN_CHANGE).add( {spawn, "conky-install.sh"} );

    getEventRules(X_CONNECTION).add(USER_EVENT_P(onXConnectionStart, HIGHEST_PRIORITY));
    getEventRules(CLIENT_MAP_ALLOW).add( {
        +[](WindowInfo*winInfo){
            if(matchesTitle(winInfo,"Steam - News"))
                winInfo->moveToWorkspace(8);
    },"HideSteamNews"});
    getEventRules(CLIENT_MAP_ALLOW).add(USER_EVENT(handleSteamGame));
    getEventRules(CLIENT_MAP_ALLOW).add(USER_EVENT(configureWindowsByName));
    getEventRules(CLIENT_MAP_ALLOW).add({+[](WindowInfo*winInfo) {if(winInfo->getType()==ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION)winInfo->addMask(ABOVE_MASK);},"AOT notifications"} );


    addKeepTransientsOnTopRule();

    // Extensions
    addResumeCustomStateRules();
    addAutoMPXRules();
    addCloneRules();
    startAutoUpdatingClones();
    addDefaultXMouseControlBindings();
    addStartXMouseControlRule();

    addAllDebugRules();

}
