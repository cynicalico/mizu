#ifndef GLOO_CONTEXT_HPP
#define GLOO_CONTEXT_HPP

#include <glad/gl.h>
#include <optional>

namespace gloo {
struct GlContextVersion {
    int major;
    int minor;

    GlContextVersion(int major, int minor) : major(major), minor(minor) {}
};

enum class GlCapability : GLenum {
    Blend = GL_BLEND,
    // GL_CLIP_DISTANCE 0-?
    ColorLogicOp = GL_COLOR_LOGIC_OP,
    CullFace = GL_CULL_FACE,
    DebugOutput = GL_DEBUG_OUTPUT,
    DebugOutputSync = GL_DEBUG_OUTPUT_SYNCHRONOUS,
    DepthClamp = GL_DEPTH_CLAMP,
    DepthTest = GL_DEPTH_TEST,
    Dither = GL_DITHER,
    FramebufferSrgb = GL_FRAMEBUFFER_SRGB,
    LineSmooth = GL_LINE_SMOOTH,
    Multisample = GL_MULTISAMPLE,
    PolygonOffsetFill = GL_POLYGON_OFFSET_FILL,
    PolygonOffsetLine = GL_POLYGON_OFFSET_LINE,
    PolygonOffsetPoint = GL_POLYGON_OFFSET_POINT,
    PolygonSmooth = GL_POLYGON_SMOOTH,
    PrimitiveRestart = GL_PRIMITIVE_RESTART,
    PrimitiveRestartFixedIndex = GL_PRIMITIVE_RESTART_FIXED_INDEX,
    RasterizerDiscard = GL_RASTERIZER_DISCARD,
    SampleAlphaToCoverage = GL_SAMPLE_ALPHA_TO_COVERAGE,
    SampleAlphaToOne = GL_SAMPLE_ALPHA_TO_ONE,
    SampleCoverage = GL_SAMPLE_COVERAGE,
    // SampleShading = GL_SAMPLE_SHADING,
    SampleMask = GL_SAMPLE_MASK,
    ScissorTest = GL_SCISSOR_TEST,
    StencilTest = GL_STENCIL_TEST,
    TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS,
    ProgramPointSize = GL_PROGRAM_POINT_SIZE,
};

enum class GlBlendFunc : GLenum {
    Zero = GL_ZERO,
    One = GL_ONE,
    SrcColor = GL_SRC_COLOR,
    OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
    DstColor = GL_DST_COLOR,
    OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
    SrcAlpha = GL_SRC_ALPHA,
    OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
    ConstantColor = GL_CONSTANT_COLOR,
    OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
    ConstantAlpha = GL_CONSTANT_ALPHA,
    OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
    SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE,
    Src1Color = GL_SRC1_COLOR,
    OneMinusSrc1Color = GL_ONE_MINUS_SRC1_COLOR,
    Src1Alpha = GL_SRC_ALPHA_SATURATE,
    OneMinusSrc1Alpha = GL_ONE_MINUS_SRC_ALPHA
};

class GlContext {
public:
    GladGLContext ctx;

    std::optional<GlContextVersion> load(GLADloadfunc func);

    void enable(GlCapability cap);
    void disable(GlCapability cap);
    bool is_enabled(GlCapability cap) const;

    void blend_func(GlBlendFunc sfactor, GlBlendFunc dfactor);

    void debug_message_callback(GLDEBUGPROC callback, const void *user_param);
};
} // namespace gloo

#endif // GLOO_CONTEXT_HPP
