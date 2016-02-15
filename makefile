CC = g++
STD_VERSION = -std=c++11
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
FLAGS = $(STD_VERSION) $(LLIBFLAGS) $(LDFLAGS) $(LINKFLAGS)
SOURCES=main.cpp simEngine.cpp simGUI.cpp eventHandler.cpp simEntity.cpp\
	simPhysics.cpp simSensor.cpp simSensorCamera.cpp simRobot.cpp
OBJECTS=$(SOURCES:.cpp=.o)

ifeq ($(OS_NAME),Darwin)

main: $(OBJECTS)
	@mkdir -p irr.app
	@mkdir -p irr.app/Contents
	@mkdir -p irr.app/Contents/MacOS
	$(CC) -o irr.app/Contents/MacOS/irr $(OBJECTS) $(FLAGS)

else ifeq ($(OS_NAME),Linux)

main: $(OBJECTS)
	$(CC) -o main.out $(OBJECTS) $(FLAGS)

endif

main.o: main.cpp simEngine.o
	$(CC) -c main.cpp $(LDFLAGS)

simEngine.o: simEngine.h simGUI.o simEntity.o simPhysics.o
	$(CC) -c simEngine.cpp $(LDFLAGS)

simGUI.o: simGUI.h eventHandler.o simEntity.o 
	$(CC) -c simGUI.cpp $(LDFLAGS)

eventHandler.o: eventHandler.h 
	$(CC) -c eventHandler.cpp $(LDFLAGS)

simSensorCamera.o: simSensorCamera.h simSensor.o
	$(CC) -c simSensorCamera.cpp $(LDFLAGS)

simSensor.o: simSensor.h simEntity.o
	$(CC) -c simSensor.cpp $(LDFLAGS)

simRobot.o: simRobot.h simEntity.o
	$(CC) -c simRobot.cpp $(LDFLAGS)

simEntity.o: simEntity.h
	$(CC) -c simEntity.cpp $(LDFLAGS)

simPhysics.o: simPhysics.h
	$(CC) -c simPhysics.cpp $(LDFLAGS)

re: 
	rm *.o; make

rt: 
	rm *.o; make test

test: $(OBJECTS)
	$(CC) -o main.out $(OBJECTS) $(FLAGS)
