#!/bin/sh

if [ "$NOTIFY_CATEGORY" = call ]; then
    NOTIFY_FONT_SIZE=16 exec notify "$@"
fi
