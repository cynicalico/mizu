#include "gloo/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "mizu/core/log.hpp"

namespace gloo {
Shader::~Shader() {
    if (id != 0) {
        gl_.DeleteProgram(id);
        CHECK_GL_ERROR(gl_, DeleteProgram);
        MIZU_LOG_TRACE("Deleted shader program id={}", id);
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
    CHECK_GL_ERROR(gl_, UseProgram);
}

std::optional<GLuint> Shader::attrib_location(const std::string &name) {
    auto it = attrib_locs_.find(name);
    if (it == attrib_locs_.end()) {
        GLint loc = gl_.GetAttribLocation(id, name.c_str());
        CHECK_GL_ERROR(gl_, GetAttribLocation);
        if (loc == -1) {
            if (auto it2 = bad_attrib_locs_.find(name); it2 == bad_attrib_locs_.end()) {
                MIZU_LOG_ERROR("Attrib \"{}\" not found", name);
                bad_attrib_locs_.insert(it2, name);
            }
            return std::nullopt;
        }
        it = attrib_locs_.emplace_hint(it, name, loc);
    }
    return it->second;
}

void Shader::uniform(const std::string &name, float v0) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform1f(*loc, v0);
        CHECK_GL_ERROR(gl_, Uniform1f);
    }
}

void Shader::uniform(const std::string &name, float v0, float v1) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform2f(*loc, v0, v1);
        CHECK_GL_ERROR(gl_, Uniform2f);
    }
}

void Shader::uniform(const std::string &name, float v0, float v1, float v2) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform3f(*loc, v0, v1, v2);
        CHECK_GL_ERROR(gl_, Uniform3f);
    }
}

void Shader::uniform(const std::string &name, float v0, float v1, float v2, float v3) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform4f(*loc, v0, v1, v2, v3);
        CHECK_GL_ERROR(gl_, Uniform4f);
    }
}

void Shader::uniform(const std::string &name, int v0) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform1i(*loc, v0);
        CHECK_GL_ERROR(gl_, Uniform1i);
    }
}

void Shader::uniform(const std::string &name, int v0, int v1) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform2i(*loc, v0, v1);
        CHECK_GL_ERROR(gl_, Uniform2i);
    }
}

void Shader::uniform(const std::string &name, int v0, int v1, int v2) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform3i(*loc, v0, v1, v2);
        CHECK_GL_ERROR(gl_, Uniform3i);
    }
}

void Shader::uniform(const std::string &name, int v0, int v1, int v2, int v3) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform4i(*loc, v0, v1, v2, v3);
        CHECK_GL_ERROR(gl_, Uniform4i);
    }
}

void Shader::uniform(const std::string &name, unsigned int v0) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform1ui(*loc, v0);
        CHECK_GL_ERROR(gl_, Uniform1ui);
    }
}

void Shader::uniform(const std::string &name, unsigned int v0, unsigned int v1) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform2ui(*loc, v0, v1);
        CHECK_GL_ERROR(gl_, Uniform2ui);
    }
}

void Shader::uniform(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform3ui(*loc, v0, v1, v2);
        CHECK_GL_ERROR(gl_, Uniform3ui);
    }
}

void Shader::uniform(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform4ui(*loc, v0, v1, v2, v3);
        CHECK_GL_ERROR(gl_, Uniform4ui);
    }
}

void Shader::uniform(const std::string &name, const glm::vec1 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform1fv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform1fv);
    }
}

void Shader::uniform(const std::string &name, const glm::vec2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform2fv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform2fv);
    }
}

void Shader::uniform(const std::string &name, const glm::vec3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform3fv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform3fv);
    }
}

void Shader::uniform(const std::string &name, const glm::vec4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform4fv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform4fv);
    }
}

void Shader::uniform(const std::string &name, const glm::ivec1 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform1iv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform1iv);
    }
}

void Shader::uniform(const std::string &name, const glm::ivec2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform2iv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform2iv);
    }
}

void Shader::uniform(const std::string &name, const glm::ivec3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform3iv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform3iv);
    }
}

void Shader::uniform(const std::string &name, const glm::ivec4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform4iv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform4iv);
    }
}

void Shader::uniform(const std::string &name, const glm::uvec1 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform1uiv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform1uiv);
    }
}

void Shader::uniform(const std::string &name, const glm::uvec2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform2uiv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform2uiv);
    }
}

void Shader::uniform(const std::string &name, const glm::uvec3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform3uiv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform3uiv);
    }
}

void Shader::uniform(const std::string &name, const glm::uvec4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.Uniform4uiv(*loc, 1, value_ptr(v));
        CHECK_GL_ERROR(gl_, Uniform4uiv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix2fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix2fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix3fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix3fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix4fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix4fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat2x3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix2x3fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix2x3fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat3x2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix3x2fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix3x2fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat2x4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix2x4fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix2x4fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat4x2 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix4x2fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix4x2fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat3x4 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix3x4fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix3x4fv);
    }
}

