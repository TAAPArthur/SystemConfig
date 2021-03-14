#!/bin/sh

# Kiss linux specific vars
REPO_HOME=~/Forks/mykiss-repo
KISS_PATH_PREFIX=$REPO_HOME/personal:$REPO_HOME/forks:$REPO_HOME/prebuilt:$REPO_HOME/ban:$REPO_HOME/meta_packages
export KISS_PATH=$KISS_PATH_PREFIX:$REPO_HOME/extra:$REPO_HOME/python:$REPO_HOME/nonfree:$REPO_HOME/fallback:$KISS_PATH
export KISS_FALLBACK_REPO=~/Forks/mykiss-repo/fallback
