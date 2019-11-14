#include "Cuestick.h"

std::vector<Vertex> CueStick::vertices;
std::vector<Material*> CueStick::mtls;

CueStick::CueStick(int id) : id(id) {
    this->model = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //this->model = glm::translate(glm::vec3(0.0, 0.0, 1.5));       //just for debugging
    //this->animate = false;
    this->animate_factor = 0.9f;

    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(1, &(this->vbo));

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, CueStick::vertices.size()*sizeof(Vertex), &(CueStick::vertices[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)(2*sizeof(glm::vec3) + sizeof(glm::vec2)));

    this->texture_id = loadTexture(CueStick::mtls[0]->map_kd, 0);
    printf("Cuestick id: %d, texture_id: %d\n", this->id, this->texture_id);
}

CueStick::~CueStick() {
    glDeleteVertexArrays(1, &(this->vao));
    glDeleteBuffers(1, &(this->vbo));
    glDeleteTextures(1, &(this->texture_id));
}

void CueStick::animate(float radius) {
    //this->animate = true;
    //std::cout << radius << std::endl;
    this->animate_factor = 0.7 * exp(-radius / 40) + 0.2;
}

void CueStick::unanimate() {
    //this->animate = false;
    this->animate_factor = 0.9;
}

void CueStick::loadVertexData() {
    loadOBJ("models/cuestick.obj", CueStick::vertices, CueStick::mtls);
}

glm::mat4& CueStick::getModelMatrix() {
    //this->model = glm::translate(this->pos);
    return this->model;
}

void CueStick::render(Shader* shader) {
    //application needs to set shader before calling render
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    shader->setInt("diffuse_textures["+std::to_string(0)+"]", 0);
    //the last argument in the above method calls states from which texture unit 
    //the sampler_2d will take its texture from
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, CueStick::vertices.size());
}