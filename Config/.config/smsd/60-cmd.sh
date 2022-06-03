#!/bin/sh
#shellcheck disable=SC2034

case "$MSG" in
    /ping)
        send-sms "$NUMBER" "#pong"
        exit
        ;;
    /uname)
        send-sms "$NUMBER" "#$(uname -a)"
        exit
        ;;
    /uptime)
        send-sms "$NUMBER" "#$(uptime)"
        exit
        ;;
esac

if [ "${AUTH:-99}" -le 1 ]; then
    case "$MSG" in
        /sos|/SOS)
            send-sms "$NUMBER" "sos-received"
            COLOR=white
            TIRGGER=heartbeat
            HEADER="SOS from $NAME"
            ;;
    esac
fi
