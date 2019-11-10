#ifndef CUESTICK_H
#define CUESTICK_H

#include "opengl_includes.h"
#include "glm_includes.h"
#include "objloader.hpp"
#include "shader.h"
#include "App.h"

#include <stdio.h>
#include <vector>

class CueStick {
public:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture_id;
    int id;

    glm::vec3 pos;
    glm::mat4 model;
    //all objects share the same common vertex data
    //and the common material data
    static std::vector<Vertex> vertices;
    static std::vector<Material*> mtls;
    //static Shader* shader;

    CueStick(int);
    ~CueStick();
    glm::mat4& getModelMatrix();
    static void loadVertexData();
    void render(Shader*);
};

#endif