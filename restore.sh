#~/bin/sh
set -xe

export USER=${USER:-arthur}

if [[ "$1" == "update" || "$1" == "-u" ]]; then
    cd /tmp
    [[ -d SystemConfig ]] && (cd SystemConfig && git pull) || git clone https://github.com/TAAPArthur/SystemConfig.git
    cd SystemConfig
    echo "Newest version is in $(pwd)/restore.sh"
    shift
    exec ./restore.sh $@
    exit 1
fi



root=$1
[ ! -z $root ]
shift
export PKG_MANAGER=aurman
if [ -x bin/system-manager ]; then
    BIN=bin/system-manager
else
    [ -d /$root/SystemConfig ] || (cd /$root && git clone https://github.com/TAAPArthur/SystemConfig.git)
    BIN=/$root/SystemConfig/bin/system-manager
fi
[ -x bin/system-manager ]

arg=$1
shift
case "$arg" in
    1|make|prebootstrap)
        $BIN bootstrap $PACSTRAP_ARGS $root
        sudo arch-chroot $root pacman-key --populate archlinux
        sudo arch-chroot $root pacman-key --populate artix
        ;;
    bootstrap)
        sudo -E arch-chroot $root pacman -S --needed $($BIN get $1)
        ;;
    install|2)
        $BIN pre-install $PACSTRAP_ARGS $root
        sudo -E arch-chroot -u $USER $root /bin/$PKG_MANAGER -S --needed $($BIN get $1)
        ;;
esac

#su $USER -P -c "$BIN/system-manager link-normal"
