
#pragma once 

#include "../../../include/Lumino/Graphics/RenderState.h"
#include "../DeviceInterface.h"
#include "GLTexture.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class GLGraphicsDevice;
class GLVertexBuffer;
class GLIndexBuffer;
class GLShaderPass;

/// OpenGL 用の IRenderer の実装
class GLRenderer
	: public IRenderer
{
public:
	GLRenderer();
	virtual ~GLRenderer();

public:
	void Initialize();

	/// ShaderPass::Begin から呼ばれる
	void SetCurrentShaderPass(GLShaderPass* pass) { m_currentShaderPass = pass; }

	GLuint GetVertexArrayObject();

	void OnLostDevice();
	void OnResetDevice();
	void ResetRenderState() { m_justSawReset = true; }	// Present の内部描画の後に呼ばれる

public:
	virtual void Begin();
	virtual void End();
	virtual void SetRenderState(const RenderState& state);
	virtual const RenderState& GetRenderState();
	virtual void SetDepthStencilState(const DepthStencilState& state);
	virtual const DepthStencilState& GetDepthStencilState();
	virtual void SetRenderTarget(int index, ITexture* texture);
	virtual ITexture* GetRenderTarget(int index);
	virtual void SetDepthBuffer(ITexture* texture);
	virtual ITexture* GetDepthBuffer();
	virtual void SetViewport(const Rect& rect);
	virtual const Rect& GetViewport();
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer);
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer);
	virtual void Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil);
	virtual void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);
	virtual void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

private:
	void UpdateRenderState(const RenderState& newState, bool reset);
	void UpdateDepthStencilState(const DepthStencilState& newState, bool reset);
	void UpdateFrameBuffer();
	void UpdateVAO();
	void UpdateVertexAttribPointer();
	void GetPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount);

private:
	RenderState				m_requestedRenderState;
	RenderState				m_currentRenderState;
	DepthStencilState		m_requestedDepthStencilState;
	DepthStencilState		m_currentDepthStencilState;
	Rect					m_currentViewportRect;
	GLVertexBuffer*			m_currentVertexBuffer;
	GLIndexBuffer*			m_currentIndexBuffer;
	GLRenderTargetTexture*	m_currentRenderTargets[MaxMultiRenderTargets];
	GLDepthBuffer*			m_currentDepthBuffer;
	GLShaderPass*			m_currentShaderPass;
	GLuint					m_vertexArray;
	GLuint					m_framebuffer;
	bool					m_modifiedFrameBuffer;
	volatile bool			m_justSawReset;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
