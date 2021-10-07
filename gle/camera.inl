#include <glm/gtc/matrix_transform.hpp>

GLE_NAMESPACE_BEGIN

inline Camera::Camera(glm::vec3 origin, glm::vec3 up, glm::vec3 target,
                      float aspect, float fov, float z_near, float z_far)
    : aspect(aspect), fov(fov), z_near(z_near), z_far(z_far), _origin(origin),
      _up(up), _target(target) {
  update_view_projection();
}

inline void Camera::update_view_projection() {
  _view_matrix = glm::lookAt(_origin, _target, _up);
  _projection_matrix = glm::perspective(fov, aspect, z_near, z_far);
}

inline const glm::mat4 &Camera::view_matrix() const { return _view_matrix; }
inline const glm::mat4 &Camera::projection_matrix() const {
  return _projection_matrix;
}

inline const glm::vec3 &Camera::origin() const { return _origin; };

inline glm::vec3 Camera::direction() const {
  return glm::normalize(_target - _origin);
}

inline void Camera::origin(const glm::vec3 &origin) {
  _origin = origin;
  update_view_projection();
}

GLE_NAMESPACE_END