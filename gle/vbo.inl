GLE_NAMESPACE_BEGIN

template <class T>
constexpr VBO<T>::VBO(GLuint type, bool dynamic)
    : handle(0), type(type), dynamic(dynamic) {}

template <class T> inline VBO<T>::~VBO() { glDeleteBuffers(1, &handle); }

template <class T> inline void VBO<T>::init() { glGenBuffers(1, &handle); }

template <class T> inline void VBO<T>::bind() const {
  glBindBuffer(type, handle);
}

template <class T> inline void VBO<T>::write(const std::vector<T> &data) {
  bind();
  glBufferData(type, sizeof(T) * data.size(), data.data(),
               dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

GLE_NAMESPACE_END