#include "Internal.hpp"
#include "OpenGLDeviceContext.hpp"
#include "GLUniformBuffer.hpp"
#include "GLShaderPass.hpp"
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

GLDescriptor::GLDescriptor()
    : m_uniformBufferView() {
}

Result GLDescriptor::init(OpenGLDevice* owner/*, GLUniformBufferView view*/) {
    //m_uniformBufferView = view;
    return ok();
}

void GLDescriptor::onUpdateData(const ShaderDescriptorTableUpdateInfo& data) {
    // OpenGL は DirectX12 や Vulkan のように、各リソースを GPU へマッピングするための専用のインターフェースは無い。
}

void GLDescriptor::bind(const GLShaderPass* shaderPass) {
    const GLShaderDescriptorTable* layout = shaderPass->layout();
    GLuint program = shaderPass->program();

    {
        for (const auto& info : layout->bufferInfos()) {
            const auto& slot = bufferSlot(info.layoutSlotIndex);
            if (LN_ASSERT(slot.object)) return;

            GLUniformBuffer* buf = static_cast<GLUniformBuffer*>(slot.object.get());
            GLuint ubo = buf->ubo();

            // TODO: 超暫定対応。
            // Vulkan は commit までにバッファを unmap すればよいが、OpenGL では glDraw* を呼ぶ前に unmap しなければならない。
            // map/unmap よりも setData のほうがいいかも。
            buf->flush();

            
            // bindingPoint は View のようなもの。
            // ここに対して ubo, offset, size をセットする。
            GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, info.bindingPoint, ubo, slot.offset, info.blockSize));

            // bindingPoint 番目にセットされている ubo を、blockIndex 番目の UniformBuffer として使う
            GL_CHECK(glUniformBlockBinding(program, info.blockIndex, info.bindingPoint));
        }
    }

#if 0


    for (int i = 0; i < m_samplerUniforms.size(); i++) {
        const auto& uniform = m_samplerUniforms[i];
        int unitIndex = i;

        LN_CHECK(uniform.m_textureExternalUnifromIndex >= 0);
        // LN_CHECK(uniform.m_samplerExternalUnifromIndex >= 0);
        GLTextureBase* t = m_externalTextureUniforms[uniform.m_textureExternalUnifromIndex].texture;
        GLSamplerState* samplerState = nullptr;
        if (uniform.m_samplerExternalUnifromIndex >= 0)
            samplerState = m_externalSamplerUniforms[uniform.m_samplerExternalUnifromIndex].samplerState;
        if (!samplerState) {
            samplerState = m_externalTextureUniforms[uniform.m_textureExternalUnifromIndex].samplerState;
        }

        GL_CHECK(glActiveTexture(GL_TEXTURE0 + unitIndex));

        bool mipmap = false;
        bool renderTarget = false;
        if (t) {
            if (t->type() == DeviceTextureType::Texture3D) {
                GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
                GL_CHECK(glBindTexture(GL_TEXTURE_3D, t->id()));
            }
            else {
                GL_CHECK(glBindTexture(GL_TEXTURE_2D, t->id()));
                GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
            }
            mipmap = t->mipmap();
            renderTarget = (t->type() == DeviceTextureType::RenderTarget);
        }
        else {
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
            GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
        }
        // GL_CHECK(glBindSampler(unitIndex, (entry.samplerState) ? entry.samplerState->resolveId(t->mipmap()) : 0));
        GL_CHECK(glBindSampler(unitIndex, (samplerState) ? samplerState->resolveId(mipmap) : 0));
        GL_CHECK(glUniform1i(uniform.uniformLocation, unitIndex));

        if (uniform.isRenderTargetUniformLocation >= 0) {
            GL_CHECK(glUniform1i(uniform.isRenderTargetUniformLocation, (renderTarget) ? 1 : 0));
            // if (t->type() == DeviceTextureType::RenderTarget) {
            //     GL_CHECK(glUniform1i(entry.isRenderTargetUniformLocation, 1));
            // }
            // else {
            //     GL_CHECK(glUniform1i(entry.isRenderTargetUniformLocation, 0));
            // }
        }
    }
#endif
}

//=============================================================================
// GLDescriptorPool

GLDescriptorPool::GLDescriptorPool() {
}

Result GLDescriptorPool::init(OpenGLDevice* owner, GLShaderPass* shaderPass) {
    m_uniformBufferAllocator = makeRef<GLUniformBufferAllocator>(owner->uniformBufferAllocatorPageManager());
    m_pool = makeRef<ObjectPool>(owner->descriptorObjectPoolManager());
    return ok();
}

void GLDescriptorPool::dispose() {
    if (m_uniformBufferAllocator) {
        m_uniformBufferAllocator->cleanup();
        m_uniformBufferAllocator = nullptr;
    }
    IDescriptorPool::dispose();
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
