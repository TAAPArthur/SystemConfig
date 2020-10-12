#!/bin/bash

    file="/tmp/.weather"
checkCache() {
    if [[ -f "$file" && ($(date -r "$file" +%s) > $(date -d 'now - 1 hour' +%s)) ]]; then
        cat $file
        exit 0
    fi
}

main () {
    checkCache

    if ! ping -q -c 1 -W 1 google.com &>/dev/null; then
        touch $file
        echo $(cat $file)'*'
        exit 1
    fi
    temp=$(curl -s 'wttr.in?m&format=%t&lang=uk' & curl -s 'wttr.in?u&format=%t&lang=uk' )
    if [ "$?" -ne 0 ] || [ -z "$temp" ] || [[ "$temp" == "Unknown"* ]] ; then
        touch $file
        echo $(cat $file)'*'
    else
        output=$(echo "$temp" | sed "s/[^CF0-9]//g" )
        echo $output >$file
        echo $output
    fi

}
if [  "$1" == "-c" ]; then
    checkCache
    exit
fi

main
