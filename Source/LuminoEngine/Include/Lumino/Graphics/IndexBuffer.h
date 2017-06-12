
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
	int GetIndexCount() const;

	/** インデックスバッファの容量を確保します。 */
	void reserve(int indexCount);

	/** インデックスバッファのサイズを変更します。 */
	void resize(int indexCount);

	/** インデックスバッファが保持するデータにアクセスします。 */
	void* GetMappedData();

	/** インデックスバッファが保持するデータにアクセスします。サイズが indexCount より小さい場合はバッファを拡張します。 */
	void* RequestMappedData(int indexCount);

	/** インデックスバッファをクリアします。 */
	void clear();

	/** インデックスバッファのフォーマットを取得します。 */
	IndexBufferFormat GetIndexFormat() const { return m_format; }

	/** インデックスの値を設定します。 */
	void SetIndex(int index, int vertexIndex);

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS:
	IndexBuffer();
	virtual ~IndexBuffer();
	void initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage, bool sizeConst);
	int GetIndexStride() const;
	Driver::IIndexBuffer* ResolveRHIObject();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);
	void SetFormat(IndexBufferFormat format) { m_format = format; }

private:
	void UpdateFormat(int indexCount);
	bool IsRHIDirect() const { return m_initialUpdate && !m_rhiObject.isNull(); }

	RefPtr<Driver::IIndexBuffer>	m_rhiObject;
	IndexBufferFormat				m_format;
	ResourceUsage					m_usage;
	GraphicsResourcePool			m_pool;
	std::vector<byte_t>				m_buffer;
	void*							m_rhiLockedBuffer;
	bool							m_initialUpdate;
	bool							m_locked;
};

LN_NAMESPACE_END
