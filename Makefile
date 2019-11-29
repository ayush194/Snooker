SHELL = /bin/bash

TARGET = snooker
OBJS = main.o App.o Environment.o Gamestate.o Pooltable.o Cuestick.o Ball.o objloader.o texture.o
CPPFLAGS = -std=c++11 -Wall -g
CC = g++
INCLUDEPATH = -I/usr/local/include/reactphysics3d -I/usr/local/include -I.
INCLUDES = Headers/opengl_includes.h Headers/glm_includes.h
INCLUDES_DIR = Headers
SOURCES_DIR = Sources
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

App.o : ${SOURCES_DIR}/App.cpp $(INCLUDES_DIR)/App.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/App.cpp

Environment.o : ${SOURCES_DIR}/Environment.cpp $(INCLUDES_DIR)/Environment.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/Environment.cpp

Gamestate.o : ${SOURCES_DIR}/Gamestate.cpp $(INCLUDES_DIR)/Gamestate.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/Gamestate.cpp

Pooltable.o : ${SOURCES_DIR}/Pooltable.cpp $(INCLUDES_DIR)/Pooltable.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/Pooltable.cpp

Cuestick.o : ${SOURCES_DIR}/Cuestick.cpp $(INCLUDES_DIR)/Cuestick.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/Cuestick.cpp

Ball.o : ${SOURCES_DIR}/Ball.cpp $(INCLUDES_DIR)/Ball.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/Ball.cpp

objloader.o : ${SOURCES_DIR}/objloader.cpp $(INCLUDES_DIR)/objloader.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/objloader.cpp

texture.o : ${SOURCES_DIR}/texture.cpp $(INCLUDES_DIR)/texture.h ${INCLUDES}
	@${CC} ${CPPFLAGS} ${INCLUDEPATH} -c ${SOURCES_DIR}/texture.cpp

$(INCLUDES_DIR)/App.h $(INCLUDES_DIR)/Environment.h $(INCLUDES_DIR)/Gamestate.h :
$(INCLUDES_DIR)/Pooltable.h $(INCLUDES_DIR)/Cuestick.h $(INCLUDES_DIR)/Ball.h :
$(INCLUDES_DIR)/objloader.h $(INCLUDES_DIR)/texture.h : 

${INCLUDES} : 

clean : 
	@rm ${OBJS} ${TARGET}
