
#include "../Internal.h"
#include "GraphicsDeviceBase.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// GraphicsDeviceBase
//==============================================================================

//------------------------------------------------------------------------------
GraphicsDeviceBase::GraphicsDeviceBase()
	: m_attachRenderingThreadId(0)
{
}

//------------------------------------------------------------------------------
GraphicsDeviceBase::~GraphicsDeviceBase()
{
}

//------------------------------------------------------------------------------
void GraphicsDeviceBase::dispose()
{
	MutexScopedLock lock(m_deviceObjectListMutex);
	for (IDeviceObject* obj : m_deviceObjectList) {
		obj->release();
	}
	m_deviceObjectList.clear();
}

//------------------------------------------------------------------------------
void GraphicsDeviceBase::addDeviceResource(IDeviceObject* obj)
{
	MutexScopedLock lock(m_deviceObjectListMutex);
	m_deviceObjectList.add(obj);
	obj->addRef();
}

//------------------------------------------------------------------------------
//void GraphicsDeviceBase::RemoveDeviceResource(IDeviceObject* obj)
//{
//	m_deviceObjectList.Remove(obj);
//}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void GraphicsDeviceBase::gcDeviceResource()
{
	/*
	この関数は描画スレッドの、SwapChain::present() の直後で実行される。
	リソースがこのクラス以外から参照されていなければ開放する。

	なお、このような仕組みにしたのは、リソースの開放を容易にするため。
	Create 系はスレッドセーフかつ GPU 固有の不具合に備えるため、
	作成の前後でコンテキストをアクティブ/ディアクティブしているが、
	開放時 (glDelete～) の前後でも当然必要になる。

	遅延描画の都合上、メインスレッドで不要になっても描画スレッドではまだ使っていることは普通にある。
	描画スレッドでも必要なくなった時点でリソースを Release すれば良いのだが、
	それだとデストラクタで「現在のスレッドがメインスレッドであれば MakeCurrent する」のような処理が必要になる。
	この場合問題となるのは、
	・Create系の内部でエラーが発生し、Release したいときにデッドロックの危険性がある
	・デストラクタで例外を発生させる可能性がある
	やりようはいくらでもあるが、シンプルに実装するのは少し難しい。

	作成もメインスレッド・ローディングスレッドなど様々なスレッドからの呼び出しもサポートするし、
	作成と解放のスレッドを一致させる必要もないわけで、この辺で開放するのが一番シンプル。
	*/
	MutexScopedLock lock(m_deviceObjectListMutex);

	List<IDeviceObject*>::iterator itr = m_deviceObjectList.begin();
	List<IDeviceObject*>::iterator end = m_deviceObjectList.end();
	for (; itr != end;)
	{
		if ((*itr)->getReferenceCount() == 1) {
			(*itr)->release();
			itr = m_deviceObjectList.erase(itr);
			end = m_deviceObjectList.end();
		}
		else {
			++itr;
		}
	}
}




//------------------------------------------------------------------------------
void GraphicsDeviceBase::attachRenderingThread()
{
	LN_THROW(m_attachRenderingThreadId == 0, InvalidOperationException);
	m_attachRenderingThreadId = Thread::getCurrentThreadId();
}

//------------------------------------------------------------------------------
void GraphicsDeviceBase::detachRenderingThread()
{
	LN_THROW(m_attachRenderingThreadId != 0, InvalidOperationException);
	m_attachRenderingThreadId = 0;
}

//------------------------------------------------------------------------------
IVertexDeclaration* GraphicsDeviceBase::createVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	ScopedAccessContext lock(this);
	auto obj = createVertexDeclarationImplement(elements, elementsCount);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
IVertexBuffer* GraphicsDeviceBase::createVertexBuffer(size_t bufferSize, const void* data, ResourceUsage usage)
{
	ScopedAccessContext lock(this);
	auto obj = createVertexBufferImplement(bufferSize, data, usage);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
IIndexBuffer* GraphicsDeviceBase::createIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage)
{
	ScopedAccessContext lock(this);
	auto obj = createIndexBufferImplement(indexCount, initialData, format, usage);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::createTexture(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData)
{
	ScopedAccessContext lock(this);
	auto obj = createTextureImplement(size, mipmap, format, initialData);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::createTexturePlatformLoading(Stream* stream, bool mipmap, TextureFormat format)
{
	ScopedAccessContext lock(this);
	auto obj = createTexturePlatformLoadingImplement(stream, mipmap, format);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::createTexture3D(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData)
{
	ScopedAccessContext lock(this);
	auto obj = createTexture3DImplement(width, height, depth, mipLevels, format, usage, initialData);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::createRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format)
{
	ScopedAccessContext lock(this);
	auto obj = ceateRenderTargetImplement(width, height, mipLevels, format);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::createDepthBuffer(uint32_t width, uint32_t height, TextureFormat format)
{
	ScopedAccessContext lock(this);
	auto obj = createDepthBufferImplement(width, height, format);
	addDeviceResource(obj);
	return obj.detachMove();
}

//------------------------------------------------------------------------------
IShader* GraphicsDeviceBase::createShader(const void* textData, size_t size, ShaderCompileResult* result)
{
	ScopedAccessContext lock(this);
	auto obj = createShaderImplement(textData, size, result);
	if (!obj.isNull()) { 
		addDeviceResource(obj);
	}
	return obj.detachMove();
}

//------------------------------------------------------------------------------
ISwapChain* GraphicsDeviceBase::createSwapChain(PlatformWindow* window)
{
	ScopedAccessContext lock(this);
	auto obj = createSwapChainImplement(window);
	addDeviceResource(obj);
	return obj.detachMove();
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
