#ifndef GLE_SHADERS_SOLID_COLOR_SHADER_HPP
#define GLE_SHADERS_SOLID_COLOR_SHADER_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>

GLE_NAMESPACE_BEGIN

struct SolidColorMaterial : public Material {
  glm::vec3 color;
  float diffuse;
  float specular;
  inline SolidColorMaterial(const glm::vec3 &color, float diffuse,
                            float specular);
  inline virtual void load(const Shader &shader) const override;
};

class SolidColorShader : public Shader {
public:
  typedef SolidColorMaterial material_type;

  inline SolidColorShader();
};

GLE_NAMESPACE_END

#include "solid_color_shader.inl"

#endif // GLE_SHADERS_SOLID_COLOR_SHADER_HPP
