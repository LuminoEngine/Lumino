
#include "../../../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include "NSGLGraphicsDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// NSGLContext
//=============================================================================

//-----------------------------------------------------------------------------
NSGLContext::NSGLContext()
	: m_pixelFormat(nil)
	, m_context(nil)
{
}

//-----------------------------------------------------------------------------
NSGLContext::~NSGLContext()
{
    [m_pixelFormat release];
    m_pixelFormat = nil;

    [m_context release];
    m_context = nil;
}

//-----------------------------------------------------------------------------
void NSGLContext::Initialize(NSGLGraphicsDevice* device, PlatformWindow* window, NSGLContext* parentContext)
{
#define DONT_CARE -1
	int major = device->GetOpenGLMajorVersio();
	int minor = device->GetOpenGLMinorVersio();
	bool forward = true;
	int redBits = 8;
	int greenBits = 8;
	int blueBits = 8;
	int alphaBits = 8;
	int depthBits = 24;
	int stencilBits = 8;
	int samples = 0;
    int auxBuffers = DONT_CARE;
	int accumRedBits = DONT_CARE;
	int accumGreenBits = DONT_CARE;
	int accumBlueBits = DONT_CARE;
	int accumAlphaBits = DONT_CARE;

    unsigned int attributeCount = 0;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    if (major == 3 && minor < 2)
    {
		LN_THROW(0, InvalidOperationException, "NSGL: The targeted version of OS X does not support OpenGL 3.0 or 3.1");
    }

    if (major > 2)
    {
        if (!forward)
        {
			LN_THROW(0, InvalidOperationException, "NSGL: The targeted version of OS X only supports forward-compatible contexts for OpenGL 3.2 and above");
        }

		// TODO:
        //if (CORE_PROFILEではない)
        //{
		//	LN_THROW(0, InvalidOperationException, "NSGL: The targeted version of OS X only supports core profile contexts for OpenGL 3.2 and above");
        //}
    }
#else
    // 3.0 以上は NG
    if (major > 2)
    {
		LN_THROW(0, InvalidOperationException, "NSGL: The targeted version of OS X does not support OpenGL version 3.0 or above");
        return GL_FALSE;
    }
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/

    // Context robustness modes (GL_KHR_robustness) are not yet supported on
    // OS X but are not a hard constraint, so ignore and continue

    // Context release behaviors (GL_KHR_context_flush_control) are not yet
    // supported on OS X but are not a hard constraint, so ignore and continue

#define ADD_ATTR(x) { attributes[attributeCount++] = x; }
#define ADD_ATTR2(x, y) { ADD_ATTR(x); ADD_ATTR(y); }


    // Arbitrary array size here
    NSOpenGLPixelFormatAttribute attributes[40];

    ADD_ATTR(NSOpenGLPFAAccelerated);
    ADD_ATTR(NSOpenGLPFAClosestPolicy);

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 101000
    if (major >= 4)
    {
        ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core);
    }
    else
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
    if (major >= 3)
    {
        ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
    }
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/

    if (major <= 2)
    {
        if (auxBuffers != DONT_CARE)
            ADD_ATTR2(NSOpenGLPFAAuxBuffers, auxBuffers);

        if (accumRedBits != DONT_CARE &&
            accumGreenBits != DONT_CARE &&
            accumBlueBits != DONT_CARE &&
            accumAlphaBits != DONT_CARE)
        {
            const int accumBits = accumRedBits +
                                  accumGreenBits +
                                  accumBlueBits +
                                  accumAlphaBits;
            ADD_ATTR2(NSOpenGLPFAAccumSize, accumBits);
        }
    }

    if (redBits != DONT_CARE &&
        greenBits != DONT_CARE &&
        blueBits != DONT_CARE)
    {
        int colorBits = redBits +
                        greenBits +
                        blueBits;

        // OS X needs non-zero color size, so set reasonable values
        if (colorBits == 0)
            colorBits = 24;
        else if (colorBits < 15)
            colorBits = 15;

        ADD_ATTR2(NSOpenGLPFAColorSize, colorBits);
    }

    if (alphaBits != DONT_CARE)
        ADD_ATTR2(NSOpenGLPFAAlphaSize, alphaBits);

    if (depthBits != DONT_CARE)
        ADD_ATTR2(NSOpenGLPFADepthSize, depthBits);

    if (stencilBits != DONT_CARE)
        ADD_ATTR2(NSOpenGLPFAStencilSize, stencilBits);

    ADD_ATTR(NSOpenGLPFAStereo);
    ADD_ATTR(NSOpenGLPFADoubleBuffer);

	// マルチサンプリングの設定
    if (samples != DONT_CARE)
    {
        if (samples == 0)
        {
            ADD_ATTR2(NSOpenGLPFASampleBuffers, 0);
        }
        else
        {
            ADD_ATTR2(NSOpenGLPFASampleBuffers, 1);
            ADD_ATTR2(NSOpenGLPFASamples, samples);		// 4 とかが普通？
        }
    }

    // NOTE: All NSOpenGLPixelFormats on the relevant cards support sRGB
    //       framebuffer, so there's no need (and no way) to request it

    ADD_ATTR(0);

