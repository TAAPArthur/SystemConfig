#!/bin/sh

NAME=$(contacts get-name "$NUMBER" || echo "$NUMBER")
header="SMS from $NAME"
label="sms-$NUMBER"
{
    COLOR=green
    TRIGGER=timer
    PRIORITY=1
    read -r group <<EOF
$(contacts get-group "$NAME")
EOF
    COUNT=10
    DURATION=10
    LENGTH=100
    case "$group" in
        ICE)
            COLOR=blue
            TRIGGER=heartbeat
            PRIORITY=99
            COUNT=1
            DURATION=10
            LENGTH=1000
            ;;
        GoodFriends)
            COLOR=blue
            TRIGGER=activity
            PRIORITY=50
            ;;
        FriendLike)
            TRIGGER=timer
            PRIORITY=10
            ;;
    esac
    vibrator -c$COUNT -d$DURATION -l $LENGTH &
    notify-send -R "$label" "$header" "$MSG"
    errCode=$?
    if [ "$errCode" -eq 255 ]; then
        notify_log add "$header" "$MSG" "led push $COLOR $label $TRIGGER $PRIORITY" "led pop $COLOR $label"
    fi
} &
sleep 1
