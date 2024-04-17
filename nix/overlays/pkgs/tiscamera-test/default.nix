{ stdenv
, clang
, tiscamera
, gst_all_1
, cmake
, pkg-config
, bear
, glib
, gobject-introspection
, makeWrapper
}:

stdenv.mkDerivation rec {
  name = "tiscamera-test";

  src = ../../../..;

  nativeBuildInputs = [
    clang
    cmake
    pkg-config
    bear
    makeWrapper
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
    #export NIX_CFLAGS_COMPILE+=" $(pkg-config --cflags gstreamer-video-1.0)"
    export NIX_CFLAGS_COMPILE+=" $(pkg-config --cflags gobject-introspection-1.0)"
  '';

  buildPhase = ''
    runHook preBuild
    bear -- cmake --build .
    runHook postBuild
  '';

  postInstall = ''
    cp ./compile_commands.json $out
    
    wrapProgram "$out/bin/${name}" --prefix GST_PLUGIN_SYSTEM_PATH_1_0 : "$GST_PLUGIN_SYSTEM_PATH_1_0"
  '';

}
