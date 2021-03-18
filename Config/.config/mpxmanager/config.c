#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

#include <mpxmanager/Extensions/compatibility-rules.h>
#include <mpxmanager/Extensions/env-injector.h>
#include <mpxmanager/Extensions/containers.h>
#include <mpxmanager/Extensions/ewmh.h>
#include <mpxmanager/Extensions/extra-rules.h>
#include <mpxmanager/Extensions/mpx.h>
#include <mpxmanager/Extensions/session.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/boundfunction.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/globals.h>
#include <mpxmanager/layouts.h>
#include <mpxmanager/settings.h>
#include <mpxmanager/system.h>
#include <mpxmanager/util/logger.h>
#include <mpxmanager/windows.h>
#include <mpxmanager/wm-rules.h>
#include <mpxmanager/wmfunctions.h>
#include <mpxmanager/xutil/window-properties.h>

xcb_atom_t STEAM_GAME;
void loadCustomAtoms() {
    STEAM_GAME = getAtom("STEAM_GAME");
    INFO("Loading steam atom %d\n", STEAM_GAME);
}
bool isSteamGame(WindowID win) {
    return getWindowPropertyValueInt(win, STEAM_GAME, XCB_ATOM_CARDINAL);
}
void handleSteamGame(WindowInfo* winInfo) {
    if(isSteamGame(winInfo->id)) {
        addMask(winInfo, FULLSCREEN_MASK);
    }
}

void configureWindowsByName(WindowInfo* winInfo) {
    if(matchesTitle(winInfo, "mystatusbar")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK | X_CENTERED_MASK);
        removeMask(winInfo, ABOVE_MASK);
        setTilingOverrideEnabled(winInfo, 1 | 4 | 32);
        setTilingOverride(winInfo, (Rect) {-30, 0, 520, 0});
    }
    else if(matchesClass(winInfo, "dzen2")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK | BELOW_MASK);
        removeMask(winInfo, ABOVE_MASK);
        setTilingOverrideEnabled(winInfo, 3);
        lowerWindowInfo(winInfo, 0);
    }
    if(matchesClass(winInfo, "remote")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK | ABOVE_MASK | Y_MAXIMIZED_MASK);
        setTilingOverrideEnabled(winInfo, 3);
        moveToWorkspace(winInfo, getActiveWorkspaceIndex());
    }
    if(matchesClass(winInfo, "tabbed")) {
        addMask(winInfo, INPUT_MASK);
    }
    if(matchesClass(winInfo, "vimb")) {
        strcpy(winInfo->role, "browser");
        //if(winInfo->getCreationTime())
        //    winInfo->moveToWorkspace(3);
    }
    if(matchesClass(winInfo, "st-256color")) {
        strcpy(winInfo->role, "terminal");
        setWindowClass(winInfo->id, "st", "st");
        loadWindowProperties(winInfo);
    }
    if(!hasMask(winInfo, INPUT_MASK) && winInfo->type == ewmh->_NET_WM_WINDOW_TYPE_NORMAL) {
        addMask(winInfo, INPUT_MASK);
    }
    if(winInfo->dock) {
        removeMask(winInfo, ABOVE_MASK);
        setTilingOverrideEnabled(winInfo, 16);
    }
    if(winInfo->type == ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION)
        addMask(winInfo, ABOVE_MASK);
}

void onXConnectionStart() {
    loadCustomAtoms();
}

void addNormalBindings();
void loadSettings() {
    setLogLevel(LOG_LEVEL_DEBUG);
    DEFAULT_BORDER_COLOR = 0x00FF00;
    LD_PRELOAD_INJECTION = 1;
    ASSUME_PRIMARY_MONITOR = 1;
	onChildSpawn = setClientMasterEnvVar;
    // IDLE_TIMEOUT = 100;
    ALLOW_SETTING_UNSYNCED_MASKS = 1;
    spawnPipe("status-bar -title-name 'wm-status'", REDIRECT_CHILD_INPUT_ONLY);
    loadNormalSettings();
    addNormalBindings();
    setWorkspaceName(3, "VimB");
    setWorkspaceName(4, "St");
    setWorkspaceName(6, "7Gaming");
    addWorkspaceMask(getWorkspace(6), FLOATING_MASK);
    addEvent(X_CONNECTION, USER_EVENT(onXConnectionStart, HIGH_PRIORITY));
    /*
    getEventRules(CLIENT_MAP_ALLOW).add({
        +[](WindowInfo * winInfo) {
            if(matchesTitle(winInfo, "Steam - News"))
                winInfo->moveToWorkspace(8);
        }, "HideSteamNews"});
    */
    addEvent(CLIENT_MAP_ALLOW, USER_EVENT(handleSteamGame));
    addEvent(CLIENT_MAP_ALLOW, USER_EVENT(configureWindowsByName));
    //getEventRules(CLIENT_MAP_ALLOW).add([](WindowInfo*winInfo){addMask(winInfo,INPUT_MASK);}, HIGH_PRIORITY);
    //getEventRules(CLIENT_MAP_ALLOW).add(USER_EVENT_P(configureWindows, LOW_PRIORITY));
    addSuggestedRules();
    addInsertWindowsAtPositionRule(BEFORE_FOCUSED);
    // Extensions
    addAutoDetectDockPosition();
    addAutoMPXRules();
    addEWMHRules();
    addFloatRule();
    addMoveNonTileableWindowsToWorkspaceBounds();
    addPrintStatusRule();
    addResumeContainerRules();
    addResumeCustomStateRules();
    addStickyPrimaryMonitorRule();
}
