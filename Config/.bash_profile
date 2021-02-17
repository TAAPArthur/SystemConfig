#!/bin/bash
# invoked on login

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
attach() { NESTED_SHELL_LEVEL="" SESSION_NAME="$1:" abduco -A $1; }
export -f attach
export PROMPT_COMMAND='_EXIT_CODE=$?;history -a;title $NESTED_SHELL_LEVEL_STR$SESSION_NAME$PWD;'

if [ -f $HOME/.profile ]; then
    source $HOME/.profile
fi

# source iff interactive shell
if [ -f $HOME/.bashrc ] && [ -n "$PS1" ]; then
    source $HOME/.bashrc
fi
if [ -z "$DISPLAY" ] && [ "$SHLVL" -eq 1 ] && [ -x /bin/startx ] && [ "$(tty)" = "/dev/tty1" ]; then
  exec startx
fi
