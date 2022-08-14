{ lib, mkDerivation, fetchFromGitHub, cmake, qtbase, wrapQtAppsHook, kcoreaddons, kwidgetsaddons, kconfig }:

# Use this file either for nix-shell or for nix-build when developing locally.
# nix-shell -E 'with import <nixpkgs> {}; pkgs.libsForQt5.callPackage ./dev.nix {}'
# nix-build -E 'with import <nixpkgs> {}; pkgs.libsForQt5.callPackage ./dev.nix {}'
# See default.nix to build release version.

mkDerivation rec{
  name = "koi";
  
  src = ./src;
  
  nativeBuildInputs = [ cmake ];
  
  buildInputs = [ wrapQtAppsHook kcoreaddons kwidgetsaddons kconfig ];
  
  installPhase = ''
    make install DESTDIR=appdir
  '';
  
  meta = with lib; {
    description = "Theme scheduling for the KDE Plasma Desktop";
    homepage = "https://github.com/baduhai/Koi";
    license = licenses.lgpl3Plus;
    platforms = platforms.linux;
  };
} 
