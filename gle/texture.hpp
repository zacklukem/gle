#ifndef GLE_TEXTURE_HPP
#define GLE_TEXTURE_HPP

#include <gle/common.hpp>
#include <istream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLE_NAMESPACE_BEGIN

struct ImageData {
  ImageData(ImageData &) = delete;
  ImageData(ImageData &&) = delete;
  inline ImageData(uint8_t *data, size_t width, size_t height,
                   size_t num_channels);
  inline ~ImageData();

  uint8_t *data;
  size_t width;
  size_t height;
  size_t num_channels;
};

struct TextureOptions {
  GLint wrap_s = 0;
  GLint wrap_t = 0;
  GLint min_filter = 0;
  GLint mag_filter = 0;
};

class ImageReader {
public:
  inline ImageReader(std::istream &stream);
  inline ~ImageReader();

  inline ImageData read();

private:
  inline stbi_io_callbacks get_callbacks();
  inline static int stbi_read(ImageReader *self, char *data, int size);
  inline static void stbi_skip(ImageReader *self, int n);
  inline static int stbi_eof(ImageReader *self);
  std::istream &stream;
};

class Texture {
public:
  inline Texture(const TextureOptions &options);
  inline void init();
  inline void bind() const;
  inline ~Texture();

private:
  TextureOptions options;
  GLuint handle;
};

class ImageTexture : public Texture {
public:
  static constexpr TextureOptions default_options =
      TextureOptions{.wrap_s = GL_REPEAT,
                     .wrap_t = GL_REPEAT,
                     .min_filter = GL_LINEAR_MIPMAP_LINEAR,
                     .mag_filter = GL_LINEAR};

  inline ImageTexture(const TextureOptions &options = default_options);
  inline void load(const std::string &file);
  inline void load(std::istream &stream);
};

GLE_NAMESPACE_END

#include "texture.inl"

#endif // GLE_TEXTURE_HPP
