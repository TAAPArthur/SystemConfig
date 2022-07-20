#!/bin/sh

if [ "$TYPE" = CALL ] && [ "$MSG" != RING ]; then
    notify-send -c call -r "phone-$NUMBER" "$HEADER" "Call state changed to $MSG" &

    if [ "$MSG" = CONNECT ]; then
        notify_log clear "$HUMAN_LABEL"
    fi
    exit 0
fi
