#include "Model.h"

// Constructor
Model::Model()
    : m_pos(glm::vec3(0.0f)), m_angle(0.0f), m_euler(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)),
      m_mesh(nullptr) {

  static unsigned int id = 0;
  _id = id++;
}

// Draws the model to the screen
ERROR Model::draw(const Camera &camera) {
  // Check that we have a mesh and shader to use
  ERROR errCode = ERROR_OK;
  if (!m_mesh)
    errCode = ERROR_MISSING_MESH;
  else if (!m_shader)
    errCode = ERROR_MISSING_SHADER;

  if (errCode != ERROR_OK) {
    printErrorMsg(errCode);
  } else {
    // Calculate the MVP matrix and draw
    const glm::mat4 mvp = camera.getProjection() * camera.getView() * getMatrix();
    m_shader->setMVP(mvp);
    m_mesh->draw();

    // Apply shader (and texture if applicable)
    m_shader->use();
    if (m_texture)
      m_texture->use();
  }

  return errCode;
}

// Gets the model matrix
glm::mat4 Model::getMatrix() const {
  glm::mat4 matrix = glm::mat4(1.0f);
  matrix = glm::translate(matrix, m_pos);
  matrix = glm::rotate(matrix, glm::radians(m_angle), m_euler);
  matrix = glm::scale(matrix, m_scale);

  return matrix;
}