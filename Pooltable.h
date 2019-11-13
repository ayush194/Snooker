#ifndef POOLTABLE_H
#define POOLTABLE_H

#include "opengl_includes.h"
#include "glm_includes.h"
#include "objloader.hpp"
#include "shader.h"
#include "App.h"

#include <stdio.h>
#include <vector>

class PoolTable {
public:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture_id[2];
    int id;

    glm::vec3 pos;
    glm::mat4 model;
    //all objects share the same common vertex data
    //and the common material data
    static std::vector<Vertex> vertices;
    static std::vector<Material*> mtls;
    static float holes[][3];
    //static Shader* shader;

    PoolTable(int);
    ~PoolTable();
    glm::mat4& getModelMatrix();
    static void loadVertexData();
    void drawBoundary();
    void render(Shader*);
};

#endif