#!/bin/bash

../build/vmc --add-disk $1

pty=/var/cache/pty
if [ -e "$pty" ];then
	pty=$(cat $pty)
else
	exit 1
fi
pid=$(pidof vmc)

echo "mount /dev/vda1 /mnt" > $pty
echo "find /mnt/etc"        > $pty
echo "umount /mnt"          > $pty

socat -T1 - $pty,raw
kill $pid
