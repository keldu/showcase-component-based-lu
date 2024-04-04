{ pkgs ? import <nixpkgs> {}
, stdenv ? pkgs.llvmPackages_15.stdenv
, clang-tools ? pkgs.clang-tools_15
}:

let
  forstio = (import ((builtins.fetchGit {
    url = "git@git.keldu.de:forstio/forstio";
    ref = "dev";
  }).outPath + "/default.nix"){
    inherit stdenv;
    inherit clang-tools;
  }).forstio;
in pkgs.callPackage ./.nix/derivation.nix {
  inherit forstio;
  inherit stdenv;
  inherit clang-tools;
}
