
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

	virtual void Finalize();

	/// リソース登録
	void AddDeviceResource(IDeviceObject* obj);

	/// リソース除外
	//void RemoveDeviceResource(IDeviceObject* obj);

	/// GC 実行
	void GCDeviceResource();

	virtual void AttachRenderingThread();
	virtual void DetachRenderingThread();

	virtual IVertexDeclaration* CreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
	virtual IVertexBuffer* CreateVertexBuffer(size_t bufferSize, const void* data, ResourceUsage usage) override;
	virtual IIndexBuffer* CreateIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) override;
	virtual ITexture* CreateTexture(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) override;
	virtual ITexture* CreateTexturePlatformLoading(Stream* stream, bool mipmap, TextureFormat format) override;
	virtual ITexture* CreateTexture3D(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) override;
	virtual ITexture* CreateRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual ITexture* CreateDepthBuffer(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual IShader* CreateShader(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual ISwapChain* CreateSwapChain(PlatformWindow* window) override;

protected:
	virtual RefPtr<IVertexDeclaration> CreateVertexDeclarationImplement(const VertexElement* elements, int elementsCount) = 0;
	virtual RefPtr<IVertexBuffer> CreateVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage) = 0;
	virtual RefPtr<IIndexBuffer> CreateIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) = 0;
	virtual RefPtr<ITexture> CreateTextureImplement(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) = 0;
	virtual RefPtr<ITexture> CreateTexturePlatformLoadingImplement(Stream* stream, bool mipmap, TextureFormat format) = 0;
	virtual RefPtr<ITexture> CreateTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) = 0;
	virtual RefPtr<ITexture> CreateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) = 0;
	virtual RefPtr<ITexture> CreateDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) = 0;
	virtual RefPtr<IShader> CreateShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) = 0;
	virtual RefPtr<ISwapChain> CreateSwapChainImplement(PlatformWindow* window) = 0;
	virtual void OnBeginAccessContext();
	virtual void OnEndAccessContext();

protected:

	class ScopedAccessContext
	{
	public:
		GraphicsDeviceBase* m_device;
		ScopedAccessContext(GraphicsDeviceBase* d)
		{
			m_device = d;
			m_device->m_contextAccessMutex.Lock();
			m_device->OnBeginAccessContext();
		}
		~ScopedAccessContext()
		{
			m_device->OnEndAccessContext();
			m_device->m_contextAccessMutex.Unlock();
		}
	};

	typedef Array<IDeviceObject*>	DeviceObjectList;
	
	uint32_t			m_attachRenderingThreadId;
	DeviceObjectList	m_deviceObjectList;
	Mutex				m_deviceObjectListMutex;
	Mutex				m_contextAccessMutex;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
