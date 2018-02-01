
#include "../../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLShader.h"
#include "GLRenderer.h"

LN_NAMESPACE_BEGIN
namespace Driver
{

//==============================================================================
// GLRenderer
//==============================================================================

//------------------------------------------------------------------------------
GLRenderer::GLRenderer()
	: m_currentVertexBuffer(NULL)
	, m_currentIndexBuffer(NULL)
	, m_currentShaderPass(NULL)
	, m_vertexArray(0)
	, m_framebuffer(0)
{
}

//------------------------------------------------------------------------------
GLRenderer::~GLRenderer()
{
}

//------------------------------------------------------------------------------
void GLRenderer::activate()
{
	// VAO はコンテキスト間で共有できないため、描画スレッド上で作る必要がある。
	// なお、メインスレッドの MainContext で作った後、描画スレッドの Context に glBindVertexArray すると、
	// GL_INVALID_OPERATION が発生する。
	// http://stackoverflow.com/questions/18492878/crash-on-vaos-loaded-from-non-main-thread
	//
	// FBO や他にも共有できないものがある
	// http://stackoverflow.com/questions/16782279/how-to-render-to-an-fbo-on-a-shared-context

	if (LN_REQUIRE(m_vertexArray == 0)) return;
	if (LN_REQUIRE(m_framebuffer == 0)) return;

	glGenVertexArrays(1, &m_vertexArray);
	glGenFramebuffers(1, &m_framebuffer);
}

//------------------------------------------------------------------------------
void GLRenderer::Deactivate()
{
	if (m_vertexArray != 0)
	{
		glDeleteVertexArrays(1, &m_vertexArray);
		m_vertexArray = 0;
	}
	if (m_framebuffer != 0)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}

	LN_SAFE_RELEASE(m_currentVertexBuffer);
	LN_SAFE_RELEASE(m_currentIndexBuffer);
}

//------------------------------------------------------------------------------
GLuint GLRenderer::getVertexArrayObject()
{
	
	return m_vertexArray;
}

////------------------------------------------------------------------------------
//void GLRenderer::setRenderTarget(int index, ITexture* texture)
//{
//	if (texture != nullptr) {
//		LN_THROW((texture->getTextureType() == TextureType_RenderTarget), ArgumentException);
//	}
//	LN_REFOBJ_SET(m_currentRenderTargets[index], static_cast<GLRenderTargetTexture*>(texture));
//	m_modifiedFrameBuffer = true;
//}
//
////------------------------------------------------------------------------------
//ITexture* GLRenderer::getRenderTarget(int index)
//{
//	return m_currentRenderTargets[index];
//}
//
////------------------------------------------------------------------------------
//void GLRenderer::setDepthBuffer(ITexture* texture)
//{
//	if (texture != nullptr)
//	{
//		LN_THROW((texture->getTextureType() == TextureType_DepthBuffer), ArgumentException);
//	}
//	LN_REFOBJ_SET(m_currentDepthBuffer, static_cast<GLDepthBuffer*>(texture));
//	m_modifiedFrameBuffer = true;
//}

//------------------------------------------------------------------------------
//ITexture* GLRenderer::getDepthBuffer()
//{
//	LN_THROW(0, NotImplementedException);
//	return 0;
//}

////------------------------------------------------------------------------------
//void GLRenderer::setViewport(const RectI& rect)
//{
//	const SizeI& scr = m_currentRenderTargets[0]->GetSize();
//	glViewport(rect.x, scr.height - (rect.y + rect.height), rect.width, rect.height);
//	//LN_THROW(0, NotImplementedException);
//}

//------------------------------------------------------------------------------
void GLRenderer::onEnterRenderState()
{
}

//------------------------------------------------------------------------------
void GLRenderer::onLeaveRenderState()
{
}

//------------------------------------------------------------------------------
void GLRenderer::onBeginRendering()
{
}

//------------------------------------------------------------------------------
void GLRenderer::onEndRendering()
{
}

