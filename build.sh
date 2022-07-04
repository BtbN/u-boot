#!/bin/bash
export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-

board="bpi-r3"
device="sdmmc"
#device="emmc"
#device="spim_nand"
#device="spim_nor"

DEFCONFIG="mt7986_rfb_${device}_defconfig"


case $1 in
	"importconfig")
		make $DEFCONFIG
	;;
	"config")
		make menuconfig
	;;
	"build")
		make
	;;
	"install")
		if [[ "$device" != "sdmmc" ]];then echo "$1 not supported for $device";exit 1;fi
		DEV=/dev/sdb
		read -e -i "$DEV" -p "Please enter target device: " DEV
		case $board in
			"bpi-r3")
				set -x
				sudo dd if=build/mt7986/release/bl2.img of=${DEV}1 conv=notrunc,fsync #1> /dev/null 2>&1
				sudo dd if=build/mt7986/release/fip.bin of=${DEV}4 conv=notrunc,fsync #1> /dev/null 2>&1
				set +x
			;;
		esac
		;;
	"createimg")
		if [[ "$device" != "sdmmc" ]];then echo "$1 not supported for $device";exit 1;fi
		IMGDIR=.
		IMGNAME=${board}_${device}
		REALSIZE=7000
		dd if=/dev/zero of=$IMGDIR/$IMGNAME.img bs=1M count=$REALSIZE 1> /dev/null 2>&1
		LDEV=`losetup -f`
		DEV=`echo $LDEV | cut -d "/" -f 3`     #mount image to loop device
		echo "run losetup to assign image $IMGNAME.img to loopdev $LDEV ($DEV)"
		sudo losetup $LDEV $IMGDIR/$IMGNAME.img 1> /dev/null #2>&1
		case $board in
			"bpi-r3")
				sudo dd if=gpt_${device}_100m6g.img of=$LDEV conv=notrunc,fsync #1> /dev/null 2>&1

				#re-read part table
				sudo losetup -d $LDEV
				sudo losetup -P $LDEV $IMGDIR/$IMGNAME.img 1> /dev/null #2>&1
				#sudo gdisk -l /dev/sdb #try to repair MBR/GPT

				#sudo partprobe $LDEV #1> /dev/null 2>&1
				sudo dd if=build/mt7986/release/bl2.img of=${LDEV}p1 conv=notrunc,fsync #1> /dev/null 2>&1
				sudo dd if=build/mt7986/release/fip.bin of=${LDEV}p4 conv=notrunc,fsync #1> /dev/null 2>&1
				#sudo mkfs.vfat "${LDEV}p5" -n BPI-BOOT #1> /dev/null 2>&1
				#sudo mkfs.ext4 -O ^metadata_csum,^64bit "${LDEV}p6" -L BPI-ROOT #1> /dev/null 2>&1
			;;
		esac
		#sudo losetup -d $LDEV
	;;
	"")
		$0 build
	;;
esac
