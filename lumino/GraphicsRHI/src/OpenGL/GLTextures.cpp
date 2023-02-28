#include <LuminoGraphicsRHI/RHIHelper.hpp>
#include "GLHelper.hpp"
#include "GLTextures.hpp"
#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLTexture2D

GLTextureBase::GLTextureBase() {
}


//==============================================================================
// GLTexture2D

GLTexture2D::GLTexture2D()
    : m_id(0)
    , m_usage(GraphicsResourceUsage::Static)
    , m_textureFormat(TextureFormat::Unknown)
    , m_pixelFormat(0)
    , m_elementType(0)
    , m_mipmap(false) {
}

GLTexture2D::~GLTexture2D() {
}

Result GLTexture2D::init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_TRY(GLTextureBase::initAsTexture2D(usage, width, height, requestFormat, mipmap));

    m_usage = usage;
    m_textureFormat = requestFormat;
    m_mipmap = mipmap;

    // http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20040914

    GLint levels = 0; // (m_mipmap) ? 4 : 0;	// TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので

    GLenum internalFormat;
    OpenGLHelper::getGLTextureFormat(requestFormat, &internalFormat, &m_pixelFormat, &m_elementType);

    // テクスチャ作成
    GL_CHECK(glGenTextures(1, &m_id));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, levels, internalFormat, width, height, 0, m_pixelFormat, m_elementType, initialData));

    //if (mipmap) {
    //    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    //}

    // デフォルトのサンプラステート (セットしておかないとサンプリングできない)
    //setGLSamplerState(m_samplerState);
    //{

    GLint filter[] = {
        GL_NEAREST, // TextureFilterMode_Point,
        GL_LINEAR,  // TextureFilterMode_Linear,
    };
    //	GLint wrap[] =
    //	{
    //		GL_REPEAT,			// TextureWrapMode_Repeat
    //		GL_CLAMP_TO_EDGE,	// TextureWrapMode_Clamp
    //	};

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter[1]));
    //if (LN_ENSURE_GLERROR()) return;
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter[1]));
    //if (LN_ENSURE_GLERROR()) return;

    //	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap[1]);
    //	//if (LN_ENSURE_GLERROR()) return;
    //	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap[1]);
    //	//if (LN_ENSURE_GLERROR()) return;
    //}

    if (m_mipmap) {
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    return ok();
}

void GLTexture2D::dispose() {
    if (m_id != 0) {
        GL_CHECK(glDeleteTextures(1, &m_id));
        m_id = 0;
    }

    GLTextureBase::dispose();
}

RHIRef<RHIBitmap> GLTexture2D::readData() {
    LN_UNREACHABLE();
    return nullptr;
}

//SizeI GLTexture2D::realSize() {
//    return m_size;
//}
//
//TextureFormat GLTexture2D::getTextureFormat() const {
//    return m_textureFormat;
//}

void GLTexture2D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) {
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

    GLint levels = (m_mipmap) ? 4 : 0; // TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので

    /* テクスチャ画像はバイト単位に詰め込まれている */
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GL_CHECK(glTexSubImage2D(
        GL_TEXTURE_2D,
        levels,
        x,
        /*m_realSize.Height - */ y,
        width,
        height,
        m_pixelFormat,
        m_elementType,
        data));

    if (m_mipmap) {
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void GLTexture2D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    LN_UNREACHABLE();
}

//==============================================================================
// GLTexture3D

GLTexture3D::GLTexture3D()
    : m_id(0)
    , m_usage(GraphicsResourceUsage::Static)
    , m_textureFormat(TextureFormat::Unknown)
    , m_pixelFormat(0)
    , m_elementType(0) {
}

GLTexture3D::~GLTexture3D() {
}

Result GLTexture3D::init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_TRY(GLTextureBase::initAsTexture3D(usage, width, height, depth, requestFormat));
    m_usage = usage;
    m_textureFormat = requestFormat;
    GLint levels = (mipmap) ? 4 : 0; // TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので

    GLenum internalFormat;
    OpenGLHelper::getGLTextureFormat(requestFormat, &internalFormat, &m_pixelFormat, &m_elementType);

    GLuint tex;
    GL_CHECK(glGenTextures(1, &m_id));
    GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_id));
    GL_CHECK(glTexImage3D(GL_TEXTURE_3D, levels, internalFormat, width, height, depth, 0, m_pixelFormat, m_elementType, initialData));

    //// テクスチャの拡大・縮小に線形補間を用いる
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //// テクスチャからはみ出た部分には境界色を用いる
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    // テクスチャの境界色を設定する (ボリュームの外には何もない)
    //static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    //glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, black);

    GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
}

