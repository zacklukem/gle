#ifndef GLE_VAO_HPP
#define GLE_VAO_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <gle/vbo.hpp>

GLE_NAMESPACE_BEGIN

/// @brief A VAO object
///
class VAO {
public:
  VAO(VAO &) = delete;
  VAO(VAO &&) = delete;
  VAO(const VAO &) = delete;
  VAO(const VAO &&) = delete;

  inline VAO();

  inline ~VAO();

  /// @brief initialize this vao
  ///
  inline void init();

  /// @brief bind this vao
  ///
  inline void bind() const;

  /// @brief Attribute vbo to this VAO
  ///
  /// @tparam T
  /// @param index
  /// @param vbo
  template <class T> inline void attr(GLuint index, const VBO<T> &vbo) const;

private:
  GLuint handle;
};

GLE_NAMESPACE_END

#endif // GLE_VAO_HPP
