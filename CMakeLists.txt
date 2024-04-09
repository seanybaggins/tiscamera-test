cmake_minimum_required(VERSION 3.10)
project(tiscamera-test)

# Find the GStreamer libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules (GLIB2 REQUIRED glib-2.0)
pkg_check_modules(TCAM REQUIRED tcam)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

# Include GStreamer header files
#include_directories(${GST_INCLUDE_DIRS})

# Add definitions and compiler flags needed by GStreamer
#add_definitions(${GST_CFLAGS_OTHER})

# Define the executable to build (e.g., my_app from main.c)
add_executable(tiscamera-test src/main.c)

# Link the GStreamer libraries with your application
target_link_libraries(tiscamera-test 
    ${GLIB2_LIBRARIES}
    ${TCAM_LIBRARIES}
    ${GST_LIBRARIES}
)

install(TARGETS tiscamera-test DESTINATION bin)
