#~/bin/bash
set -xe

if grep -q nonfree /etc/passwd;then
    usermod -G games,users,input,video nonfree
else
    useradd -m -G games,users,input,video nonfree
fi
if grep -q arthur /etc/passwd;then
    usermod -G sys,disk,lp,wheel,rfkill,video,optical,storage,scanner,input,users arthur
else
    useradd -m -G sys,disk,lp,wheel,rfkill,video,optical,storage,scanner,input,users arthur
    passwd -d arthur
fi
echo "Remember to use visudo to allow wheel to use sudo"

sudo pacman -S git vim jq --needed

su arthur -c "cd ~;[[ -d SystemConfig ]] || git clone https://github.com/TAAPArthur/SystemConfig.git"
cd /tmp
su -p arthur -c "[[ -d /tmp/system-manager ]] || (git clone https://github.com/TAAPArthur/system-manager.git && cd /tmp/system-manager && makepkg -sc;)"
pacman -U /tmp/system-manager/*.tar.xz --needed

su arthur -P -c "system-manager init"
su arthur --login -P -c "system-manager install"
su arthur -P -c "system-manager link"
