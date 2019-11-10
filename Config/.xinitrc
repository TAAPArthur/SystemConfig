#!/bin/bash

[ -x /usr/bin/nitrogen ] && nitrogen --restore

dbus-update-activation-environment --systemd DISPLAY

[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr

[ -x /usr/bin/lxqt-policykit-agent ] && lxqt-policykit-agent &

xset s 1200 180

[[ -x /usr/bin/xss-lock && -x /usr/bin/xsecurelock ]] && xss-lock -n '(brightness set .1 && trap "brightness set" EXIT && sleep 1000s)' -- xsecurelock &
[[ -x /usr/bin/xautolock ]] && xautolock -time ${SUSPEND_TIME:-30} -detectsleep -locker "systemctl suspend" &

setxkbmap -option compose:ralt

[ -x /usr/bin/conky ] && (conky -dc ~/.config/conky/.conkyrc && conky -dc ~/.config/conky/time) &>/dev/null
[ -x /usr/bin/conky ] && conky-install.sh

[ -x /usr/bin/dunst ] && dunst &

[ -x /usr/bin/trayer ] && trayer --edge top --align right  --height 19 --widthtype pixel --width 90 --SetDockType true --SetPartialStrut true --transparent true --alpha 0 --tint 0 --monitor primary &

[ -x clip-history ] && clip-history monitor &>/dev/null &

[ -x /usr/bin/pasystray ] && pasystray --notify=all &
[ -x /usr/bin/nm-applet ] && nm-applet >/dev/null &
[ -x /usr/bin/blueman-applet ] && blueman-applet >/dev/null &


#vim -c "mkspell! ~/SystemConfig/Config/vim-spell-programs.add" +qall

[ -f ~/.xprofile ] && . ~/.xprofile
NESTED_SHELL_LEVEL=
[ -f /tmp/mpxmanager.out ] && mv /tmp/mpxmanager.out /tmp/mpxmanager.out.bk
exec $WINDOW_MANAGER &> /tmp/$WINDOW_MANAGER.out
