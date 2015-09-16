
#pragma once 

#include "../../../Internal.h"
#include <Lumino/Graphics/Utils.h>
#include "GLTexture.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Device
{

//=============================================================================
// GLTextureBase
//=============================================================================

void GLTextureBase::GetGLTextureFormat(TextureFormat format, GLenum* internalFormat, GLenum* pixelFormat, GLenum* elementType)
{
	// http://angra.blog31.fc2.com/blog-entry-11.html
	static GLenum table[][3] =
	{
		// internalFormat,		pixelFormat,		elementType
		{ GL_NONE,				GL_NONE,			GL_NONE },			// TextureFormat_Unknown
		{ GL_RGBA8,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat_R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (uint32_t アクセス時の表現。lnByte[4] にすると、ABGR)
		{ GL_RGB,				GL_RGBA,			GL_UNSIGNED_BYTE },	// TextureFormat_R8G8B8X8,            ///< 32 ビットのアルファ無しフォーマット
		{ GL_BGRA,				GL_BGRA,			GL_UNSIGNED_BYTE },	// TextureFormat_B8G8R8A8,            ///< 32 ビットのアルファ付きフォーマット
		{ GL_BGR,				GL_BGRA,			GL_UNSIGNED_BYTE },	// TextureFormat_B8G8R8X8,            ///< 32 ビットのアルファ無しフォーマット
		{ GL_RGBA16F,			GL_RGBA,			GL_HALF_FLOAT },	// TextureFormat_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
		{ GL_RGBA32F,			GL_RGBA,			GL_DOUBLE },		// TextureFormat_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
		{ GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE }, // TextureFormat_D24S8,               ///< 32 ビットの Z バッファフォーマット
		{ GL_R16F,				GL_RED,				GL_HALF_FLOAT },	// TextureFormat_R16F,
		{ GL_R32F,				GL_RED,				GL_DOUBLE },		// TextureFormat_R32F,
	};
	assert(LN_ARRAY_SIZE_OF(table) == TextureFormat_Max);
	*internalFormat = table[format][0];
	*pixelFormat = table[format][1];
	*elementType = table[format][2];
}

void GLTextureBase::SetGLSamplerState(const SamplerState& state)
{
	GLint filter[] =
	{
		GL_NEAREST,			// TextureFilterMode_Point,
		GL_LINEAR,			// TextureFilterMode_Linear,
	};
	GLint wrap[] =
	{
		GL_REPEAT,			// TextureWrapMode_Repeat
		GL_CLAMP_TO_EDGE,	// TextureWrapMode_Clamp
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter[state.FilterMode]); LN_CHECK_GLERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter[state.FilterMode]); LN_CHECK_GLERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap[state.WrapMode]); LN_CHECK_GLERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap[state.WrapMode]); LN_CHECK_GLERROR();
}

