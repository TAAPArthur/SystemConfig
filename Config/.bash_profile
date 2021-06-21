#!/bin/bash
# invoked on login

if [ -f ~/.profile ]; then
    source ~/.profile
fi

shopt -s histappend

# source iff interactive shell
if [ -f ~/.bashrc ] && [ -n "$PS1" ]; then
    source ~/.bashrc
fi

if [ -z "$DISPLAY" ] && [ "$SHLVL" -eq 1 ] && [ -x /bin/startx ] && [ "$(tty)" = "/dev/tty1" ]; then
  exec startx
fi
