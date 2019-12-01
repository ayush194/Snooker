#ifndef FONT_H
#define FONT_H

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
#include <Headers/opengl_includes.h>
#include <Headers/glm_includes.h>
#include <Headers/shader.h>

#include <iostream>
#include <map>
#include <string>

/// Holds all state information relevant to a character as loaded using FreeType
typedef struct Character {
    GLuint texture_id;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    GLuint advance;    // Horizontal offset to advance to next glyph
} Character;

class Font {
public:
    Font(const std::string&);
    ~Font();
    FT_Library ft;
    FT_Face face;
    std::map<GLchar, Character> characters;
    GLuint vao;
    GLuint vbo;
    std::string font_name;

    void render(Shader*, const std::string&, GLfloat, GLfloat, GLfloat);
};

#endif