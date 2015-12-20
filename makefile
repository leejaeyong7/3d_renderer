CC = g++

OS_NAME := $(shell uname)

# ON OS X with HOMEBREW install
ifeq ($(OS_NAME),Darwin)
# SET INCLUDE FLAGS TO HOMEBREW CELLAR DIR
LDFLAGS = -I/usr/local/Cellar/Irrlicht/1.8.1/include/irrlicht -I/opt/local/include

#SET LIB FLAGS TO HOMEBREW CELLAR LIB DIR
LLIBFLAGS = -L/usr/local/Cellar/Irrlicht/1.8.1/lib -L/usr/X11/lib

OPENGL_FLAGS = -framework OpenGL -framework GLUT -framework Cocoa -framework IOkit

# SPECIFIY LINK OPTIONS
LINKFLAGS = -l Irrlicht $(OPENGL_FLAGS) -lXext -lXrandr -lX11 
else ifeq ($(OS_NAME),Linux)
endif

# FINAL FLAG
FLAGS = $(LLIBFLAGS) $(LDFLAGS) $(LINKFLAGS)

main: main.cpp
	$(CC) $(FLAGS) -o main.out main.cpp
