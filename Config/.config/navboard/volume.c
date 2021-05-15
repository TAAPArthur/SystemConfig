// Generated File Do Not Manually Edit
#include <navboard/navboard.h>
#include <navboard/functions.h>
#include "volume.h"
#undef DEFAULT_THICKNESS
#define DEFAULT_THICKNESS 70
Key __keys[] = {
    { .label="Master", .loadValue = setPressedStateFromMuteStatus, .onPress = spawnCmd, .arg.s="amixer set \"$KEY_LABEL\" $([ \"$KEY_PRESSED\" -eq 0 ] && echo mute || echo unmute)", .flags=0|LATCH},
    { .label="Master", .max=100, .loadValue = setValueFromVolume, .onDrag = spawnCmd, .arg.s="ALSA_DEVICE=\"$KEY_LABEL\" vol -s $KEY_VALUE% &", .flags=0},
    {0},
};
REGISTER(volume, __keys);