//------------------------------------------------------------------------------
void GLRenderer::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, ITexture* depthBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	if (LN_ENSURE_GLERROR()) return;

	// カラーバッファ
	for (int i = 0; i < renderTargetsCount; ++i)
	{
		// マルチターゲットの参考:http://ramemiso.hateblo.jp/entry/2013/10/20/001909
		if (renderTargets[i] != NULL)
		{
			// カラーバッファをセットする
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
				static_cast<GLTextureBase*>(renderTargets[i])->getGLTexture(), 0);
			if (LN_ENSURE_GLERROR()) return;
		}
		else
		{
			// カラーバッファを取り外す
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
				0, 0);
			if (LN_ENSURE_GLERROR()) return;
		}
	}

	// 深度バッファ
	if (depthBuffer != NULL)
	{
		// 深度バッファをセットする
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,//GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, static_cast<GLTextureBase*>(depthBuffer)->getGLTexture());
		if (LN_ENSURE_GLERROR()) return;
	}
	else
	{
		// 深度バッファを取り外す
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, 0);
		if (LN_ENSURE_GLERROR()) return;
	}
}

//------------------------------------------------------------------------------
void GLRenderer::onUpdateRenderState(const RenderState& newState, const RenderState& oldState, bool reset)
{
	/* TODO: ステート保存 https://www.khronos.org/opengles/sdk/docs/man3/html/glBlendFuncSeparate.xhtml
	Associated Gets
glGet with argument GL_BLEND_SRC_RGB

glGet with argument GL_BLEND_SRC_ALPHA

glGet with argument GL_BLEND_DST_RGB

glGet with argument GL_BLEND_DST_ALPHA

glIsEnabled with argument GL_BLEND
	*/
	// Blending
	GLenum  blendOpTable[] =	// glBlendEquation
	{
		GL_FUNC_ADD,
		GL_FUNC_SUBTRACT,
		GL_FUNC_REVERSE_SUBTRACT,
		GL_MIN,
		GL_MAX,
	};
	GLenum blendFactorTable[] =	// glBlendFuncSeparate
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA
	};
	if (newState.alphaBlendEnabled != oldState.alphaBlendEnabled || reset)
	{
		if (newState.alphaBlendEnabled) {
			glEnable(GL_BLEND);
			if (LN_ENSURE_GLERROR()) return;
		}
		else {
			glDisable(GL_BLEND);
			if (LN_ENSURE_GLERROR()) return;
		}
	}
	if (newState.blendOp != oldState.blendOp || reset)
	{
		glBlendEquation(blendOpTable[(int)newState.blendOp]);
		if (LN_ENSURE_GLERROR()) return;
	}
	if (newState.sourceBlend != oldState.sourceBlend || newState.destinationBlend != oldState.destinationBlend || reset)
	{
		glBlendFuncSeparate(
			blendFactorTable[(int)newState.sourceBlend],
			blendFactorTable[(int)newState.destinationBlend],
			blendFactorTable[(int)newState.sourceBlend],
			blendFactorTable[(int)newState.destinationBlend]);
		if (LN_ENSURE_GLERROR()) return;
	}
	
	// カリング
	if (newState.Culling != oldState.Culling || reset)
	{
		switch (newState.Culling)
		{
		case CullingMode::None:
			glDisable(GL_CULL_FACE);
			if (LN_ENSURE_GLERROR()) return;
			break;
		case CullingMode::Front:
			glEnable(GL_CULL_FACE);
			if (LN_ENSURE_GLERROR()) return;
			glCullFace(GL_BACK);
			if (LN_ENSURE_GLERROR()) return;
			break;
		case CullingMode::Back:
			glEnable(GL_CULL_FACE);
			if (LN_ENSURE_GLERROR()) return;
			glCullFace(GL_FRONT);
			if (LN_ENSURE_GLERROR()) return;
			break;
		}
	}

	// 塗りつぶし方法
	if (newState.Fill != oldState.Fill || reset)
	{
		const GLenum tb[] = { GL_FILL, GL_LINE, GL_POINT };
		glPolygonMode(GL_FRONT_AND_BACK, tb[newState.Fill]);
		if (LN_ENSURE_GLERROR()) return;

		//glPolygonMode(GL_FRONT, GL_FILL) // 表は塗りつぶす
		//glPolygonMode(GL_BACK, GL_LINE)  // 裏は線
	}

	// アルファテスト
	if (newState.AlphaTest != oldState.AlphaTest || reset)
	{
		if (newState.AlphaTest) {
			glEnable(GL_ALPHA_TEST);
			if (LN_ENSURE_GLERROR()) return;
		}
		else {
			glDisable(GL_ALPHA_TEST);
			if (LN_ENSURE_GLERROR()) return;
		}
	}
}

