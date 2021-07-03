#!/bin/sh

if [ "$NOTIFY_CATEGORY" = SMS ]; then
    NOTIFY_FONT_SIZE=16 notify "$@"
    return 1
fi
