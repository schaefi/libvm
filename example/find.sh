#!/bin/bash

../build/vmpty \
	--add-disk /tmp/mytest/LimeJeOS-openSUSE-11.4.x86_64-1.11.4.raw
pty=/var/cache/pty

if [ -e "$pty" ];then
	pty=$(cat $pty)
else
	exit 1
fi
pid=$(pidof vmpty)

echo "mount /dev/vda1 /mnt" > $pty
echo "find /mnt/etc"        > $pty
echo "umount /mnt"          > $pty

socat -T1 - $pty,raw
kill $pid
