#pragma once

#include "Error.h"

#include <unordered_map>

#include <GL/glew.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H

#define CHAR_ARRAY_SIZE 128

struct Character {
  GLuint textureId;   // ID handle of the glyph texture
  glm::ivec2 size;    // Size of glyph
  glm::ivec2 bearing; // Offset from baseline to left/top of glyph
  FT_Pos advance;     // Offset to advance to next glyph
};

class Font {
public:
  Font(const std::string &name);

private:
  ERROR load(const std::string &filepath, FT_Library &ft);
  const char *getFTErrorMsg(const FT_Error &err);

  Character m_characters[CHAR_ARRAY_SIZE];
};