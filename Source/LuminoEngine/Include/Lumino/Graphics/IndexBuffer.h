
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN

/**
	@brief		インデックスバッファのクラスです。
*/
class IndexBuffer
	: public GraphicsResourceObject
{
public:

	/** インデックスの数を取得します。 */
	int getIndexCount() const;

	/** インデックスバッファの容量を確保します。 */
	void reserve(int indexCount);

	/** インデックスバッファのサイズを変更します。 */
	void resize(int indexCount);

	/** インデックスバッファが保持するデータにアクセスします。 */
	void* getMappedData();

	/** インデックスバッファが保持するデータにアクセスします。サイズが indexCount より小さい場合はバッファを拡張します。 */
	void* requestMappedData(int indexCount);

	/** インデックスバッファをクリアします。 */
	void clear();

	/** インデックスバッファのフォーマットを取得します。 */
	IndexBufferFormat getIndexFormat() const { return m_format; }

	/** インデックスの値を設定します。 */
	void setIndex(int index, int vertexIndex);

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS:
	IndexBuffer();
	virtual ~IndexBuffer();
	void initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage, bool sizeConst);
	int getIndexStride() const;
	Driver::IIndexBuffer* resolveRHIObject();
	virtual void onChangeDevice(Driver::IGraphicsDevice* device) override;
	void setFormat(IndexBufferFormat format) { m_format = format; }

private:
	void updateFormat(int indexCount);
	bool isRHIDirect() const { return m_initialUpdate && !m_rhiObject.isNull(); }

	Ref<Driver::IIndexBuffer>	m_rhiObject;
	IndexBufferFormat				m_format;
	ResourceUsage					m_usage;
	GraphicsResourcePool			m_pool;
	std::vector<byte_t>				m_buffer;
	void*							m_rhiLockedBuffer;
	bool							m_initialUpdate;
	bool							m_locked;
};

LN_NAMESPACE_END
