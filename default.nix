{ lib, mkDerivation, fetchFromGitHub, cmake, qtbase, wrapQtAppsHook, kcoreaddons, kwidgetsaddons, kconfig }:

mkDerivation rec{
  name = "koi";
  version = "0.2.1";
  
  src = fetchFromGitHub {
    owner = "baduhai";
    repo = "Koi";
    rev = "${version}";
    sha256 = "Q6atI8+Nb4euqCSEwWzpeyhFlV2SbeMm/nIO0r+uG9o=";
  };
  
  nativeBuildInputs = [ cmake ];
  
  buildInputs = [ wrapQtAppsHook kcoreaddons kwidgetsaddons kconfig ];
  
  sourceRoot = "source/src";

#   installPhase = ''
#     cp -r ./ $out/
#   '';
  
  meta = with lib; {
    description = "Theme scheduling for the KDE Plasma Desktop";
    homepage = "https://github.com/baduhai/Koi";
    license = licenses.lgpl3Plus;
    platforms = platforms.linux;
  };
}
