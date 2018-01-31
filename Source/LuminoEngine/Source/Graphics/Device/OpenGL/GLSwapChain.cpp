
#include "../../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "GLGraphicsDevice.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLSwapChain.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// GLSwapChain
//==============================================================================

//------------------------------------------------------------------------------
GLSwapChain::GLSwapChain()
	: m_device(nullptr)
	, m_context(nullptr)
	, m_window(nullptr)
	, m_renderTarget(nullptr)
	, m_shaderProgram(0)
	, m_positionLoc(0)
	, m_texCoordLoc(0)
	, m_textureLoc(0)
	, m_vertexBuffer(0)
	, m_vertexArray(0)
{
}

//------------------------------------------------------------------------------
GLSwapChain::~GLSwapChain()
{
	onLostDevice();
	LN_SAFE_RELEASE(m_renderTarget);
	LN_SAFE_RELEASE(m_context);
	LN_SAFE_RELEASE(m_window);
}

//------------------------------------------------------------------------------
void GLSwapChain::initialize(GLGraphicsDevice* device, GLContext* context, PlatformWindow* window)
{
	m_device = device;
	LN_REFOBJ_SET(m_context, context);
	LN_REFOBJ_SET(m_window, window);


	// TODO: バックバッファサイズ
	m_backBufferSize = m_window->getSize();
	m_renderTarget = LN_NEW GLRenderTargetTexture(m_backBufferSize, TextureFormat::R8G8B8A8, 1);

	onResetDevice();
}

//------------------------------------------------------------------------------
void GLSwapChain::onLostDevice()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteVertexArrays(1, &m_vertexArray);
}

//------------------------------------------------------------------------------
void GLSwapChain::onResetDevice()
{
	/*	仮のバックバッファを本物のバックバッファに丸ごと転送するため、
		単に四角形を描画するためのシェーダと頂点バッファを作る。
	*/

	// シェーダ
	const char vsCode[] =
        "#version 120\n"            // for mac
        "attribute vec3 position;\n"
		"attribute vec2 texCoord;\n"
		"varying vec2 vTexCoord;\n"
		"void main() {\n"
		"	vTexCoord = texCoord;\n"
		"	gl_Position = vec4(position, 1.0);\n"
		"}\n";
    const char fsCode[] =
        "#version 120\n"
        "uniform sampler2D texture;"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "	gl_FragColor = texture2D(texture, vTexCoord);\n"
        "}\n";
	std::string message;
	ShaderCompileResultLevel r = GLSLUtils::makeShaderProgram(vsCode, sizeof(vsCode), fsCode, sizeof(fsCode), &m_shaderProgram, &message);
	if (LN_ENSURE(r == ShaderCompileResultLevel_Success)) return;
	m_positionLoc = glGetAttribLocation(m_shaderProgram, "position");
	m_texCoordLoc = glGetAttribLocation(m_shaderProgram, "texCoord");
	m_textureLoc = glGetUniformLocation(m_shaderProgram, "texture");

	// 頂点バッファ
	const GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// 左上
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// 左下
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// 右上
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// 右下
	};

	glGenBuffers(1, &m_vertexBuffer);
	if (LN_ENSURE_GLERROR()) return;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	if (LN_ENSURE_GLERROR()) return;

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	if (LN_ENSURE_GLERROR()) return;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (LN_ENSURE_GLERROR()) return;
}

//------------------------------------------------------------------------------
void GLSwapChain::resize(const SizeI& size)
{
	m_backBufferSize = size;
}

//------------------------------------------------------------------------------
void GLSwapChain::present(ITexture* colorBuffer)
{
	assert(colorBuffer == m_renderTarget);

	try
	{
		m_device->makeCurrentContext(m_context);

		// colorBuffer をバックバッファに描画する
		internalPresent(colorBuffer, static_cast<GLRenderer*>(m_device->getRenderer()));

		// バックバッファをフロントバッファに転送する
		m_context->swapBuffers();
	}
	catch (...)
	{
		m_device->makeCurrentContext(m_device->getMainRenderingContext());
		throw;
	}
	m_device->makeCurrentContext(m_device->getMainRenderingContext());
	m_device->gcDeviceResource();


	if (m_backBufferSize != m_renderTarget->getSize())
	{
		LN_SAFE_RELEASE(m_renderTarget);
		m_renderTarget = LN_NEW GLRenderTargetTexture(m_backBufferSize, TextureFormat::R8G8B8A8, 1);
	}
}

