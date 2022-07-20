#!/bin/sh

if [ "$SENT" -eq 1 ]; then
    notify-send -R "sent-$LABEL" "$HEADER" "$MSG" &
    if [ "$MSG" = "${MSG#\#}" ]; then
        notify_log clear "$HUMAN_LABEL"
    fi
    exit
fi
