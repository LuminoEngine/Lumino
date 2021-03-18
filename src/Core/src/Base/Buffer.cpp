
#include "Internal.hpp"
#include <algorithm>
#include <LuminoCore/Base/RefObject.hpp>
#include <LuminoCore/Base/Buffer.hpp>
#include "CRTHelper.h"

namespace ln {

static byte_t g_sharedEmpty[1] = {0};

ByteBuffer::ByteBuffer()
    : m_buffer(g_sharedEmpty)
    , m_capacity(0)
    , m_size(0)
    , m_autoClear(true)
{
}

ByteBuffer::ByteBuffer(int size)
    : ByteBuffer()
{
    resizeInternal(size, m_autoClear);
}

ByteBuffer::ByteBuffer(const void* data, int size)
    : ByteBuffer()
{
    assign(data, size);
}

ByteBuffer::ByteBuffer(const ByteBuffer& buffer)
    : ByteBuffer()
{
    if (!buffer.isEmpty()) {
        assign(buffer.data(), buffer.size());
    }
}

ByteBuffer::ByteBuffer(ByteBuffer&& buffer)
    : ByteBuffer()
{
    swap(buffer);
}

ByteBuffer::~ByteBuffer()
{
	free();
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& buffer)
{
    if (this != &buffer) {
        free();

        if (!buffer.isEmpty()) {
            assign(buffer.data(), buffer.size());
        }
    }

    return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& buffer)
{
    free();
    swap(buffer);
    return *this;
}

//void ByteBuffer::alloc(int size, bool zeroClear)
//{
//
//}

void ByteBuffer::assign(const void* data, int size)
{
    if (LN_REQUIRE(data != nullptr)) return;
    if (size <= 0) return;

    resizeInternal(size, m_autoClear);
    memcpy_s(m_buffer, size, data, size);
}

//void ByteBuffer::append(const void* data, int size)
//{
//    if (LN_REQUIRE(data != nullptr)) return;
//    if (size <= 0) return;
//
//    int oldSize = m_size;
//    resizeInternal(size, m_autoClear);
//    memcpy_s(m_buffer + oldSize, size, data, size);
//}

void ByteBuffer::resize(int size)
{
    resizeInternal(size, m_autoClear);
}

void ByteBuffer::clear()
{
    memset(m_buffer, 0, m_size);
}

void ByteBuffer::fill(const byte_t& value)
{
    for (int i = 0; i < m_size; i++) {
        m_buffer[i] = value;
    }
}

void ByteBuffer::swap(ByteBuffer& buffer) LN_NOEXCEPT
{
    std::swap(m_buffer, buffer.m_buffer);
    std::swap(m_capacity, buffer.m_capacity);
    std::swap(m_size, buffer.m_size);
}

void ByteBuffer::free()
{
    if (m_buffer && m_buffer != g_sharedEmpty) {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
    m_capacity = 0;
    m_size = 0;
}

void ByteBuffer::resizeInternal(int size, bool clear)
{
    if (m_buffer == g_sharedEmpty) // 未割り当てなら new するだけ。
    {
        m_buffer = LN_NEW byte_t[size];
        m_capacity = size;
        m_size = size;

        if (clear) {
            memset(m_buffer, 0, m_capacity);
        }
    } else {
        byte_t* newBuf = m_buffer;
        size_t newSize = size;
        size_t newCapacity = m_capacity;
        if (size > m_capacity) {
            newBuf = LN_NEW byte_t[size];
            newCapacity = size;

            if (clear) {
                memset(newBuf, 0, newCapacity);
            }
        }

        // 元のバッファがあればコピー。その後元のバッファを破棄
        if (newBuf != m_buffer) {
            memcpy_s(newBuf, size, m_buffer, LN_MIN(size, m_size));
            free();
        }

        // 新しいバッファに差し替え
        m_buffer = newBuf;
        m_capacity = newCapacity;
        m_size = newSize;
    }
}

bool operator==(const ByteBuffer& lhs, const ByteBuffer& rhs)
{
    if (lhs.data() == rhs.data()) return true;
    if (lhs.size() != rhs.size()) return false;
    return memcmp(lhs.data(), rhs.data(), lhs.size()) == 0;
}

} // namespace ln
