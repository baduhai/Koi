Name:           Koi
Version:        0.3
Release:        1%{?dist}
Summary:        Scheduling LIGHT/DARK Theme Converter for the KDE Plasma Desktop
License:        LGPL-3.0-only
URL:            https://github.com/baduhai/%{name}
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  extra-cmake-modules

BuildRequires:  cmake(Qt6Core)
BuildRequires:  cmake(Qt6DBus)
BuildRequires:  cmake(Qt6Gui)
BuildRequires:  cmake(Qt6Network)
BuildRequires:  cmake(Qt6Test)
BuildRequires:  cmake(Qt6Widgets)
BuildRequires:  cmake(Qt6Xml)

BuildRequires:  cmake(KF6Config)
BuildRequires:  cmake(KF6ConfigWidgets)
BuildRequires:  cmake(KF6CoreAddons)
BuildRequires:  cmake(KF6WidgetsAddons)

BuildRequires:  desktop-file-utils
BuildRequires:  fdupes
BuildRequires:  hicolor-icon-theme

Requires:       plasma-desktop plasma-integration plasma-workspace

%description
Koi is a program designed to provide the KDE Plasma Desktop functionality to
automatically switch between light and dark themes.

%prep
%autosetup

%build
%cmake "src/"
%cmake_build

%install
%cmake_install 

%check
desktop-file-validate "%{buildroot}/%{_datadir}/applications/koi.desktop"
%fdupes -s %{buildroot}

%files
%license LICENSE
%doc README.md
%{_bindir}/koi
%{_datadir}/applications/koi.desktop
%{_datadir}/icons/hicolor/scalable/apps/{koi.svg,koi_tray.svg}

%changelog
* Thu Jun 13 2024 Martin Stibor <martin.von.reichenberg@proton.me>
- Koi - 0.3 -- Complete Overhaul
 - Update to QT6 Build Tools / KF6 libraries
 - Updated bundled libraries to their latest changes
 - Updated UI; Updated Screenshot
 - Updated README with appropriate build instructions and more . . .

- Full Changelog**: https://github.com/baduhai/Koi/compare/0.2.4...0.3
