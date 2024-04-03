{ stdenv
, clang
, tiscamera
, gst_all_1
, glib
, pkg-config
}:

stdenv.mkDerivation {
  name = "tiscamera-test";

  src = ../../../..;

  nativeBuildInputs = [
    clang
  ];

  buildInputs = [
    tiscamera
    gst_all_1.gstreamer
  ];

}
