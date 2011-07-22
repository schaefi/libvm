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
Summary:        Yet another VM library
Url:            http://www.suse.de
Version:        1.3.0
Release:        1
Group:          System/Libraries
License:        GPLv2+
Source:         libvm.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       socat

%description
This package contains the libvm library

Authors:
--------
    Marcus Schaefer <ms@suse.de>

%package -n vmc
License:        GPLv2+
Summary:        Tools based on libvm
Group:          System/Libraries

%description -n vmc
Virtual machine connector

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
	cmake ../ $CMAKE_FLAGS \
		-DCMAKE_INSTALL_PREFIX=%{_prefix} \
		-DLIB=%{_lib} \
		-DCMAKE_BUILD_TYPE=Release
)
make -C build all

#=================================================
# install sources
#-------------------------------------------------
%install
make -C build DESTDIR=$RPM_BUILD_ROOT install
install -d -m 755 $RPM_BUILD_ROOT/%{_mandir}
install -d -m 755 $RPM_BUILD_ROOT/%{_mandir}/man1
install -m 664 man/vmc.1.gz $RPM_BUILD_ROOT/%{_mandir}/man1

%clean
%{__rm} -rf %{buildroot}

#=================================================
# qbiff files...      
#-------------------------------------------------
%files
%defattr(-,root,root)
/usr/%{_lib}/libvm.so

%files -n vmc
%defattr(-,root,root)
%doc %{_mandir}/man1/vmc.1.gz
%{_bindir}/vmc
