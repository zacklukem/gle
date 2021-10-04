GLE_NAMESPACE_BEGIN

inline VAO::VAO() : handle(0) {}

inline VAO::~VAO() { glDeleteVertexArrays(1, &handle); }
inline void VAO::init() { glGenVertexArrays(1, &handle); }
inline void VAO::bind() { glBindVertexArray(handle); }

template <class T> inline void VAO::attr(GLuint index, const VBO<T> &vbo) {

  bind();
  vbo.bind();

  switch (VBO<T>::gl_value_type) {
  case GL_BYTE:
  case GL_UNSIGNED_BYTE:
  case GL_SHORT:
  case GL_UNSIGNED_SHORT:
  case GL_INT:
  case GL_UNSIGNED_INT:
    glVertexAttribIPointer(index, VBO<T>::gl_value_size, VBO<T>::gl_value_type,
                           sizeof(T), (void *)0);
    break;
  default:
    glVertexAttribPointer(index, VBO<T>::gl_value_size, VBO<T>::gl_value_type,
                          GL_FALSE, sizeof(T), (void *)0);
    break;
  }
}

GLE_NAMESPACE_END