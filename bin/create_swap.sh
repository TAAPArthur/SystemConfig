#!/bin/sh -ex

SWAP_FILE=${SWAP_FILE:-$PKGMAN_ROOT/.swapfile}
if [ ! -e "$SWAP_FILE" ]; then
    # Create a 4GB swap file by default
    SWAP_FILE_SIZE=${SWAP_FILE_SIZE:-4096}
    dd if=/dev/zero of="$PKGMAN_ROOT/$SWAP_FILE" bs=$((1<<20)) count="$SWAP_FILE_SIZE"
    chmod 600 "$PKGMAN_ROOT/$SWAP_FILE"
    mkswap "$PKGMAN_ROOT/$SWAP_FILE"
    swapon "$PKGMAN_ROOT/$SWAP_FILE"
else
    echo "Swap file $SWAP_FILE exists"
fi
if ! grep -q swap "$PKGMAN_ROOT/etc/fstab"; then
    echo "$SWAP_FILE none swap defaults 0 0" >> "$PKGMAN_ROOT/etc/fstab"
else
    echo "Swap file is already mentioned in /etc/fstab"
fi
