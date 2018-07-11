
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

protected:
	virtual void dispose() override;

LN_CONSTRUCT_ACCESS:
	IndexBuffer();
	virtual ~IndexBuffer();
	void initialize(int indexCount, IndexBufferFormat format, GraphicsResourceUsage usage);
	void initialize(int indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage);

LN_INTERNAL_ACCESS:
	int getIndexStride() const;
	detail::IIndexBuffer* resolveRHIObject();
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	static int getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat::Index16) ? 2 : 4; }
	static int getIndexBufferSize(IndexBufferFormat format, int indexCount) { return getIndexStride(format) * indexCount; }
	static bool shortLifeBuffer(GraphicsResourceUsage usage, GraphicsResourcePool pool) { return usage == GraphicsResourceUsage::Static && pool == GraphicsResourcePool::None; }

	Ref<detail::IIndexBuffer>	m_rhiObject;
	IndexBufferFormat				m_format;
	GraphicsResourceUsage					m_usage;
	GraphicsResourcePool			m_pool;
	std::vector<byte_t>				m_buffer;
	void*							m_rhiLockedBuffer;
	bool							m_initialUpdate;
	bool							m_modified;
};

} // namespace ln
