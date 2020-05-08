<p align="center"><img src="src/resources/icons/koi.svg" alt="Koi" width="128" height="128"></p>

<h1 align="center">Koi</h1> 

<h3 align="center">Theme scheduling for the KDE Plasma Desktop</h3>

Koi is a program designed to provide the KDE Plasma Desktop functionality to automatically switch between light and dark themes.​

### Features

- Toggle between light and dark presets based on time.
- Toggle between light and dark presets based on sunset and sunrise.
- Change Plasma style.
- Change Qt colour scheme.
- Change Icon theme.
- Change GTK theme.
- Change wallpaper.
- Hide application to system tray.

#### Planned features

- Change Kate scheme.
- Change Atom theme.

Want Koi to support changing themes for another application? Open a [feature request]()!

### Screenshots

![Screenshot](screenshot.png)

### Get Koi

Lastest Koi version: 

#### Arch Linux

AUR packages planned.

[//]: # "Install it from the AUR. [koi](https://aur.archlinux.org) or [koi-git](https://aur.archlinux.org)."

#### Other Distros

AppImage planned.

[//]: # "Find the latest AppImage in the [Releases](https://github.com/baduhai/Koi/releases) page."

#### Build from source

1. `git clone https://github.com/baduhai/Koi.git`
2. `cd Koi`
3. `mkdir build`
4. `cd build`
5. `qmake ../src/koi.pro`
6. `make`
7. `make install` - requires root privileges.

### References

The following is a list of software that I used as reference or inspiration for writing Koi.

- [Yin-Yang](https://github.com/daehruoydeef/Yin-Yang) - For UI layout and features inspiration.
- [system-tray-icon-qt](https://github.com/C0D1UM/system-tray-icon-qt) - For teaching me how to implement a system tray icon with Qt.
- [plasma-theme-switcher](https://github.com/maldoinc/plasma-theme-switcher) - For teaching me how to set current Qt colour scheme.
- [ksetwallpaper](https://github.com/pashazz/ksetwallpaper) - For teaching me how to set the wallpaper on Plasma.
- [This blog post from Zren](https://zren.github.io/2020/04/28/how-to-change-plasma-icon-theme-in-the-terminal) - For teaching me how to set the icon theme.

### License

[**LGPL**](LICENSE)