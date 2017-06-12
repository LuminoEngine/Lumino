
#pragma once 
#include "GLCommon.h"
#include "GLTexture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class GLGraphicsDevice;
class GLContext;
class GLRenderer;

// OpenGL 用の ISwapChain の実装
class GLSwapChain
	: public ISwapChain
{
public:
	GLSwapChain();
	virtual ~GLSwapChain();
	void initialize(GLGraphicsDevice* device, GLContext* context, PlatformWindow* window);

public:
	virtual ITexture* GetBackBuffer() override { return m_renderTarget; }
	virtual void resize(const SizeI& size) override;
	virtual void Present(ITexture* colorBuffer) override;
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;

protected:
	void InternalPresent(ITexture* colorBuffer, GLRenderer* renderer/*GLuint rendererVAO*/);

private:
	GLGraphicsDevice*		m_device;
	GLContext*				m_context;
	PlatformWindow*			m_window;
	SizeI					m_backBufferSize;
	GLRenderTargetTexture*	m_renderTarget;

	GLuint					m_shaderProgram;
	GLint					m_positionLoc;
	GLint					m_texCoordLoc;
	GLint					m_textureLoc;
	GLuint					m_vertexBuffer;
	GLuint					m_vertexArray;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
