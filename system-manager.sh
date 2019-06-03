#!/bin/bash

set -e
export SYSTEM_CONFIG_DIR=~/SystemConfig

getAllPackages(){
    cat <(jq -r '. | flatten | join("\n")' $SYSTEM_CONFIG_DIR/packages.json) <(pacman -Qqg base-devel base) |sort|uniq
}
linkFiles(){
    str='ln -si \(.key) \(.value|tostring)'
    bash <(jq -r ".links|to_entries|map(\"$str\")|.[]"  $SYSTEM_CONFIG_DIR/hooks.json)
    bash <(jq -r ".links_root|to_entries|map(\"sudo $str\")|.[]"  $SYSTEM_CONFIG_DIR/hooks.json)
}

options=( $(jq -r 'keys | join(" ")' $SYSTEM_CONFIG_DIR/packages.json |sort))
menu() {
    for i in ${!options[@]}; do
        printf "%3d%s) %s\n" $((i+1)) "${choices[i]:- }" "${options[i]}" 1>&2
    done
}
selectPackages(){
    #taken from https://serverfault.com/questions/144939/multi-select-menu-in-bash-script
    # customize with your own.


    prompt="Check an option (again to uncheck, ENTER when done): "
    while true; do
        menu
        read -rp "$prompt" nums
        while read num; do
            [[ "$num" != *[![:digit:]]* ]] &&
            (( num > 0 && num <= ${#options[@]} )) ||
            { msg="Invalid option: $num"; continue; }
            ((num--)); msg="${options[num]} was ${choices[num]:+un}checked"
            [[ "${choices[num]}" ]] && choices[num]="" || choices[num]="+"
        done < <(echo $nums |sed "s/ /\n/g")
        if [[ ! -z "${choices[*]}" ]]; then
            break
        fi
    done

    for i in ${!options[@]}; do
        [[ "${choices[i]}" ]] && { echo ".${options[i]}"; }
    done
    return 0

}

case "$1" in
    run-scripts)
        jq -r '.scripts |join("\n")' $SYSTEM_CONFIG_DIR/hooks.json
        jq -r '.scripts |join("; ")' $SYSTEM_CONFIG_DIR/hooks.json |bash
        ;;
    install-packages)
        shift
        args="$*"
        if [[ -z "$args" ]]; then
            args=$(echo $(selectPackages)|sed "s/ /,/g")
        fi
        packages=$(jq -r "$args | flatten | join(\"\n\")" $SYSTEM_CONFIG_DIR/packages.json |sort)
        ${PKG_MANAGER:-sudo pacman} -S $(echo $packages) --needed

        set -xe
        hooks=$(sort <(jq -r '.install_hooks | keys | join("\n")' $SYSTEM_CONFIG_DIR/hooks.json) <(echo $packages) |uniq -d | xargs -i{} '.{} + "; " + ')
        if [[ ! -z "$hooks" ]]; then
            $(jq "$hooks '' " $SYSTEM_CONFIG_DIR/hooks.json)
        fi
        ;;
     link-files)
         linkFiles
         ;;
     list-groups)
        jq -r 'keys | join("\n")' $SYSTEM_CONFIG_DIR/packages.json |sort
       ;;
     list)
        sort <(pacman -Qqe) <(getAllPackages) |uniq -d
       ;;
     list-all-known)
        getAllPackages
       ;;
     list-all-unknown)
        shift
        diff  <(pacman -Qqe|sort) <(getAllPackages) |grep "<" |cut -d" " -f 2
        ;;
    help)
        displayHelp
        ;;
    version)
        displayVersion
        ;;
esac
