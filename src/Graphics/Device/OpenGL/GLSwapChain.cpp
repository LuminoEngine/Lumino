
#include "../../../Internal.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLSwapChain.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GLSwapChain
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLSwapChain::GLSwapChain()
	: m_vertexBuffer(NULL)
	, m_vertexArray(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLSwapChain::~GLSwapChain()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLSwapChain::Create()
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLSwapChain::OnLostDevice()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteVertexArrays(1, &m_vertexArray);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLSwapChain::OnResetDevice()
{
	/*	仮のバックバッファを本物のバックバッファに丸ごと転送するため、
		単に四角形を描画するためのシェーダと頂点バッファを作る。
	*/

	// シェーダ
	const char code[] =
		"#ifdef LN_GLSL_VERTEX\n"
		"attribute vec3 position;\n"
		"attribute vec2 texCoord;\n"
		"varying vec2 vTexCoord;\n"
		"void main() {\n"
		"	vTexCoord = texCoord;\n"
		"	gl_Position = vec4(position, 1.0);\n"
		"}\n"
		"#endif\n"

		"#ifdef LN_GLSL_FRAGMENT\n"		// Intel HD Graphics 4000 では 「#if <未定義マクロ> 」がエラーになった 
		"uniform sampler2D texture;"
		"varying vec2 vTexCoord;\n"
		"void main() {\n"
		"	gl_FragColor = texture2D(texture, vTexCoord);\n"
		"}\n"
		"#endif\n";
	StringA message;
	ShaderCompileResultLevel r = PlainGLSLBuilder::MakeShaderProgram(code, strlen(code), &m_shaderProgram, &message);
	if (r != ShaderCompileResultLevel_Success) {
		ShaderCompileResult result;
		result.Level = r;
		result.Message = message;
		LN_THROW(0, CompilationException, result);	// FailSafe. 
	}
	m_positionLoc = glGetAttribLocation(m_shaderProgram, "position");
	m_texCoordLoc = glGetAttribLocation(m_shaderProgram, "texCoord");
	m_textureLoc = glGetUniformLocation(m_shaderProgram, "texture");

	// 頂点バッファ
	const GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// 左上
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// 右上
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// 左下
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// 右下
	};
	glGenBuffers(1, &m_vertexBuffer); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); LN_CHECK_GLERROR();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLSwapChain::InternalPresent(ITexture* colorBuffer, GLRenderer* renderer)
{
	//glEnable(GL_TEXTURE_2D);
	//glDisable(GL_CULL_FACE);	// 両面描画

	// 各設定をデフォルトに戻す
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ブレンドや深度テスト無し。どうせ全部上書きする。
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	//glClearColor(0.3, 0.6, 1.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// VAO はコンテキスト間で共有できない。(WGL では動いたが、GLX では glBindVertexArray() でエラーとなった)
	// そのため、この SwapChain 用の VAO を、現在のスレッドで作る。(WGL ではスレッド間共有できない)
	if (m_vertexArray == 0) {
		glGenVertexArrays(1, &m_vertexArray); LN_CHECK_GLERROR();
	}


	glUseProgram(m_shaderProgram); LN_CHECK_GLERROR();

	// テクスチャユニット0に戻す
	glActiveTexture(GL_TEXTURE0); LN_CHECK_GLERROR();
	glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(colorBuffer)->GetGLTexture()); LN_CHECK_GLERROR();
	glUniform1i(m_textureLoc, 0); LN_CHECK_GLERROR();	// テクスチャユニット 0 を割り当てる


	// VAO と 頂点バッファを指定する
	glBindVertexArray(m_vertexArray); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); LN_CHECK_GLERROR();

	// 頂点情報の格納場所と書式を指定する (glBindVertexArray の先にしておかないと GL_INVALID_OPERATION になった)
	glVertexAttribPointer(m_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0); LN_CHECK_GLERROR();
	glVertexAttribPointer(m_texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLfloat*)0 + 3); LN_CHECK_GLERROR();

	// attribute 変数を有効にする
	glEnableVertexAttribArray(m_positionLoc); LN_CHECK_GLERROR();
	glEnableVertexAttribArray(m_texCoordLoc); LN_CHECK_GLERROR();

	// 描画
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); LN_CHECK_GLERROR();

	// attribute 変数を無効にする
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// 後始末
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
	renderer->ResetRenderState();
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

} // namespace Device
} // namespace Graphics
} // namespace Lumino
