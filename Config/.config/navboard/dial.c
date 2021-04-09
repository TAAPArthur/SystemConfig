#include <navboard/navboard.h>
#include <navboard/util.h>
void dialTone(KeyGroup*keyGroup, Key*key) {
    const char* const args[]={"call", "-t", key->label, NULL};
    spawnArgs(args);
}
Key keys_dialTone[] = {
    {.label="1", .onPress=dialTone},
    {.label="2", .onPress=dialTone},
    {.label="3", .onPress=dialTone},
    {0},
    {.label="4", .onPress=dialTone},
    {.label="5", .onPress=dialTone},
    {.label="6", .onPress=dialTone},
    {0},
    {.label="7", .onPress=dialTone},
    {.label="8", .onPress=dialTone},
    {.label="9", .onPress=dialTone},
    {0},
    {.label="#", .onPress=dialTone},
    {.label="0", .onPress=dialTone},
    {.label="*", .onPress=dialTone},
};
REGISTER(dialTone, keys_dialTone);
