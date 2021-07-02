#!/bin/sh

if [ "$CATEGORY" = SOUND ]; then
    # shellcheck disable=SC2086
    NOTIFY_FONT_SIZE=16 notify -y20 -w-300 $NOTIFY_ARGS "$@"
    return 1
fi
