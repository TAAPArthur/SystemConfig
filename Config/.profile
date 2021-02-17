#!/bin/sh

# PATH adjustments
PATH=~/.local/bin:$PATH
PATH=$PATH:~/SystemConfig/bin

# global variables
export ABDUCO_CMD=bash
export ABDUCO_SOCKET_DIR=~/.config
export BASH_COMPLETION_USER_FILE=~/SystemConfig/bash_completion.d
export BROWSER=vimb
export BROWSER_ALT=firefox
export EDITOR=vim
export LEDGER_FILE=~/Documents/Accounting/ledger
export MOZ_USE_XINPUT2=1
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

# Load profiles from /etc/profile.d
for file in ~/.profile.d/*.sh; do
    [ -r "$file" ] && . "$file"
done


