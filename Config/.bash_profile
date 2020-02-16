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
export BROWSER=firefox
export BROWSER_ALT=firefox
export EDITOR=vim
export LEDGER_FILE=~/Documents/Accounting/ledger
export PKG_MANAGER=pacaur
export QT_STYLE_OVERRIDE=adwaita-dark
export SUSPEND_TIME=30
export TASKDATA=~/Documents/Tasks/.task
export TASKRC=~/Documents/Tasks/.taskrc
export TERMINAL=terminator
export WINDOW_MANAGER=mpxmanager
export XSECURELOCK_DISCARD_FIRST_KEYPRESS=0
export XSECURELOCK_SHOW_DATETIME=1
export XSECURELOCK_WAIT_TIME_MS=12000

steam() { sudo -u nonfree steam; }
export -f steam

if [ -f $HOME/.profile ]; then
    source $HOME/.profile
fi

if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
  exec startx
fi
