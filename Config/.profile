#!/bin/sh

if [ ! -z "$PS1" ] && [ -z "$SSH_CLIENT$SSH_TTY" ] && [ -x /bin/kbdrate ]; then
    shopt -q login_shell && kbdrate -d 100 -r 20
fi

# PATH adjustments
if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
    PATH=~/.local/bin:$PATH
fi
if [[ ":$PATH:" != *":$HOME/SystemConfig/bin:"* ]]; then
    PATH=$PATH:~/SystemConfig/bin
fi
# global variables
export ABDUCO_CMD=bash
export ABDUCO_SOCKET_DIR=~/.config
export BASH_COMPLETION_USER_FILE=~/SystemConfig/bash_completion.d
export BROWSER=vimb
export BROWSER_ALT=firefox
export EDITOR=vim
export LEDGER_FILE=~/Documents/Accounting/ledger
export MOZ_USE_XINPUT2=1
export MPX_LDFLAGS="-lsgestures -lm"
export PASSWORD_STORE_DIR=~/.config/tpm
export PKG_MANAGER=pacaur
export PICTURES_DIR=~/Pictures
export QT_STYLE_OVERRIDE=adwaita-dark
export SUSPEND_TIME=30
export TASKDATA=~/Documents/Tasks/.task
export TASKRC=~/Documents/Tasks/.taskrc
export TERMINAL=st
export VIDEO_PAYER=mpv
export WINDOW_MANAGER=mpxmanager
export XSECURELOCK_DISCARD_FIRST_KEYPRESS=0
export XSECURELOCK_SHOW_DATETIME=1
export XSECURELOCK_WAIT_TIME_MS=12000

#shell vars
export HISTCONTROL=ignoredups
export HISTSIZE=
export HISTFILESIZE=               # big big history
export HISTTIMEFORMAT="[%F %T] "
shopt -s histappend

# Prompt and title
_oldTile=""
title(){
    if [ "$_EXIT_CODE" -eq 148 ] || [ "$_oldTile" != "$*" ] ;then
        echo -ne "\033]0;$*\007"
        _oldTile="$*"
    fi
}
export -f title
attach() { NESTED_SHELL_LEVEL= SESSION_NAME="$1:" abduco -A $1; }
export -f attach
export PROMPT_COMMAND='_EXIT_CODE=$?;history -a;title $NESTED_SHELL_LEVEL_STR$SESSION_NAME$PWD;'

# Load profiles from /etc/profile.d
for file in ~/.profile.d/*.sh; do
    [ -r "$file" ] && . "$file"
done

if [ -z $DISPLAY ] && [ "$SHLVL" -eq 1 ] && [ -x /bin/startx ] && [ $(tty) = "/dev/tty1" ]; then
  exec startx
fi
