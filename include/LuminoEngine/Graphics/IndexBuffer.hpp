
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IIndexBuffer; }

/** インデックスバッファのクラスです。 */
class IndexBuffer
	: public GraphicsResource
{
public:
    /**
     * インデックスバッファを作成します。
     * @param[in]	indexCount		: インデックスの数
     * @param[in]	format			: インデックスバッファのフォーマット
     * @param[in]	usage			: リソースの使用方法
     */
    static Ref<IndexBuffer> create(int indexCount, IndexBufferFormat format, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

    /**
     * インデックスバッファを作成します。
     * @param[in]	indexCount		: インデックスの数
     * @param[in]	format			: インデックスバッファのフォーマット
     * @param[in]	initialData		: 初期値として設定するインデックスデータ
     * @param[in]	usage			: リソースの使用方法
     */
    static Ref<IndexBuffer> create(int indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

	/** インデックスの数を取得します。 */
	int size() const;

	/** インデックスバッファのバイトサイズを取得します。 */
	int bytesSize() const;

	/** インデックスバッファの容量を確保します。 */
	void reserve(int indexCount);

	/** インデックスバッファのサイズを変更します。 */
	void resize(int indexCount);

	/** インデックスバッファが保持するデータにアクセスします。 */
	void* map(MapMode mode);

	/** インデックスバッファをクリアします。 */
	void clear();

	/** インデックスバッファのフォーマットを変更します。 */
	void setFormat(IndexBufferFormat format);

	/** インデックスバッファのフォーマットを取得します。 */
	IndexBufferFormat format() const { return m_format; }

	/** インデックスの値を設定します。 */
	void setIndex(int index, int vertexIndex);

    /** インデックスの値を取得します。 */
    int index(int index);

	/** リソースの使用方法を変更します。(default: Static) */
	void setResourceUsage(GraphicsResourceUsage usage);

	/** リソースの管理方法を変更します。(default: Managed) */
	void setResourcePool(GraphicsResourcePool pool);

protected:
	virtual void onDispose(bool explicitDisposing) override;

LN_CONSTRUCT_ACCESS:
	IndexBuffer();
	virtual ~IndexBuffer();

    /** @copydoc create(int, IndexBufferFormat, GraphicsResourceUsage) */
	void init(int indexCount, IndexBufferFormat format, GraphicsResourceUsage usage);

    /** @copydoc create(int, IndexBufferFormat, const void*, GraphicsResourceUsage) */
	void init(int indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage);

LN_INTERNAL_ACCESS:
	int getIndexStride() const;
	detail::IIndexBuffer* resolveRHIObject();
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

private:
	bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	static int getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat::UInt16) ? 2 : 4; }
	static int getIndexBufferSize(IndexBufferFormat format, int indexCount) { return getIndexStride(format) * indexCount; }
	static bool shortLifeBuffer(GraphicsResourceUsage usage, GraphicsResourcePool pool) { return usage == GraphicsResourceUsage::Static && pool == GraphicsResourcePool::None; }

	Ref<detail::IIndexBuffer>	m_rhiObject;
	IndexBufferFormat				m_format;
	GraphicsResourceUsage					m_usage;
	GraphicsResourcePool			m_pool;
	int m_primaryIndexCount;
	std::vector<byte_t>				m_buffer;
    void*					m_rhiMappedBuffer;
    void*					m_mappedBuffer;
	bool							m_initialUpdate;
	bool							m_modified;
};

} // namespace ln
