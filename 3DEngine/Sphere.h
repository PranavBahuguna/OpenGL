#pragma once

#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

class Sphere : public Model {
public:
  Sphere();
  ~Sphere(){};

  void update(ERROR &errCode) override;
};