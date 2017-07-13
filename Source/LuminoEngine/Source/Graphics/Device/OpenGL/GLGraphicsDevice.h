
#pragma once 
#include <Lumino/Platform/PlatformWindow.h>
#include "../GraphicsDriverInterface.h"
#include "../GraphicsDeviceBase.h"
#include "GLCommon.h"
#include "GLRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class GLContext;
class GLSwapChain;

/*
	OpenGL のコンテキストの使われ方

	Threading モードの場合、Lumino は以下のように分類したたくさんの Context を作る。
	- SwapChain Context
		SwapChain を通して PlatformWindow へバックバッファを転送するための Context。
		この種類の Context は、SwapChain 1つに対して必ず 1 つ存在する。
		レンダリングスレッド上で Current になる。
	- Renderer Context
		実際に描画を行うためのコンテキスト。
		他の分類の Context とは独立して 1 つ存在する。
		レンダリングスレッド上で Current になる。
	- Main Context
		メインスレッドで常にアクティブになっている Context。
		他の分類の Context とは独立して 1 つ存在する。
		他の分類の Context は、全て Main Context の共有 Context である。
	- Resource Context
		ユーザーが別のスレッドでリソースを作る場合に使う Context。
		他の分類の Context とは独立して 1 つ存在する。
		メインスレッド以外で、リソースにアクセスする必要がある場合に Current になる。
		解放のタイミングが難しいので、1つしか作らず、排他処理して使うことになる。

		
	Immediate モードの場合、

	- SwapChain 1つに対して1つの Context を割り当てる。(Swap の都合上必須)
	- メインスレッドでは常に MainWindow の SwapChain に割り当てられた Context がカレントになっている。
	- メインスレッドの Context とレンダリングスレッドの Context は等しい。
	- リソースロードスレッドを使う場合、メインスレッド以外ではロード用の 1 つの Context を使いまわす。(排他制御)
*/
class GLGraphicsDevice
	: public GraphicsDeviceBase
{
public:
	struct ConfigData
	{
		PlatformWindow*		mainWindow = nullptr;
		int					openGLMajorVersion = 0;
		int					openGLMinorVersion = 0;
		bool				createSharedRenderingContext = false;
	};

public:
	GLGraphicsDevice();
	virtual ~GLGraphicsDevice();
	void initialize(const ConfigData& configData);

	int getOpenGLMajorVersion() const { return m_openGLMajorVersion; }
	int getOpenGLMinorVersion() const { return m_openGLMinorVersion; }
	GLContext* getMainContext() const;
	GLContext* getMainRenderingContext() const;
	MemoryStream* getUniformTempBuffer() { return &m_uniformTempBuffer; }
	BinaryWriter* getUniformTempBufferWriter() { return &m_uniformTempBufferWriter; }

	virtual RefPtr<GLContext> initializeMainContext(const ConfigData& configData) = 0;
	virtual RefPtr<GLContext> createContext(PlatformWindow* window) = 0;
	virtual void makeCurrentContext(GLContext* context) = 0;

public:
	// IGraphicsDevice interface
	virtual void dispose() override;
	virtual bool isStandalone() const override { return true; }
	virtual GraphicsAPI getGraphicsAPI() const override { return GraphicsAPI::OpenGL; }
	virtual IRenderer* getRenderer() override { return m_renderer; }
	virtual ISwapChain* getDefaultSwapChain() override;
	virtual ISwapChain* createSwapChain(PlatformWindow* window) override;
	virtual void attachRenderingThread() override;
	virtual void detachRenderingThread() override;

private:
	virtual RefPtr<IVertexDeclaration> createVertexDeclarationImplement(const VertexElement* elements, int elementsCount) override;
	virtual RefPtr<IVertexBuffer> createVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage) override;
	virtual RefPtr<IIndexBuffer> createIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) override;
	virtual RefPtr<ITexture> createTextureImplement(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) override;
	virtual RefPtr<ITexture> createTexturePlatformLoadingImplement(Stream* stream, bool mipmap, TextureFormat format) override { return NULL; }
	virtual RefPtr<ITexture> createTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) override;
	virtual RefPtr<ITexture> ceateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual RefPtr<ITexture> createDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual RefPtr<IShader> createShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual RefPtr<ISwapChain> createSwapChainImplement(PlatformWindow* window) override;
	virtual DeviceState getDeviceState() override { return m_deviceState; }
	virtual void resetDevice() override;
	virtual void onLostDevice() override;
	virtual void onResetDevice() override;
	virtual void flushResource() override;

protected:
	static void parseGLVersion(int* glMajor, int* glMinor, int* glslMajor, int* glslMinor);
	static bool containsExtensionString(const char* extensionString, const char* str);
	void selectGLVersion(int requestMajor, int requestMinor);

private:
	DeviceState					m_deviceState;
	RefPtr<PlatformWindow>		m_mainWindow;
	int							m_openGLMajorVersion;
	int							m_openGLMinorVersion;

	RefPtr<GLContext>			m_mainContext;
	RefPtr<GLContext>			m_mainRenderingContext;
	RefPtr<GLSwapChain>			m_defaultSwapChain;
	RefPtr<GLRenderer>			m_renderer;

	MemoryStream				m_uniformTempBuffer;
	BinaryWriter				m_uniformTempBufferWriter;
};

// プラットフォームごとの OpenGL コンテキスト
class GLContext
	: public RefObject
{
public:
	GLContext() = default;
	virtual ~GLContext() = default;
	virtual void swapBuffers() = 0;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
