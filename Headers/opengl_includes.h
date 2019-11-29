#ifndef OPENGL_INCLUDES_H
#define OPENGL_INCLUDES_H

//GLEW is for external OpenGL function loading
//Flag -lglew required for compiling with g++
#include <GL/glew.h>
//GLFW for window and input handling as well as the main game loop
//Flag -lglfw required for compiling with g++
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//GLUT doesn't give you control over the main loop which is why it should only be used for testing purposes
//In other cases we use GLFW as a better substitute for it
//#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

#endif