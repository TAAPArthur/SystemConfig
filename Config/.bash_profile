#!/bin/bash

# invoked on login
# sourced from bashrc

if [ ! -z "$PS1" ] && [ -z "$SSH_CLIENT$SSH_TTY" ]; then
    shopt -q login_shell && kbdrate -d 100 -r 20
fi

# source iff interactive shell
if [ -f $HOME/.bashrc ] && [ ! -z "$PS1" ]; then
    source $HOME/.bashrc
fi

# global variables
export ABDUCO_CMD=bash
export ABDUCO_SOCKET_DIR=~/.config
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

steam() { sudo -u nonfree steam; }
export -f steam
spawn-app() { $BROWSER --class $BROSWER-app --name $1 $*; }
runOrRaise() { mpxmanager find-and-raise $1 || mpxmanager spawn ${2:-1}; }

if [ -f $HOME/.profile ]; then
    source $HOME/.profile
fi

if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
  exec startx
fi
