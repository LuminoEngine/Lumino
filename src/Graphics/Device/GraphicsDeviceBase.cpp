
#pragma once 

#include "GraphicsDeviceBase.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GraphicsDeviceBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDeviceBase::GraphicsDeviceBase()
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
	*/
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);

	ArrayList<IDeviceObject*>::iterator itr = m_deviceObjectList.begin();
	ArrayList<IDeviceObject*>::iterator end = m_deviceObjectList.end();
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


} // namespace Device
} // namespace Graphics
} // namespace Lumino
