include_directories(
	Entity
    GUI
	Physics)

add_subdirectory (Entity)
add_subdirectory(Physics)

file( GLOB ENGINE_SRC *.cpp )
file( GLOB GUI_SRC GUI/*.cpp )

find_package(X11)
find_package(ZLIB)
find_package(OpenGL)

add_library( SimGUI STATIC ${GUI_SRC} )
add_library( SimEngine STATIC ${ENGINE_SRC} )

include_directories(
    "/usr/include/irrlicht"
    ${OpenGL_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    )

target_include_directories(SimGUI PUBLIC Entity Physics)
target_link_libraries(SimGUI /usr/lib/libIrrlicht.so)
target_compile_features(SimGUI PRIVATE cxx_range_for)
target_link_libraries(SimEngine /usr/lib/libIrrlicht.so)
target_link_libraries(SimGUI SimEngine SimEntity SimPhysics ${OPENGL_LIBRARIES} ${ZLIB_LIBRARIES} ${X11_LIBRARIES}) 
target_link_libraries(SimEngine SimGUI SimEntity SimPhysics ${OPENGL_LIBRARIES} ${ZLIB_LIBRARIES} ${X11_LIBRARIES})
