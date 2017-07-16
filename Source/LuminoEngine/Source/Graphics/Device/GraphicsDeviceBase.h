
#pragma once 
#include <Lumino/Threading/Mutex.h>
#include "GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

// TODO: IRenderer のように IGraphicsDevice へ統合してしまっていい気がする
class GraphicsDeviceBase
	: public IGraphicsDevice
{
public:
	GraphicsDeviceBase();
	virtual ~GraphicsDeviceBase();

public:

	virtual void dispose() override;

	/// リソース登録
	void addDeviceResource(IDeviceObject* obj);

	/// リソース除外
	//void RemoveDeviceResource(IDeviceObject* obj);

	/// GC 実行
	void gcDeviceResource();

	virtual void attachRenderingThread();
	virtual void detachRenderingThread();

	virtual IVertexDeclaration* createVertexDeclaration(const VertexElement* elements, int elementsCount) override;
	virtual IVertexBuffer* createVertexBuffer(size_t bufferSize, const void* data, ResourceUsage usage) override;
	virtual IIndexBuffer* createIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) override;
	virtual ITexture* createTexture(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) override;
	virtual ITexture* createTexturePlatformLoading(Stream* stream, bool mipmap, TextureFormat format) override;
	virtual ITexture* createTexture3D(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) override;
	virtual ITexture* createRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual ITexture* createDepthBuffer(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual IShader* createShader(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual ISwapChain* createSwapChain(PlatformWindow* window) override;

protected:
	virtual RefPtr<IVertexDeclaration> createVertexDeclarationImplement(const VertexElement* elements, int elementsCount) = 0;
	virtual RefPtr<IVertexBuffer> createVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage) = 0;
	virtual RefPtr<IIndexBuffer> createIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) = 0;
	virtual RefPtr<ITexture> createTextureImplement(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) = 0;
	virtual RefPtr<ITexture> createTexturePlatformLoadingImplement(Stream* stream, bool mipmap, TextureFormat format) = 0;
	virtual RefPtr<ITexture> createTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) = 0;
	virtual RefPtr<ITexture> ceateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) = 0;
	virtual RefPtr<ITexture> createDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) = 0;
	virtual RefPtr<IShader> createShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) = 0;
	virtual RefPtr<ISwapChain> createSwapChainImplement(PlatformWindow* window) = 0;
	//virtual void OnBeginAccessContext();
	//virtual void OnEndAccessContext();

protected:

	class ScopedAccessContext
	{
	public:
		GraphicsDeviceBase* m_device;
		ScopedAccessContext(GraphicsDeviceBase* d)
		{
			m_device = d;
			m_device->m_contextAccessMutex.lock();
			//m_device->OnBeginAccessContext();
		}
		~ScopedAccessContext()
		{
			//m_device->OnEndAccessContext();
			m_device->m_contextAccessMutex.unlock();
		}
	};

	typedef List<IDeviceObject*>	DeviceObjectList;
	
	uint32_t			m_attachRenderingThreadId;
	DeviceObjectList	m_deviceObjectList;
	Mutex				m_deviceObjectListMutex;
	Mutex				m_contextAccessMutex;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
