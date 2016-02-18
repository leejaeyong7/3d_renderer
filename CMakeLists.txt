project (SimEngine)
cmake_minimum_required(VERSION 2.8)

#-----------------------------------------------------------------------------#
#                             OS Specific Variables
#-----------------------------------------------------------------------------#
if(APPLE)
	message("Launching SimEngine in OS X")
	message("Assuming Homebrew install")
	set(IRRLICHT_INCLUDE_DIRS "/usr/local/include/irrlicht")
	set(IRRLICHT_LIBRARY "/usr/local/lib/libIrrlicht.a")
elseif(UNIX)
	message("Launching SimEngine in Linux")
	set(IRRLICHT_INCLUDE_DIRS "/usr/include/irrlicht")
	set(IRRLICHT_LIBRARY "/usr/lib/libIrrlicht.so")
else()
	message("Windows currently not supported")
endif()

#-----------------------------------------------------------------------------#
#                             Package & Includes
#-----------------------------------------------------------------------------#

find_package(X11 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenGL REQUIRED)

if(APPLE)
	find_library(COCOA_LIBRARY Cocoa REQUIRED)
	find_library(IOKIT_LIBRARY IOkit REQUIRED)
	set(EXTRA_LIBRARIES ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
endif()

include_directories(
    ${IRRLICHT_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    )

#-----------------------------------------------------------------------------#
#                               Subdirectories
#-----------------------------------------------------------------------------#
include_directories(Engine Engine/Entity Engine/Physics)
add_subdirectory(Engine)

#-----------------------------------------------------------------------------#
#                                   Settings
#-----------------------------------------------------------------------------#
file(COPY Engine/Models DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Engine)
file(COPY Engine/Textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Engine)
file(COPY Engine/font DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Engine)

add_executable( main main.cpp)
target_compile_features(main PRIVATE cxx_range_for)

#-----------------------------------------------------------------------------#
#                                   Linking
#-----------------------------------------------------------------------------#
target_link_libraries( main SimEngine ${EXTRA_LIBRARIES})