//------------------------------------------------------------------------------
void GLRenderer::onUpdateDepthStencilState(const DepthStencilState& newState, const DepthStencilState& oldState, bool reset)
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
	if (newState.DepthTestEnabled != oldState.DepthTestEnabled || reset)
	{
		if (newState.DepthTestEnabled) {
			glEnable(GL_DEPTH_TEST);
			if (LN_ENSURE_GLERROR()) return;
		}
		else {
			glDisable(GL_DEPTH_TEST);
			if (LN_ENSURE_GLERROR()) return;
		}
	}
	// 深度書き込み
	if (newState.DepthWriteEnabled != oldState.DepthWriteEnabled || reset)
	{
		glDepthMask(newState.DepthWriteEnabled ? GL_TRUE : GL_FALSE);
		if (LN_ENSURE_GLERROR()) return;
	}
	// 深度比較関数
	if (newState.DepthTestFunc != oldState.DepthTestFunc || reset)
	{
		glDepthFunc(cmpFuncTable[newState.DepthTestFunc]);
		if (LN_ENSURE_GLERROR()) return;
	}


	// ステンシルテスト有無
	if (newState.StencilEnabled != oldState.StencilEnabled || reset)
	{
		if (newState.StencilEnabled) {
			glEnable(GL_STENCIL_TEST);
			if (LN_ENSURE_GLERROR()) return;
		}
		else {
			glDisable(GL_STENCIL_TEST);
			if (LN_ENSURE_GLERROR()) return;
		}
	}

	// ステンシルテスト比較関数・ステンシルテスト参照値
	if (newState.StencilFunc != oldState.StencilFunc ||
		newState.StencilReferenceValue != oldState.StencilReferenceValue ||
		reset)
	{
		glStencilFunc(cmpFuncTable[newState.StencilFunc], newState.StencilReferenceValue, 0xFFFFFFFF);
		if (LN_ENSURE_GLERROR()) return;
	}

	// ステンシルテスト処理
	GLenum stencilOpTable[] = { GL_KEEP, GL_REPLACE };
	if (newState.StencilFailOp != oldState.StencilFailOp ||
		newState.StencilDepthFailOp != oldState.StencilDepthFailOp ||
		newState.StencilPassOp != oldState.StencilPassOp ||
		reset)
	{
		glStencilOp(stencilOpTable[newState.StencilFailOp], stencilOpTable[newState.StencilDepthFailOp], stencilOpTable[newState.StencilPassOp]);
		if (LN_ENSURE_GLERROR()) return;
	}
}

void GLRenderer::onUpdateViewport(const RectI& viewport)
{
	const SizeI& scr =getRenderTarget(0)->getSize();
	glViewport(viewport.x, scr.height - (viewport.y + viewport.height), viewport.width, viewport.height);
	if (LN_ENSURE_GLERROR()) return;
}

//------------------------------------------------------------------------------
void GLRenderer::onUpdatePrimitiveData(IVertexDeclaration* decls, const List<Ref<IVertexBuffer>>& vertexBuufers, IIndexBuffer* indexBuffer)
{
	if (vertexBuufers.isEmpty())
		m_currentVertexBuffer = nullptr;
	else
		LN_REFOBJ_SET(m_currentVertexBuffer, static_cast<GLVertexBuffer*>(vertexBuufers[0].get()));
	LN_REFOBJ_SET(m_currentIndexBuffer, static_cast<GLIndexBuffer*>(indexBuffer));
}

//------------------------------------------------------------------------------
void GLRenderer::onClear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	glDepthMask(GL_TRUE);   // これがないと Depth が正常にクリアされない
	if (LN_ENSURE_GLERROR()) return;

	glClearColor(color.r, color.g, color.b, color.a);
	if (LN_ENSURE_GLERROR()) return;

	glClearDepth(z);
	if (LN_ENSURE_GLERROR()) return;

	glClearStencil(0);
	if (LN_ENSURE_GLERROR()) return;

	/*
	GLenum fs = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if (fs == GL_FRAMEBUFFER_COMPLETE)
	{
	printf("GL_FRAMEBUFFER_COMPLETE\n");
	}
	if (fs == GL_FRAMEBUFFER_UNSUPPORTED)
	{
	printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
	}
	if (fs == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	{
	printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
	}
	*/

	// TODO: フレームバッファのサイズが０だと失敗する
	glClear(
		((flags.TestFlag(ClearFlags::Color)) ? GL_COLOR_BUFFER_BIT : 0) |
		((flags.TestFlag(ClearFlags::Depth)) ? GL_DEPTH_BUFFER_BIT : 0) |
		((flags.TestFlag(ClearFlags::Stencil)) ? GL_STENCIL_BUFFER_BIT : 0));
	if (LN_ENSURE_GLERROR()) return;
}

