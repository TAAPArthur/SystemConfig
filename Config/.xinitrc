#!/bin/sh
exec > ~/.local/share/xorg/xinitrc."${DISPLAY}".out 2> ~/.local/share/xorg/xinitrc."${DISPLAY}".err

which mrandr >/dev/null 2>&1 && mrandr auto

[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr
[ -x /usr/bin/xrdb ] && xrdb ~/.config/xresources


# setxkbmap -option compose:ralt
(
[ -x /usr/bin/lxqt-policykit-agent ] && lxqt-policykit-agent &
[ -x /usr/bin/dunst ] && dunst &
[ -x /usr/bin/trayer ] && trayer --edge top --align right --height 18 --widthtype pixel --width 90 --SetDockType true --SetPartialStrut true --transparent true --alpha 0 --tint 0 --monitor primary &
[ -x /usr/bin/pasystray ] && pasystray --notify=all &
[ -x /usr/bin/nm-applet ] && nm-applet &
[ -x /usr/bin/xhost ] && xhost +

[ -x /usr/bin/xset ] && xset s 1200 180 && xset b off
[ -x /usr/bin/apples ] && [ -x /usr/bin/mpxlocker ] && exec apples "mpxlocker" "power-save" &
[ -x /usr/bin/clip-history ] && exec clip-history monitor &
[ -x /usr/bin/dzen2 ] && [ -x /usr/bin/mystatusbar ] && mystatusbar | status-bar -ta r -w 500 -title-name "mystatusbar" &
[ -x /usr/bin/sgestures ] && mqbus-receive -u input | sgestures &
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
