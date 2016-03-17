include_directories(
	Entity
	Physics
	SceneNodes)
#-----------------------------------------------------------------------------#
#                               Subdirectories
#-----------------------------------------------------------------------------#
add_subdirectory (Entity)
add_subdirectory(Physics)
add_subdirectory(SceneNodes)

#-----------------------------------------------------------------------------#
#                                   Settings
#-----------------------------------------------------------------------------#

file( GLOB ENGINE_SRC *.*pp )
add_library( SimEngine STATIC ${ENGINE_SRC}  )

#-----------------------------------------------------------------------------#
#                                   Linking
#-----------------------------------------------------------------------------#
target_link_libraries(SimEngine 
	SimEntity 
	SimPhysics
	${IRRLICHT_LIBRARY} 
	${OPENGL_LIBRARIES} 
	${X11_LIBRARIES})
