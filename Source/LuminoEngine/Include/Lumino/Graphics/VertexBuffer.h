
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN

/**
	@brief		頂点バッファのクラスです。
*/
class VertexBuffer
	: public GraphicsResourceObject
{
public:

	/** 頂点バッファのバイトサイズを取得します。 */
	int getSize() const;

	/** 頂点バッファの容量を確保します。 */
	void reserve(int size);

	/** 頂点バッファのサイズを変更します。 */
	void resize(int size);

	/** 頂点バッファが保持するデータにアクセスします。 */
	void* getMappedData();

	/** 頂点が保持するデータにアクセスします。サイズが size より小さい場合はバッファを拡張します。 */
	void* requestMappedData(int size);

	/** 頂点バッファをクリアします。 */
	void clear();

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS:
	VertexBuffer();
	virtual ~VertexBuffer();
	void initialize(detail::GraphicsManager* manager, size_t bufferSize, const void* data, ResourceUsage usage, bool sizeConst);
	Driver::IVertexBuffer* resolveRHIObject();
	virtual void onChangeDevice(Driver::IGraphicsDevice* device);

private:
	bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	// TODO
	friend struct SetVertexBufferCommand;
	Driver::IVertexBuffer*	m_rhiObject;
	ResourceUsage			m_usage;
	GraphicsResourcePool	m_pool;
	std::vector<byte_t>		m_buffer;
	void*					m_rhiLockedBuffer;
	bool					m_initialUpdate;
	bool					m_locked;
};

LN_NAMESPACE_END
