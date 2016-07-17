
#pragma once 

#include "../GraphicsDriverInterface.h"
#include "GLTexture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
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
	virtual void EnterRenderState();
	virtual void LeaveRenderState();
	virtual void Begin();
	virtual void End();
	virtual void SetRenderTarget(int index, ITexture* texture);
	virtual ITexture* GetRenderTarget(int index);
	virtual void SetDepthBuffer(ITexture* texture);
	//virtual ITexture* GetDepthBuffer();
	virtual void SetViewport(const Rect& rect);
	//virtual const Rect& GetViewport();
	virtual void Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil);
	virtual void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);
	virtual void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

protected:
	virtual	void OnUpdateRenderState(const RenderState& newState, const RenderState& oldState, bool reset) override;
	virtual	void OnUpdateDepthStencilState(const DepthStencilState& newState, const DepthStencilState& oldState, bool reset) override;
	virtual void OnUpdatePrimitiveData(IVertexDeclaration* decls, const Array<RefPtr<IVertexBuffer>>& vertexBuufers, IIndexBuffer* indexBuffer) override;

	void UpdateFrameBuffer();
	void UpdateVAO();
	void UpdateVertexAttribPointer();
	void GetPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount);

private:
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

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
