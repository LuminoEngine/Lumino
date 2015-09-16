
/*
[OpenGL] FrameBufferとRenderBufferについてメモ
http://qiita.com/edo_m18/items/95483cabf50494f53bb5
*/

#include "../../../Internal.h"
#include "WGLGraphicsDevice.h"
#include "WGLSwapChain.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Device
{

//=============================================================================
// WGLSwapChain
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WGLSwapChain::WGLSwapChain()
	: m_device(NULL)
	, m_window(NULL)
	, m_context(NULL)
	, m_renderTarget(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WGLSwapChain::~WGLSwapChain()
{
	LN_SAFE_RELEASE(m_renderTarget);
	LN_SAFE_RELEASE(m_context);
	LN_SAFE_RELEASE(m_window);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLSwapChain::Create(WGLGraphicsDevice* device, Platform::Window* window, WGLContext* parentContext)
{
	m_device = device;
	LN_REFOBJ_SET(m_window, window);

	m_context = LN_NEW WGLContext(m_device, m_window, parentContext);

	// TODO: バックバッファサイズ
	m_renderTarget = LN_NEW GLRenderTargetTexture(m_window->GetSize(), TextureFormat_R8G8B8A8, 1);

	GLSwapChain::Create();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void WGLSwapChain::MakeCurrentContext()
//{
//	m_context->MakeCurrentContext();
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLSwapChain::Resize(const Size& size)
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//ITexture* WGLSwapChain::GetBackBuffer()
//{
//	return m_backBuffer;
//}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void WGLSwapChain::Begin()
//{
//	//BOOL r = wglMakeCurrent(m_hDC, m_hGLRC);
//	//LN_THROW(r, Win32Exception, ::GetLastError());
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void WGLSwapChain::End()
//{
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLSwapChain::Present(ITexture* colorBuffer)
{
	//HGLRC ff = wglGetCurrentContext();
	//m_device->MakeCurrentContext(NULL);
	try
	{
		// colorBuffer をバックバッファに描画する
		InternalPresent(colorBuffer, static_cast<GLRenderer*>(m_device->GetRenderer()));
		m_device->MakeCurrentContext(m_context);
		//printf("swap\n");
		// バックバッファをフロントバッファに転送する
		BOOL r = ::SwapBuffers(m_context->GetDC());
		LN_THROW(r, Win32Exception, ::GetLastError());
	}
	catch (...)
	{
		m_device->MakeCurrentContext(m_device->GetMainRenderingContext());
		throw;
	}
	m_device->MakeCurrentContext(m_device->GetMainRenderingContext());
	m_device->GCDeviceResource();

#if 0
	glUseProgram(0);

	//glClearColor(1, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); LN_CHECK_GLERROR();

	glEnable(GL_TEXTURE_2D); LN_CHECK_GLERROR();
	glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(colorBuffer)->GetGLTexture()); LN_CHECK_GLERROR();

	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(-1, 0.9);
	glTexCoord2f(1, 1); glVertex2f(1, 0.9);

	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(-1, -0.9);
	glTexCoord2f(1, 1); glVertex2f(1, -0.9);
	glEnd();
	glFlush(); LN_CHECK_GLERROR();
	::SwapBuffers(m_hDC);
#endif

#if 0
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D); LN_CHECK_GLERROR();
	glBindTexture(GL_TEXTURE_2D, static_cast<GLTextureBase*>(colorBuffer)->GetGLTexture()); LN_CHECK_GLERROR();
	//GL_INVALID_ENUM
	// 正方形を描く
	glBegin(GL_TRIANGLE_FAN);// LN_CHECK_GLERROR();	// glBegin～glEnd の間で glGetError は禁止 https://www.opengl.org/discussion_boards/showthread.php/163474-GL_INVALID_OPERATION-after-glEnd%28%29
	glTexCoord2f(0, 0); glVertex2d(-1.0, -1.0);
	glTexCoord2f(0, 1); glVertex2d(1.0, -1.0);
	glTexCoord2f(1, 1); glVertex2d(1.0, 1.0);
	glTexCoord2f(1, 0); glVertex2d(-1.0, 1.0);
	glEnd(); LN_CHECK_GLERROR();
	//GL_INVALID_OPERATION
	glFlush(); LN_CHECK_GLERROR();
	glDisable(GL_TEXTURE_2D); LN_CHECK_GLERROR();
	::SwapBuffers(m_hDC);
#endif
}


//=============================================================================
// WGLContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WGLContext::WGLContext(WGLGraphicsDevice* device, Platform::Window* window, WGLContext* parentContext)
	: m_hWnd(NULL)
	, m_hDC(NULL)
	, m_hGLRC(NULL)
{
	m_hWnd = Platform::PlatformSupport::GetWindowHandle(window);
	m_hDC = ::GetDC(m_hWnd);

	HGLRC share = NULL;
	if (parentContext) {
		share = parentContext->m_hGLRC;
	}

	// とりあえず今はこのフォーマット固定で。
	/*
	Note:
	PFD_DEPTH_DONTCARE を指定すると Create が失敗する。サポート外？
	depth=24, stencil=8 でも、glGetFramebufferAttachmentParameteriv で 24, 0 となった。
	depth や stencil に 0 を指定しても、glGetFramebufferAttachmentParameteriv で 24, 0 となった。
	depth=32, stencil=0 でも、glGetFramebufferAttachmentParameteriv で 24, 0 となった。
	*/
	static PIXELFORMATDESCRIPTOR   pformat = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER/* | PFD_DEPTH_DONTCARE*/,
		PFD_TYPE_RGBA,
		32,     // color
		0, 0,   // R
		0, 0,   // G
		0, 0,   // B
		0, 0,   // A
		0, 0, 0, 0, 0,      // AC R G B A
		24,     // depth
		8,      // stencil
		0,      // aux
		0,      // layertype	… このオプションは使用されなくなった https://www.opengl.org/discussion_boards/showthread.php/127004-What-does-the-PFD_MAIN_PLANE-mean
		0,		// reserved
		0,		// layermask
		0,		// visiblemask
		0,		// damagemask
	};
	int pfmt = ::ChoosePixelFormat(m_hDC, &pformat);
	LN_THROW(pfmt, Win32Exception, ::GetLastError());

	BOOL r = ::SetPixelFormat(m_hDC, pfmt, &pformat);
	LN_THROW(r, Win32Exception, ::GetLastError());

	if (device->GetOpenGLMajorVersio() != 0)
	{
		// 使用する OpenGL のバージョンとプロファイルの指定
		//	WGL_CONTEXT_FLAGS_ARB
		//		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	
		//			前方互換プロファイル, 古い機能が使えない (基本的に 0 でよいはず)
		//	WGL_CONTEXT_PROFILE_MASK_ARB
		//		3.2 で意味がある。それ以前では無視される。省略した場合はシステム設定。
		//		WGL_CONTEXT_CORE_PROFILE_BIT_ARB
		//			Core Profile (OpenGL 3.1 と上位互換。(OpenGL 3.0 以前とは非互換))
		//		WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
		//			Compatibility Profile (OpenGL 3.1 + GL_ARB_compatibility と上位互換。(OpenGL 3.0 以前と上位互換))
		// http://www.asahi-net.or.jp/~yw3t-trns/opengl/version/
		// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
		// https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
		int attr[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, device->GetOpenGLMajorVersio(),
			WGL_CONTEXT_MINOR_VERSION_ARB, device->GetOpenGLMinorVersio(),
			WGL_CONTEXT_FLAGS_ARB, 0,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,	// 古い機能と互換性あり (glBegin 使いたい)
			//WGL_CONTEXT_FLAGS_ARB, 0,//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
			//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//WGL_CONTEXT_CORE_PROFILE_BIT_ARB,//WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
			0, 0,
		};
		//	HGLRC hGLRC = wglGetCurrentContext();
		//HGLRC hGLRC = wglCreateContext(hDC);
		m_hGLRC = WGLGraphicsDevice::CreateContextAttribsARB(m_hDC, share, attr);	// wglCreateContextAttribsARB なら wglShareLists は必要ない
		LN_THROW(m_hGLRC, Win32Exception, ::GetLastError());
	}
	else
	{
		m_hGLRC = wglCreateContext(m_hDC);
		LN_THROW(m_hGLRC, Win32Exception, ::GetLastError());

		if (share != NULL)
		{
			r = wglShareLists(share, m_hGLRC);
			LN_THROW(r, Win32Exception, ::GetLastError());
		}
	}




	// バックバッファは 0 を指定して作成する
	//MakeCurrentContext();
	//m_backBuffer = LN_NEW GLBackbufferRenderTarget();
	//m_backBuffer->Reset(0);


	//GLint tt1, tt2;
	// 失敗
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &tt);
	// 成功
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &tt1); LN_CHECK_GLERROR();
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &tt2); LN_CHECK_GLERROR();
	//↑ glGetFramebufferAttachmentParameteriv() のリファレンスには、Depth と Stensil が同じ Renderbuffer であれば成功、
	//	 そうでなければ失敗、とある。
	//	つまり、デフォルトで作られる Depth と Stensil は別のオブジェクトということ。


	//glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT)

	//glCheckFramebufferStatus

	//// コンテキストは元に戻しておく (できるだけ Main Context をアクティブにする)
	//if (parentContext != NULL) {
	//	parentContext->MakeCurrentContext();
	//}

