// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class IRenderPass;
}

/**  */
class RenderPass
    : public GraphicsResource
{
public:

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	RenderPass();
    virtual ~RenderPass();

    /** @copydoc create(int, int) */
    void init();

private:
    detail::IRenderPass* resolveRHIObject(GraphicsContext* context, bool* outModified);

    Ref<detail::IRenderPass> m_rhiObject;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