void Shader::uniform(const std::string &name, const glm::mat4x3 &v) {
    if (const auto loc = find_uniform_loc_(name); loc) {
        gl_.UniformMatrix4x3fv(*loc, 1, GL_FALSE, value_ptr(v));
        CHECK_GL_ERROR(gl_, UniformMatrix4x3fv);
    }
}

Shader::Shader(GladGLContext &gl, GLuint id)
    : id(id), gl_(gl) {}

std::optional<GLint> Shader::find_uniform_loc_(const std::string &name) {
    auto it = uniform_locs_.find(name);
    if (it == uniform_locs_.end()) {
        GLint loc = gl_.GetUniformLocation(id, name.c_str());
        CHECK_GL_ERROR(gl_, GetUniformLocation);
        if (loc == -1) {
            if (auto it2 = bad_uniform_locs_.find(name); it2 == bad_uniform_locs_.end()) {
                MIZU_LOG_ERROR("Uniform \"{}\" not found", name);
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
    CHECK_GL_ERROR(gl_, CreateShader);
    MIZU_LOG_TRACE("Created {} shader id={}", shader_type_str(type), id);
    stages_.emplace_back(id, type, src);

    return *this;
}

std::unique_ptr<Shader> ShaderBuilder::link() {
    GLuint program_id = gl_.CreateProgram();
    CHECK_GL_ERROR(gl_, CreateProgram);
    MIZU_LOG_TRACE("Created shader program id={}", program_id);

    bool had_compile_error = false;
    for (auto &[id, type, src]: stages_) {
        if (try_compile_(id, type, src)) {
            gl_.AttachShader(program_id, id);
            CHECK_GL_ERROR(gl_, AttachShader);
        } else {
            had_compile_error = true;
            break;
        }
    }

    if (had_compile_error || !try_link_(program_id)) {
        delete_stages_();
        gl_.DeleteProgram(program_id);
        CHECK_GL_ERROR(gl_, DeleteProgram);
        MIZU_LOG_TRACE("Deleted shader program id={}", program_id);

        return nullptr;
    }

    delete_stages_();
    return std::unique_ptr<Shader>(new Shader(gl_, program_id));
}

bool ShaderBuilder::try_compile_(GLuint id, ShaderType type, const std::string &src) const {
    auto src_p = src.c_str();
    gl_.ShaderSource(id, 1, &src_p, nullptr);
    CHECK_GL_ERROR(gl_, ShaderSource);
    gl_.CompileShader(id);
    CHECK_GL_ERROR(gl_, CompileShader);

    int success;
    gl_.GetShaderiv(id, GL_COMPILE_STATUS, &success);
    CHECK_GL_ERROR(gl_, GetShaderiv);

    if (!success) {
        int info_log_length;
        gl_.GetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
        CHECK_GL_ERROR(gl_, GetShaderiv);

        std::vector<GLchar> info_log(info_log_length);
        gl_.GetShaderInfoLog(id, info_log_length, nullptr, &info_log[0]);
        CHECK_GL_ERROR(gl_, GetShaderInfoLog);
        std::string info_log_str = std::string(&info_log[0]);

        MIZU_LOG_ERROR("Failed to compile {} shader id={}: {}", shader_type_str(type), id, info_log_str);
        return false;
    }

    MIZU_LOG_TRACE("Successfully compiled {} shader id={}", shader_type_str(type), id);
    return true;
}

bool ShaderBuilder::try_link_(GLuint id) const {
    gl_.LinkProgram(id);
    CHECK_GL_ERROR(gl_, LinkProgram);

    int success;
    gl_.GetProgramiv(id, GL_LINK_STATUS, &success);
    CHECK_GL_ERROR(gl_, GetProgramiv);

    if (!success) {
        int info_log_length;
        gl_.GetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
        CHECK_GL_ERROR(gl_, GetProgramiv);

        std::vector<GLchar> info_log(info_log_length);
        gl_.GetProgramInfoLog(id, info_log_length, nullptr, &info_log[0]);
        CHECK_GL_ERROR(gl_, GetProgramInfoLog);
        std::string info_log_str = std::string(&info_log[0]);

        MIZU_LOG_ERROR("Failed to link shader program id={}: {}", id, info_log_str);
        return false;
    }

    MIZU_LOG_TRACE("Successfully linked shader program id={}", id);
    return true;
}

void ShaderBuilder::delete_stages_() {
    for (auto &[id, type, src]: stages_) {
        gl_.DeleteShader(id);
        CHECK_GL_ERROR(gl_, DeleteShader);
        MIZU_LOG_TRACE("Deleted {} shader id={}", shader_type_str(type), id);
    }
}
} // namespace gloo
