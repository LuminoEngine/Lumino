/*
 *	EXT とか ARB とか
 *	https://shikihuiku.wordpress.com/2013/10/08/hardtounderstandoglext/
 */
#include "../../../Internal.h"
#include <Lumino/Graphics/Utils.h>
#include <Lumino/Platform/PlatformWindow.h>
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLGraphicsDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// GLGraphicsDevice
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLGraphicsDevice::GLGraphicsDevice()
	: m_deviceState(DeviceState_Lost)
	, m_renderer(NULL)
	, m_openGLMajorVersion(3)
	, m_openGLMinorVersion(2)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLGraphicsDevice::~GLGraphicsDevice()
{
	LN_ASSERT(m_renderer == NULL);	// Finalize 済みであること
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::Initialize(const ConfigData& configData)
{
	m_mainWindow = configData.MainWindow;
	m_deviceState = DeviceState_Enabled;


	Logger::WriteLine("GLGraphicsDevice::Initialize");
	Logger::WriteLine("    Requested OpenGL version : %d.%d", configData.OpenGLMajorVersion, configData.OpenGLMinorVersion);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::Finalize()	// 仮想関数呼び出しが必要なのでデストラクタとは別に開放用関数を用意した
{
	OnBeginAccessContext();
	GraphicsDeviceBase::Finalize();
	LN_SAFE_RELEASE(m_renderer);
	OnEndAccessContext();

	//ScopedContext lock(this);
	//LN_FOREACH(IDeviceObject* obj, m_allDeviceResourceList) {
	//	obj->Release();
	//}
	//m_allDeviceResourceList.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<IVertexBuffer> GLGraphicsDevice::CreateVertexBufferImplement(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
{
	RefPtr<GLVertexBuffer> obj(LN_NEW GLVertexBuffer(), false);
	obj->Create(vertexElements, elementsCount, vertexCount, data, usage);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<IIndexBuffer> GLGraphicsDevice::CreateIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	RefPtr<GLIndexBuffer> obj(LN_NEW GLIndexBuffer(), false);
	obj->Create(indexCount, initialData, format, usage);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateTextureImplement(const Size& size, uint32_t mipLevels, TextureFormat format, const void* initialData)
{
	RefPtr<GLTexture> obj(LN_NEW GLTexture(size, format, mipLevels), false);
	if (initialData != nullptr) {
		obj->SetSubData(Point(0, 0), initialData, Utils::GetTextureFormatByteCount(format) * size.Width * size.Height, size);
	}
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//ITexture* GLGraphicsDevice::CreateTexture(const Graphics::Bitmap* bitmap, uint32_t mipLevels, TextureFormat format)
//{
//	LN_THROW(0, NotImplementedException);
//	return 0;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format)
{
	RefPtr<GLRenderTargetTexture> obj(LN_NEW GLRenderTargetTexture(Size(width, height), format, mipLevels), false);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<ITexture> GLGraphicsDevice::CreateDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format)
{
	RefPtr<GLDepthBuffer> obj(LN_NEW GLDepthBuffer(Size(width, height), format), false);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<IShader> GLGraphicsDevice::CreateShaderImplement(const void* textData, size_t size, ShaderCompileResult* result)
{
	//printf("▼\n");

	GLShader* shader = NULL;
	result->Level = PlainGLSLBuilder::Build(this, textData, size, &shader, &result->Message);

	//if (shader != NULL) {
	//	AddDeviceResource(shader);
	//}
	/*
		シェーダを作った直後、Shader の Apply() → DrawPrimitive() → Present() すると、
		glFlush() とか wglMakeCurrent() とかでビジー状態になり、「ディスプレイドライバが応答しません」とか右下からエラー出た。
		リソースをメインスレッドで作って、描画スレッドで ドライバに描画情報送るときに落ちたと思われるが、ホントの原因は不明。
		SwapChain を 2 つ作ったりすると何故か正常に動作した。

		http://stackoverflow.com/questions/21613842/wglmakecurrent-in-second-thread-fails
		によると、グラボによってはバグを回避するために wglMakeCurrent(NULL, NULL) を仕込む必要があるとのこと。
		とりあえずそのようにしてみたところ、うまく動作するようになった。
		問題のグラボは AMD Radeon(TM) HD8490。

		GeForce GTX 560M では正常に動作した。
	*/
	//MakeCurrentContext(NULL);
	//MakeCurrentContext(GetMainContext());

	//printf("▲\n");
	RefPtr<IShader> obj(shader, false);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<ISwapChain> GLGraphicsDevice::CreateSwapChainImplement(PlatformWindow* window)
{
	LN_THROW(0, NotImplementedException);
	return nullptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::ResetDevice()
{
	// 先に OnLostDevice() を呼ぶこと
	LN_THROW(m_deviceState == DeviceState_Pausing, InvalidOperationException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::OnLostDevice()
{
	m_deviceState = DeviceState_Pausing;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::OnResetDevice()
{
	m_deviceState = DeviceState_Enabled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::LockContext()
{
	m_mutex.Lock();
	OnBeginAccessContext();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::UnlockContext()
{
	OnEndAccessContext();
	m_mutex.Unlock();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::ParseGLVersion(int* glMajor, int* glMinor, int* glslMajor, int* glslMinor)
{
	// GL_VERSION の文字列フォーマットは決まっている。
	// https://www.opengl.org/wiki/GLAPI/glGetString
	// 現在は 1桁バージョンしかないので、Haskell の GLHUI はこんな単純なとり方をしていたりする。
	// http://hackage.haskell.org/package/GLHUI-1.1.0/src/c/HsGLWindow.c
	const char* version = (const char*)glGetString(GL_VERSION);
	*glMajor = version[0] - '0';
	*glMinor = version[2] - '0';
	if ((*glMajor) >= 2) {
		version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		*glslMajor = version[0] - '0';
		*glslMinor = version[2] - '0';
	}
	else {
		*glslMajor = 0;
		*glslMinor = 0;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GLGraphicsDevice::ContainsExtensionString(const char* extensionString, const char* str)
{
	const char* readPos = extensionString;
	const char* extensionStringEnd = extensionString + strlen(extensionString);
	while (true)
	{
		const char* found = strstr(readPos, str);
		if (found == NULL) {
			return false;
		}

		// 見つかったところは、ホントにトークンの先頭を指しているか
		if (found == readPos || *(found - 1) == ' ')
		{
			const char* endPos = found + strlen(str);
			if (endPos <= extensionStringEnd)
			{
				// 見つかった範囲の終端は次のトークンとの区切りのスペース文字か、\0
				if (*endPos == ' ' || *endPos == '\0') {
					break;	// 見つかった
				}
			}
		}
	}

	// break で抜けてきて、true を返す
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLGraphicsDevice::SelectGLVersion(int requestMajor, int requestMinor)
{
	if (requestMajor == 0)
	{
		// 要求されていない場合は環境のデフォルトバージョンでコンテキストを作成する
		m_openGLMajorVersion = 0;
		m_openGLMinorVersion = 0;
	}
	else
	{
		// OpenGL バージョンの選択 (最大でも GL_VERSION にする)
		int glMajor, glMinor, glslMajor, glslMinor;
		ParseGLVersion(&glMajor, &glMinor, &glslMajor, &glslMinor);
		if (requestMajor > glMajor) {
			m_openGLMajorVersion = glMajor;
			m_openGLMinorVersion = glMinor;
		}
		else if (requestMinor > glMinor) {
			m_openGLMajorVersion = requestMajor;
			m_openGLMinorVersion = glMinor;
		}
		else {
			m_openGLMajorVersion = requestMajor;
			m_openGLMinorVersion = requestMinor;
		}
	}

	Logger::WriteLine("Active OpenGL version : %d.%d", m_openGLMajorVersion, m_openGLMinorVersion);
}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void GLGraphicsDevice::AddDeviceResource(IDeviceObject* obj)
//{
//	Threading::MutexScopedLock lock(m_allDeviceResourceListMutex);
//
//	m_allDeviceResourceList.Add(obj);
//	obj->AddRef();
//}
//
////-----------------------------------------------------------------------------
//// 
////-----------------------------------------------------------------------------
//void GLGraphicsDevice::GCDeviceResource()
//{
//	/*
//		この関数は描画スレッドの、SwapChain::Present() の直後で実行される。
//		リソースがこのクラス以外から参照されていなければ開放する。
//
//		なお、このような仕組みにしたのは、リソースの開放を容易にするため。
//		Create 系はスレッドセーフかつ GPU 固有の不具合に備えるため、
//		作成の前後でコンテキストをアクティブ/ディアクティブしているが、
//		開放時 (glDelete～) の前後でも当然必要になる。
//
//		遅延描画の都合上、メインスレッドで不要になっても描画スレッドではまだ使っていることは普通にある。
//		描画スレッドでも必要なくなった時点でリソースを Release すれば良いのだが、
//		それだとデストラクタで「現在のスレッドがメインスレッドであれば MakeCurrent する」のような処理が必要になる。
//		この場合問題となるのは、
//		・Create系の内部でエラーが発生し、Release したいときにデッドロックの危険性がある
//		・デストラクタで例外を発生させる可能性がある
//		やりようはいくらでもあるが、シンプルに実装するのは少し難しい。
//	*/
//	Threading::MutexScopedLock lock(m_allDeviceResourceListMutex);
//
//	Array<IDeviceObject*>::iterator itr = m_allDeviceResourceList.begin();
//	Array<IDeviceObject*>::iterator end = m_allDeviceResourceList.end();
//	for (; itr != end;)
//	{
//		if ((*itr)->GetRefCount() == 1) {
//			(*itr)->Release();
//			itr = m_allDeviceResourceList.erase(itr);
//			end = m_allDeviceResourceList.end();
//		}
//		else {
//			++itr;
//		}
//	}
//	
//
//	//LN_FOREACH(IDeviceObject* obj, m_allDeviceResourceList)
//	//{
//	//	if (obj->GetRefCount() == 1) {
//	//		obj->Release();
//	//	}
//	//}
//}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
