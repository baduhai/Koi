{
  lib,
  stdenv,
  fetchFromGitHub,
  cmake,
  kconfig,
  kcoreaddons,
  kwidgetsaddons,
  wrapQtAppsHook,
}:
stdenv.mkDerivation rec {
  pname = "koi";
  version = "0.7";

  src = fetchFromGitHub {
    owner = "baduhai";
    repo = "Koi";
    rev = version;
    sha256 = "lib.fakeSha256";
  };

  # See https://github.com/baduhai/Koi/blob/master/development/Nix%20OS/dev.nix
  sourceRoot = "${src.name}/";
  nativeBuildInputs = [
    cmake
    wrapQtAppsHook
  ];
  buildInputs = [
    kconfig
    kcoreaddons
    kwidgetsaddons
  ];

  meta = with lib; {
    description = "Scheduling LIGHT/DARK Theme Converter for the KDE Plasma Desktop";
    longDescription = ''
      Koi is a program designed to provide the KDE Plasma Desktop functionality to automatically switch between light and dark themes. Koi is under semi-active development, and while it is stable enough to use daily, expect bugs. Koi is designed to be used with Plasma, and while some features may function under different desktop environments, they are unlikely to work and untested.

      Features:

      - Toggle between LIGHT and DARK presets based on time & place
      - Change Plasma style
      - Change QT colour scheme
      - Change Icon theme
      - Change GTK theme
      - Change KDE Konsole theme
      - Change Wallpaper
      - Hide application to system tray
      - Toggle between LIGHT/DARK themes by clicking mouse wheel
    '';
    license = licenses.lgpl3;
    platforms = platforms.linux;
    homepage = "https://github.com/baduhai/Koi";
    maintainers = with lib.maintainers; [ fnune ];
  };
}
