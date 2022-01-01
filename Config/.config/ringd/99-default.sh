#!/bin/sh
vibrator -c1 -d0 -l 1000 &
if notify-send -c call -r "phone-$NUMBER" "Call from $(contacts get-name "$NUMBER" || echo "$NUMBER") $ALPHA"; then
    exec call -a
else
    [ "$?" -eq 2 ] && exec call -h
fi
