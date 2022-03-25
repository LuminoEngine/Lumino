#pragma once
#include "Common.hpp"
#include "../GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class OpenGLDevice;
class GLGraphicsContext;
class GLSwapChain;
class GLRenderPass;
class GLCommandQueue;
class GLVertexBuffer;
class GLIndexBuffer;
class GLUniformBuffer;
class GLTextureBase;
class GLRenderTargetTexture;
class GLDepthBuffer;
class GLShaderPass;
class GLShaderDescriptorTable;

struct GLUniformBufferView {
    GLUniformBuffer* buffer;
    uint32_t offset;
};

class OpenGLHelper {
public:
    static const char* glEnumName(GLenum _enum) {
#define GLENUM(_ty) \
    case _ty:       \
        return #_ty
        switch (_enum) {
            GLENUM(GL_TEXTURE);
            GLENUM(GL_INVALID_ENUM);
            GLENUM(GL_INVALID_VALUE);
            GLENUM(GL_INVALID_OPERATION);
            GLENUM(GL_OUT_OF_MEMORY);
            GLENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
        }
#undef GLENUM
        return "<GLenum?>";
    }

    static void convertVariableTypeGLToLN(
        const char* name,
        GLenum gl_type,
        GLsizei gl_var_size,
        ShaderUniformType* outType,
        int* outRows,
        int* outColumns,
        int* outElements) {
        *outElements = 0;

#define SET_LNDESC(c_, t_, row_, col_) \
    {                                  \
        *outType = (t_);               \
        *outRows = (row_);             \
        *outColumns = (col_);          \
    }
        switch (gl_type) {
            case GL_FLOAT:
                SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Float, 1, 1);
                break;
            case GL_FLOAT_VEC2:
                SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_Vector, 1, 2);
                break;
            case GL_FLOAT_VEC3:
                SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_Vector, 1, 3);
                break;
            case GL_FLOAT_VEC4:
                SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_Vector, 1, 4);
                break;

            case GL_INT:
                SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Int, 1, 1);
                break;
                //case GL_INT_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_IntVector, 1, 2); break;
                //case GL_INT_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_IntVector, 1, 3); break;
                //case GL_INT_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_IntVector, 1, 4); break;

            case GL_UNSIGNED_INT:
                SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Int, 1, 1);
                break;

            case GL_BOOL:
                SET_LNDESC(LN_SVC_SCALAR, ShaderUniformType_Bool, 1, 1);
                break;
                //case GL_BOOL_VEC2:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_BoolVector, 1, 2); break;
                //case GL_BOOL_VEC3:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_BoolVector, 1, 3); break;
                //case GL_BOOL_VEC4:   SET_LNDESC(LN_SVC_VECTOR, ShaderUniformType_BoolVector, 1, 4); break;

            case GL_FLOAT_MAT2:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 2, 2);
                break;
            case GL_FLOAT_MAT3:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 3, 3);
                break;
            case GL_FLOAT_MAT4:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 4, 4);
                break;

                // es で使えない
                //case GL_SAMPLER_1D:         SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1); break;

            case GL_SAMPLER_2D:
                SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1);
                break;
            case GL_SAMPLER_CUBE:
                SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1);
                break;
            case GL_SAMPLER_3D:
                SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Texture, 1, 1);
                break;

                //#if !defined(LNOTE_GLES)
                // https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)#Matrices
                // GL_FLOAT_MAT3x4 -> mat3x4 -> matNxM (N columns and M rows)
            case GL_FLOAT_MAT2x3:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 3, 2);
                break;
            case GL_FLOAT_MAT2x4:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 4, 2);
                break;
            case GL_FLOAT_MAT3x2:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 2, 3);
                break;
            case GL_FLOAT_MAT3x4:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 4, 3);
                break;
            case GL_FLOAT_MAT4x2:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 2, 4);
                break;
            case GL_FLOAT_MAT4x3:
                SET_LNDESC(LN_SVC_MATRIX, ShaderUniformType_Matrix, 3, 4);
                break;

            //case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
            //case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
            //case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
            //case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

            // es で使えない
            //case GL_SAMPLER_1D_SHADOW:  SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Unknown, 1, 1); break;
            case GL_SAMPLER_2D_SHADOW:
                SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Unknown, 1, 1);
                break;
                //#endif
            default:
                SET_LNDESC(LN_SVC_SAMPLER, ShaderUniformType_Unknown, 0, 0);
                break;
        }

        // check array type (e.g. "list[0]")
        if (StringHelper::indexOf(name, -1, "[", 1) >= 0) {
            *outElements = gl_var_size;

            //switch (*outType)
            //{
            //case ShaderVariableType::Bool:
            //	desc->Type = ShaderVariableType::BoolArray;
            //	break;
            //case ShaderVariableType::Float:
            //	desc->Type = ShaderVariableType::FloatArray;
            //	break;
            //case ShaderVariableType::Vector:
            //	desc->Type = ShaderVariableType::VectorArray;
            //	break;
            //case ShaderVariableType::Matrix:
            //	desc->Type = ShaderVariableType::MatrixArray;
            //	break;
            //default:
            //	LN_UNREACHABLE();
            //	break;
            //}
        }
    }

    static void getGLTextureFormat(TextureFormat format, GLenum* internalFormat, GLenum* pixelFormat, GLenum* elementType) {
        struct TextureFormatConversionItem {
            TextureFormat format;
            GLenum internalFormat;
            GLenum pixelFormat;
            GLenum elementType;
        };
#if defined(LN_GRAPHICS_OPENGLES)
        static const TextureFormatConversionItem table[] = {
            { TextureFormat::Unknown, GL_NONE, GL_NONE, GL_NONE },
            { TextureFormat::RGBA8, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
            { TextureFormat::RGB8, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
            { TextureFormat::RGBA16F, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT },
            { TextureFormat::RGBA32F, GL_RGBA32F, GL_RGBA, GL_FLOAT },
            { TextureFormat::R16F, GL_R16F, GL_RED, GL_HALF_FLOAT },
            { TextureFormat::R32F, GL_R32UI, GL_RED, GL_FLOAT },
            { TextureFormat::R32S, GL_R32I, GL_RED_INTEGER, GL_INT },
            //{ TextureFormat::R32U, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT },
            //{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE }, // TextureFormat::D24S8,               ///< 32 ビットの Z バッファフォーマット
        };

#else
        // http://angra.blog31.fc2.com/blog-entry-11.html
        static const TextureFormatConversionItem table[] = {
            { TextureFormat::Unknown, GL_NONE, GL_NONE, GL_NONE },
            { TextureFormat::RGBA8, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE },
            { TextureFormat::RGB8, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE },
            { TextureFormat::RGBA16F, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT },
            { TextureFormat::RGBA32F, GL_RGBA32F, GL_RGBA, GL_FLOAT },
            { TextureFormat::R16F, GL_R16F, GL_RED, GL_HALF_FLOAT },
            { TextureFormat::R32F, GL_R32UI, GL_RED, GL_FLOAT },
            { TextureFormat::R32S, GL_R32I, GL_RED_INTEGER, GL_INT },
            //{ TextureFormat::R32U, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT },
            //{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE }, // TextureFormat::D24S8,               ///< 32 ビットの Z バッファフォーマット
        };
#endif
        assert(table[(int)format].format == format);
        *internalFormat = table[(int)format].internalFormat;
        *pixelFormat = table[(int)format].pixelFormat;
        *elementType = table[(int)format].elementType;
    }

    static void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
        GLuint glflags = 0;

        if (testFlag(flags, ClearFlags::Color)) {
            GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
            GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
            glflags |= GL_COLOR_BUFFER_BIT;
        }

        if (testFlag(flags, ClearFlags::Depth)) {
            GL_CHECK(glDepthMask(GL_TRUE));
            GL_CHECK(glClearDepth(z));
            glflags |= GL_DEPTH_BUFFER_BIT;
        }

        if (testFlag(flags, ClearFlags::Stencil)) {
            GL_CHECK(glClearStencil(stencil));
            glflags |= GL_STENCIL_BUFFER_BIT;
        }

        if (glflags != 0) {
            // Lumino の仕様としては、Viewport や Scissor の影響を受けないようにクリアしたい。
            // しかし glClear は Scissor の影響を受けるので GL_SCISSOR_TEST を切っておく。
            GL_CHECK(glDisable(GL_SCISSOR_TEST));

            // アタッチされているすべてのカラーバッファ・デプスバッファをクリアする。
            // 個別クリアしたいときは glClearBufferiv
            GL_CHECK(glClear(glflags));

            //GLint c[] = { 255, 0, 0, 255 };
            //GL_CHECK(glClearBufferiv(GL_COLOR, GL_DRAW_BUFFER0, c));
            //GLuint c[] = { 255, 0, 0, 255 };
            //GL_CHECK(glClearBufferuiv(GL_COLOR, GL_DRAW_BUFFER0, c));
        }
    }
};

} // namespace detail
} // namespace ln
