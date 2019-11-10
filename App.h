#ifndef APP_H
#define APP_H

#include "opengl_includes.h"

//glm for matrix operations
//Flag -I/usr/local/include required for compiling with g++
#include "glm_includes.h"
#include "camera.h"
#include <iostream>

class App {
public:
    //Add an extra property APP_NAME which stores the name of the app
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    static float LAST_TIME;
    static float DELTA_TIME;
    static float CURSOR_POS_X;
    static float CURSOR_POS_Y;
    static bool FIRST_MOUSE;
    static Camera CAMERA;
    static GLFWwindow* window;

    App();
    static void updateFrame();
    static void endFrame();
    static void clearColor();
    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix();
    static void mouseCallback(GLFWwindow*, double, double);
    static void scrollCallback(GLFWwindow*, double, double);
    static void framebufferSizeCallback(GLFWwindow*, int, int);
    static void processInput();
    static unsigned int loadTexture(char const*, unsigned int);
    static void drawAxes();
};

#endif