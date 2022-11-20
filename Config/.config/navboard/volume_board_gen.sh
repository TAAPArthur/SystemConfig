#!/bin/sh -e

generateRow() {
    set_pressed_cmd="amixer set \\\"\$KEY_LABEL\\\" \$([ \$KEY_PRESSED -eq 0 ] && echo mute || echo unmute)"
    load_pressed_cmd="amixer get \\\"\$KEY_LABEL\\\" | grep -F -q '[on]'";

    set_value_cmd="ALSA_DEVICE=\\\"\$KEY_LABEL\\\" vol -s \$KEY_VALUE%";
    load_value_cmd="amixer get \\\"$KEY_LABEL\\\" | sed -nE 's/.* .([0-9]+)%.*/\\1/p'"

    pressed_cmd="if [ \$READING -eq 0 ]; then $set_pressed_cmd; else $load_pressed_cmd; fi"
    value_cmd="if [ \$READING -eq 0 ]; then $set_value_cmd; else $load_value_cmd; fi"

    cat - <<EOF
    { .label="$LAST_NAME", .loadValue = readValueFromCmd, .onPress = spawnCmd, .arg.s="$pressed_cmd", .flags=$SWITCH_FLAGS|LATCH},
    { .label="$LAST_NAME", .max=100, .loadValue = readValueFromCmd, .onDrag = spawnCmd, .arg.s="$value_cmd", .flags=$VOLUME_FLAGS},
    {0},
EOF
}
{
cat - <<EOF
// Generated File Do Not Manually Edit
#include <navboard/navboard.h>
#include <navboard/functions.h>
#undef DEFAULT_THICKNESS
#define DEFAULT_THICKNESS 70
static Key __keys[] = {
EOF

amixer controls | grep Playback | grep -v Route | sed -E 's/^.*name=.([A-z0-9 ]+) Playback (.*).$/\1,\2/g' | sort -t, -k1,1 | {
    while IFS=, read -r name type; do
        echo "$name $type" 1>&2
        if [ "$name" != "$LAST_NAME" ] ; then
            [ -n "$LAST_NAME" ] && generateRow
            LAST_NAME="$name"
            SWITCH_FLAGS=KEY_DISABLED
            VOLUME_FLAGS=KEY_DISABLED
        fi
        [ "$type" = "Switch" ] && SWITCH_FLAGS=0
        [ "$type" = "Volume" ] && VOLUME_FLAGS=0
    done

    [ -n "$LAST_NAME" ] && generateRow
}
cat - <<EOF
};
REGISTER(volume, __keys);
EOF
} > volume.c