//=============================================================================
// GLTexture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLTexture::GLTexture(const Size& size, TextureFormat format, uint32_t mipLevels)
	: m_glTexture(0)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
	, m_mipLevels(mipLevels)
	, m_pixelFormat(GL_NONE)
	, m_elementType(GL_NONE)
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//GLTexture::GLTexture(const Graphics::Bitmap* bitmap, TextureFormat format, uint32_t mipLevels)
//{
//	LN_THROW(0, InvalidOperationException);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLTexture::~GLTexture()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLTexture::OnLostDevice()
{
	if (m_glTexture != 0)
	{
		glDeleteTextures(1, &m_glTexture);
		m_glTexture = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLTexture::OnResetDevice()
{
	// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20040914

	// ミップレベル決定
	GLint levels = (m_mipLevels == 0) ? 4 : m_mipLevels - 1;	// TODO:DirectX だと 0 の場合は全レベル作成するけど、今ちょっと計算めんどうなので 
	
	// テクスチャフォーマット選択
	GLenum internalFormat, pixelFormat, elementType;
	GetGLTextureFormat(m_format, &internalFormat, &pixelFormat, &elementType);

	// テクスチャ作成
	glGenTextures(1, &m_glTexture); LN_CHECK_GLERROR();
	glBindTexture(GL_TEXTURE_2D, m_glTexture); LN_CHECK_GLERROR();
	glTexImage2D(GL_TEXTURE_2D, levels, internalFormat, m_realSize.Width, m_realSize.Height, 0, pixelFormat, elementType, NULL); LN_CHECK_GLERROR();
	
	// デフォルトのサンプラステート (セットしておかないとサンプリングできない)
	SetGLSamplerState(m_samplerState);

	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLTexture::SetSamplerState(const SamplerState& state)
{
	m_samplerState = state;
	glGenTextures(1, &m_glTexture); LN_CHECK_GLERROR();
	SetGLSamplerState(m_samplerState);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLTexture::SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize)
{
	//LN_THROW(point.IsZero(), NotImplementedException);

	// ※同一フォーマットであることが前提

	// フォーマットが同一ならそのまま転送すればよい
	//if (bitmap->GetPixelFormat() == Utils::TranslatePixelFormat(m_format))
	{
		// テクスチャフォーマット選択
		GLenum internalFormat, pixelFormat, elementType;
		GetGLTextureFormat(m_format, &internalFormat, &pixelFormat, &elementType);

		glBindTexture(GL_TEXTURE_2D, m_glTexture); LN_CHECK_GLERROR();
		/* テクスチャ画像はバイト単位に詰め込まれている */
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,//mMipLevel,
			point.X,
			/*m_realSize.Height - */point.Y,
			dataBitmapSize.Width,//bitmap->GetSize().Width,
			dataBitmapSize.Height,///bitmap->GetSize().Height,
			pixelFormat,
			elementType,
			data);
		LN_CHECK_GLERROR();
		//glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// フォーマットが違う場合は一時ビットマップを作って変換する必要がある
	//else
	//{
	//	LN_THROW(0, NotImplementedException);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap* GLTexture::Lock()
{
	m_lockedTexture.Attach(LN_NEW Bitmap(m_size, Utils::TranslatePixelFormat(m_format)));
	return m_lockedTexture;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLTexture::Unlock()
{
	SetSubData(Point(0, 0), m_lockedTexture->GetBitmapBuffer()->GetConstData(), m_lockedTexture->GetBitmapBuffer()->GetSize(), m_size);
	m_lockedTexture.SafeRelease();
}


//=============================================================================
// GLRenderTargetTexture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLRenderTargetTexture::GLRenderTargetTexture(const Size& size, TextureFormat format, int mipLevels)
	: m_glTexture(0)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
	, m_mipLevels(mipLevels)
	, m_lockingBitmap(NULL)
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLRenderTargetTexture::~GLRenderTargetTexture()
{
	OnLostDevice();
	LN_SAFE_DELETE(m_lockingBitmap);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderTargetTexture::OnLostDevice()
{
	if (m_glTexture != 0) {
		glDeleteTextures(1, &m_glTexture);
		m_glTexture = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderTargetTexture::OnResetDevice()
{
	if (m_glTexture == 0)
	{
		glGenTextures(1, &m_glTexture); LN_CHECK_GLERROR();
		glBindTexture(GL_TEXTURE_2D, m_glTexture); LN_CHECK_GLERROR();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); LN_CHECK_GLERROR();

		GLenum internalFormat;
		GetGLTextureFormat(m_format, &internalFormat, &m_pixelFormat, &m_elementType);

		glTexImage2D(
			GL_TEXTURE_2D,
			0/*m_mipLevels*/,	// 0 でないと glCheckFramebufferStatus が GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT を返した。MipMap についてはちゃんと調査する必要がありそう。
			internalFormat,
			m_realSize.Width,
			m_realSize.Height,
			0,
			m_pixelFormat,
			m_elementType,
			NULL);
		LN_CHECK_GLERROR();

		// glTexParameteri() を一つも指定しないと, テクスチャが正常に使用できない場合がある
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  LN_CHECK_GLERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  LN_CHECK_GLERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     LN_CHECK_GLERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);     LN_CHECK_GLERROR();
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);     LN_CHECK_GLERROR();

		glBindTexture(GL_TEXTURE_2D, 0); LN_CHECK_GLERROR();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap* GLRenderTargetTexture::Lock()
{
	m_lockingBitmap = LN_NEW Bitmap(m_size, Utils::TranslatePixelFormat(m_format), true);

	glBindTexture(GL_TEXTURE_2D, m_glTexture); LN_CHECK_GLERROR();
	glGetTexImage(GL_TEXTURE_2D, 0, m_pixelFormat, m_elementType, m_lockingBitmap->GetBitmapBuffer()->GetData()); LN_CHECK_GLERROR();
	glBindTexture(GL_TEXTURE_2D, 0); LN_CHECK_GLERROR();

	return m_lockingBitmap;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderTargetTexture::Unlock()
{
	LN_SAFE_DELETE(m_lockingBitmap);
}

//=============================================================================
// GLDepthBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLDepthBuffer::GLDepthBuffer(const Size& size, TextureFormat format)
	: m_glBuffer(0)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLDepthBuffer::~GLDepthBuffer()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLDepthBuffer::OnLostDevice()
{
	if (m_glBuffer != 0) {
		glDeleteRenderbuffers(1, &m_glBuffer);
		m_glBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLDepthBuffer::OnResetDevice()
{
	if (m_glBuffer == 0)
	{
		GLenum internalFormat, pixelFormat, elementType;
		GetGLTextureFormat(m_format, &internalFormat, &pixelFormat, &elementType);

		glGenRenderbuffers(1, &m_glBuffer); LN_CHECK_GLERROR();
		glBindRenderbuffer(GL_RENDERBUFFER, m_glBuffer); LN_CHECK_GLERROR();
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, m_realSize.Width, m_realSize.Height); LN_CHECK_GLERROR();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

#if 0
//=============================================================================
// GLBackbufferRenderTarget
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLBackbufferRenderTarget::GLBackbufferRenderTarget()
	: m_glRenderBuffer(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLBackbufferRenderTarget::~GLBackbufferRenderTarget()
{
	if (m_glRenderBuffer != 0) {
		glDeleteRenderbuffers(1, &m_glRenderBuffer);
		m_glRenderBuffer = 0;
	}
	// 0 の場合はデフォルトのバックバッファを示しているので、自分で開放する必要はない
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLBackbufferRenderTarget::Reset(GLuint renderBuffer)
{
	m_glRenderBuffer = renderBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLBackbufferRenderTarget::OnLostDevice()
{
	if (m_glRenderBuffer != 0) {
		glDeleteRenderbuffers(1, &m_glRenderBuffer);
		m_glRenderBuffer = 0;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLBackbufferRenderTarget::OnResetDevice()
{
	// SwapChain からもらうものなのでここでは何もできない
}
#endif

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
