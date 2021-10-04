#ifndef GLE_SHADER_HPP
#define GLE_SHADER_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <optional>
#include <string>

GLE_NAMESPACE_BEGIN

/// @brief A complete shader program including vertex, fragment and geometry
///        shaders
class Shader {
public:
  Shader(Shader &other) = delete;
  Shader(Shader &&other) = delete;
  Shader(const Shader &other) = delete;
  Shader(const Shader &&other) = delete;

  /// @brief Construct a new Shader object
  ///
  /// @param vertex_source
  /// @param fragment_source
  inline Shader(const std::string &vertex_source,
                const std::string &fragment_source);

  /// @brief Construct a new Shader object with a geometry shader
  ///
  /// @param vertex_source
  /// @param fragment_source
  /// @param geometry_source
  inline Shader(const std::string &vertex_source,
                const std::string &fragment_source,
                const std::string &geometry_source);

  inline ~Shader();

  /// @brief Loads and compiles the given shader sources.
  /// Must be called after GL is initialized
  inline void load();

  /// @brief Activates this shader program
  /// Must be called after load()
  inline void use();

private:
  std::string vertex_source;
  std::string fragment_source;
  std::optional<std::string> geometry_source;
  GLuint program;
  GLuint vertex_shader;
  GLuint geometry_shader;
  GLuint fragment_shader;
};

GLE_NAMESPACE_END

#include "shader.inl"

#endif // GLE_SHADER_HPP
