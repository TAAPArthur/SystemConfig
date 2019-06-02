#!/bin/bash

if [ -n "$DESKTOP_SESSION" ];then
    eval $(gnome-keyring-daemon --start)
    export SSH_AUTH_SOCK
fi

kbdrate -d 100 -r 20

#export PS1='\u@\h:\w\$ '
#export PS1='\u:\w\$ '

# Save and reload the history after each command finishes
#echo "PROMPT_COMMAND $PROMPT_COMMAND"

#echo "PROMPT_COMMAND $PROMPT_COMMAND"


#export TERM=xfce4-terminal

if [ -f $HOME/.bashrc ]; then
        source $HOME/.bashrc
fi


