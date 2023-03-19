#!/bin/sh -ex

file="/tmp/.weather"
tmpfile="$file.tmp"
timestampFile="$file.ts"
checkCache() {
    if [ -s "$file" ] && [ -f "$timestampFile" ] && [ "$(date -u +%s)" -lt $(($(cat $timestampFile) + 3600)) ]; then
        cat $file
        exit 0
    fi
}

checkCache
if [ "$1" = "-c" ]; then
    exit
fi

[ -n "$WEATHER_LOCK" ] || WEATHER_LOCK=1 exec flock -n "$file" "$0"

updateTimeStamp() {
    date -u +%s > "$timestampFile"
}
trap 'rm "$tmpfile"' EXIT

if curl -m 2 -s 'wttr.in?{m,u}&format=%t&lang=uk' > "$tmpfile" && grep -q "F" "$tmpfile" && grep -q "C" "$tmpfile"; then
    sed "s/[^CF0-9]//g" "$tmpfile" | tee $file
    updateTimeStamp
else
    [ -s "$file" ] && cat "$file"
    printf '*'
    updateTimeStamp
    exit 1
fi
