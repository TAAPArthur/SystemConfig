#!/bin/sh

if [ "$NOTIFY_CATEGORY" = sms ] || [ "$NOTIFY_CATEGORY" = SMS ]; then
    NOTIFY_FONT_SIZE=16 exec notify -y20 "$@"
fi