#undef ADD_ATTR
#undef ADD_ATTR2

    m_pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    if (m_pixelFormat == nil)
    {
		LN_THROW(0, InvalidOperationException, "NSGL: Failed to find a suitable pixel format");
    }

    NSOpenGLContext* share = NULL;
    if (parentContext != nullptr)
	{
        share = parentContext->m_context;
	}

    m_context = [[NSOpenGLContext alloc] initWithFormat:m_pixelFormat shareContext:share];
    if (m_context == nil)
    {
		LN_THROW(0, InvalidOperationException, "NSGL: Failed to create OpenGL context");
    }
}

//-----------------------------------------------------------------------------
void NSGLContext::SwapBuffers()
{
	[m_context flushBuffer];
}


//=============================================================================
// NSGLGraphicsDevice
//=============================================================================

//-----------------------------------------------------------------------------
NSGLGraphicsDevice::NSGLGraphicsDevice()
	: m_defaultSwapChain(nullptr)
	, m_mainContext(nullptr)
	, m_mainRenderingContext(nullptr)
{
}

//-----------------------------------------------------------------------------
NSGLGraphicsDevice::~NSGLGraphicsDevice()
{
	LN_SAFE_RELEASE(m_defaultSwapChain);
	LN_SAFE_RELEASE(m_mainContext);
	LN_SAFE_RELEASE(m_mainRenderingContext);
}

//-----------------------------------------------------------------------------
void NSGLGraphicsDevice::Initialize(const ConfigData& configData)
{
	GLGraphicsDevice::Initialize(configData);

	// レンダリングスレッド以外から OpenGL の API へアクセスする用
	m_mainContext = LN_NEW NSGLContext();
	m_mainContext->Initialize(this, configData.MainWindow, nullptr);

	// レンダリングスレッド用
	m_mainRenderingContext = LN_NEW NSGLContext();
	m_mainRenderingContext->Initialize(this, configData.MainWindow, m_mainContext);

	// m_defaultSwapChain の初期化でシェーダとか作るので先にアクティブにしておく
	// TODO: 共通処理にしていいかも？
	MakeCurrentContext(m_mainContext);

	m_defaultSwapChain = LN_NEW GLSwapChain();
	RefPtr<NSGLContext> context(LN_NEW NSGLContext(), false);
	context->Initialize(this, m_mainWindow, m_mainContext);
	m_defaultSwapChain->Initialize(this, context, m_mainWindow);

	// Renderer の初期化でもオブジェクトを生成したりするのでメインのコンテキストがアクティブになっているときに初期化する必要がある
	m_renderer = LN_NEW GLRenderer();
	m_renderer->Initialize();

	MakeCurrentContext(nullptr);
}

//-----------------------------------------------------------------------------
void NSGLGraphicsDevice::MakeCurrentContext(GLContext* context)
{
    NSGLContext* c = static_cast<NSGLContext*>(context);
    if (c != nullptr)
        [c->GetContext() makeCurrentContext];
    else
        [NSOpenGLContext clearCurrentContext];
}

//-----------------------------------------------------------------------------
GLContext* NSGLGraphicsDevice::GetMainContext()
{
	return m_mainContext;
}

//-----------------------------------------------------------------------------
GLContext* NSGLGraphicsDevice::GetMainRenderingContext()
{
	return m_mainRenderingContext;
}

//-----------------------------------------------------------------------------
ISwapChain* NSGLGraphicsDevice::GetDefaultSwapChain()
{
	return m_defaultSwapChain;
}

//-----------------------------------------------------------------------------
ISwapChain* NSGLGraphicsDevice::CreateSwapChain(PlatformWindow* window)
{
	RefPtr<GLSwapChain> obj(LN_NEW GLSwapChain(), false);
	RefPtr<NSGLContext> context(LN_NEW NSGLContext(), false);
	context->Initialize(this, window, m_mainContext);
	obj->Initialize(this, context, window);
	AddDeviceResource(obj);
	obj.SafeAddRef();
	return obj;
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
