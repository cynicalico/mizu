#include "gloo/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "mizu/log.hpp"

namespace gloo {
Shader::~Shader() {
    if (id != 0) {
        gl_.DeleteProgram(id);
        SPDLOG_TRACE("Deleted shader program id={}", id);
    }
}

MOVE_CONSTRUCTOR_IMPL(Shader)
    : id(other.id),
      gl_(other.gl_),
      uniform_locs_(std::move(other.uniform_locs_)),
      bad_uniform_locs_(std::move(other.bad_uniform_locs_)),
      attrib_locs_(std::move(other.attrib_locs_)),
      bad_attrib_locs_(std::move(other.bad_attrib_locs_)) {
    other.id = 0;
    other.uniform_locs_.clear();
    other.bad_uniform_locs_.clear();
    other.attrib_locs_.clear();
    other.bad_attrib_locs_.clear();
}

MOVE_ASSIGN_OP_IMPL(Shader) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        gl_ = other.gl_;

        std::swap(uniform_locs_, other.uniform_locs_);
        other.uniform_locs_.clear();

        std::swap(bad_uniform_locs_, other.bad_uniform_locs_);
        other.bad_uniform_locs_.clear();

        std::swap(attrib_locs_, other.attrib_locs_);
        other.attrib_locs_.clear();

        std::swap(bad_attrib_locs_, other.bad_attrib_locs_);
        other.bad_attrib_locs_.clear();
    }
    return *this;
}

void Shader::use() {
    gl_.UseProgram(id);
}

std::optional<GLuint> Shader::attrib_location(const std::string &name) {
    auto it = attrib_locs_.find(name);
    if (it == attrib_locs_.end()) {
        GLint loc = gl_.GetAttribLocation(id, name.c_str());
        if (loc == -1) {
            if (auto it2 = bad_attrib_locs_.find(name); it2 == bad_attrib_locs_.end()) {
                SPDLOG_ERROR("Attrib \"{}\" not found", name);
                bad_attrib_locs_.insert(it2, name);
            }
            return std::nullopt;
        }
        it = attrib_locs_.emplace_hint(it, name, loc);
    }
    return it->second;
}

void Shader::uniform(const std::string &name, float v0) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform1f(*loc, v0);
}

void Shader::uniform(const std::string &name, float v0, float v1) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform2f(*loc, v0, v1);
}

void Shader::uniform(const std::string &name, float v0, float v1, float v2) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform3f(*loc, v0, v1, v2);
}

void Shader::uniform(const std::string &name, float v0, float v1, float v2, float v3) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform4f(*loc, v0, v1, v2, v3);
}

void Shader::uniform(const std::string &name, int v0) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform1i(*loc, v0);
}

void Shader::uniform(const std::string &name, int v0, int v1) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform2i(*loc, v0, v1);
}

void Shader::uniform(const std::string &name, int v0, int v1, int v2) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform3i(*loc, v0, v1, v2);
}

void Shader::uniform(const std::string &name, int v0, int v1, int v2, int v3) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform4i(*loc, v0, v1, v2, v3);
}

void Shader::uniform(const std::string &name, unsigned int v0) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform1ui(*loc, v0);
}

void Shader::uniform(const std::string &name, unsigned int v0, unsigned int v1) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform2ui(*loc, v0, v1);
}

void Shader::uniform(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform3ui(*loc, v0, v1, v2);
}

void Shader::uniform(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform4ui(*loc, v0, v1, v2, v3);
}

void Shader::uniform(const std::string &name, const glm::vec1 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform1fv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::vec2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform2fv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::vec3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform3fv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::vec4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform4fv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::ivec1 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform1iv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::ivec2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform2iv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::ivec3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform3iv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::ivec4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform4iv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::uvec1 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform1uiv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::uvec2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform2uiv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::uvec3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform3uiv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::uvec4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.Uniform4uiv(*loc, 1, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix2fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix3fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix4fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat2x3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix2x3fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat3x2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix3x2fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat2x4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix2x4fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat4x2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix4x2fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat3x4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix3x4fv(*loc, 1, GL_FALSE, value_ptr(v));
}

void Shader::uniform(const std::string &name, const glm::mat4x3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc)
        gl_.UniformMatrix4x3fv(*loc, 1, GL_FALSE, value_ptr(v));
}

