#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "opengl_includes.h"
#include "glm_includes.h"
#include "objloader.hpp"
#include "shader.h"
#include "texture.h"

#include <iostream>

class Environment {
public:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture_id;

    glm::vec3 pos;
    glm::mat4 model;
    //all objects share the same common vertex data
    //and the common material data
    static std::vector<Vertex> vertices;
    static std::vector<Material*> mtls;
    static std::vector<std::vector<std::string>> cubemap_paths;

    Environment();
    ~Environment();
    glm::mat4& getModelMatrix();
    static void loadVertexData();
    void render(Shader*);
};

#endif