#!/bin/sh

if [ "$NUMBER" = 2300 ]; then
    SENDER=$(echo "$MSG" | sed -n 's/.*\([0-9]\{10\}\).*$/\1/pg')
    if [ -n "$SENDER" ]; then
        NAME=$(contacts get-name "$SENDER" || echo "$SENDER")
        send-sms "$SENDER" "#Hello $NAME. I think you sent me a MMS message which I can't receive. Please re-send a normal sms message, email me or use my other number"
    fi
    exit
fi
