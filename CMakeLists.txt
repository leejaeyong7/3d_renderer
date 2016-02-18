project (SimEngine)
cmake_minimum_required(VERSION 2.8)

find_package(X11)
find_package(GLUT)
find_package(ZLIB)
find_package(OpenGL)

#-----------------------------------------------------#
#----------------------- Build -----------------------#
#-----------------------------------------------------#
include_directories(
    Engine
    Engine/GUI
    /usr/include/irrlicht
    )

add_subdirectory(Engine)


add_executable( main main.cpp)
target_link_libraries( main SimEngine )
target_compile_features(main PRIVATE cxx_range_for)
#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