//------------------------------------------------------------------------------
void GLSwapChain::internalPresent(ITexture* colorBuffer, GLRenderer* renderer)
{
	//glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);	// 両面描画

	// 各設定をデフォルトに戻す
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (LN_ENSURE_GLERROR()) return;

	const SizeI& size = m_window->getSize();
	glViewport(0, 0, size.width, size.height);
	if (LN_ENSURE_GLERROR()) return;
    
    /*
	GLenum fs = glCheckFramebufferStatus( GL_FRAMEBUFFER );
     GL_FRAMEBUFFER_COMPLETE	フレームバッファが完全である	36053
     GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT	フレームバッファのアタッチメントポイントが不完全である。必要なアタッチメントが無いか、テクスチャレンダラーバッファが有効でない	36054
     GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	フレームバッファに有効なアタッチメントが無い	36055
     GL_FRAMEBUFFER_INCOMPLETE_DISMISIONS	アタッチメントの高さか幅が異なる	36057
     GL_FRAMEBUFFER_INCOMPLETE_FORMATS	アタッチメントが使用している内部フォーマットがレンダリング可能なものではない
     GL_FRAMEBUFFER_UNSUPPORTED	フレームバッファ内のアタッチメントが使用している内部フォーマットの組み合わせがレンダリングできないターゲットになっている	36061
     */

	// ブレンドや深度テスト無し。どうせ全部上書きする。
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	//glClearColor(0.3, 0.6, 1.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// VAO はコンテキスト間で共有できない。(WGL では動いたが、GLX では glBindVertexArray() でエラーとなった)
	// そのため、この SwapChain 用の VAO を、現在のスレッドで作る。(WGL ではスレッド間共有できない)
	if (m_vertexArray == 0) {
		glGenVertexArrays(1, &m_vertexArray);
		if (LN_ENSURE_GLERROR()) return;
	}


	glUseProgram(m_shaderProgram);
	if (LN_ENSURE_GLERROR()) return;

	// テクスチャユニット0に戻す
	glActiveTexture(GL_TEXTURE0);
	if (LN_ENSURE_GLERROR()) return;
	glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(colorBuffer)->getGLTexture());
	if (LN_ENSURE_GLERROR()) return;
	glUniform1i(m_textureLoc, 0);	// テクスチャユニット 0 を割り当てる
	if (LN_ENSURE_GLERROR()) return;


	// VAO と 頂点バッファを指定する
	glBindVertexArray(m_vertexArray);
	if (LN_ENSURE_GLERROR()) return;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	if (LN_ENSURE_GLERROR()) return;

	// 頂点情報の格納場所と書式を指定する (glBindVertexArray の先にしておかないと GL_INVALID_OPERATION になった)
	glVertexAttribPointer(m_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	if (LN_ENSURE_GLERROR()) return;
	glVertexAttribPointer(m_texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLfloat*)0 + 3);
	if (LN_ENSURE_GLERROR()) return;

	// attribute 変数を有効にする
	glEnableVertexAttribArray(m_positionLoc);
	if (LN_ENSURE_GLERROR()) return;
	glEnableVertexAttribArray(m_texCoordLoc);
	if (LN_ENSURE_GLERROR()) return;

	//glClearColor(1, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	// 描画
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (LN_ENSURE_GLERROR()) return;

	// attribute 変数を無効にする
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

    // 後始末
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
#if 0
	// colorBuffer 全体をバックバッファに描画する
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D); LN_CHECK_GLERROR();
	glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(colorBuffer)->GetGLTexture()); LN_CHECK_GLERROR();
	glBegin(GL_POLYGON/*GL_TRIANGLE_FAN*/);// LN_CHECK_GLERROR();	// glBegin～glEnd の間で glGetError は禁止 https://www.opengl.org/discussion_boards/showthread.php/163474-GL_INVALID_OPERATION-after-glEnd%28%29
	glTexCoord2f(0.0, 0.0); glVertex2d(-1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex2d(-1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex2d(1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex2d(1.0, -1.0);
	glEnd();

	glDisable(GL_TEXTURE_2D); LN_CHECK_GLERROR();
	glFlush(); LN_CHECK_GLERROR();
#endif
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
