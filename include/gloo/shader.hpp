#ifndef GLOO_SHADER_HPP
#define GLOO_SHADER_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <tuple>
#include "mizu/class_helpers.hpp"

namespace gloo {
class Shader {
    friend class ShaderBuilder;

public:
    GLuint id;

    ~Shader();

    NO_COPY(Shader)

    MOVE_CONSTRUCTOR(Shader);
    MOVE_ASSIGN_OP(Shader);

private:
    GladGLContext &gl_;

    explicit Shader(GladGLContext &gl, GLuint id);
};

enum class ShaderType : GLenum {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

inline std::string shader_type_str(ShaderType type) {
    switch (type) {
    case ShaderType::Vertex: return "vertex";
    case ShaderType::Fragment: return "fragment";
    default: std::unreachable();
    }
}

class ShaderBuilder {
public:
    explicit ShaderBuilder(GladGLContext &gl);

    NO_COPY(ShaderBuilder)
    NO_MOVE(ShaderBuilder)

    ShaderBuilder &stage_src(ShaderType type, const std::string &src);

    std::optional<Shader> link();

private:
    GladGLContext &gl_;
    std::vector<std::tuple<GLuint, ShaderType, std::string>> stages_{};

    bool try_compile_(GLuint id, ShaderType type, const std::string &src) const;
    bool try_link_(GLuint id) const;

    void delete_stages_();
};
} // namespace gloo

#endif // GLOO_SHADER_HPP
