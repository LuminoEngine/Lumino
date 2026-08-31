#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

// Emscripten では WebGL 2.0 が、デスクトップでは ANGLE の libGLESv2 が
// 同じ OpenGL ES 3.0 のエントリポイントを提供する。GL 関数ローダは不要。
#include <GLES3/gl3.h>

namespace ln::rhi::webgl2 {

/**
 * GL のテクスチャフォーマット。
 * internalFormat は glTexStorage2D に、format/type は glTexSubImage2D と glReadPixels に使う。
 */
struct GLFormatInfo {
    GLenum internalFormat;
    GLenum format;
    GLenum type;
    uint32_t bytesPerPixel;
    bool supported;
};

inline GLFormatInfo toGLFormat(TextureFormat fmt) {
    switch (fmt) {
        case TextureFormat::RGBA8Unorm:      return {GL_RGBA8,               GL_RGBA,            GL_UNSIGNED_BYTE,        4, true};
        case TextureFormat::RGBA8UnormSrgb:  return {GL_SRGB8_ALPHA8,        GL_RGBA,            GL_UNSIGNED_BYTE,        4, true};
        case TextureFormat::Depth24Stencil8: return {GL_DEPTH24_STENCIL8,    GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8,    4, true};
        case TextureFormat::Depth32Float:    return {GL_DEPTH_COMPONENT32F,  GL_DEPTH_COMPONENT, GL_FLOAT,                4, true};
        case TextureFormat::R8Unorm:         return {GL_R8,                  GL_RED,             GL_UNSIGNED_BYTE,        1, true};
        case TextureFormat::RG8Unorm:        return {GL_RG8,                 GL_RG,              GL_UNSIGNED_BYTE,        2, true};
        case TextureFormat::RGBA16Float:     return {GL_RGBA16F,             GL_RGBA,            GL_HALF_FLOAT,           8, true};
        case TextureFormat::RGBA32Float:     return {GL_RGBA32F,             GL_RGBA,            GL_FLOAT,               16, true};
        // BGRA8 は WebGL2 に内部フォーマットが無く、テクスチャスウィズルも使えないため
        // エミュレートできない。lumino.h で deprecated としており RGBA8 に寄せる方針。
        default:                             return {0, 0, 0, 0, false};
    }
}

inline GLenum toGLPrimitive(PrimitiveTopology v) {
    switch (v) {
        case PrimitiveTopology::TriangleList:  return GL_TRIANGLES;
        case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
        case PrimitiveTopology::LineList:      return GL_LINES;
        case PrimitiveTopology::LineStrip:     return GL_LINE_STRIP;
        case PrimitiveTopology::PointList:     return GL_POINTS;
    }
    return GL_TRIANGLES;
}

inline GLenum toGLBlendFactor(BlendFactor v) {
    switch (v) {
        case BlendFactor::Zero:             return GL_ZERO;
        case BlendFactor::One:              return GL_ONE;
        case BlendFactor::SrcColor:         return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::SrcAlpha:         return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstColor:         return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::DstAlpha:         return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
    }
    return GL_ZERO;
}

inline GLenum toGLBlendOp(BlendOp v) {
    switch (v) {
        case BlendOp::Add:             return GL_FUNC_ADD;
        case BlendOp::Subtract:        return GL_FUNC_SUBTRACT;
        case BlendOp::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendOp::Min:             return GL_MIN;
        case BlendOp::Max:             return GL_MAX;
    }
    return GL_FUNC_ADD;
}

inline GLenum toGLCompareFunc(CompareFunction v) {
    switch (v) {
        case CompareFunction::Never:        return GL_NEVER;
        case CompareFunction::Less:         return GL_LESS;
        case CompareFunction::LessEqual:    return GL_LEQUAL;
        case CompareFunction::Greater:      return GL_GREATER;
        case CompareFunction::GreaterEqual: return GL_GEQUAL;
        case CompareFunction::Equal:        return GL_EQUAL;
        case CompareFunction::NotEqual:     return GL_NOTEQUAL;
        case CompareFunction::Always:       return GL_ALWAYS;
    }
    return GL_ALWAYS;
}

inline GLenum toGLStencilOp(StencilOp v) {
    switch (v) {
        case StencilOp::Keep:           return GL_KEEP;
        case StencilOp::Zero:           return GL_ZERO;
        case StencilOp::Replace:        return GL_REPLACE;
        case StencilOp::IncrementClamp: return GL_INCR;
        case StencilOp::DecrementClamp: return GL_DECR;
        case StencilOp::Invert:         return GL_INVERT;
    }
    return GL_KEEP;
}

inline GLenum toGLFilter(FilterMode v) {
    return (v == FilterMode::Nearest) ? GL_NEAREST : GL_LINEAR;
}

inline GLenum toGLWrap(AddressMode v) {
    switch (v) {
        case AddressMode::Repeat:         return GL_REPEAT;
        case AddressMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case AddressMode::ClampToEdge:    return GL_CLAMP_TO_EDGE;
    }
    return GL_CLAMP_TO_EDGE;
}

inline GLenum toGLIndexType(IndexFormat v) {
    return (v == IndexFormat::Uint16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
}

inline uint32_t indexStride(IndexFormat v) {
    return (v == IndexFormat::Uint16) ? 2 : 4;
}

/** 頂点属性 1 つ分の glVertexAttribPointer 引数。 */
struct GLVertexFormatInfo {
    GLint size;
    GLenum type;
    GLboolean normalized;
};

inline GLVertexFormatInfo toGLVertexFormat(VertexFormat v) {
    switch (v) {
        case VertexFormat::Float32x2:   return {2, GL_FLOAT,         GL_FALSE};
        case VertexFormat::Float32x3:   return {3, GL_FLOAT,         GL_FALSE};
        case VertexFormat::Float32x4:   return {4, GL_FLOAT,         GL_FALSE};
        case VertexFormat::Uint8x4Norm: return {4, GL_UNSIGNED_BYTE, GL_TRUE};
        case VertexFormat::Sint16x2:    return {2, GL_SHORT,         GL_FALSE};
        case VertexFormat::Sint16x4:    return {4, GL_SHORT,         GL_FALSE};
    }
    return {3, GL_FLOAT, GL_FALSE};
}

} // namespace ln::rhi::webgl2
