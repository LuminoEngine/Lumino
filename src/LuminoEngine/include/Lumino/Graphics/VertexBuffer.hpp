
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IVertexBuffer; }

/**
	@brief		頂点バッファのクラスです。
*/
class VertexBuffer
	: public GraphicsResource
{
public:

	/** 頂点バッファのバイトサイズを取得します。 */
	int size() const;

	/** 頂点バッファの容量を確保します。 */
	void reserve(int size);

	/** 頂点バッファのサイズを変更します。 */
	void resize(int size);

	/** 頂点バッファが保持するデータにアクセスします。 */
	void* getMappedData();

	/** 頂点が保持するデータにアクセスします。サイズが size より小さい場合はバッファを拡張します。 */
	//void* requestMappedData(int size);

	/** 頂点バッファをクリアします。 */
	void clear();

protected:
	virtual void dispose() override;

LN_INTERNAL_ACCESS:
	VertexBuffer();
	virtual ~VertexBuffer();
	void initialize(size_t bufferSize, GraphicsResourceUsage usage);
	void initialize(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage);
	detail::IVertexBuffer* resolveRHIObject();
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	// TODO
	//friend struct SetVertexBufferCommand;
	Ref<detail::IVertexBuffer>	m_rhiObject;
	size_t m_rhiBufferByteSize;
	GraphicsResourceUsage			m_usage;
	GraphicsResourcePool	m_pool;
	std::vector<byte_t>		m_buffer;
	void*					m_rhiLockedBuffer;
	bool					m_initialUpdate;
	bool					m_modified;
};

} // namespace ln
