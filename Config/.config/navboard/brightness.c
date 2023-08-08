#include <navboard/functions.h>
#include <navboard/navboard.h>
#undef DEFAULT_THICKNESS
#define DEFAULT_THICKNESS 10

static Key __keys[] = {
    { .label="-", .onPress = spawnCmd, .arg.s="brightness -1%", .flags = TRIGGER_RELOAD, .weight = 1},
    { .label="Brightness%", .max=100, .loadValue = readValueFromCmd, .onDrag = spawnCmd, .arg.s="if [ $READING -eq 0 ]; then brightness \"=$KEY_VALUE%\"; else brightness %; fi", .weight = 10},
    { .label="+", .onPress = spawnCmd, .arg.s="brightness +1%", .flags = TRIGGER_RELOAD, .weight = 1},
    {0},
};
REGISTER(brightness, __keys);
