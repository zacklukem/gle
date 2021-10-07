#ifndef GLE_SHADERS_STANDARD_SHADER_HPP
#define GLE_SHADERS_STANDARD_SHADER_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>

GLE_NAMESPACE_BEGIN

struct StandardMaterial : public Material {
  std::shared_ptr<Texture> color;
  std::shared_ptr<Texture> normal;
  float diffuse;
  float specular;
  inline StandardMaterial(std::shared_ptr<Texture> color,
                          std::shared_ptr<Texture> normal, float diffuse,
                          float specular);
  inline virtual void load(Shader &shader) const override;
};

class StandardShader : public Shader {
public:
  typedef StandardMaterial material_type;

  inline StandardShader();
};

GLE_NAMESPACE_END

#include "standard_shader.inl"

#endif // GLE_SHADERS_STANDARD_SHADER_HPP
