
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Graphics
{
class GraphicsManager;

/**
	@brief		頂点バッファのクラスです。
*/
class VertexBuffer
	: public RefObject
{
public:

	/**
		@brief		頂点バッファを作成します。
		@param[in]	vertexElements	: 頂点データレイアウトを表す VertexElement の配列
		@param[in]	elementsCount	: vertexElements の要素数
		@param[in]	vertexCount		: 頂点の数
		@param[in]	data			: 作成と同時に書き込む初期データ (必要なければ NULL)
		@param[in]	usage			: 頂点バッファリソースの使用方法
	*/
	static VertexBuffer* Create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data = NULL, DeviceResourceUsage usage = DeviceResourceUsage_Static);

	/**
		@brief		頂点バッファを作成します。
		@param[in]	manager			: 作成に使用する GraphicsManager
		@param[in]	vertexElements	: 頂点データレイアウトを表す VertexElement の配列
		@param[in]	elementsCount	: vertexElements の要素数
		@param[in]	vertexCount		: 頂点の数
		@param[in]	data			: 作成と同時に書き込む初期データ (必要なければ NULL)
		@param[in]	usage			: 頂点バッファリソースの使用方法
		@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
	*/
	static VertexBuffer* Create(GraphicsManager* manager, const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data = NULL, DeviceResourceUsage usage = DeviceResourceUsage_Static);
	
public:

	/**
		@brief		リソースをロックします。
	*/
	ByteBuffer* Lock();
	//ByteBuffer* Lock(int offset, int byteCount);	// テスト用

	/**
		@brief		リソースをアンロックします。
	*/
	void Unlock();

protected:
	VertexBuffer(Device::IVertexBuffer* deviceObj);
	virtual ~VertexBuffer();

private:	// TODO
	friend struct SetVertexBufferCommand;
	Device::IVertexBuffer*	m_deviceObj;
	ByteBuffer				m_lockedBuffer;
	bool					m_initialUpdate;
	friend class Helper;
};

/**
	@brief		VertexBuffer のリソースロックを補助します。
*/
class ScopedVertexBufferLock
{
public:
	ScopedVertexBufferLock(VertexBuffer* indexBuffer)
	{
		m_vertexBuffer = indexBuffer;
		m_lockedBuffer = m_vertexBuffer->Lock();
	}
	
	~ScopedVertexBufferLock()
	{
		m_vertexBuffer->Unlock();
	}

	byte_t* GetData()
	{
		return m_lockedBuffer->GetData();
	}

	size_t GetSize()
	{
		return m_lockedBuffer->GetSize();
	}

private:
	VertexBuffer*	m_vertexBuffer;
	ByteBuffer*		m_lockedBuffer;
};

} // namespace Graphics
} // namespace Lumino
