#!/bin/sh

# Kiss linux specific vars
REPO_HOME=~/Forks/mykiss-repo
export KISS_PATH=$REPO_HOME/personal:$REPO_HOME/forks:$REPO_HOME/prebuilt:$REPO_HOME/ban:$REPO_HOME/meta_packages:$KISS_PATH_ORIGINAL
KISS_PATH=$KISS_PATH:$REPO_HOME/extra:$REPO_HOME/python
KISS_PATH=$KISS_PATH:$REPO_HOME/fallback
KISS_PATH=$KISS_PATH:/etc/repo/kiss-all/audio
KISS_PATH=$KISS_PATH:$REPO_HOME/nonfree
export KISS_FALLBACK_REPO=~/Forks/mykiss-repo/fallback
