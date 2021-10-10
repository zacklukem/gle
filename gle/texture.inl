GLE_NAMESPACE_BEGIN

// lovely C
// you make me
// feel so free
// oh shit now its
// pointer hell
// pointer well
// pointer sell
// take the l
typedef int (*stb_io_clbk_read)(void *user, char *data, int size);
typedef void (*stb_io_clbk_skip)(void *user, int n);
typedef int (*stb_io_clbk_eof)(void *user);

inline ImageReader::ImageReader(std::istream &stream) : stream(stream) {}

inline ImageReader::~ImageReader() {}

inline int ImageReader::stbi_read(ImageReader *self, char *data, int size) {
  self->stream.read(data, size);
  return self->stream.gcount();
}

inline void ImageReader::stbi_skip(ImageReader *self, int n) {
  if (n > 0) {
    self->stream.ignore(n);
  } else if (n < 0) {
    for (int i = 0; i < -n; ++i) {
      self->stream.unget();
    }
  }
}

inline int ImageReader::stbi_eof(ImageReader *self) {
  return !((bool)self->stream);
}

inline stbi_io_callbacks ImageReader::get_callbacks() {
  return stbi_io_callbacks{.read = (stb_io_clbk_read)ImageReader::stbi_read,
                           .skip = (stb_io_clbk_skip)ImageReader::stbi_skip,
                           .eof = (stb_io_clbk_eof)ImageReader::stbi_eof};
}

inline ImageData ImageReader::read() {
  stbi_set_flip_vertically_on_load(true);

  int width, height, channels;
  auto callbacks = get_callbacks();
  auto data =
      stbi_load_from_callbacks(&callbacks, this, &width, &height, &channels, 0);
  if (!data) {
    throw std::runtime_error("Failed to load image");
  }
  return ImageData(data, width, height, channels);
}

inline ImageData::ImageData(uint8_t *data, size_t width, size_t height,
                            size_t num_channels)
    : data(data), width(width), height(height), num_channels(num_channels) {}

inline ImageData::~ImageData() { stbi_image_free(data); }

inline ImageTexture::ImageTexture(const TextureOptions &options)
    : Texture(options) {}

inline void ImageTexture::load(const std::string &file) {
  auto stream = std::ifstream(file, std::ios_base::in);
  load(stream);
}

inline void ImageTexture::load(std::istream &stream) {
  bind();
  auto image = ImageReader(stream).read();

  auto format = GL_RGB;
  if (image.num_channels == 1) format = GL_RED;

  glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format,
               GL_UNSIGNED_BYTE, image.data);

  glGenerateMipmap(GL_TEXTURE_2D);
}

inline void Texture::init() {
  glGenTextures(1, &handle);
  bind();
  if (options.wrap_s)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  if (options.wrap_t)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  if (options.min_filter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
  if (options.mag_filter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

inline void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, handle); }

inline Texture::Texture(const TextureOptions &options)
    : options(options), handle(0) {}
inline Texture::~Texture() { glDeleteTextures(1, &handle); }

GLE_NAMESPACE_END
