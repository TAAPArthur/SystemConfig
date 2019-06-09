#!/bin/bash
nitrogen --restore

dbus-update-activation-environment --systemd DISPLAY
eval $(/usr/bin/gnome-keyring-daemon --start --components=pkcs11,secrets,ssh)
export SSH_AUTH_SOCK

[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr

export QT_STYLE_OVERRIDE=gtk2

[ -x /usr/bin/lxqt-policykit-agent ] && lxqt-policykit-agent &

xset s 1200 1500

export XSECURELOCK_DISCARD_FIRST_KEYPRESS=0
export XSECURELOCK_SHOW_DATETIME=1
export XSECURELOCK_WAIT_TIME_MS=12000
[ -x /usr/bin/xss-lock ] && xss-lock -n /usr/lib/xsecurelock/dimmer -l -- xsecurelock &
[ -x /usr/bin/xautolock ] && xautolock -time 30 -detectsleep -locker "systemctl suspend" &

[[ -f ~/.Xmodmap ]] && xmodmap ~/.Xmodmap

[ -x /usr/bin/conky ] && conky -dc ~/.config/conky/.conkyrc &>/dev/null && conky -dc ~/.config/conky/time &>/dev/null

[ -x dunst ] && dunst &

trayer --edge top --align right  --height 19 --widthtype pixel --width 90 --SetPartialStrut true --transparent true --alpha 0 --tint 0 --monitor primary &

[ -x clip-history ] && clip-history monitor &>/dev/null &

[ -x /usr/bin/pasystray ] && pasystray --notify=all &
[ -x /usr/bin/nm-applet ] && nm-applet >/dev/null &
[ -x /usr/bin/blueman-applet ] && blueman-applet >/dev/null &


[ -x vim ] && vim -c "mkspell! ~/SystemConfig/Config/vim-spell-programs.add" +qall
#pyinotify-runner &
#sync-clip &

$TERMINAL &

mpxmanager &> /tmp/mpxmanager.out
