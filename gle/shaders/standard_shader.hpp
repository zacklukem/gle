#ifndef GLE_SHADERS_STANDARD_SHADER_HPP
#define GLE_SHADERS_STANDARD_SHADER_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>

GLE_NAMESPACE_BEGIN

struct StandardMaterial : public Material {
  const Texture &color;
  const Texture &normal;
  const Texture &depth_map;
  float height_scale;
  float diffuse;
  float specular;
  inline StandardMaterial(const Texture &color, const Texture &normal,
                          const Texture &depth_map, float height_scale,
                          float diffuse, float specular);
  inline virtual void load(const Shader &shader) const override;
  inline virtual void preload(const Shader &shader) const override;
};

class StandardShader : public Shader {
public:
  typedef StandardMaterial material_type;

  inline StandardShader();
};

GLE_NAMESPACE_END

#endif // GLE_SHADERS_STANDARD_SHADER_HPP
