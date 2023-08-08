#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <sys/poll.h>

#include <mpxmanager/bindings.h>
#include <mpxmanager/functions.h>
#include <mpxmanager/util/logger.h>
#include <mpxmanager/system.h>
#include <mpxmanager/layouts.h>
#include <mpxmanager/xevent.h>
#include <mpxmanager/xutil/test-functions.h>
#include <sgestures/event.h>

#include "config.h"


/**
 * Used to sub-divide a gesture device
 * The GestureRegion will be apart of the GestureID thus events starting in different regions
 * are independent of each other
 */
typedef struct GestureRegion {
    Rect rect;
    char percent;
} GestureRegion;
enum {
    DEFAULT_REGION,

    TOP_LEFT, TOP_MIDDLE, TOP_RIGHT,
    LEFT, RIGHT,
    BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT,
};
GestureRegion gestureRegions[] = {
    {{0,     0, 100, 100}, 0b1100},
    {{0,     0,  33,  20},  5},
    {{33,    0,  33,  20},  5},
    {{33,    0,  33,  20},  5},
    {{0,     0,  20,   0}    },
    {{-20,   0,  20,   0}    },
    {{0,   -20,  33,  20},  5},
    {{33,  -20,  33,  20},  5},
    {{33,  -20,  33,  20},  5},
};

GesturePoint getXPoint(GesturePoint point) {
    return (GesturePoint) {point.x* getRootWidth() / 100, point.y* getRootHeight() / 100};
}
static GestureEvent* currentEvent;

void moveAndClick(int button) {
    GesturePoint point = getXPoint(currentEvent->endPixelPoint);
    movePointer(point.x, point.y);
    clickButton(button, getActiveMasterPointerID());
}
void clickProportional(int button) {
    double ratio = currentEvent->flags.avgSqDisplacement / (currentEvent->flags.duration / 100.0 + .01);
    for(int i = 0; i <= ratio; i++)
        clickButton(button, getActiveMasterPointerID());
}

ProductID generateIDHighBits(ProductID id __attribute__((unused)), GesturePoint startingGesturePoint) {
    Rect base = {0, 0, getRootWidth(), getRootHeight()};
    GesturePoint  point = getXPoint(startingGesturePoint);
    Rect startingPoint = {point.x * getRootWidth() / 100, point.y * getRootHeight() / 100, 0, 0};
    for(int i = LEN(gestureRegions) - 1; i > 0; --i)
        if(contains(getRelativeRegion(base, gestureRegions[i].rect, gestureRegions[i].percent), startingPoint))
            return i + 1;
    return 0;
}

Binding phoneBindings[] = {
    {
        0, XF86XK_PowerOff, .chainMembers = CHAIN_MEM(
        {
            0, XF86XK_PowerOff, .chainMembers = CHAIN_MEM(
            {0, XF86XK_PowerOff,  {spawn,  .arg = {.str = SUSPEND_CMD} } },
            {0, XF86XK_AudioLowerVolume,  {spawn,  .arg = {.str = REBOOT_CMD}}},
            {0, XF86XK_AudioRaiseVolume,  {spawn,  .arg = {.str = SHUTDOWN_CMD}}},
            )},
        { 0, XF86XK_AudioLowerVolume, {spawn,  .arg = {.str = "maim -i $_WIN_ID $HOME/Picture/Screenshots/$_WIN_CLASS-$(date +%s).png"}}},
        { 0, XF86XK_AudioRaiseVolume, {raiseOrRun,  .arg = {.str = "$TERMINAL"}}}
        )
    },
};

typedef struct {
    BindingFunc func;
    GestureBindingArg arg;
} GestureBinding ;

GestureBinding gestureBindings[] = {
    // normal
    {{swapPosition,  .arg = {.i = UP}},     {{GESTURE_NORTH}, {.fingers = 3}}},
    {{swapPosition,  .arg = {.i = DOWN}},   {{GESTURE_SOUTH}, {.fingers = 3}}},
    {{shiftFocus,  .arg = {.i = UP}},       {{GESTURE_EAST},  {.fingers = 3}}},
    {{shiftFocus,  .arg = {.i = DOWN}},     {{GESTURE_WEST}, {.fingers = 3}}},
    {{shiftWorkspace,  .arg = {.i = UP}},   {{GESTURE_EAST}, {.fingers = 4}}},
    {{shiftWorkspace,  .arg = {.i = DOWN}}, {{GESTURE_WEST}, {.fingers = 4}}},
    // mouse emulation
    {{clickProportional,  .arg = {.i = 4}}, {{GESTURE_NORTH}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 5}}, {{GESTURE_SOUTH}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 6}}, {{GESTURE_EAST}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 7}}, {{GESTURE_WEST}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{moveAndClick,  .arg = {.i = 1}}, {{GESTURE_TAP}},  },
    // rotation
    {{spawn,  .arg = {.str = "xsane-xrandr -a rotate normal"}}, {{GESTURE_SOUTH, GESTURE_EAST}, }},
    {{spawn,  .arg = {.str = "xsane-xrandr -a rotate left"}}, {{GESTURE_EAST, GESTURE_NORTH}, }},
    {{spawn,  .arg = {.str = "xsane-xrandr -a rotate inverted"}}, {{GESTURE_NORTH, GESTURE_WEST}, }},
    {{spawn,  .arg = {.str = "xsane-xrandr -a rotate right"}}, {{GESTURE_WEST, GESTURE_SOUTH}, }},
};

void triggerGestureBindings(GestureEvent* event) {
    currentEvent = event;
    DEBUG("Attempting to trigger gesture");
    for(int i = 0; i < LEN(gestureBindings); i++)
        if(matchesGestureEvent(&gestureBindings[i].arg, event)) {
            DEBUG("Match found");
            callBinding(&gestureBindings[i].func);
        }
    dumpAndFreeGesture(event);
}

void readGestures(int fd) {
    INFO("Gesture bindings %d %d", fd, STATUS_FD_READ);
    assert(fd == STATUS_FD_READ);
    readTouchEvent(fd);
}
void initGestures() {
    INFO("init gestures");
    spawnPipe("sgestures-libinput-writer", REDIRECT_CHILD_OUTPUT_ONLY);
    addExtraEvent(STATUS_FD_READ, POLLIN, readGestures);
    registerEventHandler(triggerGestureBindings);
}
void enableGestureBindings() {
    addEvent(X_CONNECTION, DEFAULT_EVENT(initGestures, LOWEST_PRIORITY));
    addBindings(phoneBindings, LEN(phoneBindings));
}

