#pragma once

#include <GL/gl.h>
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class GLContext;

class OpenGLDeviceContext
	: public GraphicsDeviceContext
{
public:
	OpenGLDeviceContext();
	virtual ~OpenGLDeviceContext() = default;

	void initialize();
	void dispose();

protected:
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;

private:
	Ref<GLContext> m_glContext;
};

class GLSwapChain
	: public ISwapChain
{
public:
	virtual ~GLSwapChain() = default;
};

class GLContext
	: public RefObject
{
public:
	GLContext() = default;
	virtual ~GLContext() = default;
};

} // namespace detail
} // namespace ln

