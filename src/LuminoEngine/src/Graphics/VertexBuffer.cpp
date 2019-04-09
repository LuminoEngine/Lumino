﻿/*

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
#include "../Engine/RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/VertexBuffer.hpp>

namespace ln {

//==============================================================================
// VertexBuffer

Ref<VertexBuffer> VertexBuffer::create(size_t bufferSize, GraphicsResourceUsage usage)
{
    return newObject<VertexBuffer>(bufferSize, usage);
}

Ref<VertexBuffer> VertexBuffer::create(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage)
{
    return newObject<VertexBuffer>(bufferSize, initialData, usage);
}

VertexBuffer::VertexBuffer()
    : m_rhiObject(nullptr)
    , m_usage(GraphicsResourceUsage::Static)
    , m_pool(GraphicsResourcePool::Managed)
    , m_primarySize(0)
    , m_buffer()
    , m_rhiMappedBuffer(nullptr)
    , m_mappedBuffer(nullptr)
    , m_initialUpdate(true)
    , m_modified(false)
{
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::init(size_t bufferSize, GraphicsResourceUsage usage)
{
    GraphicsResource::init();
    m_usage = usage;
    m_modified = true;
    resize(bufferSize);
}

void VertexBuffer::init(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage)
{
    VertexBuffer::init(bufferSize, usage);
    if (initialData) {
        m_rhiObject = manager()->deviceContext()->createVertexBuffer(m_usage, bufferSize, initialData);
        m_modified = false;
    }
}

void VertexBuffer::onDispose(bool explicitDisposing)
{
    m_rhiObject.reset();
    GraphicsResource::onDispose(explicitDisposing);
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
}

void* VertexBuffer::map(MapMode mode)
{
    if (m_mappedBuffer) {
        return m_mappedBuffer;
    }

    // if have not entried the Command List at least once, can rewrite directly with map().
    if (m_initialUpdate && m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None) {
        if (!m_rhiObject) {
            m_rhiObject = manager()->deviceContext()->createVertexBuffer(m_usage, size(), nullptr);
        }

        if (m_rhiMappedBuffer == nullptr) {
            m_rhiMappedBuffer = manager()->deviceContext()->map(m_rhiObject);
        }

        m_modified = true;
        m_mappedBuffer = m_rhiMappedBuffer;
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
    if (LN_REQUIRE(!m_rhiMappedBuffer)) return;
    if (m_usage != usage) {
        m_usage = usage;
        m_modified = true;
    }
}

void VertexBuffer::setResourcePool(GraphicsResourcePool pool)
{
    m_pool = pool;
}

detail::IVertexBuffer* VertexBuffer::resolveRHIObject()
{
    m_mappedBuffer = nullptr;

    if (m_modified) {
        detail::IGraphicsDevice* device = manager()->deviceContext();
        if (m_rhiMappedBuffer) {
            device->unmap(m_rhiObject);
            m_rhiMappedBuffer = nullptr;
        } else {
            size_t requiredSize = size();
            if (!m_rhiObject || m_rhiObject->getBytesSize() != requiredSize || m_rhiObject->usage() != m_usage) {
                m_rhiObject = device->createVertexBuffer(m_usage, m_buffer.size(), m_buffer.data());
            } else {
                detail::RenderBulkData data(m_buffer.data(), m_buffer.size());
                detail::IVertexBuffer* rhiObject = m_rhiObject;
                LN_ENQUEUE_RENDER_COMMAND_3(
                    VertexBuffer_SetSubData, manager(), detail::IGraphicsDevice*, device, detail::RenderBulkData, data, Ref<detail::IVertexBuffer>, rhiObject, {
                        device->setSubData(rhiObject, 0, data.data(), data.size());
                    });
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
