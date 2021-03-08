#!/bin/bash

_attachAutocomplete ()   #  By convention, the function name
{                 #+ starts with an underscore.
    # shellcheck disable=SC2207
    COMPREPLY=( $(abduco | tail -n +2 | awk '{print $4}') )
}
complete -F _attachAutocomplete attach
