#ifndef GLE_LIGHT_HPP
#define GLE_LIGHT_HPP

#include <cstdint>
#include <gle/common.hpp>
#include <glm/glm.hpp>

GLE_NAMESPACE_BEGIN

enum LightType : std::uint32_t { POINT_LIGHT = 0, DIRECTIONAL_LIGHT = 1 };

struct Light {
  LightType type;
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 attn;
  inline Light(LightType type, const glm::vec3 &position,
               const glm::vec3 &direction, const glm::vec3 &color,
               float strength);
};

GLE_NAMESPACE_END

#include "light.inl"

#endif // GLE_LIGHT_HPP
