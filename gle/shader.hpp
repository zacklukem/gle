#ifndef GLE_SHADER_HPP
#define GLE_SHADER_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <gle/light.hpp>
#include <gle/scene.hpp>
#include <gle/texture.hpp>
#include <optional>
#include <string>

GLE_NAMESPACE_BEGIN

struct Material {
  /// @brief Load the material into a shader
  ///
  /// @pure
  /// @param shader
  inline virtual void load(const Shader &shader) const = 0;
  inline virtual void preload(const Shader &shader) const;

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
  inline void load(const Shader &shader) const;
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
                const std::string &fragment_source,
                bool include_headers = true);

  /// @brief Construct a new Shader object with a geometry shader
  ///
  /// @param vertex_source
  /// @param fragment_source
  /// @param geometry_source
  inline Shader(const std::string &vertex_source,
                const std::string &fragment_source,
                const std::string &geometry_source,
                bool include_headers = true);

  virtual inline ~Shader();

  /// @brief Loads and compiles the given shader sources.
  ///
  /// Must be called after GL is initialized
  /// @exception std::runtime_error thrown if the shader fails to compile
  inline void load();

  inline void use() const;

  /// @brief Use the shader and load the given uniforms
  ///
  /// @param uniforms
  inline void use(const Scene &scene, const MVPShaderUniforms &uniforms,
                  const Material &material) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, float val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::vec2 &val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::vec3 &val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::vec4 &val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::mat2 &val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::mat3 &val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, const glm::mat4 &val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, std::uint32_t val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param val
  inline void uniform(const char *name, std::int32_t val) const;

  /// @brief Set the given uniform to the given value
  ///
  /// @param name
  /// @param i
  /// @param val
  inline void uniform(const char *name, GLuint i, const Texture &val) const;

  inline bool is_loaded() const;

protected:
  /// @brief Runs when the shader is used
  ///
  inline virtual void on_use() const;

private:
  std::string vertex_source;
  std::string fragment_source;
  std::optional<std::string> geometry_source;
  GLuint program;
  GLuint vertex_shader;
  GLuint geometry_shader;
  GLuint fragment_shader;
  bool _is_loaded;
};

GLE_NAMESPACE_END

#endif // GLE_SHADER_HPP
