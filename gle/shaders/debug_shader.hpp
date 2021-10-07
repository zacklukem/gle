#ifndef GLE_SHADERS_DEBUG_SHADER_HPP
#define GLE_SHADERS_DEBUG_SHADER_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>

GLE_NAMESPACE_BEGIN

class DebugMaterial : public Material {
public:
  inline virtual void load(Shader &shader) const override;
};

class DebugShader : public Shader {
public:
  typedef DebugMaterial material_type;

  inline DebugShader();
};

GLE_NAMESPACE_END

#include "debug_shader.inl"

#endif // GLE_SHADERS_DEBUG_SHADER_HPP
