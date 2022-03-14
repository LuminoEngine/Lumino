#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Base/ObjectPool.hpp>
#include "GLHelper.hpp"

namespace ln {
namespace detail {

// 巨大な UniformBuffer 1つを、 offset と size を使って複数の Binding から使う。
// Vulkan だと転送用バッファでも使いたいが、OpenGL では不要なので GLUniformBuffer 固定でよい。
class GLUniformBufferAllocatorPage : public AbstractLinearAllocatorPage {
public:
    virtual ~GLUniformBufferAllocatorPage();
    Result init(OpenGLDevice* device, size_t size);
    GLUniformBuffer* buffer() const { return m_buffer.get(); }

private:
    std::unique_ptr<GLUniformBuffer> m_buffer;
};

class GLUniformBufferAllocatorPageManager : public LinearAllocatorPageManager {
public:
    GLUniformBufferAllocatorPageManager(OpenGLDevice* device, size_t pageSize);

protected:
    Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
    OpenGLDevice* m_device;
};

class GLUniformBufferAllocator : public AbstractLinearAllocator {
public:
    GLUniformBufferAllocator(GLUniformBufferAllocatorPageManager* manager);
    GLUniformBufferView allocate(size_t size, size_t alignment = 64);

protected:
};

class GLDescriptorObjectPoolManager : public ObjectPoolManager {
public:
    GLDescriptorObjectPoolManager(OpenGLDevice* device);

protected:
    Result onCreateObjects(int32_t count, Array<Ref<RefObject>>* result) override;

private:
    OpenGLDevice* m_device;
};
    


class GLDescriptor : public IDescriptor {
public:
    GLDescriptor();
    Result init(OpenGLDevice* owner/*, GLUniformBufferView view*/);
    //void dispose();
    void bind(const GLShaderDescriptorTable* layout);

protected:
    void onUpdateData(const ShaderDescriptorTableUpdateInfo& data) override;

private:
    GLUniformBufferView m_uniformBufferView;
};

class GLDescriptorPool : public IDescriptorPool {
public:
    GLDescriptorPool();
    Result init(OpenGLDevice* owner, GLShaderPass* shaderPass);
    void dispose() override;
    void reset() override;
    Result allocate(IDescriptor** outDescriptor) override;

private:
    Ref<GLUniformBufferAllocator> m_uniformBufferAllocator;
    Ref<ObjectPool> m_pool;
};

} // namespace detail
} // namespace ln
