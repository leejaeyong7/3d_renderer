CC = g++ -std=c++11
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
SOURCES=main.cpp simEngine.cpp simGUI.cpp eventHandler.cpp simEntity.cpp\
	simPhysics.cpp simSensor.cpp simSensorCamera.cpp simRobot.cpp \
	simRobotGround.cpp
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

###############################################################################
# DEPENDENCIES
###############################################################################
main.o: main.cpp simEngine.o
	$(CC) -c $< $(LDFLAGS)

simEngine.o: simEngine.cpp simEngine.h simGUI.o simEntity.o simSensor.o simRobot.o simPhysics.o
	$(CC) -c $< $(LDFLAGS)

simGUI.o: simGUI.cpp simGUI.h eventHandler.o simEntity.o simRobotGround.o simSensorCamera.o
	$(CC) -c $< $(LDFLAGS)

eventHandler.o: eventHandler.cpp eventHandler.h 
	$(CC) -c $< $(LDFLAGS)

simSensorCamera.o: simSensorCamera.cpp simSensorCamera.h simSensor.o
	$(CC) -c $< $(LDFLAGS)

simSensor.o: simSensor.cpp simSensor.h simEntity.o
	$(CC) -c $< $(LDFLAGS)

simRobotGround.o: simRobotGround.cpp simRobotGround.h simRobot.o
	$(CC) -c $< $(LDFLAGS)

simRobot.o: simRobot.cpp simRobot.h simEntity.o
	$(CC) -c $< $(LDFLAGS)

simEntity.o: simEntity.cpp simEntity.h
	$(CC) -c $< $(LDFLAGS)

simPhysics.o: simPhysics.cpp simPhysics.h
	$(CC) -c $< $(LDFLAGS)

clean:
	rm *.o

re: 
	make clean; make

rt: 
	make clean; make bin 

bin: $(OBJECTS)
	$(CC) -o main.out $(OBJECTS) $(FLAGS)
