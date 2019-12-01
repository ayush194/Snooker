#ifndef APP_H
#define APP_H

#include "opengl_includes.h"

//glm for matrix operations
//Flag -I/usr/local/include required for compiling with g++
#include "glm_includes.h"
#include "camera.h"
//#include "Gamestate.h"

#include <cmath>
#include <iostream>

class App {
public:
    //Add an extra property APP_NAME which stores the name of the app
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    static float LAST_TIME;
    static float DELTA_TIME;
    static float CURSOR_POS[2];
    static bool F_DOWN;
    static bool LBUTTON_DOWN;
    static float LBUTTON_DOWN_CURSOR_POS[2];
    static bool FIRST_MOUSE;
    static Camera CAMERA;
    static GLFWwindow* window;
    static bool HIT;
    //static GameState* gamestate;

    App();
    static void updateFrame();
    static void endFrame();
    static void clearColor();
    static glm::mat4 getViewMatrix();
    static glm::mat4 getPerspectiveProjectionMatrix();
    static glm::mat4 getOrthographicProjectionMatrix();
    static void cursorPosCallback(GLFWwindow*, double, double);
    static void mouseButtonCallback(GLFWwindow*, int, int, int);
    static void scrollCallback(GLFWwindow*, double, double);
    static void framebufferSizeCallback(GLFWwindow*, int, int);
    static void errorCallback(int, const char*);
    static void processInput();
    static float getMouseDrag();
    static unsigned int loadTexture(char const*, unsigned int);
    static void drawAxes();
};

#endif