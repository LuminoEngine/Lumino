
#include "../../../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLShader.h"
#include "GLRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// GLRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLRenderer::GLRenderer()
	: m_currentVertexBuffer(NULL)
	, m_currentIndexBuffer(NULL)
	, m_currentDepthBuffer(NULL)
	, m_currentShaderPass(NULL)
	, m_vertexArray(0)
	, m_framebuffer(0)
	, m_modifiedFrameBuffer(false)
	, m_justSawReset(false)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLRenderer::~GLRenderer()
{
	LN_SAFE_RELEASE(m_currentVertexBuffer);
	LN_SAFE_RELEASE(m_currentIndexBuffer);

	OnLostDevice();

	for (int i = 0; i < MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
	LN_SAFE_RELEASE(m_currentDepthBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::Initialize()
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLuint GLRenderer::GetVertexArrayObject()
{
	// VAO はコンテキスト間で共有できないため、描画スレッド上で作る必要がある。
	// なお、メインスレッドの MainContext で作った後、描画スレッドの RenderingContext に glBindVertexArray すると、
	// GL_INVALID_OPERATION が発生する。
	// http://stackoverflow.com/questions/18492878/crash-on-vaos-loaded-from-non-main-thread
	if (m_vertexArray == 0) {
		glGenVertexArrays(1, &m_vertexArray);
	}
	return m_vertexArray;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::OnLostDevice()
{
	if (m_vertexArray != 0) {
		// TODO: VAO はコンテキスト間で共有できないので、描画スレッド上で開放する必要がある
		glDeleteVertexArrays(1, &m_vertexArray);
		m_vertexArray = 0;
	}
	if (m_framebuffer != 0) {
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::OnResetDevice()
{
	if (m_framebuffer == 0) {
		glGenFramebuffers(1, &m_framebuffer);
	}
	m_justSawReset = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::EnterRenderState()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::LeaveRenderState()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::Begin()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::End()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetRenderState(const RenderState& state)
{
	m_requestedRenderState = state;
	//UpdateRenderState(state, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//const RenderState& GLRenderer::GetRenderState()
//{
//	return m_currentRenderState;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetDepthStencilState(const DepthStencilState& state)
{
	m_requestedDepthStencilState = state;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//const DepthStencilState& GLRenderer::GetDepthStencilState()
//{
//	return m_currentDepthStencilState;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetRenderTarget(int index, ITexture* texture)
{
	if (texture != nullptr) {
		LN_THROW((texture->GetTextureType() == TextureType_RenderTarget), ArgumentException);
	}
	LN_REFOBJ_SET(m_currentRenderTargets[index], static_cast<GLRenderTargetTexture*>(texture));
	m_modifiedFrameBuffer = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* GLRenderer::GetRenderTarget(int index)
{
	return m_currentRenderTargets[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetDepthBuffer(ITexture* texture)
{
	if (texture != nullptr)
	{
		LN_THROW((texture->GetTextureType() == TextureType_DepthBuffer), ArgumentException);
	}
	LN_REFOBJ_SET(m_currentDepthBuffer, static_cast<GLDepthBuffer*>(texture));
	m_modifiedFrameBuffer = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//ITexture* GLRenderer::GetDepthBuffer()
//{
//	LN_THROW(0, NotImplementedException);
//	return 0;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetViewport(const Rect& rect)
{
	const Size& scr = m_currentRenderTargets[0]->GetSize();
	glViewport(rect.x, scr.height - (rect.y + rect.height), rect.width, rect.height);
	//LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//const Rect& GLRenderer::GetViewport()
//{
//	return m_currentViewportRect;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetVertexBuffer(IVertexBuffer* vertexBuffer)
{
	LN_REFOBJ_SET(m_currentVertexBuffer, static_cast<GLVertexBuffer*>(vertexBuffer));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::SetIndexBuffer(IIndexBuffer* indexBuffer)
{
	LN_REFOBJ_SET(m_currentIndexBuffer, static_cast<GLIndexBuffer*>(indexBuffer));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	// フレームバッファを最新にする
	UpdateFrameBuffer();

	glDepthMask(GL_TRUE);	LN_CHECK_GLERROR();   // これがないと Depth が正常にクリアされない

	glClearColor(color.r, color.g, color.b, color.a); LN_CHECK_GLERROR();
	glClearDepth(z); LN_CHECK_GLERROR();
	glClearStencil(0); LN_CHECK_GLERROR();
    
    /*
    GLenum fs = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if (fs == GL_FRAMEBUFFER_COMPLETE)
    {
        printf("GL_FRAMEBUFFER_COMPLETE¥n");
    }
    if (fs == GL_FRAMEBUFFER_UNSUPPORTED)
    {
        printf("GL_FRAMEBUFFER_UNSUPPORTED¥n");
    }
    if (fs == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    {
        printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT¥n");
    }
     */

    // TODO: フレームバッファのサイズが０だと失敗する
	glClear(
		((flags.TestFlag(ClearFlags::Color)) ? GL_COLOR_BUFFER_BIT : 0) |
		((flags.TestFlag(ClearFlags::Depth)) ? GL_DEPTH_BUFFER_BIT : 0) |
		((flags.TestFlag(ClearFlags::Stencil)) ? GL_STENCIL_BUFFER_BIT : 0));
	LN_CHECK_GLERROR();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::DrawPrimitive(IVertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	// TODO
	SetVertexBuffer(vertexBuffer);

	if (m_currentVertexBuffer == NULL) {
		LN_THROW(0, InvalidOperationException);
		return;
	}

	// 描画に必要な情報を最新にする
	UpdateRenderState(m_requestedRenderState, m_justSawReset);
	UpdateDepthStencilState(m_requestedDepthStencilState, m_justSawReset);
	UpdateFrameBuffer();
	UpdateVAO();
	UpdateVertexAttribPointer();
	m_justSawReset = false;


	// TODO:仮位置
	//glCullFace(GL_FRONT_AND_BACK); LN_CHECK_GLERROR();
	//glFrontFace(GL_CCW); LN_CHECK_GLERROR();
	//glEnable(GL_CULL_FACE); LN_CHECK_GLERROR();
	//glFrontFace(GL_CCW);//時計回りが表
	//glEnable(GL_CULL_FACE);//カリングON 
	//glDisable(GL_CULL_FACE);

	// プリミティブと必要な頂点数の取得
	GLenum gl_prim;
	int vertexCount;
	GetPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	// 描画
	glDrawArrays(gl_prim, startVertex, vertexCount);
	LN_CHECK_GLERROR();

	// 後始末 (解除)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::DrawPrimitiveIndexed(IVertexBuffer* vertexBuffer, IIndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	// TODO
	SetVertexBuffer(vertexBuffer);
	SetIndexBuffer(indexBuffer);

	if (m_currentVertexBuffer == NULL ||
		m_currentIndexBuffer == NULL ||
		m_currentShaderPass == NULL) {
		LN_THROW(0, InvalidOperationException);
		return;
	}

	// 描画に必要な情報を最新にする
	UpdateRenderState(m_requestedRenderState, m_justSawReset);
	UpdateDepthStencilState(m_requestedDepthStencilState, m_justSawReset);
	UpdateFrameBuffer();
	UpdateVAO();
	UpdateVertexAttribPointer();
	m_justSawReset = false;

	// プリミティブと必要な頂点数の取得
	GLenum gl_prim;
	int vertexCount;
	GetPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	// 引数 start end には、本来であれば0～vertexCountまでのインデックスの中の最大、最小の値を渡す。
	// http://wiki.livedoor.jp/mikk_ni3_92/d/glDrawRangeElements%A4%CB%A4%E8%A4%EB%C9%C1%B2%E8
	// ただ、全範囲を渡しても特に問題なさそうなのでこのまま。
	if (m_currentIndexBuffer->GetFormat() == IndexBufferFormat_UInt16)
	{
		glDrawElements(gl_prim, vertexCount, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(GLushort) * startIndex));
	}
	else
	{
		glDrawElements(gl_prim, vertexCount, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * startIndex));
	}

	// 後始末 (解除)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::UpdateRenderState(const RenderState& newState, bool reset)
{
	if (reset)
	{
		// 時計回りを表にする
		//glFrontFace(GL_CW); LN_CHECK_GLERROR();
	}

	// 合成方法
	if (newState.Blend != m_currentRenderState.Blend || reset)
	{
		switch (newState.Blend)
		{
		case BlendMode_Normal:
			glDisable(GL_BLEND);                  LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_ADD);         LN_CHECK_GLERROR();
			glBlendFunc(GL_ONE, GL_ZERO);         LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.5f );        LN_CHECK_GLERROR();
			break;
		case BlendMode_Alpha:
			glEnable(GL_BLEND);                                   LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_ADD);                         LN_CHECK_GLERROR();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.01f );                       LN_CHECK_GLERROR();
			break;
		case BlendMode_Add:
			glEnable(GL_BLEND);                   LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_ADD);         LN_CHECK_GLERROR();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);    LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.0f );        LN_CHECK_GLERROR();
			break;
		case BlendMode_AddAlphaDisable:
			glEnable(GL_BLEND);                   LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_ADD);         LN_CHECK_GLERROR();
			glBlendFunc(GL_ONE, GL_ONE);          LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.0f );        LN_CHECK_GLERROR();
			break;
		case BlendMode_Sub:
			glEnable(GL_BLEND);                           LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);    LN_CHECK_GLERROR();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);            LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			break;
		case BlendMode_SubAlphaDisable:
			glEnable(GL_BLEND);                           LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);    LN_CHECK_GLERROR();
			glBlendFunc(GL_ONE, GL_ONE);                  LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			break;
		case BlendMode_Mul:
			glEnable(GL_BLEND);                           LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_ADD);                 LN_CHECK_GLERROR();
			glBlendFunc(GL_BLEND_SRC_RGB, GL_ZERO);       LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			break;
		case BlendMode_Screen:
			glEnable(GL_BLEND);                           LN_CHECK_GLERROR();
			glBlendEquation(GL_FUNC_ADD);                 LN_CHECK_GLERROR();
			glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_ALPHA);  LN_CHECK_GLERROR();
			//glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			break;
			// http://memo.devjam.net/clip/538
			// http://d.hatena.ne.jp/melpon/20070824/p1
		}
	}

	// カリング
	if (newState.Culling != m_currentRenderState.Culling || reset)
	{
		switch (newState.Culling)
		{
		case CullingMode_None:
			glDisable(GL_CULL_FACE); LN_CHECK_GLERROR();
			break;
		case CullingMode_Front:
			glEnable(GL_CULL_FACE); LN_CHECK_GLERROR();
			glCullFace(GL_FRONT); LN_CHECK_GLERROR();
			break;
		case CullingMode_Back:
			glEnable(GL_CULL_FACE); LN_CHECK_GLERROR();
			glCullFace(GL_BACK); LN_CHECK_GLERROR();
			break;
		}
	}

	// 塗りつぶし方法
	if (newState.Fill != m_currentRenderState.Fill || reset)
	{
		const GLenum tb[] = { GL_FILL, GL_LINE, GL_POINT };
		glPolygonMode(GL_FRONT_AND_BACK, tb[newState.Fill]); LN_CHECK_GLERROR();
		//glPolygonMode(GL_FRONT, GL_FILL) // 表は塗りつぶす
		//glPolygonMode(GL_BACK, GL_LINE)  // 裏は線
	}

	// アルファテスト
	if (newState.AlphaTest != m_currentRenderState.AlphaTest || reset)
	{
		if (newState.AlphaTest) {
			glEnable(GL_ALPHA_TEST); LN_CHECK_GLERROR();
		}
		else {
			glDisable(GL_ALPHA_TEST); LN_CHECK_GLERROR();
		}
	}

	m_currentRenderState = newState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::UpdateDepthStencilState(const DepthStencilState& newState, bool reset)
{
	GLenum cmpFuncTable[] =
	{
		GL_NEVER,		// Never
		GL_LESS,		// Less
		GL_LEQUAL,		// LessEqual
		GL_GREATER,		// Greater
		GL_GEQUAL,		// GreaterEqual
		GL_EQUAL,		// Equal
		GL_NOTEQUAL,	// NotEqual
		GL_ALWAYS,		// Always
	};

	// 深度テスト
	if (newState.DepthTestEnabled != m_currentDepthStencilState.DepthTestEnabled || reset)
	{
		if (newState.DepthTestEnabled) {
			glEnable(GL_DEPTH_TEST); LN_CHECK_GLERROR();
		}
		else {
			glDisable(GL_DEPTH_TEST); LN_CHECK_GLERROR();
		}
	}
	// 深度書き込み
	if (newState.DepthWriteEnabled != m_currentDepthStencilState.DepthWriteEnabled || reset)
	{
		glDepthMask(newState.DepthWriteEnabled ? GL_TRUE : GL_FALSE); LN_CHECK_GLERROR();
	}
	// 深度比較関数
	if (newState.DepthTestFunc != m_currentDepthStencilState.DepthTestFunc || reset)
	{
		glDepthFunc(cmpFuncTable[newState.DepthTestFunc]); LN_CHECK_GLERROR();
	}


	// ステンシルテスト有無
	if (newState.StencilEnabled != m_currentDepthStencilState.StencilEnabled || reset)
	{
		if (newState.StencilEnabled) {
			glEnable(GL_STENCIL_TEST); LN_CHECK_GLERROR();
		}
		else {
			glDisable(GL_STENCIL_TEST); LN_CHECK_GLERROR();
		}
	}

	// ステンシルテスト比較関数・ステンシルテスト参照値
	if (newState.StencilFunc != m_currentDepthStencilState.StencilFunc ||
		newState.StencilReferenceValue != m_currentDepthStencilState.StencilReferenceValue ||
		reset)
	{
		glStencilFunc(cmpFuncTable[newState.StencilFunc], newState.StencilReferenceValue, 0xFFFFFFFF); LN_CHECK_GLERROR();
	}

	// ステンシルテスト処理
	GLenum stencilOpTable[] = { GL_KEEP, GL_REPLACE };
	if (newState.StencilFailOp != m_currentDepthStencilState.StencilFailOp || 
		newState.StencilDepthFailOp != m_currentDepthStencilState.StencilDepthFailOp ||
		newState.StencilPassOp != m_currentDepthStencilState.StencilPassOp ||
		reset)
	{
		glStencilOp(stencilOpTable[newState.StencilFailOp], stencilOpTable[newState.StencilDepthFailOp], stencilOpTable[newState.StencilPassOp]); LN_CHECK_GLERROR();
	}

	m_currentDepthStencilState = newState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::UpdateFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer); LN_CHECK_GLERROR();
    
    if (m_modifiedFrameBuffer)
    {
        // カラーバッファ
		for (int i = 0; i < MaxMultiRenderTargets; ++i)
		{
			// マルチターゲットの参考:http://ramemiso.hateblo.jp/entry/2013/10/20/001909
			if (m_currentRenderTargets[i] != NULL)
			{
				// カラーバッファをセットする
				glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
					m_currentRenderTargets[i]->GetGLTexture(), 0);
				LN_CHECK_GLERROR();
			}
			else
			{
				// カラーバッファを取り外す
				glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
					0, 0);
				LN_CHECK_GLERROR();
			}
		}
        
		// 深度バッファ
		if (m_currentDepthBuffer != NULL)
        {
			// 深度バッファをセットする
			glFramebufferRenderbuffer(
				GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,//GL_DEPTH_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, m_currentDepthBuffer->GetGLTexture());
			LN_CHECK_GLERROR();
		}
		else
		{
			// 深度バッファを取り外す
			glFramebufferRenderbuffer(
				GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, 0);
			LN_CHECK_GLERROR();
		}
		//GL_FRAMEBUFFER_COMPLETE
		//int t = glCheckFramebufferStatus(GL_FRAMEBUFFER);// != GL_FRAMEBUFFER_COMPLETE)
		//printf("");
	}
	m_modifiedFrameBuffer = false;
    
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::UpdateVAO()
{
	// VAO はコンテキスト間で共有できないため、描画スレッド上で作る必要がある。
	// なお、メインスレッドの MainContext で作った後、描画スレッドの RenderingContext に glBindVertexArray すると、
	// GL_INVALID_OPERATION が発生する。
	// http://stackoverflow.com/questions/18492878/crash-on-vaos-loaded-from-non-main-thread
	if (m_vertexArray == 0) {
		glGenVertexArrays(1, &m_vertexArray);
	}

	// GL 3.2 Core からは VertexArrayObject (VAO) が必須となった
	glBindVertexArray(m_vertexArray); LN_CHECK_GLERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_currentVertexBuffer->GetGLVertexBuffer()); LN_CHECK_GLERROR();

	if (m_currentIndexBuffer != NULL) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_currentIndexBuffer->GetIndexBufferObject()); LN_CHECK_GLERROR();
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); LN_CHECK_GLERROR();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::UpdateVertexAttribPointer()
{
	if (m_currentShaderPass == nullptr) return;

	// シェーダの頂点属性の更新
	const Array<LNGLVertexElement>& elements = m_currentVertexBuffer->GetVertexElements();
	LN_FOREACH(const LNGLVertexElement& elm, elements)
	{
		int index = m_currentShaderPass->GetUsageAttributeIndex(elm.Usage, elm.UsageIndex);
		if (index == -1) {
			continue;	// elm に対応するセマンティクスをもつ attribute 変数は見つからなかった
		}

		glVertexAttribPointer(
			index,
			elm.Size,
			elm.Type,
			elm.Normalized,
			elm.Stride,
			(void*)(elm.ByteOffset));
		LN_CHECK_GLERROR();

		glEnableVertexAttribArray(index);
		LN_CHECK_GLERROR();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLRenderer::GetPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount)
{
	switch (primitive)
	{
	case PrimitiveType_TriangleList:
		*gl_prim = GL_TRIANGLES;
		*vertexCount = primitiveCount * 3;
		break;
	case PrimitiveType_TriangleStrip:
		*gl_prim = GL_TRIANGLE_STRIP;
		*vertexCount = 2 + primitiveCount;
		break;
	case PrimitiveType_LineList:
		*gl_prim = GL_LINES;
		*vertexCount = primitiveCount * 2;
		break;
	case PrimitiveType_LineStrip:
		*gl_prim = GL_LINE_STRIP;
		*vertexCount = 1 + primitiveCount;
		break;
	case PrimitiveType_PointList:
		*gl_prim = GL_POINTS;
		*vertexCount = primitiveCount;
		break;
	default:
		LN_THROW(0, ArgumentException);
		break;
	}
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
