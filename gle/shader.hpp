#ifndef GLE_SHADER_HPP
#define GLE_SHADER_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <optional>
#include <string>

GLE_NAMESPACE_BEGIN

struct Material {
  /// @brief Load the material into a shader
  ///
  /// @pure
  /// @param shader
  inline virtual void load(Shader &shader) const = 0;

  inline virtual ~Material();
};

/// @brief Structure representing the matrices for model view and projection
///
struct MVPShaderUniforms {
  /// @brief the model matrix
  ///
  glm::mat4 model;

  /// @brief the view matrix
  ///
  glm::mat4 view;

  /// @brief the projection matrix
  ///
  glm::mat4 projection;

  /// @brief Construct a new MVPShaderUniforms object with the given values
  ///
  /// @param model
  /// @param view
  /// @param projection
  inline MVPShaderUniforms(const glm::mat4 &model, const glm::mat4 &view,
                           const glm::mat4 &projection);
  /// @brief Load the current values into the given shader's uniforms
  ///
  /// @param shader
  inline void load(Shader &shader) const;
};

/// @brief A complete shader program including vertex, fragment and geometry
///        shaders
class Shader {
public:
  Shader(Shader &other) = delete;
  Shader(Shader &&other) = delete;
  Shader(const Shader &other) = delete;
  Shader(const Shader &&other) = delete;

  typedef Material material_type;

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

  virtual inline ~Shader();

  /// @brief Loads and compiles the given shader sources.
  ///
  /// Must be called after GL is initialized
  /// @exception std::runtime_error thrown if the shader fails to compile
  inline void load();

  /// @brief Use the shader and load the given uniforms
  ///
  /// @param uniforms
  inline void use(const MVPShaderUniforms &uniforms,
                  std::shared_ptr<Material> material);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, float val);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::vec2 &val);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::vec3 &val);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::vec4 &val);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::mat2 &val);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::mat3 &val);

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::mat4 &val);

protected:
  /// @brief Runs when the shader is used
  ///
  inline virtual void on_use();

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
