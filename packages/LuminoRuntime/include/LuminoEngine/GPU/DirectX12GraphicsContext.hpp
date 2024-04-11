// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

/**
 */
class DirectX12GraphicsContext : public GraphicsContext {
public:

private:
    DirectX12GraphicsContext();
    ~DirectX12GraphicsContext() override;
};

} // namespace ln
