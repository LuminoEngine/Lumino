
#include "../Internal.h"
#include "GraphicsDeviceBase.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// GraphicsDeviceBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDeviceBase::GraphicsDeviceBase()
	: m_attachRenderingThreadId(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDeviceBase::~GraphicsDeviceBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::Finalize()
{
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);
	LN_FOREACH(IDeviceObject* obj, m_deviceObjectList) {
		obj->Release();
	}
	m_deviceObjectList.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::AddDeviceResource(IDeviceObject* obj)
{
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);
	m_deviceObjectList.Add(obj);
	obj->AddRef();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void GraphicsDeviceBase::RemoveDeviceResource(IDeviceObject* obj)
//{
//	m_deviceObjectList.Remove(obj);
//}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::GCDeviceResource()
{
	/*
	この関数は描画スレッドの、SwapChain::Present() の直後で実行される。
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
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);

	Array<IDeviceObject*>::iterator itr = m_deviceObjectList.begin();
	Array<IDeviceObject*>::iterator end = m_deviceObjectList.end();
	for (; itr != end;)
	{
		if ((*itr)->GetRefCount() == 1) {
			(*itr)->Release();
			itr = m_deviceObjectList.erase(itr);
			end = m_deviceObjectList.end();
		}
		else {
			++itr;
		}
	}
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::AttachRenderingThread()
{
	LN_THROW(m_attachRenderingThreadId == 0, InvalidOperationException);
	m_attachRenderingThreadId = Threading::Thread::GetCurrentThreadID();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::DetachRenderingThread()
{
	LN_THROW(m_attachRenderingThreadId != 0, InvalidOperationException);
	m_attachRenderingThreadId = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IVertexBuffer* GraphicsDeviceBase::CreateVertexBuffer(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
{
	ScopedAccessContext lock(this);
	auto obj = CreateVertexBufferImplement(vertexElements, elementsCount, vertexCount, data, usage);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IIndexBuffer* GraphicsDeviceBase::CreateIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	ScopedAccessContext lock(this);
	auto obj = CreateIndexBufferImplement(indexCount, initialData, format, usage);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::CreateTexture(const Size& size, uint32_t mipLevels, TextureFormat format, const void* initialData)
{
	ScopedAccessContext lock(this);
	auto obj = CreateTextureImplement(size, mipLevels, format, initialData);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::CreateTexturePlatformLoading(Stream* stream, uint32_t mipLevels, TextureFormat format)
{
	ScopedAccessContext lock(this);
	auto obj = CreateTexturePlatformLoadingImplement(stream, mipLevels, format);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::CreateRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format)
{
	ScopedAccessContext lock(this);
	auto obj = CreateRenderTargetImplement(width, height, mipLevels, format);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* GraphicsDeviceBase::CreateDepthBuffer(uint32_t width, uint32_t height, TextureFormat format)
{
	ScopedAccessContext lock(this);
	auto obj = CreateDepthBufferImplement(width, height, format);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShader* GraphicsDeviceBase::CreateShader(const void* textData, size_t size, ShaderCompileResult* result)
{
	ScopedAccessContext lock(this);
	auto obj = CreateShaderImplement(textData, size, result);
	if (!obj.IsNull()) { 
		AddDeviceResource(obj);
	}
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* GraphicsDeviceBase::CreateSwapChain(PlatformWindow* window)
{
	ScopedAccessContext lock(this);
	auto obj = CreateSwapChainImplement(window);
	AddDeviceResource(obj);
	return obj.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::OnBeginAccessContext()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::OnEndAccessContext()
{
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
