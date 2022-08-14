{ lib, mkDerivation, fetchFromGitHub, cmake, qtbase, wrapQtAppsHook, kcoreaddons, kwidgetsaddons, kconfig }:

mkDerivation rec{
  name = "koi";
  version = "0.2.0";
  
  src = fetchFromGitHub {
    owner = "baduhai";
    repo = "Koi";
    rev = "${version}";
    sha256 = "XVS/GWy8sB/B3tZ/ZSBqt0vVkzlGaW8zgqCBaFf+uzQ=";
  };
  
  nativeBuildInputs = [ cmake ];
  
  buildInputs = [ wrapQtAppsHook kcoreaddons kwidgetsaddons kconfig ];
  
  sourceRoot = "source/src";
  
  installPhase = ''
    mkdir -p $out/bin
    mv Koi $out/bin
  '';
  
  meta = with lib; {
    description = "Theme scheduling for the KDE Plasma Desktop";
    homepage = "https://github.com/baduhai/Koi";
    license = licenses.lgpl3Plus;
    platforms = platforms.linux;
  };
}
