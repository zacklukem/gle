#ifndef GLE_OBJECT_HPP
#define GLE_OBJECT_HPP

#include <gle/common.hpp>
#include <gle/shader.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

/// @brief An object in a scene
///
class Object {
public:
  /// @brief Construct a new Object using the given material and shader
  ///
  /// The S type must be extend gle::Shader, and the M type must extend
  /// gle::Material.  S::material_type must be the same as M::material_type
  ///
  /// @tparam S The shader type
  /// @tparam M The material type
  /// @param shader
  /// @param material
  /// @param mesh
  /// @param position
  /// @param rotation
  /// @param scale
  template <class S, class M>
  inline Object(std::shared_ptr<S> shader, std::shared_ptr<M> material,
                std::shared_ptr<Mesh> mesh, const glm::vec3 &position,
                const glm::quat &rotation, const glm::vec3 &scale);

  /// @brief Get the object's shader
  ///
  /// @return the object's shader
  inline std::shared_ptr<Shader> shader();

  /// @brief Get the object's shader
  ///
  /// @return the object's shader
  inline std::shared_ptr<const Shader> shader() const;

  /// @brief Get the objet's material
  ///
  /// @return the object's material
  inline std::shared_ptr<const Material> material() const;

  /// @brief Get the object's mesh
  ///
  /// @return the object's mesh
  inline std::shared_ptr<Mesh> mesh();

  /// @brief Get the object's mesh
  ///
  /// @return the object's mesh
  inline std::shared_ptr<const Mesh> mesh() const;

  /// @brief get the object's position
  ///
  /// @return const glm::vec3&
  inline const glm::vec3 &position() const;

  /// @brief get the object's scale
  ///
  /// @return const glm::vec3&
  inline const glm::vec3 &scale() const;

  /// @brief get the object's euler rotation in radians
  ///
  /// @return const glm::vec3&
  inline glm::vec3 rotation_euler() const;

  /// @brief get the object's quaternion rotation
  ///
  /// @return const glm::vec3&
  inline const glm::quat &rotation() const;

  /// @brief set the position
  ///
  /// @param value
  inline void position(const glm::vec3 &value);

  /// @brief set the scale
  ///
  /// @param value
  inline void scale(const glm::vec3 &value);

  /// @brief set the rotation by euler angles in radians
  ///
  /// @param value
  inline void rotation(const glm::vec3 &value);

  /// @brief set the rotation by quaternion
  ///
  /// @param value
  inline void rotation(const glm::quat &value);

  /// @brief get the model matrix for this object
  ///
  /// @return const glm::mat4&
  inline const glm::mat4 &model_matrix() const;

private:
  inline void update_model_matrix();
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Material> _material;
  std::shared_ptr<Mesh> _mesh;
  glm::vec3 _position;
  glm::vec3 _scale;
  glm::quat _rotation;

  glm::mat4 _model;
};

GLE_NAMESPACE_END

#endif // GLE_OBJECT_HPP
