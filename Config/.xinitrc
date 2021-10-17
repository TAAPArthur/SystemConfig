#!/bin/sh
exec > ~/.local/share/xorg/.xinitrc.out 2> ~/.local/share/xorg/.xinitrc.err

[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr



# setxkbmap -option compose:ralt

[ -x /usr/bin/lxqt-policykit-agent ] && lxqt-policykit-agent &
[ -x /usr/bin/dunst ] && dunst &
[ -x /usr/bin/trayer ] && trayer --edge top --align right --height 18 --widthtype pixel --width 90 --SetDockType true --SetPartialStrut true --transparent true --alpha 0 --tint 0 --monitor primary &
[ -x /usr/bin/pasystray ] && pasystray --notify=all &
[ -x /usr/bin/nm-applet ] && nm-applet &
[ -x /usr/bin/xhost ] && [ -x /usr/bin/steam ] && xhost +

[ -x /usr/bin/xset ] && xset s 1200 180 && xset b off
[ -x /usr/bin/apples ] && [ -x /usr/bin/mpxlocker ] && exec apples "mpxlocker" "/bin/suspend" &
[ -x /usr/bin/clip-history ] && exec clip-history monitor &
[ -x /usr/bin/dzen2 ] && [ -x /usr/bin/mystatusbar ] && mystatusbar | status-bar -title-name "mystatusbar" &
[ -x /usr/bin/ringd ] && exec ringd &
[ -x /usr/bin/sgestures ] && sgestures-libinput-writer | sgestures &
[ -x /usr/bin/smsd ] && exec smsd &
[ -x /usr/bin/xmouse-control ] && exec xmouse-control &

for file in ~/.xprofile.d/*.sh; do
    [ -r "$file" ] && . "$file"
done
export NESTED_SHELL_LEVEL=""
outputFile=/tmp/$WINDOW_MANAGER.out.${DISPLAY#:}
[ -f "$outputFile" ] && mv "$outputFile" "$outputFile.bk"
#$TERMINAL
exec "$WINDOW_MANAGER" > "$outputFile" 2>&1
