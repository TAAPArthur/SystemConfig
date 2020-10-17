#!/bin/bash

DIR=${ABDUCO_SOCKET_DIR:-~/.abduco}
_attachAutocomplete ()   #  By convention, the function name
{                 #+ starts with an underscore.
    local cur
    # Pointer to current completion word.
    # By convention, it's named "cur" but this isn't strictly necessary.

    COMPREPLY=()   # Array variable storing the possible completions.
    cur=${COMP_WORDS[COMP_CWORD]}
    COMPREPLY=( $(compgen -W "$(ls $DIR)" -- $cur))
    return 0

}
complete -F _attachAutocomplete attach
