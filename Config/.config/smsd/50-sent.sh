#!/bin/sh

if [ "$SENT" -eq 1 ]; then
    notify-send -R "sent-$LABEL" "$HEADER" "$MSG"
    exit
fi
