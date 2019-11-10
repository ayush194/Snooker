#ifndef BALL_H
#define BALL_H

#include "glm_includes.h"
#include "opengl_includes.h"
#include <string>
#include "shader.h"
#include "App.h"

//model loading library
#include "objloader.hpp"
#include <stdio.h>

class Ball {
public:
    //stores the index to the pointer of the vertex struct which stores
    //the configuration for vertex attribtes and which buffer to use 
    //for each vertex attribute
    unsigned int vao;
    //stores the index to the pointer to the buffer
    //this buffer will store the complete vertex data in interleaved form
    unsigned int vbo;
    //id is the index of the material which the ball uses from the mtls array
    //the id for cue ball is -1 which means it uses no material
    unsigned int texture_id;
    int id;
    //can get the model matrix using position and rotation attributes
    glm::vec3 pos;
    glm::mat4 model;
    //all objects share the same common vertex data
    //and the common material data
    static std::vector<Vertex> vertices;
    static std::vector<Material*> mtls;
    static float radius;
    static float mass;
    //static Shader* shader;

    Ball(int);
    ~Ball();
    static void loadVertexData();
    glm::mat4& getModelMatrix();
    void render(Shader*);
};

#endif