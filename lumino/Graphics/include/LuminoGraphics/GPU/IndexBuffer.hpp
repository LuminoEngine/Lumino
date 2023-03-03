// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {

/** インデックスバッファのクラスです。 */
class IndexBuffer
    : public Object
    , public IGraphicsResource
{
public:
    /**
     * インデックスバッファを作成します。
     * @param[in]   indexCount  : インデックスの数
     * @param[in]   format      : インデックスバッファのフォーマット
     * @param[in]   usage       : リソースの使用方法
     */
    static Ref<IndexBuffer> create(int indexCount, IndexBufferFormat format, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

    /**
     * インデックスバッファを作成します。
     * @param[in]   indexCount  : インデックスの数
     * @param[in]   format      : インデックスバッファのフォーマット
     * @param[in]   initialData : 初期値として設定するインデックスデータ
     * @param[in]   usage       : リソースの使用方法
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

    /** インデックスバッファが保持するデータにアクセスします。このバッファが次の描画に使用されるとき、自動的に unmap されます。 */
    void* map(MapMode mode);

	void unmap() {}	// TODO: 直接転送

    /** インデックスバッファをクリアします。 */
    void clear();

    /** インデックスバッファのフォーマットを変更します。 */
    void setFormat(IndexBufferFormat format);

    /** インデックスバッファのフォーマットを取得します。 */
    IndexBufferFormat format() const { return m_format; }

    /** インデックスバッファのフォーマットを取得します。 */
    int stride() const;

    /** インデックスの値を設定します。 */
    void setIndex(int index, int vertexIndex);

    /** インデックスの値を取得します。 */
    int index(int index);

    /** リソースの使用方法を変更します。(default: Static) */
    void setResourceUsage(GraphicsResourceUsage usage);

    /** リソースの管理方法を変更します。(default: Managed) */
    void setResourcePool(GraphicsResourcePool pool);

protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    IndexBuffer();
    virtual ~IndexBuffer();

    /** @copydoc create(int, IndexBufferFormat, GraphicsResourceUsage) */
    void init(int indexCount, IndexBufferFormat format, GraphicsResourceUsage usage);

    /** @copydoc create(int, IndexBufferFormat, const void*, GraphicsResourceUsage) */
    void init(int indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage);

	//void init(int indexCount, GraphicsResourceUsage usage);

private:
    int getIndexStride() const;
    detail::RHIResource* resolveRHIObject(GraphicsCommandList* context, bool* outModified);
    bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

    static int getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat::UInt16) ? 2 : 4; }
    static int getIndexBufferSize(IndexBufferFormat format, int indexCount) { return getIndexStride(format) * indexCount; }
    static bool shortLifeBuffer(GraphicsResourceUsage usage, GraphicsResourcePool pool) { return usage == GraphicsResourceUsage::Static && pool == GraphicsResourcePool::None; }

    detail::GraphicsManager* m_manager;
    URef<detail::RHIResource> m_rhiObject;
    IndexBufferFormat m_format;
    GraphicsResourceUsage m_usage;
    GraphicsResourcePool m_pool;
    int m_primaryIndexCount;
    std::vector<byte_t> m_buffer;
    //void* m_rhiMappedBuffer;
    void* m_mappedBuffer;
    bool m_initialUpdate;
    bool m_modified;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
