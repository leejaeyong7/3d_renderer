file( GLOB GUI_SRC *.cpp )
include_directories("/usr/local/Cellar/irrlicht/1.8.1/include/irrlicht")
add_library(SimGUI STATIC ${GUI_SRC})
target_link_libraries(SimGUI /usr/local/Cellar/irrlicht/1.8.1/lib/libirrlicht.a)
