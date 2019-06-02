#!/bin/bash

main () {

file="/tmp/.weather"

if [[ -f "$file" && $(date -r "$file" +%s) > $(date -d 'now - 1 hour' +%s) ]]; then
    cat $file
    exit 0
fi

if ! ping -q -c 1 -W 1 google.com &>/dev/null; then
    echo " ... "
    exit 1
fi


tempC=$(curl -s wttr.in?m | sed -E -n '3,7{s/\d27\[[0-9;]*m//g;s/^.* (-?[0-9]+)..\+?(-?[0-9]+) (.[CF]).*$/\1-\2\3/p'})
tempF=$(curl -s wttr.in?u | sed -E -n '3,7{s/\d27\[[0-9;]*m//g;s/^.* (-?[0-9]+)..\+?(-?[0-9]+) (.[CF]).*$/\1-\2\3/p'})
if [ "$?" -ne 0 ] || [ -z "$tempC" ]; then
    tempC=$(curl -s wttr.in?m | sed -E -n '3,7{s/\d27\[[0-9;]*m//g;s/^.* (-?[0-9]+) (.[CF]).*$/\1\2/p'})
    tempF=$(curl -s wttr.in?u | sed -E -n '3,7{s/\d27\[[0-9;]*m//g;s/^.* (-?[0-9]+) (.[CF]).*$/\1\2/p'})
fi
if [ "$?" -ne 0 ] || [ -z "$tempC" ]; then
    echo $(cat $file)'*'
else
    output=$(echo "$tempC;$tempF" |tr -d " ")
    echo "$output" >$file
    echo "$output"
fi

}

main &
