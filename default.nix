{ lib, mkDerivation, fetchFromGitHub, cmake, qtbase, wrapQtAppsHook, kcoreaddons, kwidgetsaddons, kconfig }:

mkDerivation rec{
  name = "koi";
  version = "0.2.1";
  
  src = fetchFromGitHub {
    owner = "baduhai";
    repo = "Koi";
    rev = "f89d3a6723eb85e0255d985e868573781d38def8";
    sha256 = "j86XAJbB8E0P4KT2mAaf4BtaMar2riXct7qcvwxZJ1Y=";
  };
  
  nativeBuildInputs = [ cmake ];
  
  buildInputs = [ wrapQtAppsHook kcoreaddons kwidgetsaddons kconfig ];
  
  sourceRoot = "source/src";
  
  meta = with lib; {
    description = "Theme scheduling for the KDE Plasma Desktop";
    homepage = "https://github.com/baduhai/Koi";
    license = licenses.lgpl3Plus;
    platforms = platforms.linux;
  };
}
