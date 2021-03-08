#!/bin/bash

# load bash completion
for path in /usr/share/bash-completion/completions ~/.local/completions/bash; do
    if [ -d $path ]; then
        for file in "$path"/*; do
            source "$file"
        done
    fi
done

silent_pushd()
{
  if [ $# -eq 0 ]; then
    DIR="${HOME}"
  else
    DIR="$1"
  fi

  # shellcheck disable=SC2164
  pushd "${DIR}" > /dev/null
}

silent_popd()
{
  popd > /dev/null || popd -n > /dev/null
}

#interactive shell specific aliases
alias cd='silent_pushd'
alias bk='silent_popd'
alias tg='pushd > /dev/null'
alias oneline="git log --oneline"
alias git-branch-recent="git branch --sort=-committerdate |head -n10"
attach() { NESTED_SHELL_LEVEL='' SESSION_NAME="$1:" abduco -A $1; }
export -f attach
find-replace() {
    find . -name "$1" -exec sed -Ei "$2" {} \;
}
find-replace-dryrun() {
    find . -name "$1" -exec sed -En "$2" {} \;
}

export NESTED_SHELL_LEVEL=$((${NESTED_SHELL_LEVEL:--1}+1));

red='\001\e[1;31m\002'
green='\001\e[1;32m\002'
blue='\001\e[1;34m\002'
cyan='\001\e[1;36m\002'
end='\001\e[0m\002'
[ "$NESTED_SHELL_LEVEL" -eq 0 ] || NESTED_SHELL_LEVEL_STR="$NESTED_SHELL_LEVEL"
#interactive shell specific variables
# shellcheck disable=SC2154
alias get_branch='branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null) && echo -en $green[$branch]'
export PS1="$cyan$NESTED_SHELL_LEVEL_STR$end"'$(get_branch)'"$cyan$SESSION_NAME$blue\u@\h$end:"'$( [[ "$_EXIT_CODE" -eq 0 ]] && echo -en $green || echo -en $red"($_EXIT_CODE)")'"\w$end$ "


# Allow C-S to cycle forward the command history
stty -ixon

# If there are multiple matches for completion, Tab should cycle through them

bind '\C-SPACE':menu-complete;
bind 'TAB':menu-complete

# Display a list of the matching files

bind "set show-all-if-ambiguous on"

# Perform partial completion on the first Tab press,
# only start cycling full results on the second Tab press

bind "set menu-complete-display-prefix on"


#if [ -f $HOME/.profile ]; then
#    source $HOME/.profile
#fi
