
#pragma once 

#include "../DeviceInterface.h"
#include "../../../../include/Lumino/Graphics/GraphicsDevice.h"
#include "GLRenderer.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class GLContext;

/**
	@brief	OpenGL 用の IGraphicsDevice の実装
*/
class GLGraphicsDevice
	: public IGraphicsDevice
{
public:
	GLGraphicsDevice();
	virtual ~GLGraphicsDevice();

public:
	/// 描画を行うスレッドで初期化する必要がある
	void Initialize(const GraphicsDeviceConfigData& configData);

	int GetOpenGLMajorVersio() const { return m_openGLMajorVersion; }
	int GetOpenGLMinorVersio() const { return m_openGLMinorVersion; }
	void GCDeviceResource();

	/// 指定コンテキストをアクティブにする
	virtual void MakeCurrentContext(GLContext* context) = 0;

	/// メインコンテキストの取得
	virtual GLContext* GetMainContext() = 0;

	/// メイン描画用コンテキストの取得
	virtual GLContext* GetMainRenderingContext() = 0;

public:
	virtual void Finalize();
	virtual IRenderer* GetRenderer() { return m_renderer; }
	virtual IVertexBuffer* CreateVertexBuffer(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage);
	virtual IIndexBuffer* CreateIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage);
	virtual ITexture* CreateTexture(const Size& size, uint32_t mipLevels, TextureFormat format);
	virtual ITexture* CreateTexturePlatformLoading(Stream* stream, uint32_t mipLevels, TextureFormat format) { return NULL; }
	virtual ITexture* CreateRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format);
	virtual ITexture* CreateDepthBuffer(uint32_t width, uint32_t height, TextureFormat format);
	virtual IShader* CreateShader(const void* textData, size_t size, ShaderCompileResult* result);
	virtual DeviceState GetDeviceState() { return m_deviceState; }
	virtual void ResetDevice();
	virtual void OnLostDevice();
	virtual void OnResetDevice();
	virtual void LockContext();
	virtual void UnlockContext();

protected:
	static void ParseGLVersion(int* glMajor, int* glMinor, int* glslMajor, int* glslMinor);
	static bool ContainsExtensionString(const char* extensionString, const char* str);
	void SelectGLVersion(int requestMajor, int requestMinor);
	void AddDeviceResource(IDeviceObject* obj);
	//int CheckPlatformExtensionSupported(const char* extension);
	//bool CheckContainsExtensionString(const char* string, const GLubyte* extensions);

	class ScopedContext
	{
	public:
		GLGraphicsDevice* m_device;
		ScopedContext(GLGraphicsDevice* d)
		{
			m_device = d;
			m_device->m_mutex.Lock();
			m_device->MakeCurrentContext(m_device->GetMainContext());
		}
		~ScopedContext()
		{
			m_device->MakeCurrentContext(NULL);
			m_device->m_mutex.Unlock();
		}
	};

protected:
	DeviceState					m_deviceState;
	RefPtr<Platform::Window>	m_mainWindow;
	GLRenderer*					m_renderer;
	int							m_openGLMajorVersion;
	int							m_openGLMinorVersion;
	Threading::Mutex			m_mutex;
	Array<IDeviceObject*>		m_allDeviceResourceList;	///< 全てのデバイスリソースのリスト;
	Threading::Mutex			m_allDeviceResourceListMutex;
};

class GLContext
	: public RefObject
{
public:
	GLContext() {}
	virtual ~GLContext() {}
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
