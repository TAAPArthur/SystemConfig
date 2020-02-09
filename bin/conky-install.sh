#!/bin/bash
set -e
match=" connected primary"
xrandr -q | grep -q  "$match" || match=" connected"
xrandr -q | grep -q  "$match"
X=$(xrandr |grep -Po "$match \d+x\d+\+\K\d+\+\d+"| cut -d"+" -f 1 )
Y=$(xrandr |grep -Po "$match \d+x\d+\+\K\d+\+\d+"| cut -d"+" -f 2 )
WIDTH=$(xrandr |grep -Po "$match \K\d+x\d+" |cut -d"x" -f 1)
HEIGHT=$(xrandr |grep -Po "$match \K\d+x\d+" |cut -d"x" -f2)

values=( "X" "Y" "WIDTH" "HEIGHT" )
for var in "${values[@]}"
do
    value="${!var}"
    echo value="${!var}"
    if [[ ! "$value" ]];then
        value=0
    fi
    export $var=$value
done

xrandr -q | grep -q  "$match"
dir="$HOME/.config/conky"
mkdir -p $dir
function subFile(){
    f1=$1
    f2="/tmp/$f1$(uuidgen)"
    cp $f1 $f2
    while read -r expr; do
        result=$(bash -c "echo $expr |bc" 2>/dev/null)
        escapedExpr=$(echo $expr | sed -E "s/([\*\/])/\\\\\1/g")
        sed -i -e "s/$escapedExpr/$result/g" $f2
    done < <(sed -n 's/[^\$]*\(\$[A-Z]\+.*\),/\1/p' $f1 |sort |uniq)
    mv $f2 $dir/$f1
}
cd "$HOME/SystemConfig/Config/Conky"
subFile time
subFile .conkyrc
