CC = g++

OS_NAME := $(shell uname)
MKDIR_P  = mkdir -p

###############################################################################
# ON OS X with HOMEBREW install
###############################################################################
ifeq ($(OS_NAME),Darwin)
# SET INCLUDE FLAGS TO HOMEBREW CELLAR DIR
LDFLAGS = -I/usr/local/Cellar/Irrlicht/1.8.1/include/irrlicht -I/opt/local/include

#SET LIB FLAGS TO HOMEBREW CELLAR LIB DIR
LLIBFLAGS = -L/usr/local/Cellar/irrlicht/1.8.1/lib -L/usr/X11/lib 

#SET OPENGL FLAG for os x (requires Xquartz)
OPENGL_FLAGS = -framework OpenGL -framework GLUT -framework Cocoa -framework IOkit

# SPECIFIY LINK OPTIONS
LINKFLAGS = -l Irrlicht $(OPENGL_FLAGS) -lXext -lXrandr -lX11 

###############################################################################
# ON LINUX
###############################################################################
else ifeq ($(OS_NAME),Linux)

# SET INCLUDE FLAGS FOR IRRLICHT
LDFLAGS = -I/usr/include/irrlicht

#SET LIB FLAGS TO HOMEBREW CELLAR LIB DIR
LLIBFLAGS = -L/usr/lib/x86_64-linux-gnu/

# SPECIFIY LINK OPTIONS
LINKFLAGS = -lIrrlicht -lGL -lGLU -lXxf86vm
endif

# FINAL FLAG
FLAGS = $(LLIBFLAGS) $(LDFLAGS) $(LINKFLAGS)


ifeq ($(OS_NAME),Darwin)

main: main.o eventHandler.o
	@mkdir -p main.out.app
	@mkdir -p main.out.app/Contents
	@mkdir -p main.out.app/Contents/MacOS
	$(CC) -o main.out.app/Contents/MacOS/main.out main.o eventHandler.o $(FLAGS)

else ifeq ($(OS_NAME),Linux)

main: main.o eventHandler.o
	$(CC) -o main.out main.o simEngine.o simGUI.o eventHandler.o simEntity.o simPhysics.o $(FLAGS)

endif

main.o: main.cpp
	$(CC) -c main.cpp $(LDFLAGS)

simEngine.o: simEngine.h simGUI.o simEntity.o simPhysics.o
	$(CC) -c simEngine.cpp simGUI.o simEntity.o simPhysics.o $(LDFLAGS)

simGUI.o: simGUI.h eventHandler.o
	$(CC) -c simGUI.cpp $(LDFLAGS)

eventHandler.o: eventHandler.h
	$(CC) -c eventHandler.cpp $(LDFLAGS)

simEntity.o: simEntity.h
	$(CC) -c simEntity.cpp $(LDFLAGS)

simPhysics.o: simPhysics.h
	$(CC) -c simPhysics.cpp $(LDFLAGS)
