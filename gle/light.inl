
GLE_NAMESPACE_BEGIN

inline Light::Light(LightType type, const glm::vec3 &position,
                    const glm::vec3 &direction, const glm::vec3 &color,
                    float strength)
    : type(type), position(position), direction(direction),
      attn(color * strength) {}

GLE_NAMESPACE_END
