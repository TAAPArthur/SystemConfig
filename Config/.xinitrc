#!/bin/bash
nitrogen --restore

dbus-update-activation-environment --systemd DISPLAY

[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr


[ -x /usr/bin/lxqt-policykit-agent ] && lxqt-policykit-agent &

xset s 1200 180

export XSECURELOCK_DISCARD_FIRST_KEYPRESS=0
export XSECURELOCK_SHOW_DATETIME=1
export XSECURELOCK_WAIT_TIME_MS=12000
[[ -x /usr/bin/xss-lock && -x /usr/bin/xsecurelock ]] && xss-lock -n '(brightness set .1 && trap "brightness set" EXIT && sleep 1000s)' -- xsecurelock &
[[ -x /usr/bin/xautolock ]] && xautolock -time 30 -detectsleep -locker "systemctl suspend" &

setxkbmap -option compose:ralt

(conky -dc ~/.config/conky/.conkyrc && conky -dc ~/.config/conky/time) &>/dev/null

dunst &

trayer --edge top --align right  --height 19 --widthtype pixel --width 90 --SetPartialStrut true --transparent true --alpha 0 --tint 0 --monitor primary &

clip-history monitor &>/dev/null &

[ -x /usr/bin/pasystray ] && pasystray --notify=all &
[ -x /usr/bin/nm-applet ] && nm-applet >/dev/null &
[ -x /usr/bin/blueman-applet ] && blueman-applet >/dev/null &


vim -c "mkspell! ~/SystemConfig/Config/vim-spell-programs.add" +qall

[ -f ~/.xprofile ] && . ~/.xprofile

mpxmanager &> /tmp/mpxmanager.out
