// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class ICommandList;
class SingleFrameUniformBufferAllocator;
}

#if 0
/**  */
class GraphicsCommandBuffer
    : public Object
    , public IGraphicsResource
{
public:
    detail::UniformBufferView allocateUniformBuffer(size_t size);
    void end();

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	GraphicsCommandBuffer();
    virtual ~GraphicsCommandBuffer();

    /** @copydoc create(int, int) */
    void init();

private:
    detail::ICommandList* resolveRHIObject(GraphicsContext* context, bool* outModified);

    detail::GraphicsManager* m_manager;
    Ref<detail::ICommandList> m_rhiObject;
    Ref<detail::SingleFrameUniformBufferAllocator> m_singleFrameUniformBufferAllocator;

    friend class detail::GraphicsResourceInternal;
};
#endif

} // namespace ln
