#!/bin/sh

vibrator -c10 -d10 -l 100 &
notify-send -r "sms-$NUMBER" "SMS from $(contacts get-name "$NUMBER") $MSG";
