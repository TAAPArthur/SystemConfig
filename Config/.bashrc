#!/bin/bash

# load bash completion
for path in /usr/share/bash-completion/completions ~/.local/completions/bash; do
    if [ -d $path ]; then
        for file in "$path"/*; do
            source "$file"
        done
    fi
done

[ -f ~/.shellrc ] && source ~/.shellrc

PROMPT_COMMAND='_EXIT_CODE=$?;history -a;printf "\033]0;%s\007" "$NESTED_SHELL_LEVEL_STR$SESSION_NAME$PWD"'
PS1="$CYAN$NESTED_SHELL_LEVEL_STR$END"'$(get_branch)'"$CYAN$SESSION_NAME$BLUE\u@\h$END:"'$( [ "$_EXIT_CODE" -eq 0 ] && echo -en $GREEN || echo -en $RED"($_EXIT_CODE)")'"\w$END$ "

# If there are multiple matches for completion, Tab should cycle through them

bind '\C-SPACE':menu-complete;
bind 'TAB':menu-complete

# Display a list of the matching files
bind "set show-all-if-ambiguous on"

# Perform partial completion on the first Tab press,
# only start cycling full results on the second Tab press
bind "set menu-complete-display-prefix on"
