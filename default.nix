{ lib, mkDerivation, fetchFromGitHub, cmake, qtbase, wrapQtAppsHook, kcoreaddons, kwidgetsaddons, kconfig }:

mkDerivation rec{
  name = "koi";
  version = "0.2.1";
  
  src = fetchFromGitHub {
    owner = "baduhai";
    repo = "Koi";
    rev = "${version}";
    sha256 = "AWag7VSPQ3I2/9v2WuYR1gJqEAF/xfMJBoW3pZ7fhCs=";
  };
  
  nativeBuildInputs = [ cmake ];
  
  buildInputs = [ wrapQtAppsHook kcoreaddons kwidgetsaddons kconfig ];
  
  sourceRoot = "source/src";
  
  installPhase = ''
    mkdir -p $out/bin
    mv Koi $out/bin
  '';
  
  files = {
    desktop = {
      name = "koi.desktop";
      path = ;
    };
    icon = {
      name = "icon.svg";
      path = ;
    };
  };
  
  meta = with lib; {
    description = "Theme scheduling for the KDE Plasma Desktop";
    homepage = "https://github.com/baduhai/Koi";
    license = licenses.lgpl3Plus;
    platforms = platforms.linux;
  };
}
