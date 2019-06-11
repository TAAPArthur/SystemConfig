#!/bin/bash

eval $(/usr/bin/gnome-keyring-daemon --start --components=pkcs11,secrets,ssh)
export SSH_AUTH_SOCK

kbdrate -d 100 -r 20

if [ -f $HOME/.bashrc ]; then
    source $HOME/.bashrc
fi


if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
  exec startx
fi
