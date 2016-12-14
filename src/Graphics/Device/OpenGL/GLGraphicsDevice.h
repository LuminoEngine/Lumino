
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
class GLSwapChain;

// OpenGL 用の IGraphicsDevice の実装
/*
	OpenGL のコンテキストの持ち方

	- SwapChain 1つに対して1つの Context を割り当てる。(Swap の都合上必須)
	- メインスレッドでは常に MainWindow の SwapChain に割り当てられた Context がカレントになっている。
	- Threading モードの場合、レンダリングスレッドは常に専用の Context がカレントになっている。
	- Immediate モードの場合、メインスレッドの Context とレンダリングスレッドの Context は等しい。
	- リソースロードスレッドを使う場合、メインスレッド以外ではロード用の 1 つの Context を使いまわす。(排他制御)
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
		bool				createSharedRenderingContext = false;

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
	void Initialize(const ConfigData& configData);

	int GetOpenGLMajorVersio() const { return m_openGLMajorVersion; }
	int GetOpenGLMinorVersio() const { return m_openGLMinorVersion; }
	//void GCDeviceResource();

	GLContext* GetMainContext() const;
	GLContext* GetMainRenderingContext() const;

	MemoryStream* GetUniformTempBuffer() { return &m_uniformTempBuffer; }
	BinaryWriter* GetUniformTempBufferWriter() { return &m_uniformTempBufferWriter; }

	virtual RefPtr<GLContext> InitializeMainContext(const ConfigData& configData) = 0;
	virtual RefPtr<GLContext> CreateContext(PlatformWindow* window) = 0;


	/// 指定コンテキストをアクティブにする
	virtual void MakeCurrentContext(GLContext* context) = 0;

	

	virtual void AttachRenderingThread() override;
	virtual void DetachRenderingThread() override;
	//virtual void OnBeginAccessContext() override;
	//virtual void OnEndAccessContext() override;

public:
	// IGraphicsDevice interface
	virtual void Finalize() override;
	virtual bool IsStandalone() const override { return true; }
	virtual GraphicsAPI GetGraphicsAPI() const override { return GraphicsAPI::OpenGL; }
	virtual IRenderer* GetRenderer() override { return m_renderer; }
	virtual ISwapChain* GetDefaultSwapChain() override;
	virtual ISwapChain* CreateSwapChain(PlatformWindow* window) override;

private:
	virtual RefPtr<IVertexDeclaration> CreateVertexDeclarationImplement(const VertexElement* elements, int elementsCount) override;
	virtual RefPtr<IVertexBuffer> CreateVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage) override;
	virtual RefPtr<IIndexBuffer> CreateIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) override;
	virtual RefPtr<ITexture> CreateTextureImplement(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) override;
	virtual RefPtr<ITexture> CreateTexturePlatformLoadingImplement(Stream* stream, bool mipmap, TextureFormat format) override { return NULL; }
	virtual RefPtr<ITexture> CreateTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) override;
	virtual RefPtr<ITexture> CreateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual RefPtr<ITexture> CreateDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual RefPtr<IShader> CreateShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual RefPtr<ISwapChain> CreateSwapChainImplement(PlatformWindow* window) override;
	virtual DeviceState GetDeviceState() override { return m_deviceState; }
	virtual void ResetDevice() override;
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;


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

private:
	DeviceState					m_deviceState;
	RefPtr<PlatformWindow>		m_mainWindow;
	int							m_openGLMajorVersion;
	int							m_openGLMinorVersion;


	RefPtr<GLContext>			m_mainContext;
	RefPtr<GLContext>			m_mainRenderingContext;
	RefPtr<GLSwapChain>			m_defaultSwapChain;
	RefPtr<GLRenderer>			m_renderer;

	Mutex						m_mutex;	// TODO: いらないかな

	MemoryStream				m_uniformTempBuffer;
	BinaryWriter				m_uniformTempBufferWriter;
};


} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
