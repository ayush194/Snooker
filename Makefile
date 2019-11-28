SHELL = /bin/bash

TARGET = snooker
OBJS = main.o App.o Environment.o Gamestate.o Pooltable.o Cuestick.o Ball.o objloader.o texture.o
CPPFLAGS = -std=c++11 -Wall -g
CC = g++
INCLUDEPATH = -I/usr/local/include/reactphysics3d -I/usr/local/include -I.
INCLUDES = opengl_includes.h glm_includes.h
LIBS = -lglfw -lreactphysics3d
UNAME := $(shell uname)
FRAMEWORKS = 
ifeq ($(UNAME), Linux)
	FRAMEWORKS += -lGL -lGLU
	LIBS += -lGLEW
endif
ifeq ($(UNAME), Darwin)
	FRAMEWORKS += -framework OpenGL
	LIBS += -lglew
endif


${TARGET} : ${OBJS}
	#Note that on Linux, the linker flags ${LIBS} ${FRAMEWORKS} are required to be given at the end
	#but on OSX there is no such necessity. The linker goes through the files from left to right, 
	#and it has to know which symbols from libraries you need, before the libraries are processed.
	#view post https://stackoverflow.com/questions/53028300/cant-link-glfw-library-with-its-header-file-on-ubuntu-18-04
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -o ${TARGET} ${OBJS} ${LIBS} ${FRAMEWORKS}

main.o : main.cpp ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c main.cpp

App.o : App.cpp App.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c App.cpp

Environment.o : Environment.cpp Environment.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c Environment.cpp

Gamestate.o : Gamestate.cpp Gamestate.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c Gamestate.cpp

Pooltable.o : Pooltable.cpp Pooltable.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c Pooltable.cpp

Cuestick.o : Cuestick.cpp Cuestick.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c Cuestick.cpp

Ball.o : Ball.cpp Ball.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c Ball.cpp

objloader.o : objloader.cpp objloader.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c objloader.cpp

texture.o : texture.cpp texture.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c texture.cpp

App.h Environment.h Gamestate.h Pooltable.h Cuestick.h Ball.h objloader.h texture.h : 

${INCLUDES} : 

clean : 
	@rm ${OBJS} ${TARGET}
