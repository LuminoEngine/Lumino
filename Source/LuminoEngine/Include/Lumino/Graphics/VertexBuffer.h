
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
	int GetSize() const;

	/** 頂点バッファの容量を確保します。 */
	void Reserve(int size);

	/** 頂点バッファのサイズを変更します。 */
	void Resize(int size);

	/** 頂点バッファが保持するデータにアクセスします。 */
	void* GetMappedData();

	/** 頂点が保持するデータにアクセスします。サイズが size より小さい場合はバッファを拡張します。 */
	void* RequestMappedData(int size);

	/** 頂点バッファをクリアします。 */
	void Clear();

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS:
	VertexBuffer();
	virtual ~VertexBuffer();
	void Initialize(detail::GraphicsManager* manager, size_t bufferSize, const void* data, ResourceUsage usage, bool sizeConst);
	Driver::IVertexBuffer* ResolveRHIObject();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	bool IsRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

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
