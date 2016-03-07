project (SimEngine)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.8)

#-----------------------------------------------------------------------------#
#                             OS Specific Variables
#-----------------------------------------------------------------------------#
if(APPLE)
	message("Launching SimEngine in OS X")
	message("Assuming Homebrew install")
	set(IRRLICHT_INCLUDE_DIRS "/usr/local/include/irrlicht")
	set(IRRLICHT_LIBRARY "/usr/local/lib/libIrrlicht.a")
	set(BIN_DIRECTORY ${SimEngine_SOURCE_DIR}/irr.app/Contents/MacOS/irr)
elseif(UNIX)
	message("Launching SimEngine in Linux")
	set(IRRLICHT_INCLUDE_DIRS "/usr/include/irrlicht")
	if(EXISTS "/usr/lib/libIrrlicht.so")
		set(IRRLICHT_LIBRARY "/usr/lib/libIrrlicht.so")
	elseif(EXISTS "/usr/lib/x86_64-linux-gnu/libIrrlicht.so")
		set(IRRLICHT_LIBRARY 
			"/usr/lib/x86_64-linux-gnu/libIrrlicht.so")
	else()
		message("Irrlicht library Not found...")
	endif()
	set(BIN_DIRECTORY ${SimEngine_SOURCE_DIR})
else()
	message("Windows currently not supported")
endif()

#-----------------------------------------------------------------------------#
#                             Package & Includes
#-----------------------------------------------------------------------------#

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

if(APPLE)
	find_library(COCOA_LIBRARY Cocoa REQUIRED)
	find_library(IOKIT_LIBRARY IOkit REQUIRED)
	set(EXTRA_LIBRARIES ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
endif()

include_directories(
    ${IRRLICHT_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
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
#file(COPY Engine/Models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY Engine/Textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY Engine/font DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_executable( main main.cpp)
INSTALL(TARGETS main DESTINATION ${BIN_DIRECTORY})
#-----------------------------------------------------------------------------#
#                                   Linking
#-----------------------------------------------------------------------------#
target_link_libraries( main SimEngine ${EXTRA_LIBRARIES})
