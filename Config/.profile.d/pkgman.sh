#!/bin/sh
# shellcheck disable=SC1091

MY_REPO_DIR=~/Forks/mykiss-repo
[ -d "$MY_REPO_DIR" ] && [ -r /etc/profile.d/00-pkgman.sh ] && . /etc/profile.d/00-pkgman.sh
