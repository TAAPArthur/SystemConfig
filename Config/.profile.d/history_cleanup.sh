#!/bin/sh

if [ -n "$HISTFILE" ] && [ -n "$HISTSIZE" ]; then
    [ "$(wc -l "$HISTFILE" | cut -d" " -f1)" -gt $((HISTSIZE/2)) ] && history_cleanup.sh "$HISTFILE"
fi
