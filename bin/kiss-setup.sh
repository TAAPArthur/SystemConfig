#!/bin/sh -e

KISS_REPO_DIR=${KISS_REPO_DIR:-/etc/repo}
mkdir -p $KISS_REPO_DIR

cd /etc/repo

# https://github.com/kisslinux/community
while read -r url name; do
    [ -n "$name" ] || name=$(basename $url .git)
    if [ ! -e "$name" ]; then
        git clone $url $name
    else
        (
        cd $name
        if [ "$(git remote get-url origin)" != "$url" ]; then
            git remote remove origin
            git remote add origin $url
            git fetch
            git stash
            git reset --hard origin/master || git reset --hard origin/main
        fi
        )
    fi
done <<EOF
https://github.com/kisslinux/repo
https://github.com/kiss-community/repo-community community
https://github.com/TAAPArthur/mykiss-repo.git
EOF
