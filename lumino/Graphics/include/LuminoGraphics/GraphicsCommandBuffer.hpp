// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class ICommandList;
class IDescriptorPool;
class SingleFrameUniformBufferAllocator;
} // namespace detail

class GraphicsCommandList : public RefObject {
public:
    GraphicsCommandList();




    /** @defgroup TODO: Internal */
    /** @{ */

    void init(detail::GraphicsManager* manager);
    void dispose();
    const Ref<detail::ICommandList>& rhiResource() const { return m_rhiResource; }
    void reset();
    detail::ConstantBufferView allocateUniformBuffer(size_t size);
    Ref<detail::SingleFrameUniformBufferAllocator> m_singleFrameUniformBufferAllocator;

    detail::ShaderSecondaryDescriptor* acquireShaderDescriptor(Shader* shader);

    detail::IDescriptorPool* getDescriptorPool(ShaderPass* shaderPass);

    void* allocateBulkData(size_t size);

    void enterRenderState();
    void leaveRenderState();

    // Profiling
    int32_t m_drawCall;
    int64_t m_vertexBufferDataTransferredSize;
    int64_t m_indexBufferDataTransferredSize;
    int64_t m_textureDataTransferredSize;

    /** @} */

private:
    struct ShaderPassDescriptorPair {
        Ref<ShaderPass> shaderPass; // m_usingDescriptorSetsPools で持っている DescriptorSetsPool は ShaderPass への強い参照を持たないので、これでカバーする
        Ref<detail::IDescriptorPool> descriptorPool;
    };

    Ref<detail::ICommandList> m_rhiResource;
    Ref<detail::LinearAllocator> m_allocator;
    size_t m_uniformBufferOffsetAlignment;

    std::vector<ShaderPassDescriptorPair> m_usingDescriptorPools;
};

} // namespace ln
