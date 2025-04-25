#
# spec file for package koi
#
# Copyright (c) 2025 SUSE LLC
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via https://bugs.opensuse.org/
#


%define original_name Koi


Name:           koi
Version:        0.5.1
Release:        0%{?dist}
Summary:        Scheduled LIGHT/DARK Theme Switching for the KDE Plasma Desktop
License:        LGPL-3.0-only
URL:            https://github.com/baduhai/%{original_name}
Source0:        %{url}/archive/%{version}.tar.gz#/%{name}-%{version}.tar.gz

%if 0%{?suse_version} >= 1600 && 0%{?is_opensuse} || 0%{?fedora} >= 40 || 0%{?mageia} >= 10
BuildRequires:  cmake
BuildRequires:  (cmake-full or cmake-fedora or cmake-extras or extra-cmake-modules)
BuildRequires:  cmake(KF6Config)
BuildRequires:  cmake(KF6ConfigWidgets)
BuildRequires:  cmake(KF6CoreAddons)
BuildRequires:  cmake(KF6WidgetsAddons)

BuildRequires:  cmake(Qt6Core)
BuildRequires:  cmake(Qt6DBus)
BuildRequires:  cmake(Qt6Gui)
BuildRequires:  cmake(Qt6Widgets)
BuildRequires:  cmake(Qt6Xml)

BuildRequires:  (kf6-extra-cmake-modules or extra-cmake-modules)

Requires:       (plasma6-desktop   or plasma-desktop)
Requires:       (plasma6-workspace or plasma-workspace)

%if 0%{?sle_version} <= 150600 && 0%{?is_opensuse} || 0%{?suse_version} < 1600 && 0%{?is_opensuse}
BuildRequires:  cmake
BuildRequires:  (cmake-full or cmake-fedora or cmake-extras or extra-cmake-modules)
BuildRequires:  cmake(KF5Config)
BuildRequires:  cmake(KF5ConfigWidgets)
BuildRequires:  cmake(KF5CoreAddons)
BuildRequires:  cmake(KF5WidgetsAddons)

BuildRequires:  cmake(Qt5Core)
BuildRequires:  cmake(Qt5DBus)
BuildRequires:  cmake(Qt5Gui)
BuildRequires:  cmake(Qt5Widgets)
BuildRequires:  cmake(Qt5Xml)

Requires:       plasma5-desktop
Requires:       plasma-workspace
Requires:       plasma-framework

%if 0%{?mageia} >= 10
BuildRequires:  cmake
BuildRequires:  cmake-rpm-macros
BuildRequires:  (cmake-full or cmake-fedora or cmake-extras or extra-cmake-modules)
BuildRequires:  cmake(KF6Config)
BuildRequires:  cmake(KF6ConfigWidgets)
BuildRequires:  cmake(KF6CoreAddons)
BuildRequires:  cmake(KF6WidgetsAddons)

BuildRequires:  cmake(Qt6Core)
BuildRequires:  cmake(Qt6DBus)
BuildRequires:  cmake(Qt6Gui)
BuildRequires:  cmake(Qt6Widgets)
BuildRequires:  cmake(Qt6Xml)

BuildRequires:  (lib64kf6config-devel        or libkf6config-devel)
BuildRequires:  (lib64kf6configwidgets-devel or libkf6configwidgets-devel)
BuildRequires:  (lib64kf6coreaddons-devel    or libkf6coreaddons-devel)
BuildRequires:  (lib64kf6widgetsaddons-devel or libkf6widgetsaddons-devel)

BuildRequires:  (lib64qt6core-devel    or libqt6core-devel)
BuildRequires:  (lib64qt6dbus-devel    or libqt6dbus-devel)
BuildRequires:  (lib64qt6gui-devel     or libqt6gui-devel)
BuildRequires:  (lib64qt6widgets-devel or libqt6widgets-devel)
BuildRequires:  (lib64qt6xml-devel     or libqt6xml-devel)

Requires:       plasma-desktop
Requires:       plasma6-framework
Requires:       plasma-integration
Requires:       plasma-workspace

%if 0%{?mageia} <= 9
BuildRequires:  cmake
BuildRequires:  cmake-rpm-macros
BuildRequires:  (cmake-full or cmake-fedora or cmake-extras or extra-cmake-modules)
BuildRequires:  cmake(KF5Config)
BuildRequires:  cmake(KF5ConfigWidgets)
BuildRequires:  cmake(KF5CoreAddons)
BuildRequires:  cmake(KF5WidgetsAddons)

BuildRequires:  cmake(Qt5Core)
BuildRequires:  cmake(Qt5DBus)
BuildRequires:  cmake(Qt5Gui)
BuildRequires:  cmake(Qt5Widgets)
BuildRequires:  cmake(Qt5Xml)

BuildRequires:  (lib64kf5config-devel        or libkf5config-devel)
BuildRequires:  (lib64kf5configwidgets-devel or libkf5configwidgets-devel)
BuildRequires:  (lib64kf5coreaddons-devel    or libkf5coreaddons-devel)
BuildRequires:  (lib64kf5widgetsaddons-devel or libkf5widgetsaddons-devel)

BuildRequires:  (lib64qt5core-devel    or libqt5core-devel)
BuildRequires:  (lib64qt5dbus-devel    or libqt5dbus-devel)
BuildRequires:  (lib64qt5gui-devel     or libqt5gui-devel)
BuildRequires:  (lib64qt5widgets-devel or libqt5widgets-devel)
BuildRequires:  (lib64qt5xml-devel     or libqt5xml-devel)

Requires:       kf5-plasma-framework
Requires:       plasma-desktop
Requires:       plasma-framework
Requires:       plasma-integration
Requires:       plasma-workspace

%endif
%endif
%endif
%endif

BuildRequires:  desktop-file-utils
BuildRequires:  fdupes


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
%if 0%{?suse_version} >= 1600 && 0%{?is_opensuse} || 0%{?fedora} >= 40 || 0%{?mageia} >= 10
%cmake_kf6

%kf6_build

%if 0%{?sle_version} <= 150600 && 0%{?is_opensuse} || 0%{?suse_version} < 1600 && 0%{?is_opensuse} || 0%{?mageia} <= 9
%cmake_kf5

%kf5_build

%endif
%endif

%install
%if 0%{?suse_version} >= 1600 && 0%{?is_opensuse} || 0%{?fedora} >= 40 || 0%{?mageia} >= 10
%kf6_install

%if 0%{?sle_version} <= 150600 && 0%{?is_opensuse} || 0%{?suse_version} < 1600 && 0%{?is_opensuse} || 0%{?mageia} <= 9
%kf5_install

%endif
%endif

%check
desktop-file-validate "%{buildroot}/%{_datadir}/applications/local.%{original_name}DbusInterface.desktop"
%if 0%{?sle} && 0%{?is_opensuse} || 0%{?suse} && 0%{?is_opensuse} || 0%{?fedora} || 0%{?mageia}
%fdupes                %{buildroot}/

%else
fdupes -s              %{buildroot}/

%files
%license LICENSE
%doc    "README.md"

"%{_bindir}/%{name}"

"%{_datadir}/applications/local.%{original_name}DbusInterface.desktop"
"%{_datadir}/dbus-1/interfaces/dev.baduhai.%{original_name}.xml"
"%{_datadir}/dbus-1/services/dev.baduhai.%{original_name}.service"
 %{_datadir}/icons/hicolor/scalable/apps/{%{name}.svg,%{name}_tray.svg}

%changelog
