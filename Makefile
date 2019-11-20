SHELL = /bin/bash

TARGET = snooker
OBJS = main.o App.o Environment.o Gamestate.o Pooltable.o Cuestick.o Ball.o objloader.o texture.o
CPPFLAGS = -std=c++11 -Wall -g
CC = g++
INCLUDEPATH = -I/usr/local/include/reactphysics3d -I/usr/local/include -I.
INCLUDES = opengl_includes.h glm_includes.h
LIBS = -lglfw -lglew -lreactphysics3d
FRAMEWORKS = -framework OpenGL 

${TARGET} : ${OBJS}
	@${CC} ${CPPFLAGS} ${FRAMEWORKS} ${LIBS} ${INCLUDEPATH} -o ${TARGET} ${OBJS}

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
