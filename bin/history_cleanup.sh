#!/bin/sh -e
FILE="${1:-$HISTFILE}"
SIZE="${2:-1000}"
[ -n "$HIST_CLEAN" ] || HIST_CLEAN=1 exec flock -n "$FILE" "$0" "$@"
DIR="$(mktemp -d)"
trap 'rm -r $DIR' EXIT

cd "$DIR"

grep -v -x -e ls -e fg -e F -e fg -e bk "$FILE" | uniq > temp

if [ "$(wc -l "temp" | cut -d" " -f1)" -gt $((HISTSIZE/2)) ]; then
    tail -n +"$SIZE" "temp" | sort -u > temp2
    tail -n "$SIZE" "temp" >> temp2
    mv temp2 temp
fi
mv temp "$FILE"
