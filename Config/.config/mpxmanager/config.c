#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <stdlib.h>

#include <mpxmanager/Extensions/compatibility-rules.h>
#include <mpxmanager/Extensions/containers.h>
#include <mpxmanager/Extensions/env-injector.h>
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

#define MEDIA_PLAYER "media_player"

void updateActiveMode(WindowInfo* winInfo) {
    if (getFocusedWindow() && getFocusedWindow()->role && strcmp(getFocusedWindow()->role, MEDIA_PLAYER) == 0) {
        addActiveMode(KEYBOARD_PASSTHROUGH_MODE);
    } else {
        removeActiveMode(KEYBOARD_PASSTHROUGH_MODE);
    }

}
void configureWindowsByName(WindowInfo* winInfo) {
    if (matchesTitle(winInfo, "mystatusbar")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK);
        removeMask(winInfo, ABOVE_MASK);
        setTilingOverrideEnabled(winInfo, (1 | 4 | 16 ) );
        setTilingOverride(winInfo, (Rect) {-500, 0, 500, 0});
    }
    else if (matchesClass(winInfo, "dzen2")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK | BELOW_MASK);
        removeMask(winInfo, ABOVE_MASK);
        setTilingOverrideEnabled(winInfo, 3);
        lowerWindowInfo(winInfo, 0);
    }

    if (matchesClass(winInfo, "remote")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK | ABOVE_MASK | Y_MAXIMIZED_MASK);
        setTilingOverrideEnabled(winInfo, 3);
        moveToWorkspace(winInfo, getActiveWorkspaceIndex());
    }
    if (matchesClass(winInfo, "tabbed")) {
        addMask(winInfo, INPUT_MASK);
    }
    if (matchesClass(winInfo, "vimb") || matchesClass(winInfo, "firefox") || matchesClass(winInfo, "browser")) {
        strcpy(winInfo->role, "browser");
    }
    else if (matchesClass(winInfo, "mpv") || matchesClass(winInfo, "div") || matchesClass(winInfo, "mupdf") || matchesClass(winInfo, "Zathura")) {
        strcpy(winInfo->role, MEDIA_PLAYER);
    }
    else if (matchesClass(winInfo, "st-256color")) {
        strcpy(winInfo->role, "terminal");
        setWindowClass(winInfo->id, "st", "st");
        loadWindowProperties(winInfo);
    }
    if (!hasMask(winInfo, INPUT_MASK) && winInfo->type == ewmh->_NET_WM_WINDOW_TYPE_NORMAL) {
        addMask(winInfo, INPUT_MASK);
    }

    if (winInfo->transientFor && winInfo->transientFor != root) {
        addMask(winInfo, X_CENTERED_MASK|Y_CENTERED_MASK);
        setTilingOverrideEnabled(winInfo, 1 | 2 | 32 | 64);
        setTilingOverride(winInfo, (Rect) {-winInfo->geometry.width/2, -winInfo->geometry.height/2});
    }

    if (matchesClass(winInfo, "navboard")) {
        addMask(winInfo, PRIMARY_MONITOR_MASK);
    }

    if (hasMask(winInfo, DOCK_MASK)) {
        removeMask(winInfo, ABOVE_MASK);
        setTilingOverrideEnabled(winInfo, 16);
    }
    if (winInfo->type == ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION)
        addMask(winInfo, ABOVE_MASK);
}

void addNormalBindings();
void loadSettings() {
    SET_FROM_ENV(HIDE_WM_STATUS);
    DEFAULT_BORDER_COLOR = 0x00FF00;
    ASSUME_PRIMARY_MONITOR = 1;
	onChildSpawn = setClientMasterEnvVar;
    // IDLE_TIMEOUT = 100;
    ALLOW_SETTING_UNSYNCED_MASKS = 1;
    spawnPipe("status-bar -title-name 'wm-status'", REDIRECT_CHILD_INPUT_ONLY);
    loadNormalSettings();
    addNormalBindings();
    addWorkspaceMask(getWorkspace(6), FLOATING_MASK);
    addEvent(CLIENT_MAP_ALLOW, USER_EVENT(configureWindowsByName, LOWEST_PRIORITY));
    addEvent(WINDOW_FOCUS, USER_EVENT(updateActiveMode));

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
    addContainerRules();
    addResumeCustomStateRules();
    addStickyPrimaryMonitorRule();
}
