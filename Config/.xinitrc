#!/bin/sh
exec > ~/.local/share/xorg/xinitrc."${DISPLAY}".out 2> ~/.local/share/xorg/xinitrc."${DISPLAY}".err

# setxkbmap -option compose:ralt
(

which mrandr >/dev/null 2>&1 && mrandr auto
[ -x /usr/bin/xrdb ] && xrdb ~/.config/xresources
[ -x /usr/bin/xhost ] && xhost +
[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr

[ -x /usr/bin/xset ] && xset s 1200 180 && xset b off
[ -x /usr/bin/apples ] && [ -x /usr/bin/mpxlocker ] && exec apples "mpxlocker" "power-save" &
[ -x /usr/bin/clip-history ] && exec clip-history monitor &
[ -x /usr/bin/dzen2 ] && [ -x /usr/bin/mystatusbar ] && mystatusbar | status-bar -ta r -w 500 -title-name "mystatusbar" &
if [ -x /usr/bin/sgestures ]; then
    if [ -x /usr/bin/mqbus-receive ] && [ -e /dev/mqueue/input ]; then
        mqbus-receive -u input
    else
       sgestures-libinput-writer
    fi | sgestures &
fi
[ -x /usr/bin/xmouse-control ] && exec xmouse-control &
which notify_log >/dev/null 2>&1 && notify_log resume &
)

for file in ~/.xprofile.d/*.sh; do
    [ -r "$file" ] && . "$file"
done
export NESTED_SHELL_LEVEL=""
#$TERMINAL
outputFile=/tmp/$WINDOW_MANAGER.out.${DISPLAY#:}
[ -f "$outputFile" ] && mv "$outputFile" "$outputFile.bk"
exec "$WINDOW_MANAGER" > "$outputFile" 2>&1
