#include "Pooltable.h"

std::vector<Vertex> PoolTable::vertices;
std::vector<Material*> PoolTable::mtls;

float PoolTable::holes[][3] = {
                                {-3.028,  -5.473, 1.080},       //Hole 0
                                {-3.107,  -0.010, 1.080},       //Hole 1
                                {-3.028,  5.473,  1.080},       //Hole 2
                                {3.054,   5.473,  1.080},       //Hole 3
                                {3.107,   0.010,  1.080},       //Hole 4
                                {3.054,   -5.473, 1.080},       //Hole 5
                            };

	//Radius of hole -> 0.300
	//Right wall at x = 3.054
	//Bottom wall at y = -5.474
	//Top Wall at y = 5.473
	//Left Wall at x = -3.028
	//Surface of the table at z = 0.492

PoolTable::PoolTable(int id) : id(id) {
    this->model = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(1, &(this->vbo));

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, PoolTable::vertices.size()*sizeof(Vertex), &(PoolTable::vertices[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)(2*sizeof(glm::vec3) + sizeof(glm::vec2)));

    //this object will use TextureUnits (GL_TEXTURE0) and (GL_TEXTURE0 + 1)
    //to bind two textures (wood and fabric_green). Note that the GL_TEXTURE_2D targets of
    //these texture units will start pointing to the ball textures when the balls of id 0 and 1
    //are rendered. Hence the target GL_TEXTURE_2D of these two texture units should be rebound to
    //texture_id[0] and texture_id[1] before rendering.
    this->texture_id[0] = loadTexture(PoolTable::mtls[0]->map_kd, 0);
    this->texture_id[1] = loadTexture(PoolTable::mtls[1]->map_kd, 1);
    printf("PoolTable id: %d, texture_id[0]: %d, texture_id[1] = %d\n", this->id, this->texture_id[0], this->texture_id[1]);
}

PoolTable::~PoolTable() {
    glDeleteVertexArrays(1, &(this->vao));
    glDeleteBuffers(1, &(this->vbo));
    glDeleteTextures(2, this->texture_id);
}

void PoolTable::loadVertexData() {
    loadOBJ("models/pooltable.obj", PoolTable::vertices, PoolTable::mtls);
}

glm::mat4& PoolTable::getModelMatrix() {
    return this->model;
}

void PoolTable::drawBoundary() {
    float boundary[] = {    holes[0][0], holes[0][1], 1.2, 1.0, 1.0, 0.0,   holes[2][0], holes[2][1], 1.2, 1.0, 1.0, 0.0, 
                            holes[2][0], holes[2][1], 1.2, 1.0, 1.0, 0.0,   holes[3][0], holes[3][1], 1.2, 1.0, 1.0, 0.0, 
                            holes[3][0], holes[3][1], 1.2, 1.0, 1.0, 0.0,   holes[5][0], holes[5][1], 1.2, 1.0, 1.0, 0.0,
                            holes[5][0], holes[5][1], 1.2, 1.0, 1.0, 0.0,   holes[0][0], holes[0][1], 1.2, 1.0, 1.0, 0.0};
    unsigned int vbo_boundary, vao_boundary;
    glGenVertexArrays(1, &vao_boundary);
    glGenBuffers(1, &vbo_boundary);
    glBindVertexArray(vao_boundary);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_boundary);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boundary), boundary, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    //glLineWidth(3.0f);          //change line width
    glDrawArrays(GL_LINES, 0, 8);
    //glLineWidth(1.0f);          //reset line width

    //unbind vertex arrays and buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_boundary);
    glDeleteBuffers(1, &vbo_boundary);
}

void PoolTable::render(Shader* shader) {
    //application needs to set shader before calling render
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id[0]);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, this->texture_id[1]);
    shader->setInt("diffuse_textures["+std::to_string(0)+"]", 0);
    shader->setInt("diffuse_textures["+std::to_string(1)+"]", 1);
    //the last argument in the above method calls states from which texture unit 
    //the sampler_2d will take its texture from
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, PoolTable::vertices.size());
}