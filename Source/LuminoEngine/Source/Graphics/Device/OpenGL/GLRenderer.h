
#pragma once 
#include <Lumino/Graphics/Graphics.h>
#include "../GraphicsDriverInterface.h"
#include "GLCommon.h"
#include "GLTexture.h"

LN_NAMESPACE_BEGIN
namespace Driver
{
class GLGraphicsDevice;
class GLVertexBuffer;
class GLIndexBuffer;
class GLShaderPass;

// OpenGL 用の IRenderer の実装
class GLRenderer
	: public IRenderer
{
public:
	GLRenderer();
	virtual ~GLRenderer();

public:
	void activate();
	void Deactivate();

	/// ShaderPass::begin から呼ばれる
	void setCurrentShaderPass(GLShaderPass* pass) { m_currentShaderPass = pass; }

	GLuint getVertexArrayObject();

protected:
	virtual void onEnterRenderState() override;
	virtual void onLeaveRenderState() override;
	virtual void onBeginRendering() override;
	virtual void onEndRendering() override;
	virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, ITexture* depthBuffer) override;
	virtual	void onUpdateRenderState(const RenderState& newState, const RenderState& oldState, bool reset) override;
	virtual	void onUpdateDepthStencilState(const DepthStencilState& newState, const DepthStencilState& oldState, bool reset) override;
	virtual void onUpdateViewport(const RectI& viewport) override;
	virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, const List<Ref<IVertexBuffer>>& vertexBuufers, IIndexBuffer* indexBuffer) override;
	virtual void onClear(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	virtual void onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) override;
	virtual void onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) override;

private:
	void updateVAO();
	void updateVertexAttribPointer();
	void getPrimitiveInfo(PrimitiveType primitive, int primitiveCount, GLenum* gl_prim, int* vertexCount);

	RectI					m_currentViewportRect;
	GLVertexBuffer*			m_currentVertexBuffer;
	GLIndexBuffer*			m_currentIndexBuffer;
	GLShaderPass*			m_currentShaderPass;
	GLuint					m_vertexArray;
	GLuint					m_framebuffer;
};

} // namespace Driver
LN_NAMESPACE_END
