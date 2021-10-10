#ifndef GLE_VBO_HPP
#define GLE_VBO_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <glm/glm.hpp>
#include <type_traits>
#include <vector>

GLE_NAMESPACE_BEGIN

// C++ voodoo
namespace __internal__ {

// VOODOO MAGIC thanks to C++ reference
template <class T> struct dependent_false : std::false_type {};

// See if T is a vector with inner type U
template <class T, class U>
struct is_glm_vec_of
    : std::integral_constant<bool, std::is_same<T, glm::vec<1, U>>::value ||
                                       std::is_same<T, glm::vec<2, U>>::value ||
                                       std::is_same<T, glm::vec<3, U>>::value ||
                                       std::is_same<T, glm::vec<4, U>>::value> {
};

// Crude unit tests
static_assert(is_glm_vec_of<glm::vec1, float>::value);
static_assert(is_glm_vec_of<glm::vec2, float>::value);
static_assert(is_glm_vec_of<glm::vec3, float>::value);
static_assert(is_glm_vec_of<glm::vec4, float>::value);
static_assert(is_glm_vec_of<glm::ivec1, int>::value);
static_assert(is_glm_vec_of<glm::ivec2, int>::value);
static_assert(is_glm_vec_of<glm::ivec3, int>::value);
static_assert(is_glm_vec_of<glm::ivec4, int>::value);
static_assert(!is_glm_vec_of<glm::ivec2, float>::value);

template <class T> constexpr GLint vbo_get_value_size() {
  if constexpr (std::is_fundamental<T>::value)
    return 1;
  else
    return sizeof(T) / sizeof(typename T::value_type);
}

// Use the vbo_value_type struct instead
template <class T> constexpr GLuint vbo_get_value_type() {
  // Thanks to C++17, we dont have to chain ternary operators
  if constexpr (std::is_same<T, float>::value)
    return GL_FLOAT;
  else if constexpr (is_glm_vec_of<T, float>::value)
    return GL_FLOAT;
  else if constexpr (is_glm_vec_of<T, int>::value)
    return GL_INT;
  else if constexpr (std::is_same<T, int>::value)
    return GL_INT;
  else if constexpr (is_glm_vec_of<T, unsigned int>::value)
    return GL_UNSIGNED_INT;
  else if constexpr (std::is_same<T, unsigned int>::value)
    return GL_UNSIGNED_INT;
  else
    static_assert(dependent_false<T>::value, "Unable to detect VBO value type");
}

// Use this to make it clear that the value is computed at compile-time
template <class T>
struct vbo_value_type
    : std::integral_constant<GLuint, vbo_get_value_type<T>()> {};

template <class T>
struct vbo_value_size : std::integral_constant<GLint, vbo_get_value_size<T>()> {
};

static_assert(vbo_value_size<float>::value == 1);
static_assert(vbo_value_size<int>::value == 1);
static_assert(vbo_value_size<glm::vec3>::value == 3);
static_assert(vbo_value_size<glm::uvec2>::value == 2);

} // namespace __internal__

/// @brief A VBO
///
/// @tparam T the type of data stored in this vbo. This can be a float, int, or
///           glm float vector or glm int vector.
template <class T> class VBO {
public:
  /// @brief the type of OpenGL value stored in this vbo (e.g. GL_FLOAT)
  ///
  static constexpr GLuint gl_value_type =
      __internal__::vbo_value_type<T>::value;

  /// @brief the number of components per element
  ///
  /// For example, a VBO<glm::vec3> has a value size of 3
  static constexpr GLint gl_value_size = __internal__::vbo_value_size<T>::value;

  VBO(VBO &) = delete;
  VBO(VBO &&) = delete;
  VBO(const VBO &) = delete;
  VBO(const VBO &&) = delete;

  /// @brief Construct a new VBO
  ///
  /// @param type the type of vbo
  /// @param dynamic if the vbo is dynamic
  inline VBO(GLuint type, bool dynamic);

  inline ~VBO();

  /// @brief Gen the buffers
  ///
  /// Must be called after GL is initialized
  inline void init();

  /// @brief Bind the buffers
  ///
  inline void bind() const;

  /// @brief write data to the buffers
  ///
  /// @param data
  inline void write(const std::vector<T> &data);

private:
  GLuint type;
  GLuint handle;
  bool dynamic;
};

static_assert(VBO<float>::gl_value_size == 1);
static_assert(VBO<int>::gl_value_size == 1);
static_assert(VBO<glm::vec3>::gl_value_size == 3);
static_assert(VBO<glm::ivec3>::gl_value_size == 3);

static_assert(VBO<float>::gl_value_type == GL_FLOAT);
static_assert(VBO<int>::gl_value_type == GL_INT);
static_assert(VBO<glm::vec3>::gl_value_type == GL_FLOAT);
static_assert(VBO<glm::ivec3>::gl_value_type == GL_INT);

GLE_NAMESPACE_END

#endif // GLE_VBO_HPP
