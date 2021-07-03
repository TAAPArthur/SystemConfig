#!/bin/sh

if [ "$NOTIFY_CATEGORY" = SOUND ]; then
    NOTIFY_FONT_SIZE=16 notify -y20 -w-300 "$@"
    return 1
fi
