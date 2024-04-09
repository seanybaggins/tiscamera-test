{ stdenv
, clang
, tiscamera
, gst_all_1
, cmake
, pkg-config
, bear
, glib
, gobject-introspection
}:

stdenv.mkDerivation {
  name = "tiscamera-test";

  src = ../../../..;

  nativeBuildInputs = [
    clang
    cmake
    pkg-config
    bear
  ];

  buildInputs = [
    tiscamera
    glib
    gst_all_1.gstreamer
    gst_all_1.gst-plugins-base
    gst_all_1.gst-plugins-good
    gobject-introspection
  ];

  preConfigure = ''
    export NIX_CFLAGS_COMPILE+=" $(pkg-config --cflags glib-2.0)"
    export NIX_CFLAGS_COMPILE+=" $(pkg-config --cflags gstreamer-1.0)"
  '';

}
