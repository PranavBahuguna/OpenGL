#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed,
         GLfloat turnSpeed);
  ~Camera();

  void keyControl(bool *keys, GLfloat deltaTime);
  void mouseControl(GLfloat deltaX, GLfloat deltaY, GLfloat deltaTime);

  void setProjection(float fov, float aspectRatio, float near, float far) {
    m_projection = glm::perspective(fov, aspectRatio, near, far);
  }

  glm::mat4 getView() const { return glm::lookAt(m_position, m_position + m_front, m_up); }
  glm::mat4 getProjection() const { return m_projection; }

private:
  void updateDirection();

  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;

  glm::mat4 m_projection;

  GLfloat m_yaw;
  GLfloat m_pitch;

  GLfloat m_moveSpeed;
  GLfloat m_turnSpeed;
};