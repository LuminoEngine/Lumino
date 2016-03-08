
#pragma once 

#include "GLGraphicsDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
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
	virtual ISwapChain* GetDefaultSwapChain() override;
	virtual ISwapChain* CreateSwapChain(PlatformWindow* window) override;
	virtual void AttachRenderingThread() override;
	virtual void DetachRenderingThread() override;
	virtual void OnBeginAccessContext() override;
	virtual void OnEndAccessContext() override;

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

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
