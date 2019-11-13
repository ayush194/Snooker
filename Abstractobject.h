#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include "opengl_includes.h"
#include "glm_includes.h"
#include "objloader.hpp"
#include "shader.h"
#include "App.h"

class AbstractObject {
public:
    unsigned int vao;
    unsigned int vbo;
    //an object can load a maximum of 16 textures
    unsigned int texture_id[16];
    int id;

    glm::mat4 model;

    AbstractObject();
    ~AbstractObject();
    glm::mat4& getModelMatrix();
    virtual void loadVertexData() = 0;
    virtual void render(Shader*) = 0;
};


#endif