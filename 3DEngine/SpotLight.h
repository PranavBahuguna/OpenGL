#pragma once

#include "Light.h"
#include "Projection.h"
#include "Transform.h"

class SpotLight : public Light {
public:
  SpotLight(const Transform &transform, const glm::vec3 &color, const Phong &phong,
            const Attenuation &attenuation, float innerConeAngle, float outerConeAngle,
            bool isShadowCaster = false);

  void setLightSpaceMatrix(const Shader &shader) const override;
  void update(ERROR &errCode) override;

private:
  void use(const Shader &shader, const std::string &prefix) const override;

  Projection m_projection;
};