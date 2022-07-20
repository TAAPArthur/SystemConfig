#!/bin/sh
#shellcheck disable=SC2034

PATH=$PATH:~/SystemConfig/bin

NAME=$(contacts get-name "$NUMBER" || echo "$NUMBER")
read -r GROUP <<EOF
$(contacts get-group "$NAME")
EOF

LABEL="$TYPE-$NUMBER"
HUMAN_LABEL="$TYPE-$NAME"
if [ "$SENT" = 1 ]; then
    HEADER="$TYPE to $NAME"
else
    HEADER="$TYPE from $NAME"
fi

COLOR=green
TRIGGER=timer
PRIORITY=1
COUNT=10
DURATION=10
LENGTH=100
AUTH=99
if [ "$TYPE" = "CALL" ]; then
    COUNT=1
    LENGTH=1000
fi
case "$GROUP" in
    ICE)
        AUTH=0
        COLOR=blue
        TRIGGER=heartbeat
        PRIORITY=99
        COUNT=1
        DURATION=10
        LENGTH=1000
        ;;
    GoodFriends)
        AUTH=1
        COLOR=blue
        TRIGGER=heartbeat
        PRIORITY=50
        ;;
    FriendLike)
        AUTH=2
        TRIGGER=timer
        PRIORITY=10
        ;;
esac

if [ -z "$DISPLAY" ]; then
    for f in /tmp/.X11-unix/X*; do
        if [ -e "$f" ]; then
            export DISPLAY=:"${f##/tmp/.X11-unix/X}"
            break;
        fi
    done
fi
