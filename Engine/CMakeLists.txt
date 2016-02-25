include_directories(
	Entity
	Physics)
#-----------------------------------------------------------------------------#
#                               Subdirectories
#-----------------------------------------------------------------------------#
add_subdirectory (Entity)
add_subdirectory(Physics)

#-----------------------------------------------------------------------------#
#                                   Settings
#-----------------------------------------------------------------------------#

file( GLOB ENGINE_SRC *.*pp )
add_library( SimEngine STATIC ${ENGINE_SRC} )
target_compile_features(SimEngine PRIVATE cxx_range_for)

#-----------------------------------------------------------------------------#
#                                   Linking
#-----------------------------------------------------------------------------#
target_link_libraries(SimEngine SimEntity SimPhysics ${IRRLICHT_LIBRARY} ${OPENGL_LIBRARIES} ${ZLIB_LIBRARIES} ${X11_LIBRARIES})
