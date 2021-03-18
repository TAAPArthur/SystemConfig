#!/bin/sh -e

file="/tmp/.weather"
tmpfile="$file.tmp"
timestampFile="$file.ts"
checkCache() {
    if [ -f "$file" ] && [ -f "$timestampFile" ] && [ "$(date -u +%s)" -lt $(($(cat $timestampFile) + 3600)) ]; then
        cat $file
        exit 0
    fi
}
updateTimeStamp() {
    date -u +%s > $timestampFile
}

main () {
    checkCache

    if (curl -s 'wttr.in?m&format=%t&lang=uk' && curl -s 'wttr.in?u&format=%t&lang=uk') > $tmpfile; then
        sed "s/[^CF0-9]//g" $tmpfile | tee $file
        rm $tmpfile
        updateTimeStamp
    else
        [ -r "$file" ] || cat $file
        printf '*'
        updateTimeStamp
        exit 1
    fi
}
if [ "$1" = "-c" ]; then
    checkCache
    exit
fi

main
