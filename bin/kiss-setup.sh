#!/bin/sh -e

KISS_REPO_DIR=${KISS_REPO_DIR:-/etc/repo}
mkdir -p "$KISS_REPO_DIR"

cd /etc/repo

# https://github.com/kisslinux/community
while read -r url name; do
    [ -n "$name" ] || name="$(basename "$url" .git)"

    if [ -e "$name" ]; then
        if [ "$(cd "$name"; git remote get-url origin)" = "$url" ]; then
            continue
        else
            rm -r "$name"
        fi
    fi
    git clone --single-branch "$url" "$name"
done <<EOF
https://github.com/kiss-community/repo
https://github.com/kiss-community/repo-community community
https://github.com/ehawkvu/kiss-xorg
https://github.com/TAAPArthur/mykiss-repo
EOF
