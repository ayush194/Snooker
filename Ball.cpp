#include "Ball.h"

#include "objloader.hpp"

std::vector<Vertex> Ball::vertices;
std::vector<Material*> Ball::mtls;
float Ball::radius = 0.132;
float Ball::mass = 0.2;
//Shader* Ball::shader = new Shader("shaders/diffuse_ball.vs", "shaders/diffuse_ball.fs");

Ball::Ball(int id) : id(id) {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &(this->vbo));

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    for(auto& v : vertices) v.mtl_idx = this->id;
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)(2*sizeof(glm::vec3) + sizeof(glm::vec2)));

    this->model = glm::mat4(1.0f);
    this->texture_id = App::loadTexture(Ball::mtls[this->id]->map_kd, id);
    printf("Ball id: %d, texture_id: %d\n", this->id, this->texture_id);
}

Ball::~Ball() {
    glDeleteVertexArrays(1, &(this->vao));
    glDeleteBuffers(1, &(this->vbo));
    glDeleteTextures(1, &(this->texture_id));
}

void Ball::loadVertexData() {
    std::vector<Material*> tmp1;
    std::unordered_map<std::string, unsigned int> tmp2;
    loadOBJ("models/ball.obj", Ball::vertices, tmp1);
    parseMTL("models/ball.mtl", tmp2, Ball::mtls);
}

glm::vec3 Ball::getPosition() {
    //return the last column of the model matrix
    return glm::vec3(this->model[3]);
}

glm::mat4& Ball::getModelMatrix() {
    //note that glm postmultiplies the matrix which means
    //this->model = glm::translate(this->model, glm::vec3(a, b, c))
    //is equivalent to this->model = this->model * glm::translate(vec3(a, b, c))
    //you could also just use the * operator to multiply matrices
    //for example to scale, and then translate
    //this->model = glm::translate(glm::vec3(0, 0, 0.55)) * glm::scale(glm::vec3(3, 3, 3));
    //this->model = glm::translate(glm::vec3(this->pos));
    return this->model;
}

void Ball::render(Shader* shader) {
    //application needs to set shader before calling render
    glActiveTexture(GL_TEXTURE0 + this->id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    shader->setInt("diffuse_textures["+std::to_string(this->id)+"]", this->id);
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, Ball::vertices.size());
}