#!/bin/bash

[ -f /usr/share/doc/pkgfile/command-not-found.bash ] && source /usr/share/doc/pkgfile/command-not-found.bash

pushd()
{
  if [ $# -eq 0 ]; then
    DIR="${HOME}"
  else
    DIR="$1"
  fi

  builtin pushd "${DIR}" > /dev/null
}

pushd_builtin()
{
  builtin pushd > /dev/null
}
popd()
{
  builtin popd > /dev/null
}

#interactive shell specific aliases
alias cd='pushd'
alias bk='popd'
alias tg='pushd_builtin'
alias grep='grep --color=auto'
alias oneline="git log --oneline"
alias git-branch-recent="git branch --sort=-committerdate |head -n10"
alias ls='ls --color=auto'
ls-new() { ls -Alsth | head -n"$((${*:-1}+1))" | tail -n+2; }
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
grn='\001\e[1;32m\002'
blu='\001\e[1;34m\002'
cyn='\001\e[1;36m\002'
end='\001\e[0m\002'
[ "$NESTED_SHELL_LEVEL" -eq 0 ] || NESTED_SHELL_LEVEL_STR="$NESTED_SHELL_LEVEL"
#interactive shell specific variables
alias get-branch='branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null) && echo -en "$grn""[$branch]"'
export PS1="$cyn$NESTED_SHELL_LEVEL_STR$end"'$(get-branch)'"$cyn$SESSION_NAME$blu\u@\h$end:"'$( [[ "$_EXIT_CODE" -eq 0 ]] && echo -en $grn || echo -en $red"($_EXIT_CODE)")'"\w$end$ "

stty -ixon

bind '\C-SPACE':menu-complete;