void GLTexture3D::dispose() {
    if (m_id != 0) {
        GL_CHECK(glDeleteTextures(1, &m_id));
        m_id = 0;
    }

    GLTextureBase::dispose();
}

RHIRef<RHIBitmap> GLTexture3D::readData() {
    LN_UNREACHABLE();
    return nullptr;
}

//SizeI GLTexture3D::realSize() {
//    LN_NOTIMPLEMENTED();
//    return SizeI::Zero;
//}
//
//TextureFormat GLTexture3D::getTextureFormat() const {
//    return m_textureFormat;
//}

void GLTexture3D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) {
    LN_UNREACHABLE();
}

void GLTexture3D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_id));
    GL_CHECK(glTexSubImage3D(GL_TEXTURE_3D,
                             0, // TODO: Mipmap
                             x,
                             y,
                             z,
                             width,
                             height,
                             depth,
                             m_pixelFormat,
                             m_elementType,
                             data));
    GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
}

//==============================================================================
// GLRenderTargetTexture

GLRenderTargetTexture::GLRenderTargetTexture() {
}

GLRenderTargetTexture::~GLRenderTargetTexture() {
}

Result GLRenderTargetTexture::init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) {
    LN_TRY(GLTextureBase::initAsRenderTarget(width, height, requestFormat, mipmap, false));
    if (mipmap) {
        LN_NOTIMPLEMENTED();
        return err();
    }

    GL_CHECK(glGenTextures(1, &m_id));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    GLenum internalFormat;
    //m_pixelFormat = GL_BGRA;
    OpenGLHelper::getGLTextureFormat(requestFormat, &internalFormat, &m_pixelFormat, &m_elementType);

    //	internalFormat = GL_RGB;
    //	m_pixelFormat = GL_RGB;
    //	m_elementType = GL_UNSIGNED_BYTE;

    GL_CHECK(glTexImage2D(
        GL_TEXTURE_2D,
        0 /*m_mipLevels*/, // TODO: 0 でないと glCheckFramebufferStatus が GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT を返した。MipMap についてはちゃんと調査する必要がありそう。
        internalFormat,
        width,
        height,
        0,
        m_pixelFormat,
        m_elementType,
        nullptr));

    // glTexParameteri() を一つも指定しないと, テクスチャが正常に使用できない場合がある
    //GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    //GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    //GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    //GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    return ok();
}

Result GLRenderTargetTexture::init(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) {
    LN_TRY(GLTextureBase::initAsRenderTarget(hintWidth, hintHeight, TextureFormat::Unknown, false, false));
    m_id = nativeObject;
    return ok();
}

void GLRenderTargetTexture::dispose() {
    if (m_id != 0) {
        GL_CHECK(glDeleteTextures(1, &m_id));
        m_id = 0;
    }

    GLTextureBase::dispose();
}

