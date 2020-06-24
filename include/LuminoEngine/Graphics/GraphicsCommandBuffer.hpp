// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class ICommandList;
}

/**  */
class GraphicsCommandBuffer
    : public Object
    , public IGraphicsResource
{
public:

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

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
