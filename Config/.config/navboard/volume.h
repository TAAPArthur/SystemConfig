#include <navboard/navboard.h>
#include <navboard/util.h>
#include <stdio.h>
#include <stdlib.h>
static inline void setPressedStateFromMuteStatus(KeyGroup* keyGroup, Key* key){
    setKeyEnv(key);
    key->pressed = !spawn("amixer get \"$KEY_LABEL\" | grep -q '\\[on\\]'");
}

static inline void setValueFromVolume(KeyGroup* keyGroup, Key* key){
    setKeyEnv(key);
    char buffer[8];
    spawn("echo amixer get \"$KEY_LABEL\"   sed -nE 's/.*\\[([0-9]+)%.*/\\1/p'");
    readCmd("amixer get \"$KEY_LABEL\" | sed -nE 's/.* .([0-9]+)%.*/\\1/p'", buffer, sizeof(buffer));
    key->value = atoi(buffer);
}

void setKeyEnv(const Key* key);
