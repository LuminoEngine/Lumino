// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class ICommandList;
}

/**  */
class CommandBuffer
    : public GraphicsResource
{
public:

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	CommandBuffer();
    virtual ~CommandBuffer();

    /** @copydoc create(int, int) */
    void init();

private:
    detail::ICommandList* resolveRHIObject(GraphicsContext* context, bool* outModified);

    Ref<detail::ICommandList> m_rhiObject;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