Shader::Shader(GladGLContext &gl, GLuint id)
    : id(id), gl_(gl) {}

std::optional<GLint> Shader::find_uniform_loc_(const std::string &name) {
    auto it = uniform_locs_.find(name);
    if (it == uniform_locs_.end()) {
        GLint loc = gl_.GetUniformLocation(id, name.c_str());
        if (loc == -1) {
            if (auto it2 = bad_uniform_locs_.find(name); it2 == bad_uniform_locs_.end()) {
                SPDLOG_ERROR("Uniform \"{}\" not found", name);
                bad_uniform_locs_.insert(it2, name);
            }
            return std::nullopt;
        }
        it = uniform_locs_.emplace_hint(it, name, loc);
    }
    return it->second;
}

ShaderBuilder::ShaderBuilder(GladGLContext &gl)
    : gl_(gl) {}

ShaderBuilder &ShaderBuilder::stage_src(ShaderType type, const std::string &src) {
    GLuint id = gl_.CreateShader(static_cast<GLenum>(type));
    SPDLOG_TRACE("Created {} shader id={}", shader_type_str(type), id);
    stages_.emplace_back(id, type, src);

    return *this;
}

std::unique_ptr<Shader> ShaderBuilder::link() {
    GLuint program_id = gl_.CreateProgram();
    SPDLOG_TRACE("Created shader program id={}", program_id);

    bool had_compile_error = false;
    for (auto &[id, type, src]: stages_) {
        if (try_compile_(id, type, src)) {
            gl_.AttachShader(program_id, id);
        } else {
            had_compile_error = true;
            break;
        }
    }

    if (had_compile_error || !try_link_(program_id)) {
        delete_stages_();
        gl_.DeleteProgram(program_id);
        SPDLOG_TRACE("Deleted shader program id={}", program_id);

        return nullptr;
    }

    delete_stages_();
    return std::unique_ptr<Shader>(new Shader(gl_, program_id));
}

bool ShaderBuilder::try_compile_(GLuint id, ShaderType type, const std::string &src) const {
    auto src_p = src.c_str();
    gl_.ShaderSource(id, 1, &src_p, nullptr);
    gl_.CompileShader(id);

    int success;
    gl_.GetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        int info_log_length;
        gl_.GetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);

        std::vector<GLchar> info_log(info_log_length);
        gl_.GetShaderInfoLog(id, info_log_length, nullptr, &info_log[0]);
        std::string info_log_str = std::string(&info_log[0]);

        SPDLOG_ERROR("Failed to compile {} shader id={}: {}", shader_type_str(type), id, info_log_str);
        return false;
    }

    SPDLOG_TRACE("Successfully compiled {} shader id={}", shader_type_str(type), id);
    return true;
}

bool ShaderBuilder::try_link_(GLuint id) const {
    gl_.LinkProgram(id);

    int success;
    gl_.GetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        int info_log_length;
        gl_.GetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);

        std::vector<GLchar> info_log(info_log_length);
        gl_.GetProgramInfoLog(id, info_log_length, nullptr, &info_log[0]);
        std::string info_log_str = std::string(&info_log[0]);

        SPDLOG_ERROR("Failed to link shader program id={}: {}", id, info_log_str);
        return false;
    }

    SPDLOG_TRACE("Successfully linked shader program id={}", id);
    return true;
}

void ShaderBuilder::delete_stages_() {
    for (auto &[id, type, src]: stages_) {
        gl_.DeleteShader(id);
        SPDLOG_TRACE("Deleted {} shader id={}", shader_type_str(type), id);
    }
}
} // namespace gloo
