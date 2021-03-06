#pragma once

#include "Model.h"

class Plane : public Model {
public:
  Plane(const std::string &name, const glm::uvec2 &nTiles, const glm::vec2 &tileDimensions,
        const glm::vec2 &tileTexMapping = {1.0f, 1.0f}, bool loadNow = true);
  Plane(const std::shared_ptr<GameObject> &owner, const std::string &name, const glm::uvec2 &nTiles,
        const glm::vec2 &tileDimensions, const glm::vec2 &tileTexMapping = {1.0f, 1.0f},
        bool loadNow = true);

  void load(ERROR &errCode) override;

private:
  void generateMesh();

  glm::uvec2 m_nTiles;
  glm::vec2 m_tileDimensions;
  glm::vec2 m_tileTexMapping;
};