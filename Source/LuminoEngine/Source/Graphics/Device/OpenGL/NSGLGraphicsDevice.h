
#pragma once
#if defined(__OBJC__)
#import <Cocoa/cocoa.h>
#else
#include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#endif

#include "GLGraphicsDevice.h"
#include "GLSwapChain.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class NSGLGraphicsDevice;

// NS 固有の GLContext の実装
class NSGLContext
	: public GLContext
{
public:
	NSGLContext();
	virtual ~NSGLContext();
	void Initialize(NSGLGraphicsDevice* device, PlatformWindow* window, NSGLContext* parentContext);
	id getContext() const { return m_context; }

	// GLContext interface
	virtual void swapBuffers() override;

private:
    id	m_pixelFormat;
    id	m_context;
};

// NSGL 固有の GLGraphicsDevice の実装
class NSGLGraphicsDevice
	: public GLGraphicsDevice
{
public:
	NSGLGraphicsDevice();
	~NSGLGraphicsDevice();
	void Initialize(const ConfigData& configData);

	// GLGraphicsDevice interface
	virtual void makeCurrentContext(GLContext* context) override;
	virtual GLContext* getMainContext() override;
	virtual GLContext* getMainRenderingContext() override;

	// IGraphicsDevice interface
	virtual ISwapChain* getDefaultSwapChain() override;
	virtual ISwapChain* createSwapChain(PlatformWindow* window) override;
    
    void* GetProcAddress(const char* procname);
    
private:
    CFBundleRef     m_framework;
	GLSwapChain*	m_defaultSwapChain;
	NSGLContext*	m_mainContext;
	NSGLContext*	m_mainRenderingContext;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
