#include "Environment.h"

std::vector<Vertex> Environment::vertices;
std::vector<Material*> Environment::mtls;
std::vector<std::vector<std::string> > Environment::cubemap_paths = {
        {   "/Users/ayushk/Desktop/Snooker/skyboxes/ame_bluefreeze/bluefreeze_rt.tga",
            "/Users/ayushk/Desktop/Snooker/skyboxes/ame_bluefreeze/bluefreeze_lf.tga",
            "/Users/ayushk/Desktop/Snooker/skyboxes/ame_bluefreeze/bluefreeze_up.tga",
            "/Users/ayushk/Desktop/Snooker/skyboxes/ame_bluefreeze/bluefreeze_dn.tga",
            "/Users/ayushk/Desktop/Snooker/skyboxes/ame_bluefreeze/bluefreeze_ft.tga",
            "/Users/ayushk/Desktop/Snooker/skyboxes/ame_bluefreeze/bluefreeze_bk.tga"},
        {
            "/Users/ayushk/Desktop/Snooker/right.jpg",
            "/Users/ayushk/Desktop/Snooker/left.jpg",
            "/Users/ayushk/Desktop/Snooker/top.jpg",
            "/Users/ayushk/Desktop/Snooker/bottom.jpg",
            "/Users/ayushk/Desktop/Snooker/front.jpg",
            "/Users/ayushk/Desktop/Snooker/back.jpg"},
        {   
            "/Users/ayushk/Desktop/Snooker/skyboxes/mountain/posx.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/mountain/negx.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/mountain/posy.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/mountain/negy.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/mountain/posz.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/mountain/negz.jpg"},
        {   
            "/Users/ayushk/Desktop/Snooker/skyboxes/icelake/posx.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/icelake/negx.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/icelake/posy.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/icelake/negy.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/icelake/posz.jpg",
            "/Users/ayushk/Desktop/Snooker/skyboxes/icelake/negz.jpg"}
};

Environment::Environment() {
    this->model = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(16.0, 16.0, 16.0));

    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(1, &(this->vbo));

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, Environment::vertices.size()*sizeof(Vertex), &(Environment::vertices[0]), GL_STATIC_DRAW);
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
    this->texture_id = loadCubemap(Environment::cubemap_paths[2], 0);
    //printf("Environment id: %d, texture_id[0]: %d, texture_id[1] = %d\n", this->id, this->texture_id[0], this->texture_id[1]);
}

Environment::~Environment() {
    glDeleteVertexArrays(1, &(this->vao));
    glDeleteBuffers(1, &(this->vbo));
    glDeleteTextures(1, &(this->texture_id));
}

void Environment::loadVertexData() {
    loadOBJ("models/cube.obj", Environment::vertices, Environment::mtls);
}

glm::mat4& Environment::getModelMatrix() {
    return this->model;
}

void Environment::render(Shader* shader) {
    //application needs to set shader before calling render
    glDepthFunc(GL_LEQUAL);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_id);
    //shader->setInt("diffuse_textures["+std::to_string(0)+"]", 0);
    //shader->setInt("diffuse_textures["+std::to_string(1)+"]", 1);
    shader->setInt("skybox", 0);
    //the last argument in the above method calls states from which texture unit 
    //the sampler_2d will take its texture from
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, Environment::vertices.size());
    glDepthFunc(GL_LESS);
}