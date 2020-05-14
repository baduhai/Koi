<p align="center"><img src="src/resources/icons/koi.svg" alt="Koi" width="128" height="128"></p>

<h1 align="center">Koi</h1> 

<h3 align="center">Theme scheduling for the KDE Plasma Desktop</h3>

Koi is a program designed to provide the KDE Plasma Desktop functionality to automatically switch between light and dark themes. Koi is under active development, and while it is stable enough to use daily, expect bugs. Koi is designed to be used with Plasma, and while some features may function under different desktop environments, they are unlikely to work and untested.

### Features

- Toggle between light and dark presets based on time.
- Change Plasma style.
- Change Qt colour scheme.
- Change Icon theme.
- Change GTK theme.
- Change wallpaper.
- Hide application to system tray.

#### Planned features

- Toggle between light and dark presets based on sunset and sunrise.
- Change Kate schema.
- Change Atom theme.

Want Koi to support changing themes for another application? Open a [feature request]()!

### Screenshots

![Screenshot](screenshot.png)

### Using Koi

Koi is designed to be very simple to use. When you first open Koi, settings for the light and dark presets will be empty, all you need to do is select which themes you'd like for Koi to manage and select your desired themes by clicking the *Preferences* button. 

If you would like that Koi switch between your light and dark settings automatically, you can select this option in the main page of the program, and choose between following a user set schedule, or at sunrise and sunset(planned feature). To switch between light and dark manually right click the tray icon and select the desired preset, or do so from Koi's main window.

In order to have Koi start at login, you can do so with the Plasma System Settings. Simply navigate to *Startup and Shutdown* > *Autostart*, and select *Add Program...*. If you are to do this, I recommend that you also check the option to start Koi hidden, so not to be annoyed Koi popping up every time you start your system.

### Get Koi

Lastest Koi version: 0.1.0 [beta]

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

The following is a list of resources that I used as reference or inspiration for writing Koi.

- [Yin-Yang](https://github.com/daehruoydeef/Yin-Yang) - For UI layout and features inspiration.
- [system-tray-icon-qt](https://github.com/C0D1UM/system-tray-icon-qt) - For teaching me how to implement a system tray icon with Qt.
- [plasma-theme-switcher](https://github.com/maldoinc/plasma-theme-switcher) - For teaching me how to set current Qt colour scheme.
- [ksetwallpaper](https://github.com/pashazz/ksetwallpaper) - For teaching me how to set the wallpaper on Plasma.
- [This blog post from Zren](https://zren.github.io/2020/04/28/how-to-change-plasma-icon-theme-in-the-terminal) - For teaching me how to set the icon theme.

### Contributors

<table style="width=50%">
	<tr>
		<td align="center">
			<a href="https://github.com/jandreski"><img src="https://avatars3.githubusercontent.com/u/47537097?s=460&v=4" width="40px;" alt=""/><sub><b>Jakov Andreski</b></sub></a>
		</td>
	</tr>
</table>

### License

[**LGPL**](LICENSE)