#!/bin/sh

# PATH adjustments
PATH=~/.local/bin:$PATH:~/SystemConfig/bin

# global variables
export ABDUCO_CMD="ksh"
export ABDUCO_SOCKET_DIR=/tmp/.abduco
export BASH_COMPLETION_USER_FILE=~/SystemConfig/bash_completion.d
export BROWSER="st -c browser elinks"
export BROWSER_ALT=firefox
export EDITOR=vim
export ELINKS_CONFDIR=~/.config/elinks
export LEDGER_FILE=~/Documents/Accounting/ledger
export MOZ_USE_XINPUT2=1
export NMENU_MENU_ARGS="-l 4 -g 4"
export PASSWORD_STORE_DIR=~/.config/tpm
export PICTURES_DIR=~/Pictures
export PKG_MANAGER=pacaur
export QT_STYLE_OVERRIDE=adwaita-dark
export TERMINAL=st
export VIDEO_PAYER=mpv
export VIMINIT="source ~/.config/vim/vimrc"
export WINDOW_MANAGER=mpxmanager

#shell vars
export HISTCONTROL=ignoredups
export HISTFILE=~/.local/share/sh_history
export HISTSIZE=
export HISTFILESIZE=               # big big history
export HISTTIMEFORMAT="[%F %T]"
export ENV=~/.config/shell/shellrc

# Load profiles from /etc/profile.d
for file in ~/.profile.d/*.sh; do
    [ -r "$file" ] && . "$file"
done

if [ -z "$DISPLAY" ] && [ -x /usr/bin/xinit ] && [ "$(tty)" = "/dev/tty1" ]; then
    tty=$(tty)
    num=${tty#/dev/tty}
    display=$((num-1))
    exec xinit ~/.xinitrc  -- ~/.xserverrc :$display "vt$num" -keeptty
fi
