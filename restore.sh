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
fi

sudo pacman -S git jq --needed

su arthur -c "cd ~;[[ -d SystemConfig ]] || git clone https://github.com/TAAPArthur/SystemConfig.git;SystemConfig/system-manager.sh run-scripts"
