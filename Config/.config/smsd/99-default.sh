#!/bin/sh

{
    vibrator -c"$COUNT" -d"$DURATION" -l "$LENGTH" &
    notify_log add "$HEADER" "$MSG" "led push $COLOR $LABEL $TRIGGER $PRIORITY" "led pop $COLOR $LABEL"
    notify-send -R "$LABEL" "$HEADER" "$MSG"
    errCode=$?
    if [ "$errCode" -eq 0 ] || [ "$errCode" -eq 2 ]; then
        notify_log clear "$HEADER"
    fi
} &
sleep 1
