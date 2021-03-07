// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {

// NOTE: shader は HLSL をファースト言語で設計しているので、
// こちらの名前も UniformBuffer ではなく ConstantBuffer にしておく。
class ConstantBuffer
    : public Object
    , public IGraphicsResource
{
public:
    // TODO: internal
    const Ref<detail::RHIResource>& rhiObject() const { return m_rhiObject; }
    void* writableData();
    void unmap();
    
protected:
    void onManagerFinalizing() override;
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    ConstantBuffer();
    virtual ~ConstantBuffer();
    bool init(size_t size);

private:
    detail::GraphicsManager* m_manager;
    bool m_profiling;
    Ref<detail::RHIResource> m_rhiObject;
    void* m_mappedData;

    friend class detail::GraphicsResourceInternal;
    friend class detail::GraphicsProfiler;
};

} // namespace ln
