#include "Internal.hpp"
#include "GLUniformBuffer.hpp"
#include "OpenGLDeviceContext.hpp"
#include "GLDescriptorPool.hpp"

namespace ln {
namespace detail {

//=============================================================================
// GLUniformBufferAllocatorPage

GLUniformBufferAllocatorPage::~GLUniformBufferAllocatorPage() {
    if (m_buffer) {
        m_buffer->dispose();
        m_buffer = nullptr;
    }
}

Result GLUniformBufferAllocatorPage::init(OpenGLDevice* device, size_t size) {
    m_buffer = std::make_unique<GLUniformBuffer>();
    LN_TRY(m_buffer->init(size));
    return ok();
}

//==============================================================================
// VulkanSingleFrameAllocatorPage

GLUniformBufferAllocatorPageManager::GLUniformBufferAllocatorPageManager(OpenGLDevice* device, size_t pageSize)
    : LinearAllocatorPageManager(pageSize)
    , m_device(device) {
}

Ref<AbstractLinearAllocatorPage> GLUniformBufferAllocatorPageManager::onCreateNewPage(size_t size) {
    auto page = makeRef<GLUniformBufferAllocatorPage>();
    if (!page->init(m_device, size)) {
        return nullptr;
    }
    return page;
}

//==============================================================================
// GLUniformBufferAllocator

GLUniformBufferAllocator::GLUniformBufferAllocator(GLUniformBufferAllocatorPageManager* manager)
    : AbstractLinearAllocator(manager) {
}

GLUniformBufferView GLUniformBufferAllocator::allocate(size_t size, size_t alignment) {
    GLUniformBufferView info = { nullptr, 0 };

    AbstractLinearAllocatorPage* page;
    size_t offset;
    if (allocateCore(size, alignment, &page, &offset)) {
        auto* page2 = static_cast<GLUniformBufferAllocatorPage*>(page);
        info.buffer = page2->buffer();
        info.offset = offset;
        return info;
    }
    else {
        return info;
    }
}


//=============================================================================
// GLDescriptorObjectPoolManager

GLDescriptorObjectPoolManager::GLDescriptorObjectPoolManager(OpenGLDevice* device)
    : ObjectPoolManager(256)
    , m_device(device) {
}

Result GLDescriptorObjectPoolManager::onCreateObjects(int32_t count, Array<Ref<RefObject>>* result) {
    for (int32_t i = 0; i < count; i++) {
        auto obj = makeRef<GLDescriptor>();
        LN_TRY(obj->init(m_device));
        result->push(obj);
    }
    return ok();
}

//=============================================================================
// GLDescriptor

GLDescriptor::GLDescriptor() {
}

Result GLDescriptor::init(OpenGLDevice* owner) {
    return ok();
}

void GLDescriptor::onUpdateData(const ShaderDescriptorTableUpdateInfo& data) {
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// GLDescriptorPool

GLDescriptorPool::GLDescriptorPool() {
}

Result GLDescriptorPool::init(OpenGLDevice* owner, GLShaderPass* shaderPass) {
    m_uniformBufferAllocator = makeRef<GLUniformBufferAllocator>(owner->uniformBufferAllocatorPageManager());
    return ok();
}

void GLDescriptorPool::dispose() {
    if (m_uniformBufferAllocator) {
        m_uniformBufferAllocator->cleanup();
        m_uniformBufferAllocator = nullptr;
    }
}
void GLDescriptorPool::reset() {
    m_uniformBufferAllocator->cleanup();
}

Result GLDescriptorPool::allocate(IDescriptor** outDescriptor) {
    *outDescriptor = static_cast<GLDescriptor*>(m_pool->allocate());
    return ok();
}

} // namespace detail
} // namespace ln
