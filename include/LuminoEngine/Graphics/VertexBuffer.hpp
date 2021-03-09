// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {

/** 頂点バッファのクラスです。 */
class VertexBuffer
    : public Object
    , public IGraphicsResource
{
public:
    /**
     * 頂点バッファを作成します。
     * @param[in]   bufferSize  : 頂点バッファのサイズ (バイト単位)
     * @param[in]   usage       : リソースの使用方法
     */
    static Ref<VertexBuffer> create(size_t bufferSize, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

    /**
     * 頂点バッファを作成します。
     * @param[in]   bufferSize  : 頂点バッファのサイズ (バイト単位)
     * @param[in]   initialData : 初期値として書き込む頂点データ
     * @param[in]   usage       : リソースの使用方法
     */
    static Ref<VertexBuffer> create(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

    /** 頂点バッファのバイトサイズを取得します。 */
    int size() const;

    /** 頂点バッファの容量を確保します。 */
    void reserve(int size);

    /** 頂点バッファのサイズを変更します。 */
    void resize(int size);

    const void* data() const;

 //   /** 頂点バッファが保持するデータにアクセスします。このバッファが次の描画に使用されるとき、自動的に unmap されます。  */
 //   void* map(MapMode mode);

	//void unmap() {}	// TODO: 直接転送

    void* writableData(uint64_t lockOffset = 0, uint64_t lockSize = 0);

    /** 頂点バッファをクリアします。 */
    void clear();

    /** リソースの使用方法を変更します。(default: Static) */
    void setResourceUsage(GraphicsResourceUsage usage);

    /** リソースの管理方法を変更します。(default: Managed) */
    void setResourcePool(GraphicsResourcePool pool);

    detail::DescriptorResourceType descriptorResourceType() const override { return detail::DescriptorResourceType_Buffer; }

    Ref<detail::RHIResource> m_rhiObject;
protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    VertexBuffer();
    virtual ~VertexBuffer();

    /** @copydoc create(size_t, GraphicsResourceUsage) */
    void init(size_t bufferSize, GraphicsResourceUsage usage);

    /** @copydoc create(size_t, const void*, GraphicsResourceUsage) */
    void init(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage);

private:
    detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified);
    bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

    detail::GraphicsManager* m_manager;
    GraphicsResourceUsage m_usage;
    GraphicsResourcePool m_pool;
    size_t m_primarySize;

    std::vector<byte_t> m_buffer;
    //void* m_rhiMappedBuffer;
    void* m_mappedBuffer;
    bool m_initialUpdate;
    bool m_modified;

    uint64_t m_dirtyOffset;
    uint64_t m_dirtySize;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
