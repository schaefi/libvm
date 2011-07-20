#
# spec file for package libvm (Version 1.3.0)
#
# Copyright (c) 2004 SUSE LINUX AG, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
#
Name: libvm
BuildRequires:  fdupes cmake
BuildRequires:  libqt4 libqt4-devel libqt4-x11
Summary:      Yet another VM library
Version:      1.3.0
Release:      1
Group:        System/Libraries
License:      Other License(s), see package, GPL
Source:       libvm.tar.bz2
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
Requires:     socat

%description
This package contains the libvm library

#=================================================
# Preparation...
#-------------------------------------------------
%prep
%setup -n libvm

%build
test -e /.buildenv && . /.buildenv
export CFLAGS=$RPM_OPT_FLAGS CXXFLAGS="$RPM_OPT_FLAGS" \
#=================================================
# build sources
#-------------------------------------------------
(
	mkdir build && cd build && \
	cmake ../ -DCMAKE_INSTALL_PREFIX="/usr"
)
make -C build all

#=================================================
# install sources
#-------------------------------------------------
%install
#TODO

%clean
%{__rm} -rf %{buildroot}

#=================================================
# qbiff files...      
#-------------------------------------------------
%files
%defattr(-,root,root)
#TODO
