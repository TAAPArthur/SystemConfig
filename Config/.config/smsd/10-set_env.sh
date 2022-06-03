#!/bin/sh
#shellcheck disable=SC2034

NAME=$(contacts get-name "$NUMBER" || echo "$NUMBER")
read -r GROUP <<EOF
$(contacts get-group "$NAME")
EOF

LABEL="sms-$NUMBER"
HUMAN_LABEL="sms-$NAME"
if [ "$SENT" = 1 ]; then
    HEADER="SMS to $NAME"
else
    HEADER="SMS from $NAME"
fi


COLOR=green
TRIGGER=timer
PRIORITY=1
COUNT=10
DURATION=10
LENGTH=100
AUTH=99
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
        TRIGGER=activity
        PRIORITY=50
        ;;
    FriendLike)
        AUTH=2
        TRIGGER=timer
        PRIORITY=10
        ;;
esac
