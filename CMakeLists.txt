project (SimEngine)
cmake_minimum_required(VERSION 2.8)

find_package(X11)
find_package(GLUT)
find_package(ZLIB)
find_package(OpenGL)
find_library(IOKIT_LIBRARY IOKit)
find_library(COCOA_LIBRARY Cocoa)

#-----------------------------------------------------#
#----------------------- Build -----------------------#
#-----------------------------------------------------#
include_directories(
    Engine
    Engine/GUI)

add_subdirectory(Engine)

include_directories("/usr/local/Cellar/irrlicht/1.8.1/include/irrlicht")

add_executable( main main.cpp)
target_link_libraries( main SimEngine )

#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
