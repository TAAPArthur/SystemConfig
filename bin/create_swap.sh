#!/bin/sh -e

SWAP_FILE=${SWAP_FILE:-$KISS_ROOT/.swapfile}
if ! grep -q swap "$KISS_ROOT/etc/fstab"; then
    if [ ! -e $SWAP_FILE ]; then
        # Create a 4GB swap file by default
        SWAP_FILE_SIZE=${SWAP_FILE_SIZE:-4096}
        dd if=/dev/zero of=$KISS_ROOT/$SWAP_FILE bs=1M count=$SWAP_FILE_SIZE status=progress
        chmod 600 $KISS_ROOT/$SWAP_FILE
        mkswap $KISS_ROOT/$SWAP_FILE
        swapon $KISS_ROOT/$SWAP_FILE
        echo "$SWAP_FILE none swap defaults 0 0" >> $KISS_ROOT/etc/fstab
    else
        echo "Swap file $SWAP_FILE exists"
    fi
else
    echo "Swap file is already mentioned in /etc/fstab"
fi
