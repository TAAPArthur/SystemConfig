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
