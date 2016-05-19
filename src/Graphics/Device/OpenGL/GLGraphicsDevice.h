
#pragma once 
#include <Lumino/Platform/PlatformWindow.h>
#include "../GraphicsDriverInterface.h"
#include "../GraphicsDeviceBase.h"
#include "GLRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class GLContext;

/**
	@brief	OpenGL 用の IGraphicsDevice の実装
*/
class GLGraphicsDevice
	: public GraphicsDeviceBase
{
public:
	struct ConfigData
	{
		PlatformWindow*		MainWindow;
		int					OpenGLMajorVersion;
		int					OpenGLMinorVersion;

		ConfigData()
			: MainWindow(NULL)
			, OpenGLMajorVersion(3)
			, OpenGLMinorVersion(1)
		{}
	};

public:
	GLGraphicsDevice();
	virtual ~GLGraphicsDevice();

public:
	/// 描画を行うスレッドで初期化する必要がある
	void Initialize(const ConfigData& configData);

	int GetOpenGLMajorVersio() const { return m_openGLMajorVersion; }
	int GetOpenGLMinorVersio() const { return m_openGLMinorVersion; }
	//void GCDeviceResource();

	/// 指定コンテキストをアクティブにする
	virtual void MakeCurrentContext(GLContext* context) = 0;

	/// メインコンテキストの取得
	virtual GLContext* GetMainContext() = 0;

	/// メイン描画用コンテキストの取得
	virtual GLContext* GetMainRenderingContext() = 0;

	virtual void AttachRenderingThread() override;
	virtual void DetachRenderingThread() override;
	virtual void OnBeginAccessContext() override;
	virtual void OnEndAccessContext() override;

public:
	virtual void Finalize() override;
	virtual bool IsStandalone() const override { return true; }
	virtual GraphicsAPI GetGraphicsAPI() const override { return GraphicsAPI::OpenGL; }
	virtual IRenderer* GetRenderer() override { return m_renderer; }
	virtual RefPtr<IVertexBuffer> CreateVertexBufferImplement(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage) override;
	virtual RefPtr<IIndexBuffer> CreateIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage) override;
	virtual RefPtr<ITexture> CreateTextureImplement(const Size& size, uint32_t mipLevels, TextureFormat format, const void* initialData) override;
	virtual RefPtr<ITexture> CreateTexturePlatformLoadingImplement(Stream* stream, uint32_t mipLevels, TextureFormat format) override { return NULL; }
	virtual RefPtr<ITexture> CreateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual RefPtr<ITexture> CreateDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual RefPtr<IShader> CreateShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual RefPtr<ISwapChain> CreateSwapChainImplement(PlatformWindow* window) override;
	virtual DeviceState GetDeviceState() override { return m_deviceState; }
	virtual void ResetDevice() override;
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;
	virtual void LockContext() override;
	virtual void UnlockContext() override;

protected:
	static void ParseGLVersion(int* glMajor, int* glMinor, int* glslMajor, int* glslMinor);
	static bool ContainsExtensionString(const char* extensionString, const char* str);
	void SelectGLVersion(int requestMajor, int requestMinor);
	//void AddDeviceResource(IDeviceObject* obj);
	//int CheckPlatformExtensionSupported(const char* extension);
	//bool CheckContainsExtensionString(const char* string, const GLubyte* extensions);

	// TODO: いらないかな
	//class ScopedContext
	//{
	//public:
	//	GLGraphicsDevice* m_device;
	//	ScopedContext(GLGraphicsDevice* d)
	//	{
	//		m_device = d;
	//		m_device->m_mutex.Lock();
	//		m_device->MakeCurrentContext(m_device->GetMainContext());
	//	}
	//	~ScopedContext()
	//	{
	//		m_device->MakeCurrentContext(NULL);
	//		m_device->m_mutex.Unlock();
	//	}
	//};

protected:
	DeviceState					m_deviceState;
	RefPtr<PlatformWindow>		m_mainWindow;
	GLRenderer*					m_renderer;
	int							m_openGLMajorVersion;
	int							m_openGLMinorVersion;
	Mutex						m_mutex;	// TODO: いらないかな
	//Array<IDeviceObject*>		m_allDeviceResourceList;	///< 全てのデバイスリソースのリスト;
	//Mutex			m_allDeviceResourceListMutex;
};


} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
