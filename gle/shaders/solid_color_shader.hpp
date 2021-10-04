#ifndef GLE_SHADERS_SOLID_COLOR_SHADER_HPP
#define GLE_SHADERS_SOLID_COLOR_SHADER_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>

GLE_NAMESPACE_BEGIN

class SolidColorShader : public Shader {
public:
  inline SolidColorShader();
};

GLE_NAMESPACE_END

#include "solid_color_shader.inl"

#endif // GLE_SHADERS_SOLID_COLOR_SHADER_HPP
