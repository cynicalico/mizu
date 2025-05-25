#ifndef GLOO_SHADER_HPP
#define GLOO_SHADER_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <tuple>
#include "mizu/class_helpers.hpp"

#include <unordered_set>

namespace gloo {
class Shader {
    friend class ShaderBuilder;

public:
    GLuint id;

    ~Shader();

    NO_COPY(Shader)

    MOVE_CONSTRUCTOR(Shader);
    MOVE_ASSIGN_OP(Shader);

    void use();

    void uniform(const std::string &name, float v0);
    void uniform(const std::string &name, float v0, float v1);
    void uniform(const std::string &name, float v0, float v1, float v2);
    void uniform(const std::string &name, float v0, float v1, float v2, float v3);

    void uniform(const std::string &name, int v0);
    void uniform(const std::string &name, int v0, int v1);
    void uniform(const std::string &name, int v0, int v1, int v2);
    void uniform(const std::string &name, int v0, int v1, int v2, int v3);

    void uniform(const std::string &name, unsigned int v0);
    void uniform(const std::string &name, unsigned int v0, unsigned int v1);
    void uniform(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2);
    void uniform(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);

    void uniform(const std::string &name, const glm::vec1 &v);
    void uniform(const std::string &name, const glm::vec2 &v);
    void uniform(const std::string &name, const glm::vec3 &v);
    void uniform(const std::string &name, const glm::vec4 &v);

    void uniform(const std::string &name, const glm::ivec1 &v);
    void uniform(const std::string &name, const glm::ivec2 &v);
    void uniform(const std::string &name, const glm::ivec3 &v);
    void uniform(const std::string &name, const glm::ivec4 &v);

    void uniform(const std::string &name, const glm::uvec1 &v);
    void uniform(const std::string &name, const glm::uvec2 &v);
    void uniform(const std::string &name, const glm::uvec3 &v);
    void uniform(const std::string &name, const glm::uvec4 &v);

    void uniform(const std::string &name, const glm::mat2 &v);
    void uniform(const std::string &name, const glm::mat3 &v);
    void uniform(const std::string &name, const glm::mat4 &v);

    void uniform(const std::string &name, const glm::mat2x3 &v);
    void uniform(const std::string &name, const glm::mat3x2 &v);

    void uniform(const std::string &name, const glm::mat2x4 &v);
    void uniform(const std::string &name, const glm::mat4x2 &v);

    void uniform(const std::string &name, const glm::mat3x4 &v);
    void uniform(const std::string &name, const glm::mat4x3 &v);

private:
    GladGLContext &gl_;
    std::unordered_map<std::string, GLint> uniform_locs_;
    std::unordered_set<std::string> bad_locs_{};

    explicit Shader(GladGLContext &gl, GLuint id);

    std::optional<GLint> find_uniform_loc_(const std::string &name);
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
