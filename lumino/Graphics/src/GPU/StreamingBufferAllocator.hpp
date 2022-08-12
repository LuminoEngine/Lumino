#pragma once
#include <LuminoGraphics/GPU/Common.hpp>

namespace ln {
namespace detail {

class StreamingBufferPage : public URefObject {
public:
    enum class Type {
        VertexBuffer,
        IndexBuffer,
    };

    StreamingBufferPage();
    Result init(Type type, size_t elementSize, size_t count);
    const Ref<RefObject>& resource() const { return m_resource; }

private:
    Ref<RefObject> m_resource;
};

class StreamingBufferAllocatorManager : public URefObject {
public:
    StreamingBufferAllocatorManager(StreamingBufferPage::Type type, size_t elementSize);
    size_t pageElementCount() const { return m_pageElementCount; }
    size_t elementSize() const { return m_elementSize; }
    StreamingBufferPage* requestPage();
    void discardPage(StreamingBufferPage* page);

private:
    URef<StreamingBufferPage> createNewPage();

    StreamingBufferPage::Type m_type;
    size_t m_pageElementCount;
    size_t m_elementSize;
    Array<URef<StreamingBufferPage>> m_pagePool;     // page instances
    std::deque<StreamingBufferPage*> m_freePages;   // page references
};

class StreamingBufferAllocator : public URefObject {
public:
    struct View {
        RefObject* resource;
        size_t elementOffsets;
        size_t elementCount;
    };

    StreamingBufferAllocator(StreamingBufferAllocatorManager* manager);

    void cleanup();

    // LinearAllocator とは異なり、現在の Page が要求した count よりも小さいときは、
    // ラージバッファを確保せずに残数を返す。
    // 返されたバッファのサイズが足りないときは、呼び出し側で繰り返し allocate() を呼ぶ必要がある。
	View allocate(size_t elementCount);

private:
    StreamingBufferAllocatorManager* m_manager;
    size_t m_usedCount;
    StreamingBufferPage* m_currentPage;
    Array<StreamingBufferPage*> m_retiredPages;
};

} // namespace detail
} // namespace ln