RHIRef<RHIBitmap> GLRenderTargetTexture::readData() {
    const RHIExtent3D& size = extentSize();
    auto buf = makeRHIRef<RHIBitmap>();
    if (!buf->init(RHIHelper::getPixelSize(textureFormat()), size.width, size.height)) {
        return nullptr;
    }


#ifdef GL_GLES_PROTOTYPES
    // OpenGL ES is glGetTexImage unsupported
    // http://oppyen.hatenablog.com/entry/2016/10/21/071612
#else
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    GL_CHECK(glGetTexImage(GL_TEXTURE_2D, 0, m_pixelFormat, m_elementType, buf->writableData()));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
#endif

    // Note: RenderTarget 書き込み時に上下反転するには、gl_Position.y を反転するしかない。
    // glslangValidator や spirv-cross はオプションでこの反転コードを自動追加できるが、
    // 本来はポストエフェクト (RTコピー時に反転) にだけ使いたくて、通常のジオメトリまで反転したくない。
    // そのため Test_Graphics_LowLevelRendering.BasicTriangle のように上向きの三角形を描いたりすると、
    // glGetTexImage で取り出した この時点での outData は、上下反転している状態になっている。

    //BitmapHelper::blitRawSimple(outData, buf->data(), m_size.width, m_size.height, GraphicsHelper::getPixelSize(m_textureFormat), true);

#if 0   // 検証用
    struct Color3 { uint8_t r, g, b; };
    const Color3* c = (const Color3*)buf->data();
    for (int y = 0; y < size.height; y++) {
    	int count = 0;
    	for (int x = 0; x < size.width; x++) {
    		if (c[y *size.height + x].g < 255) {
    			count++;
    		}
    	}
    	printf("%d\n", count);
    }
#endif

    return buf;
}

//SizeI GLRenderTargetTexture::realSize() {
//    return m_size;
//}
//
//TextureFormat GLRenderTargetTexture::getTextureFormat() const {
//    return m_textureFormat;
//}

void GLRenderTargetTexture::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) {
    LN_UNREACHABLE();
}

void GLRenderTargetTexture::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    LN_UNREACHABLE();
}

//==============================================================================
// GLSamplerState

GLSamplerState::GLSamplerState()
    : m_id(0)

{
}

GLSamplerState::~GLSamplerState() {
}

void GLSamplerState::init(OpenGLDevice* device, const SamplerStateData& desc) {
    m_desc = desc;

    static const GLenum magFilterTable[] = {
        GL_NEAREST, // TextureFilterMode::Point,
        GL_LINEAR,  // TextureFilterMode::Linear,
    };

    static const GLenum minFilterTable[][2] = {
        { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST }, // TextureFilterMode::Point,
        { GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR },    // TextureFilterMode::Linear,
    };
    const GLint addressTable[] = {
        GL_REPEAT,        // TextureAddressMode::Repeat
        GL_CLAMP_TO_EDGE, // TextureAddressMode::Clamp
    };

    GL_CHECK(glGenSamplers(1, &m_id));
    GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, magFilterTable[static_cast<int>(desc.filter)]));
    GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, minFilterTable[static_cast<int>(desc.filter)][0]));
    GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, addressTable[static_cast<int>(desc.address)]));
    GL_CHECK(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, addressTable[static_cast<int>(desc.address)]));
    if (m_desc.anisotropy && device->caps().support_filter_anisotropic) {
        GL_CHECK(glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, device->caps().MAX_TEXTURE_MAX_ANISOTROPY_EXT));
    }

    GL_CHECK(glGenSamplers(1, &m_idMip));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_MAG_FILTER, magFilterTable[static_cast<int>(desc.filter)]));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_MIN_FILTER, minFilterTable[static_cast<int>(desc.filter)][1]));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_WRAP_S, addressTable[static_cast<int>(desc.address)]));
    GL_CHECK(glSamplerParameteri(m_idMip, GL_TEXTURE_WRAP_T, addressTable[static_cast<int>(desc.address)]));
    if (m_desc.anisotropy && device->caps().support_filter_anisotropic) {
        GL_CHECK(glSamplerParameterf(m_idMip, GL_TEXTURE_MAX_ANISOTROPY_EXT, device->caps().MAX_TEXTURE_MAX_ANISOTROPY_EXT));
    }
}

void GLSamplerState::dispose() {
    if (m_id) {
        GL_CHECK(glDeleteSamplers(1, &m_id));
        m_id = 0;
    }
    if (m_idMip) {
        GL_CHECK(glDeleteSamplers(1, &m_idMip));
        m_idMip = 0;
    }
    ISamplerState::dispose();
}

} // namespace detail
} // namespace ln
