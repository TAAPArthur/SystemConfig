#!/bin/bash
# invoked on login

if [ -f $HOME/.profile ]; then
    source $HOME/.profile
fi
# source iff interactive shell
if [ -f $HOME/.bashrc ] && [ ! -z "$PS1" ]; then
    source $HOME/.bashrc
fi
