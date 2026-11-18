#
# SPEC file for package: koi
#
# This is free and unencumbered package released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this package, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this package dedicate any and all copyright interest in the
# package to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# package under copyright law.
#
# THE PACKAGE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE PACKAGE OR THE USE OR
# OTHER DEALINGS IN THE PACKAGE.
#
# For more information, please refer to <https://unlicense.org/>

# Force RPM CMake macros to use Ninja across all phases
%global __builder ninja
#
%global original_name Koi
# Define CMake Flags for DEB-Based Linux Distributions
%global debian_cmake_flags -DCMAKE_BUILD_TYPE='RelWithDebInfo' -DCMAKE_INSTALL_PREFIX:PATH='/usr/'  -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL='ON' -DKDE_INSTALL_USE_QT_SYS_PATHS:BOOL='ON' -DSHARE_INSTALL_PREFIX:PATH='/usr/share/' -G 'Ninja'
#

Name:     koi
Version:  0.7
Release:  0
Summary:  Scheduled LIGHT/DARK Theme Switching for the KDE Plasma Desktop

License:  LGPL-3.0-only
URL:      https://github.com/baduhai/Koi
Source:   %{url}/archive/%{version}.tar.gz#/%{name}-%{version}.tar.gz
# Patch:  koi-qt5-build.patch

# SPECIFICATIONS for Debian & Ubuntu (DebBuild) and other Debian-based Linux Distributions:
%if 0%{?_debbuild}
%{expand:Packager: Martin Stibor <martin.von.reichenberg@gmail.com>}
%{expand:Group: utils}
%endif
#

BuildRequires:  desktop-file-utils
BuildRequires:  fdupes
BuildRequires:  hicolor-icon-theme
# BUILD Requirements for SUSE Leap 16.0+ / SUSE SlowRoll / SUSE TumbleWeed / Fedora 43+ / Mageia 10+ (Cauldron)
%if 0%{?suse_version} && 0%{?is_opensuse} || 0%{?fedora} || 0%{?mageia}
BuildRequires:  cmake  extra-cmake-modules
%if 0%{?suse_version} && 0%{?is_opensuse}
BuildRequires:  cmake-full
%endif
%if 0%{?fedora} || 0%{?mageia}
BuildRequires:  cmake-rpm-macros
%endif
BuildRequires:  cmake(KF6Config)
BuildRequires:  cmake(KF6ConfigWidgets)
BuildRequires:  cmake(KF6CoreAddons)
BuildRequires:  cmake(KF6DBusAddons)
BuildRequires:  cmake(KF6GuiAddons)
BuildRequires:  cmake(KF6WidgetsAddons)
BuildRequires:  cmake(KF6WindowSystem)
BuildRequires:  cmake(Qt6Core)
BuildRequires:  cmake(Qt6DBus)
BuildRequires:  cmake(Qt6Gui)
BuildRequires:  cmake(Qt6Widgets)
BuildRequires:  cmake(Qt6Xml)
BuildRequires:  gcc  gcc-c++
BuildRequires:  (ninja  or  ninja-build)
%endif
#
# BUILD Requirements for Debian & Ubuntu (DebBuild) and other Debian-based Linux Distributions:
%if 0%{?_debbuild}
BuildRequires:  gcc  g++
BuildRequires:  cmake  cmake-extras  extra-cmake-modules
BuildRequires:  libkf6config-dev
BuildRequires:  libkf6configwidgets-dev
BuildRequires:  libkf6coreaddons-dev
BuildRequires:  libkf6dbusaddons-dev
BuildRequires:  libkf6guiaddons-dev
BuildRequires:  libkf6widgetsaddons-dev
BuildRequires:  libkf6windowsystem-dev
BuildRequires:  ninja-build
BuildRequires:  qt6-base-dev
%endif
#

# RUNTIME Requirements for SUSE Leap 16.0+ / SUSE SlowRoll / SUSE TumbleWeed / Fedora 43+ / Mageia 10+ (Cauldron)
%if 0%{?suse_version} && 0%{?is_opensuse} || 0%{?fedora} || 0%{?mageia}
Requires:  (plasma-desktop    or  plasma6-desktop)
Requires:  (plasma-workspace  or  plasma6-workspace)
Requires:  %{_bindir}/dbus-send
%endif
#
# RUNTIME Requirements for Debian & Ubuntu (DebBuild) and other Debian-based Linux Distributions:
%if 0%{?_debbuild}
Requires:  dbus-bin
Requires:  hicolor-icon-theme
Requires:  plasma-desktop
%endif
#

%description
Koi is a program designed to provide the KDE Plasma Desktop functionality
to automatically switch between light and dark themes.
Koi allows users to automate and manage their desktop themes,
providing a convenient way to schedule theme changes on the KDE Plasma Desktop,
enhancing the user experience with timely and automated visual adjustments.
It supports full DBus service integration & running custom Bash scripts.

%prep
%autosetup -p1 -n %{original_name}-%{version}

%build
# CMake for RPM-Based Linux Distributions
%if 0%{?suse_version} && 0%{?is_opensuse} || 0%{?fedora} || 0%{?mageia}
%cmake \
    -G 'Ninja' \
    -DCMAKE_BUILD_TYPE='RelWithDebInfo' \
    -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL='ON' \
    -DKDE_INSTALL_USE_QT_SYS_PATHS:BOOL='ON'
# Ninja BUILD for RPM-Based Linux Distributions
%if 0%{?suse_version} && 0%{?is_opensuse}
%ninja_build
%endif
%if 0%{?mageia}
%ninja_build -C './build/'
%endif
%if 0%{?fedora}
%ninja_build -C './redhat-linux-build/'
%endif
#
%endif
#
# CMake for DEB-Based Linux Distributions
%if 0%{?_debbuild}
cmake -S "." -B './build/' %{?debian_cmake_flags}
#
# Ninja BUILD for DEB-Based Linux Distributions
ninja -C './build/' -j "${RPM_BUILD_NCPUS}" -v
%endif
#

%install
# Ninja INSTALL for RPM-Based Linux Distributions
%if 0%{?suse_version} && 0%{?is_opensuse} || 0%{?mageia}
%ninja_install -C './build/'
%endif
%if 0%{?fedora}
%ninja_install -C './redhat-linux-build/'
%endif
#
# Ninja INSTALL for DEB-Based Linux Distributions
%if 0%{?_debbuild}
DESTDIR=%{buildroot} ninja install -C './build/' -v
%endif
#

%check
# Check the Packaged & Renamed Application .DESKTOP File; Fails on Debian 12 Due to Obsolete `desktop-file-utils`
desktop-file-validate "%{buildroot}/%{_datadir}/applications/local.%{original_name}DbusInterface.desktop"
#
# Check for Duplicated Files at RPM-Based Linux Distributions
%if 0%{?suse_version} && 0%{?is_opensuse} || 0%{?fedora} || 0%{?mageia}
%fdupes %{buildroot}/
#
# Check for Duplicated Files at DEB-Based Linux Distributions
%else
fdupes -s %{buildroot}/
#
%endif

%files
%license LICENSE
%doc README.md

%{_bindir}/%{name}

%{_datadir}/applications/local.%{original_name}DbusInterface.desktop
%{_datadir}/dbus-1/interfaces/dev.baduhai.%{original_name}.xml
%{_datadir}/dbus-1/services/dev.baduhai.%{original_name}.service
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg
%{_datadir}/icons/hicolor/scalable/apps/%{name}_tray.svg

%changelog
