#include "Resources.h"

std::shared_ptr<Mesh> Resources::GetMesh(const std::string &name,
                                         const std::vector<GLfloat> &vertices,
                                         const std::vector<GLfloat> &texCoords,
                                         const std::vector<GLfloat> &normals,
                                         const std::vector<GLuint> &indices) {
  std::weak_ptr<Mesh> &mesh = meshMap[name];
  if (mesh.expired()) {
    std::shared_ptr<Mesh> newMesh(new Mesh(vertices, texCoords, normals, indices));
    mesh = newMesh;
    return newMesh;
  } else {
    return mesh.lock();
  }
}

std::shared_ptr<Shader> Resources::GetShader(const std::string &name) {
  std::weak_ptr<Shader> &shader = shaderMap[name];
  if (shader.expired()) {
    std::shared_ptr<Shader> newShader(new Shader(name));
    shader = newShader;
    return newShader;
  } else {
    return shader.lock();
  }
}

std::shared_ptr<Texture> Resources::GetTexture(const std::string &name) {
  std::weak_ptr<Texture> &texture = textureMap[name];
  if (texture.expired()) {
    std::shared_ptr<Texture> newTexture(new Texture(name));
    texture = newTexture;
    return newTexture;
  } else {
    return texture.lock();
  }
}

std::shared_ptr<Material> Resources::GetMaterial(const aiMaterial &mat) {
  aiString name;
  mat.Get(AI_MATKEY_NAME, name);
  std::weak_ptr<Material> &material = materialMap[name.data];
  if (material.expired()) {
    std::shared_ptr<Material> newMaterial(new Material(mat));
    material = newMaterial;
    return newMaterial;
  } else {
    return material.lock();
  }
}

std::shared_ptr<Font> Resources::GetFont(const std::string &name) {
  std::weak_ptr<Font> &font = fontMap[name];
  if (font.expired()) {
    std::shared_ptr<Font> newFont(new Font(name));
    font = newFont;
    return newFont;
  } else {
    return font.lock();
  }
}