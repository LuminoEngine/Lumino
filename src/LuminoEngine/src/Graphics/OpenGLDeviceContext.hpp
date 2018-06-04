#pragma once

#ifdef LN_GRAPHICS_OPENGLES

#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>
#include <OpenGLES/ES2/gl.h>
#else
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>
#endif

#else
#include <GL/gl.h>
#endif
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class GLContext;

/*
	Note:

	OpenGLDeviceContext はマルチスレッドレンダリングをサポートしない。
	大きな理由は、マルチスレッドでの動作がちゃんと規格化されていないから。
	プラットフォーム間での差が激しく、カバーしきれない。

	他、
	- Emscripten で使うときはそもそもスレッドが使えない。
	- GLFW で共有コンテキストを作るときは必ずウィンドウを作る必要がある。

	特に Emscripten で使うときの環境の制約が大きいかも。
	素直にシングルスレッドで動作するデバイスとして位置づけたい。

	マルチスレッドレンダリングやるなら Vulkan、DX12 などを使おう。
*/
class OpenGLDeviceContext
	: public IGraphicsDeviceContext
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
	};

	OpenGLDeviceContext();
	virtual ~OpenGLDeviceContext() = default;

	void initialize(const Settings& settings);
	virtual void dispose() override;

protected:
	virtual void onEnterMainThread() override;
	virtual void onLeaveMainThread() override;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	virtual void onPresent(ISwapChain* swapChain) override;

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

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual void makeCurrent(GLSwapChain* swapChain) = 0;
	virtual void swap(GLSwapChain* swapChain) = 0;
};

class EmptyGLContext
	: public GLContext
{
public:
	EmptyGLContext() = default;
	virtual ~EmptyGLContext() = default;

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual void makeCurrent(GLSwapChain* swapChain) override;
	virtual void swap(GLSwapChain* swapChain) override;
};

class EmptyGLSwapChain
	: public GLSwapChain
{
public:
	EmptyGLSwapChain() = default;
	virtual ~EmptyGLSwapChain() = default;
};

} // namespace detail
} // namespace ln

