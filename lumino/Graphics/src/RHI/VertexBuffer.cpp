/*

- GraphicsResourcePool::None + GraphicsResourceUsage::Static
    - resolve で m_buffer を削除する。省メモリが目的。基本的に使うべきではない。

- GraphicsResourcePool::Managed + GraphicsResourceUsage::Static
    - resolve で m_buffer を削除しない。DeviceReset 時には復元する。デフォルト。

- GraphicsResourcePool::None + GraphicsResourceUsage::Dynamic
    - resolve で m_buffer を削除しない（Dynamic 設定による）。DeviceReset 時には復元しない。毎フレーム新規作成されるような用途。

- GraphicsResourcePool::Managed + GraphicsResourceUsage::Dynamic
    - resolve で m_buffer を削除しない（Dynamic 設定による）。DeviceReset 時には復元する。毎フレームほどではないが、比較的頻繁に発生するイベントで部分変更される用途。（drawText）

バッファはできるだけ確保しないようにしたいところだけど、
GraphicsResourcePool::None + GraphicsResourceUsage::Static 以外はどうしても必要になる。

*/

#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "Backend/GraphicsDeviceContext.hpp"
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>

namespace ln {

//==============================================================================
// VertexBuffer

Ref<VertexBuffer> VertexBuffer::create(size_t bufferSize, GraphicsResourceUsage usage)
{
    return makeObject<VertexBuffer>(bufferSize, usage);
}

Ref<VertexBuffer> VertexBuffer::create(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage)
{
    return makeObject<VertexBuffer>(bufferSize, initialData, usage);
}

VertexBuffer::VertexBuffer()
    : m_manager(nullptr)
    , m_rhiObject(nullptr)
    , m_usage(GraphicsResourceUsage::Static)
    , m_pool(GraphicsResourcePool::Managed)
    , m_primarySize(0)
    , m_buffer()
    //, m_rhiMappedBuffer(nullptr)
    , m_mappedBuffer(nullptr)
    , m_initialUpdate(true)
    , m_modified(false)
    , m_dirtyOffset(0)
    , m_dirtySize(0)
{
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::init(size_t bufferSize, GraphicsResourceUsage usage)
{
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

    m_usage = usage;
    m_modified = true;
    resize(bufferSize);
}

void VertexBuffer::init(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage)
{
    VertexBuffer::init(bufferSize, usage);
    if (initialData) {
        m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createVertexBuffer(GraphicsResourceUsage::Static, bufferSize, initialData);
        m_modified = false;
    }
}

void VertexBuffer::onDispose(bool explicitDisposing)
{
    m_rhiObject.reset();

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

int VertexBuffer::size() const
{
    return m_primarySize;
}

void VertexBuffer::reserve(int size)
{
    m_buffer.reserve(static_cast<size_t>(size));
}

void VertexBuffer::resize(int size)
{
    m_primarySize = size;
    m_buffer.resize(size);

    // map 中ならバッファポインタ再取り出し
    if (m_mappedBuffer) {
        assert(m_usage == GraphicsResourceUsage::Dynamic);
        m_mappedBuffer = m_buffer.data();
    }
}

const void* VertexBuffer::data() const
{
    return m_buffer.data();
}

//void* VertexBuffer::map(MapMode mode)
void* VertexBuffer::writableData(uint64_t offset, uint64_t size)
{
    if (offset == 0 && size == 0) {
        size = this->size();
    }

    m_dirtyOffset = std::min(m_dirtyOffset, offset);
    m_dirtySize = std::max(m_dirtySize, size);

    if (m_mappedBuffer) {
        return m_mappedBuffer;
    }

    // if have not entried the Command List at least once, can rewrite directly with map().
    if (m_initialUpdate && m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None) {
#if 1
        // 使用禁止。アドホック的に使うバッファでは需要あるが、Backend の対策が必要。
        // 今のままここを通すと、 m_rhiObject->map() で HostVisible じゃないから map できないエラーになる。
        LN_NOTIMPLEMENTED();
#else
        if (!m_rhiObject) {
            m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createVertexBuffer(m_usage, size(), nullptr);
        }

        if (m_rhiMappedBuffer == nullptr) {
            m_rhiMappedBuffer = m_rhiObject->map();
        }

        m_modified = true;
        m_mappedBuffer = m_rhiMappedBuffer;
#endif
    } else {
        // prepare for GraphicsResourcePool::None
        if (m_buffer.size() < m_primarySize) {
            m_buffer.resize(m_primarySize);
        }

        m_modified = true;
        m_mappedBuffer = m_buffer.data();
    }

    return m_mappedBuffer;
}

void VertexBuffer::clear()
{
    if (LN_REQUIRE(m_usage == GraphicsResourceUsage::Dynamic)) return;
    m_buffer.clear();
    m_primarySize = 0;
    m_modified = true;
}

void VertexBuffer::setResourceUsage(GraphicsResourceUsage usage)
{
    // Prohibit while direct locking.
    //if (LN_REQUIRE(!m_rhiMappedBuffer)) return;
    if (m_usage != usage) {
        m_usage = usage;
        m_modified = true;
    }
}

void VertexBuffer::setResourcePool(GraphicsResourcePool pool)
{
    m_pool = pool;
}

detail::RHIResource* VertexBuffer::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
	*outModified = m_modified;
    m_mappedBuffer = nullptr;

    if (m_modified) {
		auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();
   //     if (m_rhiMappedBuffer) {
			//m_rhiObject->unmap();
   //         m_rhiMappedBuffer = nullptr;
   //     } else
        {
            detail::ICommandList* commandList = context->rhiResource();
            size_t requiredSize = size();
            if (!m_rhiObject || m_rhiObject->memorySize() != requiredSize/* || m_rhiObject->usage() != m_usage*/) {
                m_rhiObject = device->createVertexBuffer(GraphicsResourceUsage::Static, m_buffer.size(), m_buffer.data());
            } else {
                context->interruptCurrentRenderPassFromResolveRHI();
                detail::RHIResource* rhiObject = m_rhiObject;
                uint64_t offset = m_dirtyOffset;
                commandList->setSubData(rhiObject, 0, m_buffer.data() + m_dirtyOffset, m_dirtySize);
                context->m_vertexBufferDataTransferredSize += m_dirtySize;
            }
        }
    }

    if (LN_ENSURE(m_rhiObject)) return nullptr;

    if (m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None) {
        m_buffer.clear();
        m_buffer.shrink_to_fit();
    }

    m_initialUpdate = false;
    m_modified = false;
    return m_rhiObject;
}

void VertexBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
    if (device) {
        if (m_pool == GraphicsResourcePool::Managed) {
            // data is transferred by the next resolveRHIObject()
            m_modified = true;
        }
    } else {
        m_rhiObject.reset();
    }
}

} // namespace ln
