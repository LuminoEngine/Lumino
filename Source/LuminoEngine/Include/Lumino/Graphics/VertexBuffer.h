
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		頂点バッファのクラスです。
*/
class VertexBuffer
	: public GraphicsResourceObject
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
	//static VertexBuffer* Create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data = NULL, DeviceResourceUsage usage = DeviceResourceUsage_Static);

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
	//static VertexBuffer* Create(GraphicsManager* manager, const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data = NULL, DeviceResourceUsage usage = DeviceResourceUsage_Static);
	
public:

	/** バッファのバイトサイズを取得します。 */
	int GetSize() const;

	/** バッファの容量を確保します。 */
	void Reserve(int size);

	/** バッファのサイズを変更します。 */
	void Resize(int size);

	void* GetMappedData();

	/**
		@brief		リソースをロックします。
	*/
	//ByteBuffer* GetMappedData();
	//ByteBuffer* Lock(int offset, int byteCount);	// テスト用

	/**
		@brief		リソースをアンロックします。
	*/
	//void Unlock();

LN_INTERNAL_ACCESS:
	VertexBuffer();
	virtual ~VertexBuffer();
	void Initialize(detail::GraphicsManager* manager, size_t bufferSize, const void* data, ResourceUsage usage, bool sizeConst);
	//Driver::IVertexBuffer* GetDeviceObject() const { return m_deviceObj; }
	Driver::IVertexBuffer* ResolveDeviceObject();

	// GraphicsResourceObject interface
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:	// TODO
	bool IsRHIDirect() const { return m_initialUpdate && m_deviceObj != nullptr; }
	 
	friend struct SetVertexBufferCommand;
	Driver::IVertexBuffer*	m_deviceObj;
	//size_t					m_bufferSize;
	ResourceUsage			m_usage;
	GraphicsResourcePool	m_pool;
	std::vector<byte_t>		m_buffer;
	//ByteBuffer				m_lockedBuffer;
	bool					m_initialUpdate;
	bool					m_locked;
	void*					m_rhiLockedBuffer;
};

///**
//	@brief		VertexBuffer のリソースロックを補助します。
//*/
//class ScopedVertexBufferLock
//{
//public:
//	ScopedVertexBufferLock(VertexBuffer* indexBuffer)
//	{
//		m_vertexBuffer = indexBuffer;
//		m_lockedBuffer = m_vertexBuffer->Lock();
//	}
//	
//	~ScopedVertexBufferLock()
//	{
//		m_vertexBuffer->Unlock();
//	}
//
//	byte_t* GetData()
//	{
//		return m_lockedBuffer->GetData();
//	}
//
//	size_t GetSize()
//	{
//		return m_lockedBuffer->GetSize();
//	}
//
//private:
//	VertexBuffer*	m_vertexBuffer;
//	ByteBuffer*		m_lockedBuffer;
//};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
