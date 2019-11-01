#!/bin/bash

# invoked on login
# source from bashrc

[ -x /usr/bin/gnome-keyring-daemon ] && eval $(/usr/bin/gnome-keyring-daemon --start --components=pkcs11,secrets,ssh)
export SSH_AUTH_SOCK

shopt -q login_shell && kbdrate -d 100 -r 20

if [ -f $HOME/.bashrc ]; then
    source $HOME/.bashrc
fi

# global variables
export ABDUCO_CMD=bash
export BROWSER=firefox
export BROWSER_SECONDARY=google-chrome-stable
export EDITOR=vim
export SUSPEND_TIME=10
export FILE_MANAGER=pcmanfm
export LEDGER_FILE=~/Documents/Accounting/ledger
export PKG_MANAGER=pacaur
export QT_STYLE_OVERRIDE=gtk2
export TERMINAL=terminator
export XSECURELOCK_DISCARD_FIRST_KEYPRESS=0
export XSECURELOCK_SHOW_DATETIME=1
export XSECURELOCK_WAIT_TIME_MS=12000

google-chrome() { google-chrome-stable; }
export -f google-chrome

if [ -f $HOME/.profile ]; then
    source $HOME/.profile
fi

if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
  exec startx
fi
