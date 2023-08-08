#include <sgestures/event.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEN(X) (sizeof X / sizeof X[0])
/**
 * Used to sub-divide a gesture device
 * The GestureRegion will be apart of the GestureID thus events starting in different regions
 * are independent of each other
 */
typedef struct GestureRegion {
    char x, y, width, height;
} GestureRegion;
enum {
    DEFAULT_REGION,
    TOP_LEFT, TOP_MIDDLE, TOP_RIGHT,
    LEFT, RIGHT,
    BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT,
};
GestureRegion gestureRegions[] = {
    {0,     0, 100, 100},

    {0,     0,  20,   2},
    {20,    0,  60,   2},
    {80,    0,  20,   2},

    {0,    20,   2,   96},
    {98,   20,   2,   96},

    {0,    98,  20,   2},
    {20,   98,  60,   2},
    {80,   98,  20,   2},
};

static inline bool contains(GestureRegion rect, GesturePoint arg) {
    return (rect.x <= arg.x && arg.x <=  rect.x + rect.width &&
            rect.y <= arg.y && arg.y <= rect.y + rect.height);
}
ProductID generateIDHighBits(const TouchEvent* event) {
    for(int i = LEN(gestureRegions) - 1; i > 0; --i)
        if(contains(gestureRegions[i], event->pointPercent))
            return i;
    return 0;
}

void spawn(const char* cmd) {
    int pid = fork();
    if(pid == 0) {
        execl("/bin/sh", "/bin/sh", "-c", cmd, NULL);
        perror("exec failed; Aborting");
        exit(2);
    }
    else if(pid < 0){
        perror("error forking");
        exit(2);
    }
}

typedef struct {
    void (*func)();
    union {
        const char* str;
    } arg;
    GestureBindingArg bindingArg;
} GestureBinding ;

#define ROTATE_CMD(X) "xsane-xrandr -a rotate " # X "; xsane-xrandr -a rotate-touchscreen; rotate_terminal " # X
GestureBinding gestureBindings[] = {
    // normal
    {spawn,  {"mpxmanager swap-down"},            {{GESTURE_NORTH}, {.fingers = 3}}},
    {spawn,  {"mpxmanager swap-up"},              {{GESTURE_SOUTH}, {.fingers = 3}}},
    {spawn,  {"mpxmanager next-win"},             {{GESTURE_EAST},  {.fingers = 3}}},
    {spawn,  {"mpxmanager prev-win"},             {{GESTURE_WEST},  {.fingers = 3}}},
    {spawn,  {"mpxmanager shift-workspace-up"},   {{GESTURE_EAST},  {.fingers = 4}}},
    {spawn,  {"mpxmanager shift-workspace-down"}, {{GESTURE_WEST},  {.fingers = 4}}},

    // OSK
    {spawn,  {"navboard-local board_of_boards"},        {{GESTURE_EAST}, .regionID=BOTTOM_MIDDLE}},
    {spawn,  {"navboard-local wm"},                     {{GESTURE_WEST}, .regionID=BOTTOM_MIDDLE}},
    {spawn,  {"navboard-local"},                        {{GESTURE_EAST}, .regionID=TOP_MIDDLE}},
    {spawn,  {"pkill navboard"},                  {{GESTURE_WEST}, .regionID=TOP_MIDDLE}},

    /*
    // mouse emulation
    {{clickProportional,  .arg = {.i = 4}}, {{GESTURE_NORTH}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 5}}, {{GESTURE_SOUTH}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 6}}, {{GESTURE_EAST}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 7}}, {{GESTURE_WEST}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{moveAndClick,  .arg = {.i = 1}}, {{GESTURE_TAP}},  },
    */

    // rotation; drawn an "L" to rotate
    {spawn,  {ROTATE_CMD(normal)  }, {{GESTURE_SOUTH, GESTURE_EAST}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(normal)  }, {{GESTURE_SOUTH, GESTURE_SOUTH_EAST, GESTURE_EAST}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(left)    }, {{GESTURE_EAST,  GESTURE_NORTH}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(left)    }, {{GESTURE_EAST,  GESTURE_NORTH_EAST, GESTURE_NORTH}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(inverted)}, {{GESTURE_NORTH, GESTURE_WEST}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(inverted)}, {{GESTURE_NORTH, GESTURE_NORTH_WEST, GESTURE_WEST}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(right)   }, {{GESTURE_WEST,  GESTURE_SOUTH}, {.fingers=1}}},
    {spawn,  {ROTATE_CMD(right)   }, {{GESTURE_WEST,  GESTURE_SOUTH_WEST, GESTURE_SOUTH}, {.fingers=1}}},

    {spawn,  {"mpxmanager hide"}, {{GESTURE_PINCH}}},
    {spawn,  {"mpxmanager unhide"}, {{GESTURE_PINCH_OUT}}},
};

void triggerGestureBindings(GestureEvent* event) {
    for(int i = 0; i < LEN(gestureBindings); i++)
        if(matchesGestureEvent(&gestureBindings[i].bindingArg, event)) {
            gestureBindings[i].func(gestureBindings[i].arg);
        }
    dumpAndFreeGesture(event);
}

int main(int argc, char* const argv[]) {
    signal(SIGCHLD, SIG_IGN);
    GestureMask mask = argc > 1 ?  atoi(argv[1]) : GestureEndMask;
    listenForGestureEvents(mask);
    registerEventHandler(triggerGestureBindings);
    while(true) {
        readTouchEvent(STDIN_FILENO);
    }
    return 0;
}
