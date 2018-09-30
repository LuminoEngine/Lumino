
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IVertexBuffer; }

/** 頂点バッファのクラスです。 */
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
	void* map(MapMode mode);

	/** 頂点バッファをクリアします。 */
	void clear();

	/** リソースの管理方法を変更します。(default: Managed) */
	void setResourcePool(GraphicsResourcePool pool);

protected:
	virtual void dispose() override;

LN_CONSTRUCT_ACCESS:
	VertexBuffer();
	virtual ~VertexBuffer();

	/**
	 * 頂点バッファを作成します。
	 * @param[in]	bufferSize		: 頂点バッファのサイズ (バイト単位)
	 * @param[in]	usage			: リソースの使用方法
	 */
	void initialize(size_t bufferSize, GraphicsResourceUsage usage);

	/**
	 * 頂点バッファを作成します。
	 * @param[in]	bufferSize		: 頂点バッファのサイズ (バイト単位)
	 * @param[in]	initialData		: 初期値として書き込む頂点データ
	 * @param[in]	usage			: リソースの使用方法
	 */
	void initialize(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage);

LN_INTERNAL_ACCESS:
	detail::IVertexBuffer* resolveRHIObject();
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	Ref<detail::IVertexBuffer>	m_rhiObject;
	GraphicsResourceUsage			m_usage;
	GraphicsResourcePool	m_pool;
	size_t m_primarySize;

	std::vector<byte_t>		m_buffer;
	void*					m_rhiLockedBuffer;
	bool					m_initialUpdate;
	bool					m_modified;
};

} // namespace ln
