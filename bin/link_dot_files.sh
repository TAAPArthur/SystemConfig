#!/bin/sh -e

safeLn() {
    srcFile=$1
    target=$2
    linkTarget=$2
    [ -e $srcFile ]
    if [ -d $target ]; then
        target="$target/$(basename $srcFile)"
    fi
    if [ ! -e $target ] || diff -q $srcFile $target; then
        ln -sf $srcFile $linkTarget
    else
        diff -Ny --suppress-common-lines $srcFile $target || true
        echo "Override  and link $target to $srcFile ? Y/n"
        read -r ans
        if [ "$ans" = Y ] || [ "$ans" = y ]; then
            ln -sf $srcFile $linkTarget
        fi
    fi
}

SYSTEM_CONFIG_DIR=${SYSTEM_CONFIG_DIR:-~/SystemConfig}
cd $SYSTEM_CONFIG_DIR/Config
for dotFile in .*; do
    if [ $dotFile = "." ] || [ $dotFile = ".." ]; then
        continue
    elif [ -d $dotFile ]; then
        mkdir -p ~/$dotFile
        for config in "$dotFile"/*; do
            [ -e $config ] && safeLn $PWD/$config ~/$dotFile/
        done
    else
        safeLn $PWD/$dotFile ~/$dotFile
    fi
done
