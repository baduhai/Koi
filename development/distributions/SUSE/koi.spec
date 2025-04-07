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
Version:        0.5
Release:        0%{?dist}
Summary:        Scheduled LIGHT/DARK Theme Switching for the KDE Plasma Desktop
License:        LGPL-3.0-only
URL:            https://github.com/baduhai/Koi
Source0:        %{url}/archive/refs/tags/%{version}.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires:  cmake(Qt6Core)
BuildRequires:  cmake(Qt6DBus)
BuildRequires:  cmake(Qt6Gui)
BuildRequires:  cmake(Qt6Network)
BuildRequires:  cmake(Qt6Widgets)
BuildRequires:  cmake(Qt6Xml)

BuildRequires:  cmake(KF6Config)
BuildRequires:  cmake(KF6ConfigWidgets)
BuildRequires:  cmake(KF6CoreAddons)
BuildRequires:  cmake(KF6WidgetsAddons)

BuildRequires:  kf6-extra-cmake-modules

BuildRequires:  desktop-file-utils
BuildRequires:  fdupes


Requires:       hicolor-icon-theme
Requires:       plasma6-desktop
Requires:       plasma6-workspace


%description
Koi is a program designed to provide the KDE Plasma Desktop functionality
to automatically switch between light and dark themes.
Koi allows users to automate and manage their desktop themes,
providing a convenient way to schedule theme changes on the KDE Plasma Desktop,
enhancing the user experience with timely and automated visual adjustments.
It supports running custom Bash scripts and provides a full DBus service integration.

%prep
%autosetup -p1 -n %{original_name}-%{version}

%build
pushd src
%cmake_kf6

%kf6_build
popd

%install
pushd src
%kf6_install
popd

%check
desktop-file-validate "%{buildroot}/%{_datadir}/applications/koi.desktop"
%fdupes -s             %{buildroot}

%files
%license LICENSE
%doc    "README.md"

"%{_bindir}/%{name}"

"%{_datadir}/applications/%{name}.desktop"
"%{_datadir}/dbus-1/interfaces/dev.baduhai.%{original_name}.xml"
 %{_datadir}/icons/hicolor/scalable/apps/{%{name}.svg,%{name}_tray.svg}

%changelog
