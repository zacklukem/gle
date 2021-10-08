#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define GLE_TEST_CASES

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ostream>

namespace glm {
std::ostream &operator<<(std::ostream &os, const glm::uvec4 &value) {
  os << glm::to_string(value);
  return os;
}

std::ostream &operator<<(std::ostream &os, const glm::uvec3 &value) {
  os << glm::to_string(value);
  return os;
}

std::ostream &operator<<(std::ostream &os, const glm::uvec2 &value) {
  os << glm::to_string(value);
  return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec4 &value) {
  os << glm::to_string(value);
  return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &value) {
  os << glm::to_string(value);
  return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec2 &value) {
  os << glm::to_string(value);
  return os;
}
} // namespace glm

#include <doctest.h>

#include <gle/gle.hpp>
