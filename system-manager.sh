#!/bin/bash

set -e
export SYSTEM_CONFIG_DIR=~/SystemConfig

getAllPackages(){
    cat <(jq -r '. | flatten | join("\n")' $SYSTEM_CONFIG_DIR/packages.json) <(pacman -Qqg base-devel base) |sort|uniq
}
linkFiles(){
    sedStr="s/([^ ]*) (.*)$/[[ \$(realpath \1) == \$(readlink \2\$(basename \1)) || \$(readlink -f \1) == \$(realpath \2) ]] || (mkdir -p \$( [[ \2 == *\/ ]] \&\& echo \2 || dirname \2) \&\& ln -si \1 \2) /g"
    sudoSedStr="s/([^ ]*) (.*)$/[[ \$(realpath \1) == \$(readlink \2\$(basename \1)) || \$(readlink -f \1) == \$(realpath \2) ]] || (sudo mkdir -p \$( [[ \2 == *\/ ]] \&\& echo \2 || dirname \2) \&\& sudo ln -si \1 \2) /g"
    str='\(.key) \(.value|tostring)'
    bash <(jq -r ".links|to_entries|map(\"$str\")|.[]"  $SYSTEM_CONFIG_DIR/hooks.json | sed -E "$sedStr")
    bash <(jq -r ".links_root|to_entries|map(\"$str\")|.[]"  $SYSTEM_CONFIG_DIR/hooks.json | sed -E "$sudoSedStr")
}

options=( $(jq -r 'keys | join(" ")' $SYSTEM_CONFIG_DIR/packages.json |sort))
menu() {
    for i in ${!options[@]}; do
        printf "%3d %s) %s\n" $((i+1)) "${options[i]}" 1>&2
    done
}
selectPackages(){
    #taken from https://serverfault.com/questions/144939/multi-select-menu-in-bash-script
    # customize with your own.

    while true; do
        menu
        read -rp "What do you want installed? " nums
        while read num; do
            if [[ $num == '*' ]]; then
                echo "."
                return 0
            fi
            cmd="echo $num"
            if [[ "$num" == *-* ]]; then
                cmd="seq $(echo "$num" | sed 's/-/ /' )"
            fi
            while read n; do
                [[ "$n" != *[![:digit:]]* ]] &&
                (( n > 0 && n <= ${#options[@]} )) ||
                { echo "Invalid option: $n" 1>&2; continue; }
                ((n--));
                choices[n]="+"
            done < <($cmd)
        done < <(echo "$nums" |sed "s/[ ,]\+/\n/g")
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
            args=$(echo $(selectPackages)|sed "s/ /, /g")
            echo "Selected $args"
        fi
        packages=$(jq -r "$args | flatten | join(\"\n\")" $SYSTEM_CONFIG_DIR/packages.json |sort)
        ${PKG_MANAGER:-sudo pacman} -S $(echo $packages) --needed

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
     mark-as-dep)
         $PKG_MANAGER -S --asdeps $(sudo pacman -Rns $(./system-manager.sh list-all-unknown) 2>&1 | grep "::" |cut -d" " -f 4 |sort |uniq)
         ;;
    help)
        displayHelp
        ;;
    version)
        displayVersion
        ;;
esac
