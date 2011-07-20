#!/bin/bash
#================
# FILE          : config.sh
#----------------
# PROJECT       : OpenSuSE KIWI Image System
# COPYRIGHT     : (c) 2006 SUSE LINUX Products GmbH. All rights reserved
#               :
# AUTHOR        : Marcus Schaefer <ms@suse.de>
#               :
# BELONGS TO    : Operating System images
#               :
# DESCRIPTION   : configuration script for SUSE based
#               : operating systems
#               :
#               :
# STATUS        : BETA
#----------------
#======================================
# Functions...
#--------------------------------------
test -f /.kconfig && . /.kconfig
test -f /.profile && . /.profile

#======================================
# Greeting...
#--------------------------------------
echo "Configure image: [$kiwi_iname]..."

#======================================
# SuSEconfig
#--------------------------------------
suseConfig

#======================================
# Keep UTF-8 locale
#--------------------------------------
baseStripLocales \
	$(for i in $(echo $kiwi_language | tr "," " ");do echo -n "$i.utf8 ";done)
baseStripTranslations kiwi.mo

rm -rf /usr/share/locale
rm -rf /usr/share/syslinux
rm -rf /etc/zypp
rm -rf /etc/YaST2
rm -rf /etc/ConsoleKit
rm -rf /etc/alternatives
rm -rf /etc/xinetd.d
rm -rf /etc/gnupg
rm -rf /etc/sysconfig
rm -rf /usr/lib/zypp
rm -rf /usr/share/zypp
rm -rf /var/lib/zypp
rm -rf /lib/systemd
rm -rf /lib/mkinitrd

#======================================
# Umount kernel filesystems
#--------------------------------------
baseCleanMount

exit 0