#if 0
	// CreateContextAttribsARB が使える場合は wglShareLists() する必要ないみたい。第2引数で指定できるし。
	//

	if (parentContext == NULL)
	{
		printf("make context しないと シェーダ生成も何もできない\n");
		MakeCurrentContext();

		//GLint curRenderbuffer = 0;
		//glGetIntegerv(GL_RENDERBUFFER_BINDING, &curRenderbuffer);

		const char* version = (const char*)glGetString(GL_VERSION);
		printf(version);

		// Main Context のバックバッファは 0 を指定して作成する
		m_backBuffer = LN_NEW GLBackbufferRenderTarget();
		m_backBuffer->Reset(0);
	}
	else
	{
		MakeCurrentContext();

		GLuint newRenderbuffer = 0;
		glGenRenderbuffers(1, &newRenderbuffer); LN_CHECK_GLERROR();

		GLint curRenderbuffer = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &curRenderbuffer);

		//GL_COMPRESSED_TEXTURE_FORMATS
		//GL_IMPLEMENTATION_COLOR_READ_FORMAT


		glBindRenderbuffer(GL_RENDERBUFFER, newRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, window->GetSize().Width, window->GetSize().Height);



		//glGetIntegerv(GL_RENDERBUFFER_BINDING, &curRenderbuffer);

		// GL_FRAMEBUFFER_BINDING
		// 現在バインドされているフレームバッファの名前を返します。初期値は、デフォルトのフレームバッファを示し、0である。
		GLint fb;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb); LN_CHECK_GLERROR();


		// デフォルトのカラーバッファを別のフレームバッファに接続することはできる。
		// でも、値は 0 なので別のコンテキストに変えたときに別のデフォルトカラーバッファを示す値になってしまう。
		//GLuint fb2;
		//glGenFramebuffers(1, &fb2);
		//glBindFramebuffer(GL_FRAMEBUFFER, fb2);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, curRenderbuffer); LN_CHECK_GLERROR();

		// デフォルトのフレームバッファにカラーバッファをセットすることはできない
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, newRenderbuffer); LN_CHECK_GLERROR();
		//GL_INVALID_ENUM
		device->GetRenderer()->Clear(true, false, ColorF(0.5, 0.5, 0.5, 1), 1.0f);


		// バックバッファの参照を作成する
		m_backBuffer = LN_NEW GLBackbufferRenderTarget();
		m_backBuffer->Reset(newRenderbuffer);

		parentContext->MakeCurrentContext();
	}

#endif

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WGLContext::~WGLContext()
{
	if (m_hGLRC != NULL) {
		wglDeleteContext(m_hGLRC);
	}
	if (m_hDC != NULL) {
		::ReleaseDC(m_hWnd, m_hDC);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void WGLContext::MakeCurrentContext(bool attach)
//{
//	if (attach) {
//		wglMakeCurrent(m_hDC, m_hGLRC);
//	}
//	else {
//		wglMakeCurrent(NULL, NULL);
//	}
//}

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
