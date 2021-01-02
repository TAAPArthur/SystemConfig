#!/bin/bash
exec > ~/.xinitrc.out 2> ~/.xinitrc.err

[ -x /usr/bin/xsetroot ] && xsetroot -cursor_name left_ptr

[ -x /usr/bin/lxqt-policykit-agent ] && lxqt-policykit-agent &

xset s 1200 180

if [[ "$SUSPEND_TIME" -ne "0" ]]; then
     [[ -x /usr/bin/xss-lock && -x /usr/bin/xsecurelock ]] && xss-lock -n '(brightness set .1 && trap "brightness set")' -- xsecurelock &
     [[ -x /usr/bin/xautolock ]] && xautolock -time ${SUSPEND_TIME:-30} -detectsleep -locker "systemctl suspend" &
fi

# setxkbmap -option compose:ralt

[ -x /usr/bin/dunst ] && dunst &

[ -x /usr/bin/trayer ] && trayer --edge top --align right --height 18 --widthtype pixel --width 90 --SetDockType true --SetPartialStrut true --transparent true --alpha 0 --tint 0 --monitor primary &

[ -x /usr/bin/clip-history ] && clip-history monitor &

[ -x /usr/bin/pasystray ] && pasystray --notify=all &
[ -x /usr/bin/nm-applet ] && nm-applet &
[[ -x /usr/bin/xhost && -x /usr/bin/steam ]] && xhost +
[ -x /usr/bin/dzen2 ] && [ -x /usr/bin/mystatusbar ] && mystatusbar | status-bar -title-name "mystatusbar" &
#[ -x /usr/bin/dzen2 ] && mystatusbar | dzen2 -ta l -fg '#00FF00' -bg '#000000' -dock -fn 'xft:Bitstream Vera Sans Mono:size = 10:antialias = true' -h 18 -title-name "mystatusbar" &

[ -x /usr/bin/xmouse-control ] && xmouse-control &


#vim -c "mkspell! ~/SystemConfig/Config/vim-spell-programs.add" +qall

[ -f ~/.xprofile ] && . ~/.xprofile
NESTED_SHELL_LEVEL=
outputFile=/tmp/$WINDOW_MANAGER.out.${DISPLAY:1}
[ -f $outputFile ] && mv $outputFile $outputFile.bk
#$TERMINAL
exec $WINDOW_MANAGER &> $outputFile
