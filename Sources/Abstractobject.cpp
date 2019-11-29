#include <Headers/Abstractobject.h>

AbstractObject::AbstractObject(int id) : id(id) {
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
    printf("Object id: %d, texture_id: %d\n", this->id, this->texture_id);
}

AbstractObject::~AbstractObject() {}
