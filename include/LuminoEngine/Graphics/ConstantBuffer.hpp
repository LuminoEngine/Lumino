// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {

// NOTE: shader は HLSL をファースト言語で設計しているので、
// こちらの名前も UniformBuffer ではなく ConstantBuffer にしておく。
class ConstantBuffer
    : public Object
{
public:
    // TODO: internal
    const Ref<detail::IUniformBuffer>& rhiObject() const { return m_rhiObject; }
    void* writableData();
    void unmap();

LN_CONSTRUCT_ACCESS:
    ConstantBuffer();
    bool init(size_t size);

private:
    Ref<detail::IUniformBuffer> m_rhiObject;
    void* m_mappedData;
};

} // namespace ln
