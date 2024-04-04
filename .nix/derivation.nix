{ lib
, gnumake
, stdenv
, clang-tools
, scons
, forstio
, eigen
}:

stdenv.mkDerivation {
  pname = "kel-lr-solver-forstio";
  version = "0.0.0";
  src = ./..;

  nativeBuildInputs = [
		clang-tools
		scons
  ];

  buildInputs = [
    forstio.core
    forstio.codec
    eigen
  ];

  outputs = ["out" "dev" ];
}
