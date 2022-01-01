#!/bin/sh -e
FILE="${1:-$HISTFILE}"
SIZE="${2:-1000}"
[ -n "$HIST_CLEAN" ] || HIST_CLEAN=1 exec flock -n "$FILE" "$0" "$@"
DIR="$(mktemp -d)"
trap 'rm -r $DIR' EXIT
cd "$DIR"
tail -n +"$SIZE" "$FILE" | grep -v -e "^#" -e "^kiss" -e "^ls" -e "^fg" | sort -u > temp
tail -n "$SIZE" "$FILE" >> temp
mv temp "$FILE"
