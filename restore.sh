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

sudo pacman -S git vim jq --needed

PATH=/home/$user/.local/bin:$PATH

su $user -P -c "system-manager init"
su $user -P -c "system-manager link-normal"
su $user --login -P -c "system-manager install"
su $user -P -c "system-manager link-root"
