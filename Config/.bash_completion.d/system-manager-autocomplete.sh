#!/bin/bash
_systemmanagerAutocomplete ()   #  By convention, the function name
{                 #+ starts with an underscore.
    local cur
    # Pointer to current completion word.
    # By convention, it's named "cur" but this isn't strictly necessary.

    COMPREPLY=()   # Array variable storing the possible completions.
    cur=${COMP_WORDS[COMP_CWORD]}
    firstArg=${COMP_WORDS[1]}
    COMPREPLY=( $( compgen -W "init install link link-normal link-root list-groups list-known list-installed list-uninstalled list-unknown clean -h -v --help --version" -- $cur ) )
    return 0
    
}
complete -F _systemmanagerAutocomplete system-manager 

