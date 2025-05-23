#include "gloo/shader.hpp"
#include "mizu/log.hpp"

namespace gloo {
Shader::Shader(GladGLContext &gl, GLuint id) : id(id), gl_(gl) {}

Shader::~Shader() {
    if (id != 0) {
        gl_.DeleteProgram(id);
        SPDLOG_TRACE("Deleted shader program id={}", id);
    }
}

MOVE_CONSTRUCTOR_IMPL(Shader) : id(other.id), gl_(other.gl_) {
    other.id = 0;
}

MOVE_ASSIGN_OP_IMPL(Shader) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        gl_ = other.gl_;
    }
    return *this;
}

ShaderBuilder::ShaderBuilder(GladGLContext &gl) : gl_(gl) {}

ShaderBuilder &ShaderBuilder::stage_src(ShaderType type, const std::string &src) {
    GLuint id = gl_.CreateShader(static_cast<GLenum>(type));
    SPDLOG_TRACE("Created {} shader id={}", shader_type_str(type), id);
    stages_.emplace_back(id, type, src);

    return *this;
}

std::optional<Shader> ShaderBuilder::link() {
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

        return std::nullopt;
    }

    delete_stages_();
    return Shader(gl_, program_id);
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
