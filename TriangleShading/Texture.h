#pragma once

#include "Error.h"

#include <string>

#include <GL/glew.h>

class Texture {
public:
  Texture(const std::string &filename);
  ~Texture();

  ERROR load(const std::string &path);

  void use() const;

private:
  ERROR setFormat(int channels);

  GLuint m_textureID;
  GLenum m_format;
  int m_width;
  int m_height;
  int m_bitDepth;

  std::string m_filename;
};