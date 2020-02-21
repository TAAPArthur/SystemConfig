#~/bin/bash
set -xe

user=${user:-arthur}

if [[ ! -z "$1" ]]; then
    if [[ "$1" == "update" || "$1" == "-u" ]]; then
        cd /tmp
        [[ -d SystemConfig ]] && (cd SystemConfig && git pull) || git clone https://github.com/TAAPArthur/SystemConfig.git
        cd SystemConfig
        ./restore.sh
        echo "Newest version is in $(pwd)/restore.sh"
        exit 0
    elif [[ "$1" =~ (--)?non-?free ]]; then
        if grep -q nonfree /etc/passwd;then
            usermod -G games,audio,users,input,video nonfree
        else
            useradd -m -G games,audio,users,input,video nonfree
        fi
        exit 0
    else
        echo "unknown option $1"
        exit 1
    fi

fi
if grep -q $user /etc/passwd;then
    usermod -G sys,disk,lp,wheel,rfkill,video,optical,storage,scanner,input,users $user
else
    useradd -m -G sys,disk,lp,wheel,rfkill,video,optical,storage,scanner,input,users $user
    passwd -d $user
fi
echo "Remember to use visudo to allow wheel to use sudo"


if uname -a | grep -q aarch64 && ! pacman -Qq archlinuxarm-keyring; then
    pacman -S archlinuxarm-keyring
    pacman-key --populate archlinuxarm
fi

sudo pacman -S git base-devel wget vim jq sudo --needed

[[ -d /home/$user/SystemConfig ]] || (cd /home/$user && su $user -c "git clone https://github.com/TAAPArthur/SystemConfig.git")

TMP_AUR_PATH=/tmp/aur
if ! which aur ; then
   su $user -c "
   mkdir -p $TMP_AUR_PATH
   cd $TMP_AUR_PATH
   wget -nc https://aur.archlinux.org/cgit/aur.git/snapshot/aurutils.tar.gz
   tar -xf aurutils.tar.gz
   chmod 777 -R $TMP_AUR_PATH
   (cd aurutils;pwd; makepkg --skippgpcheck -sic)
   "
fi
if ! which pacaur ; then
   su $user -c "
   mkdir -p $TMP_AUR_PATH
    cd $TMP_AUR_PATH
    aur fetch pacaur
    aur fetch auracle-git
   chmod 777 -R $TMP_AUR_PATH
   which auracle || (cd auracle-git;pwd; makepkg -sic)
   (cd pacaur;pwd; makepkg -sic)
   "
fi

BIN=/home/$user/SystemConfig/bin
su $user -P -c "$BIN/system-manager init"
su $user -P -c "$BIN/system-manager link-normal"
su $user --login -P -c "$BIN/system-manager install"
su $user -P -c "$BIN/system-manager link-root"
