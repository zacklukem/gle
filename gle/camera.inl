#include <glm/gtc/matrix_transform.hpp>

GLE_NAMESPACE_BEGIN

inline Camera::Camera(glm::vec3 origin, glm::vec3 up, glm::vec3 target,
                      float aspect, float fov, float z_near, float z_far)
    : aspect(aspect), fov(fov), z_near(z_near), z_far(z_far), origin(origin),
      up(up), target(target) {
  update_view_projection();
}

inline void Camera::update_view_projection() {
  _view_matrix = glm::lookAt(origin, target, up);
  _projection_matrix = glm::perspective(fov, aspect, z_near, z_far);
}

inline const glm::mat4 &Camera::view_matrix() { return _view_matrix; }
inline const glm::mat4 &Camera::projection_matrix() {
  return _projection_matrix;
}

GLE_NAMESPACE_END