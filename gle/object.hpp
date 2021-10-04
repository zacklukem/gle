#ifndef GLE_OBJECT_HPP
#define GLE_OBJECT_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

class Object {
public:
  /// @brief Construct a new Object object
  ///
  /// @param shader
  /// @param mesh
  inline Object(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh);

  /// @brief Get the object's shader
  ///
  /// @return the object's shader
  inline std::shared_ptr<Shader> shader();

  /// @brief Get the object's mesh
  ///
  /// @return the object's mesh
  inline std::shared_ptr<Mesh> mesh();

private:
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Mesh> _mesh;
};

GLE_NAMESPACE_END

#include "object.inl"

#endif // GLE_OBJECT_HPP
