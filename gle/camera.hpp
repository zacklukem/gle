#ifndef GLE_CAMERA_HPP
#define GLE_CAMERA_HPP

#include <gle/common.hpp>
#include <glm/glm.hpp>

GLE_NAMESPACE_BEGIN

/// @brief A camera object
///
class Camera {
public:
  /// @brief Construct a new Camera object
  ///
  /// @param origin
  /// @param up
  /// @param target
  /// @param aspect
  /// @param fov
  /// @param z_near
  /// @param z_far
  inline Camera(glm::vec3 origin, glm::vec3 up, glm::vec3 target, float aspect,
                float fov, float z_near, float z_far);

  /// @brief get the camera view matrix
  ///
  /// @return const glm::mat4&
  inline const glm::mat4 &view_matrix();

  /// @brief get the camera projection matrix
  ///
  /// @return const glm::mat4&
  inline const glm::mat4 &projection_matrix();

private:
  glm::mat4 _view_matrix;
  glm::mat4 _projection_matrix;

  float aspect;
  float fov;
  float z_near;
  float z_far;

  glm::vec3 origin;
  glm::vec3 up;
  glm::vec3 target;
};

GLE_NAMESPACE_END

#include "camera.inl"

#endif // GLE_CAMERA_HPP
