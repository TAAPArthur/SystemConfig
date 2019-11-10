#~/bin/bash
set -xe

user=${user:-arthur}

if [[ "$1" == "update" || "$1" == "-u" ]]; then
    cd /tmp
    [[ -d SystemConfig ]] && (cd SystemConfig && git pull) || git clone https://github.com/TAAPArthur/SystemConfig.git
    cd SystemConfig
    ./restore.sh
    echo "Newest version is in $(pwd)/restore.sh"
    exit 0
fi

if [[ "$1" == "--non-free" ]]; then
	if grep -q nonfree /etc/passwd;then
	    usermod -G games,users,input,video nonfree
	else
	    useradd -m -G games,users,input,video nonfree
	fi
    exit 0
fi
if grep -q $user /etc/passwd;then
    usermod -G sys,disk,lp,wheel,rfkill,video,optical,storage,scanner,input,users $user
else
    useradd -m -G sys,disk,lp,wheel,rfkill,video,optical,storage,scanner,input,users $user
    passwd -d $user
fi
echo "Remember to use visudo to allow wheel to use sudo"

sudo pacman -S git vim jq --needed

su $user -c "cd ~;[[ -d SystemConfig ]] && (cd SystemConfig && git pull) || git clone https://github.com/TAAPArthur/SystemConfig.git"
cd /tmp
pacman -Qs system-manager >>/dev/null || su $user -c "[[ -d /tmp/system-manager ]] || (git clone https://github.com/TAAPArthur/system-manager.git && cd /tmp/system-manager && makepkg -sc;)" && pacman -U /tmp/system-manager/*.tar.xz --needed

su $user -P -c "system-manager init"
su $user -P -c "system-manager link-normal"
su $user --login -P -c "system-manager install"
su $user -P -c "system-manager link-root"
