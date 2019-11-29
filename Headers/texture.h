#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl_includes.h"
#include <iostream>
#include <vector>
#include <string>

unsigned int loadTexture(char const *, unsigned int);
unsigned int loadCubemap(std::vector<std::string> faces, unsigned int);

#endif