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
  #echo -n "DIRSTACK: "
  #dirs
}

pushd_builtin()
{
  builtin pushd > /dev/null
  #echo -n "DIRSTACK: "
  #dirs
}
popd()
{
  builtin popd > /dev/null
  #echo -n "DIRSTACK: "
  #dirs
}

alias cd='pushd'
alias bk='popd'
alias tg='pushd_builtin'
export PS1='\u@\h:\w\$ '

bind '\C-SPACE':menu-complete;

export HISTCONTROL=ignoredups 
export HISTSIZE=
export HISTFILESIZE=               # big big history
export HISTTIMEFORMAT="[%F %T] "
shopt -s histappend
#export PROMPT_COMMAND="history -a; history -c; history -r; $PROMPT_COMMAND"
#export PROMPT_COMMAND="history -a; $PROMPT_COMMAND"
export PROMPT_COMMAND='history -a; printf "\033]0;%s@%s:%s\007" "${USER}" "${HOSTNAME%%.*}" "${PWD/#$HOME/\~}"'

export EDITOR=vim
export FILE_MANAGER=pcmanfm
export BROWSER=firefox
export BROWSER_SECONDARY=google-chrome-stable
export TERMINAL=terminator
export _JAVA_AWT_WM_NONREPARENTING=1
export LEDGER_FILE=~/Documents/Accounting/ledger
export PKG_MANAGER=pacaur

#PATH=~/.local/bin:$PATH
