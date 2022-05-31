#!/bin/sh

if [ "$NOTIFY_CATEGORY" = I ] || [ "$NOTIFY_CATEGORY" = i ] || [ "$NOTIFY_CATEGORY" = info ]; then
    export NOTIFY_FONT_SIZE="${NOTIFY_TIMEOUT:-16}"
    export NOTIFY_TIMEOUT="${NOTIFY_TIMEOUT:-0}"
fi
DISPLAY=${DISPLAY:-:0}
