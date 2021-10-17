#!/bin/sh
# shellcheck disable=SC1091

MY_REPO_DIR=~/Forks/mykiss-repo
[ -e "$MY_REPO_DIR" ] && [ -r /etc/profile.d/00-kiss.sh ] && . /etc/profile.d/00-kiss.sh
export KISS_FALLBACK_REPO=~/Forks/mykiss-repo/fallback
