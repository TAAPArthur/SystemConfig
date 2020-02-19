set -xe

device=$1
[ ! -z $device ]
[ -e $device ] && [ -e "$device"1 ]

BOOT_LABEL=PHONE_BOOT
ROOT_LABEL=PHONE_ROOT
BOOT_PART="$device"1
ROOT_PART="$device"2


sfdisk $device <<EOF
label: dos
unit: sectors

4MiB,252MiB,
256MiB,,
EOF

mkfs.vfat -n $BOOT_LABEL $BOOT_PART
mkfs.f2fs -f -l $ROOT_LABEL $ROOT_PART

mount $ROOT_PART /mnt
mkdir -p /mnt/boot
mount $BOOT_PART /mnt/boot

tar -xpf ArchLinuxARM-aarch64-latest.tar.gz -C /mnt

echo "$BOOT_LABEL /boot vfat rw 0 1" >> /mnt/etc/fstab
echo "$ROOT_LABEL / f2fs rw,noatime	0 0" >> /mnt/etc/fstab
echo "g_cdc" > /mnt/etc/modules-load.d/usb0.conf
mkdir /mnt/root/.ssh/
cat /home/awilliams/.ssh/id_rsa.pub >> /mnt/root/.ssh/authorized_keys
cat <<EOF >> /mnt/etc/systemd/network/usb0.network
[Match]
Name=usb0

[Network]
Address=172.16.41.2/24
Gateway=172.16.41.1
DHCP=no
EOF

cd pp/pp-5.6
dd if=uboot.bin of=$device bs=1024 seek=8
./update-boot-script.sh
sudo cp boot.scr board.itb /mnt/boot/
sudo cp -R modules/lib/modules /mnt/lib

umount /mnt/boot /mnt
