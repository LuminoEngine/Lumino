
#pragma once 

#include "GLGraphicsDevice.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class WGLSwapChain;
class WGLContext;

/// WGL 固有の GLGraphicsDevice の実装
class WGLGraphicsDevice
	: public GLGraphicsDevice
{
public:
	WGLGraphicsDevice();
	~WGLGraphicsDevice();

public:
	/// 描画を行うスレッドで初期化する必要がある
	void Initialize(const ConfigData& configData);

	/// 指定コンテキストをアクティブにする
	virtual void MakeCurrentContext(GLContext* context);

	/// メインコンテキストの取得
	virtual GLContext* GetMainContext();

	/// メイン描画用コンテキストの取得
	virtual GLContext* GetMainRenderingContext();

public:
	// override IGraphicsDevice
	virtual ISwapChain* GetDefaultSwapChain();
	virtual ISwapChain* CreateSwapChain(Platform::Window* window);
	virtual void AttachRenderingThread();
	virtual void DetachRenderingThread();

public:
	static PFNWGLGETEXTENSIONSSTRINGEXTPROC		GetExtensionsStringEXT;
	static PFNWGLGETEXTENSIONSSTRINGARBPROC		GetExtensionsStringARB;
	static PFNWGLCREATECONTEXTATTRIBSARBPROC	CreateContextAttribsARB;

private:
	int CheckPlatformExtensionSupported(const char* extension);
	bool CheckContainsExtensionString(const char* string, const GLubyte* extensions);

private:
	WGLSwapChain*	m_defaultSwapChain;
	WGLContext*		m_mainContext;
	WGLContext*		m_mainRenderingContext;
	bool			m_glInited;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
