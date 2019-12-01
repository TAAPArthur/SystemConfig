#!/bin/bash
shopt -s histappend
if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
    PATH=~/.local/bin:$PATH
fi
if [[ ":$PATH:" != *":$HOME/SystemConfig/bin:"* ]]; then
    PATH=$PATH:~/SystemConfig/bin
fi

[[ -f /usr/share/doc/pkgfile/command-not-found.bash ]] && source /usr/share/doc/pkgfile/command-not-found.bash


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
alias highlight="grep --color -E -e '^|' -e"
alias grep='grep --color=auto'
alias ls='ls --color=auto'
alias ls-new='ls -t | head -n1'
attach() { NESTED_SHELL_LEVEL= SESSION_NAME="$1:" abduco -A $1; }
export -f attach

export HISTCONTROL=ignoredups
export HISTSIZE=
export HISTFILESIZE=               # big big history
export HISTTIMEFORMAT="[%F %T] "
shopt -s histappend
#export PROMPT_COMMAND="history -a; history -c; history -r; $PROMPT_COMMAND"
#export PROMPT_COMMAND="history -a; $PROMPT_COMMAND"
export PROMPT_COMMAND='_EXIT_CODE=$?;history -a;'
export NESTED_SHELL_LEVEL=$((${NESTED_SHELL_LEVEL:--1}+1));

red='\001\e[1;31m\002'
grn='\001\e[1;32m\002'
blu='\001\e[1;34m\002'
cyn='\001\e[1;36m\002'
end='\001\e[0m\002'
[[ "$NESTED_SHELL_LEVEL" -eq 0 ]] || NESTED_SHELL_LEVEL_STR="$cyn$NESTED_SHELL_LEVEL$end"
#interactive shell specific variables
alias get-branch='branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null) && echo -en "$grn""[$branch]"'
export PS1="$NESTED_SHELL_LEVEL_STR"'$(get-branch)'"$cyn$SESSION_NAME$blu\u@\h$end:"'$( [[ "$_EXIT_CODE" -eq 0 ]] && echo -en $grn || echo -en $red"($_EXIT_CODE)")'"\w$end$ "

bind '\C-SPACE':menu-complete;

if [ -f $HOME/.profile ]; then
    source $HOME/.profile
fi