//------------------------------------------------------------------------------
void GLRenderer::onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	if (LN_REQUIRE(m_currentVertexBuffer)) return;

	// 描画に必要な情報を最新にする
	//UpdateRenderState(m_requestedRenderState, m_justSawReset);
	//UpdateDepthStencilState(m_requestedDepthStencilState, m_justSawReset);
	updateVAO();
	updateVertexAttribPointer();


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
	getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	// 描画
	glDrawArrays(gl_prim, startVertex, vertexCount);
	if (LN_ENSURE_GLERROR()) return;

	// 後始末 (解除)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//------------------------------------------------------------------------------
void GLRenderer::onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	if (LN_REQUIRE(m_currentVertexBuffer && m_currentIndexBuffer && m_currentShaderPass)) return;

	// 描画に必要な情報を最新にする
	//UpdateRenderState(m_requestedRenderState, m_justSawReset);
	//UpdateDepthStencilState(m_requestedDepthStencilState, m_justSawReset);
	updateVAO();
	updateVertexAttribPointer();

	// プリミティブと必要な頂点数の取得
	GLenum gl_prim;
	int vertexCount;
	getPrimitiveInfo(primitive, primitiveCount, &gl_prim, &vertexCount);

	// 引数 start end には、本来であれば0～vertexCountまでのインデックスの中の最大、最小の値を渡す。
	// http://wiki.livedoor.jp/mikk_ni3_92/d/glDrawRangeElements%A4%CB%A4%E8%A4%EB%C9%C1%B2%E8
	// ただ、全範囲を渡しても特に問題なさそうなのでこのまま。
	if (m_currentIndexBuffer->getFormat() == IndexBufferFormat_UInt16)
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
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//------------------------------------------------------------------------------
void GLRenderer::updateVAO()
{
	glBindVertexArray(m_vertexArray);
	if (LN_ENSURE_GLERROR()) return;

	glBindBuffer(GL_ARRAY_BUFFER, m_currentVertexBuffer->getGLVertexBuffer());
	if (LN_ENSURE_GLERROR()) return;

	if (m_currentIndexBuffer != NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_currentIndexBuffer->getIndexBufferObject());
		if (LN_ENSURE_GLERROR()) return;
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		if (LN_ENSURE_GLERROR()) return;
	}
}

//------------------------------------------------------------------------------
void GLRenderer::updateVertexAttribPointer()
{
	if (m_currentShaderPass == nullptr) return;

	GLVertexDeclaration* glDecls = static_cast<GLVertexDeclaration*>(getVertexDeclaration());

	// シェーダの頂点属性の更新
	const List<LNGLVertexElement>& elements = glDecls->getVertexElements();
	for (const LNGLVertexElement& elm : elements)
	{
		int index = m_currentShaderPass->getUsageAttributeIndex(elm.Usage, elm.UsageIndex);
		if (index == -1) {
			continue;	// elm に対応するセマンティクスをもつ attribute 変数は見つからなかった
		}

		glVertexAttribPointer(
			index,			// 属性インデックス。glGetAttribLocation() で得られる、attribute 変数の location。
			elm.Size,
			elm.Type,
			elm.Normalized,
			elm.Stride,
			(void*)(elm.ByteOffset));
		if (LN_ENSURE_GLERROR()) return;

		glEnableVertexAttribArray(index);
		if (LN_ENSURE_GLERROR()) return;
	}
}

//------------------------------------------------------------------------------
void GLRenderer::getPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount)
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
	case PrimitiveType_TriangleFan:
		*gl_prim = GL_TRIANGLE_FAN;
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
		LN_UNREACHABLE();
		break;
	}
}

} // namespace Driver
LN_NAMESPACE_END
