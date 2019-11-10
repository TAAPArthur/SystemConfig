#!/bin/bash

main () {

    file="/tmp/.weather"

    if [[ -f "$file" && ($(date -r "$file" +%s) > $(date -d 'now - 1 hour' +%s)) ]]; then
        cat $file
        exit 0
    fi

    if ! ping -q -c 1 -W 1 google.com &>/dev/null; then
        touch $file
        echo $(cat $file)'*'
        exit 1
    fi
    tempC=$(curl -s 'wttr.in?m&format=%t&lang=uk')
    tempF=$(curl -s 'wttr.in?u&format=%t&lang=uk')
    if [ "$?" -ne 0 ] || [ -z "$tempC" ] || [[ "$tempC" == "Unknown"* ]] ; then
        touch $file
        echo $(cat $file)'*'
    else
        output=$(echo "$tempC;$tempF"|sed -r 's/[^CF0-9; ]//g' )
        echo "$output" >$file
        echo "$output"
    fi

}

main
