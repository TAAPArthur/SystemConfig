#!/bin/sh
vibrator -c1 -d0 -l 1000 &
if notify-send -r "phone-$NUMBER" "Call from $(contacts get-name "$NUMBER" || echo "$NUMBER") $ALPHA"; then
    call -a
else
    [ "$?" -eq 2 ] && call -h
fi
