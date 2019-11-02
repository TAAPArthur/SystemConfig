#!/bin/bash
set -xe
WORKING_DIR=/tmp/pkg
mkdir -p $WORKING_DIR
DEST_DIR=/src/http/Database/
DATABASE=$DEST_DIR/taap.db.tar.gz
TMP=tmp

cd $WORKING_DIR
rm -rdf $TMP
while read -r repo; do
    git clone $repo $TMP
    cd $TMP
    makepkg -sic --noconfirm
    cd ..
    rm -rd $TMP
done< <(cat << EOF
https://github.com/TAAPArthur/MPXManager.git
https://github.com/TAAPArthur/clip-history.git
https://github.com/TAAPArthur/xsane-xrandr.git
https://github.com/TAAPArthur/indent-sort.git
https://github.com/TAAPArthur/dmenu-pycalc.git
https://github.com/TAAPArthur/system-manager.git
https://github.com/TAAPArthur/CrunchyrollGuestPassFinder.git
EOF
)
