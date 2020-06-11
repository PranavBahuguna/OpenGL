#include "Material.h"

#include <stdexcept>

// Constructor
Material::Material(const aiMaterial &mat) {
  mat.Get(AI_MATKEY_COLOR_AMBIENT, m_ambient);
  mat.Get(AI_MATKEY_COLOR_DIFFUSE, m_diffuse);
  mat.Get(AI_MATKEY_COLOR_SPECULAR, m_specular);
  mat.Get(AI_MATKEY_SHININESS, m_shininess);
}

// Use this material
void Material::use(const Shader &shader, ERROR &errCode) const {
  GLuint ambientId = shader.getParamId("material.ambient", errCode);
  GLuint diffuseId = shader.getParamId("material.diffuse", errCode);
  GLuint specularId = shader.getParamId("material.specular", errCode);
  GLuint shininessId = shader.getParamId("material.shininess", errCode);

  if (errCode != ERROR_OK)
    return;

  glUniform3f(ambientId, m_ambient.r, m_ambient.g, m_ambient.b);
  glUniform3f(diffuseId, m_diffuse.r, m_diffuse.g, m_diffuse.b);
  glUniform3f(specularId, m_specular.r, m_specular.g, m_specular.b);
  glUniform1f(shininessId, m_shininess);
}
