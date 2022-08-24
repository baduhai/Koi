{ lib, mkDerivation, fetchFromGitHub, cmake, qtbase, wrapQtAppsHook, kcoreaddons, kwidgetsaddons, kconfig }:

# Use this file either for nix-shell or for nix-build when developing locally.
# nix-shell -E 'with import <nixpkgs> {}; pkgs.libsForQt5.callPackage ./dev.nix {}'
# nix-build -E 'with import <nixpkgs> {}; pkgs.libsForQt5.callPackage ./dev.nix {}'

mkDerivation rec{
  name = "koi";
  
  src = ./src;

  nativeBuildInputs = [ cmake ];
  
  buildInputs = [ wrapQtAppsHook kcoreaddons kwidgetsaddons kconfig ];
  
  sourceRoot = "source/src";
} 
