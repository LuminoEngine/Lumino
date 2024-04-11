// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

/**
 */
class VulkanGraphicsContext : public GraphicsContext {
public:

private:
    VulkanGraphicsContext();
    ~VulkanGraphicsContext() override;
};

} // namespace ln
