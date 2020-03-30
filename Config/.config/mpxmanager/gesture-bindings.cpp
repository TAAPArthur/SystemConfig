#include <stdlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>

#include <mpxmanager/Extensions/gestures.h>
#include <mpxmanager/Extensions/gesture-functions.h>
#include <mpxmanager/bindings.h>
#include <mpxmanager/chain.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/logger.h>
#include <mpxmanager/system.h>
#include <mpxmanager/test-functions.h>
#include "config.h"

void addGesturesBindings() {
    GestureRegion topLeft = {0,0,33,20,5};
    GestureRegion topMiddle = {33,0,33,20,5};
    GestureRegion topRight = {33,0,33,20,5};

    GestureRegion left = {0,0,20,0};
    GestureRegion right = {-20,0,20,0};

    GestureRegion bottomLeft = {0,-20,33,20,5};
    GestureRegion bottomMiddle = {33,-20,33,20,5};
    GestureRegion bottomRight = {33,-20,33,20,5};
    getGestureRegions().addAll({topLeft, topMiddle,topRight,left,right,bottomLeft,bottomMiddle,bottomRight});

    getGestureBindings().addAll({
        new GestureBinding{{GESTURE_SOUTH, GESTURE_EAST}, {spawn, "xsane-xrandr rotate normal"}},
        new GestureBinding{{GESTURE_SOUTH, GESTURE_EAST}, {spawn, "xsane-xrandr rotate left"}, {.reflectionMask=Rotate90Mask, .replaceWithTransform=1}},
        new GestureBinding{{GESTURE_SOUTH, GESTURE_EAST}, {spawn, "xsane-xrandr rotate invert"}, {.reflectionMask=MirroredMask, .replaceWithTransform=1}},
        new GestureBinding{{GESTURE_SOUTH, GESTURE_EAST}, {spawn, "xsane-xrandr rotate right"}, {.reflectionMask=Rotate270Mask, .replaceWithTransform=1}},
    });
    GestureBinding gestureBindingsEmulateMouse[] = {
        {{GESTURE_TAP},[]{moveAndClick(1);}, {.count = 2}},
        {{GESTURE_TAP},[]{moveAndClick(1);}},
        {{GESTURE_NORTH},[]{moveAndClick(4);}, {.fingers=2, .mask =TouchMotionMask}},
        {{GESTURE_SOUTH},[]{moveAndClick(5);}, {.fingers=2, .mask =TouchMotionMask}},
        {{GESTURE_EAST}, []{moveAndClick(6);}, {.fingers=2, .mask =TouchMotionMask}},
        {{GESTURE_WEST}, []{moveAndClick(7);}, {.fingers=2, .mask =TouchMotionMask}},
    };
    GestureBinding gestureBindings[] = {
        {{GESTURE_NORTH},[]{swapPosition(UP);}, {.fingers = 3}},
        {{GESTURE_SOUTH},[]{swapPosition(DOWN);}, {.fingers = 3}},
        {{GESTURE_EAST},[]{shiftFocus(UP);}, {.fingers = 3}},
        {{GESTURE_WEST},[]{shiftFocus(DOWN);}, {.fingers = 3}},
        {{GESTURE_EAST},[]{shiftWorkspace(UP);}, {.fingers = 4}},
        {{GESTURE_WEST},[]{shiftWorkspace(DOWN);}, {.fingers = 4}},
    };

    for(GestureBinding& b : gestureBindings)
        getGestureBindings().add(new GestureBinding(b));
}


Chain* phoneBindings[] = {
    new Chain{0, XF86XK_PowerOff,{.onEnd={spawn, "xset s off"}}, {
        Chain{0, XF86XK_PowerOff,{.onEnd={spawn, "notify-send placeholder camera"}}, {
            {0, XF86XK_PowerOff,  {spawn, SUSPEND_CMD } },
            {0, XF86XK_AudioLowerVolume,  {spawn, REBOOT_CMD}},
            {0, XF86XK_AudioRaiseVolume,  {spawn, SHUTDOWN_CMD}},
            }},
        Chain{0, XF86XK_AudioLowerVolume,{.onEnd={spawn, "maim -i $_WIN_ID $HOME/Picture/Screenshots/$_WIN_CLASS-$(date +%s).png"}}, {
            {0, XF86XK_AudioRaiseVolume,  restart},
            {0, XF86XK_PowerOff,  clean },
        }},
        Chain{0, XF86XK_AudioRaiseVolume,{.onEnd={spawn, "xset s off"}}, {
            {0, XF86XK_AudioRaiseVolume, {raiseOrRun, "$TERMINAL"}},
            {0, XF86XK_AudioLowerVolume, +[]{return raiseOrRun("browser", "$BROWSER", MATCHES_ROLE);} },
        }},
    }},
};
void addGestureBindings() {
    for(Chain* b : phoneBindings)
        getDeviceBindings().add(b);
}
