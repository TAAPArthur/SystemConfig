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
    {0,     0,  33,  20},
    {33,    0,  33,  20},
    {67,    0,  33,  20},
    {0,     0,  20,   0},
    {80,    0,  20,   0},
    {0,    80,  33,  20},
    {33,   80,  33,  20},
    {37,   80,  33,  20},
};

static GestureEvent* currentEvent;


static inline bool contains(GestureRegion rect, GesturePoint arg) {
    return (rect.x <= arg.x && arg.x <=  rect.x + rect.width &&
            rect.y <= arg.y && arg.y <= rect.y + rect.height);
}
ProductID generateIDHighBits(ProductID id __attribute__((unused)), GesturePoint startingGesturePoint) {
    for(int i = LEN(gestureRegions) - 1; i > 0; --i)
        if(contains(gestureRegions[i], startingGesturePoint))
            return i + 1;
    return 0;
}

void spawn(const char* cmd) {
    int pid = fork();
    if(pid == 0) {
        const char* const args[] = {"/bin/sh", "-c", cmd, NULL};
        execv(args[0], (char* const*)args);
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
    };
    GestureBindingArg arg;
} GestureBinding ;

GestureBinding gestureBindings[] = {
    // normal
    {spawn,  {"mpxmanager swap-down"},            {{GESTURE_NORTH}, {.fingers = 3}}},
    {spawn,  {"mpxmanager swap-up"},              {{GESTURE_SOUTH}, {.fingers = 3}}},
    {spawn,  {"mpxmanager next-win"},             {{GESTURE_EAST},  {.fingers = 3}}},
    {spawn,  {"mpxmanager prev-win"},             {{GESTURE_WEST},  {.fingers = 3}}},
    {spawn,  {"mpxmanager shift-workspace-up"},   {{GESTURE_EAST},  {.fingers = 4}}},
    {spawn,  {"mpxmanager shift-workspace-down"}, {{GESTURE_WEST},  {.fingers = 4}}},

    // OSK
    {spawn,  {"navboard"},                        {{GESTURE_EAST}, .regionID=TOP_MIDDLE}},
    {spawn,  {"pkill navboard"},                  {{GESTURE_WEST}, .regionID=TOP_MIDDLE}},

    /*
    // mouse emulation
    {{clickProportional,  .arg = {.i = 4}}, {{GESTURE_NORTH}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 5}}, {{GESTURE_SOUTH}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 6}}, {{GESTURE_EAST}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{clickProportional,  .arg = {.i = 7}}, {{GESTURE_WEST}, {.fingers = 2, .mask = TouchMotionMask}}},
    {{moveAndClick,  .arg = {.i = 1}}, {{GESTURE_TAP}},  },
    */
    // rotation
    {spawn,  {"xsane-xrandr -a rotate normal"},   {{GESTURE_SOUTH, GESTURE_EAST}, }},
    {spawn,  {"xsane-xrandr -a rotate left"},     {{GESTURE_EAST, GESTURE_NORTH}, }},
    {spawn,  {"xsane-xrandr -a rotate inverted"}, {{GESTURE_NORTH, GESTURE_WEST}, }},
    {spawn,  {"xsane-xrandr -a rotate right"},    {{GESTURE_WEST, GESTURE_SOUTH}, }},
};

void triggerGestureBindings(GestureEvent* event) {
    currentEvent = event;
    for(int i = 0; i < LEN(gestureBindings); i++)
        if(matchesGestureEvent(&gestureBindings[i].arg, event)) {
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