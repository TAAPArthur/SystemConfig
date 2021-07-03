#!/bin/sh

if [ "$NOTIFY_CATEGORY" = SMS ]; then
    NOTIFY_FONT_SIZE=16 notify -y20 "$@"
    return 1
fi
