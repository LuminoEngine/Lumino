// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

/**
 */
class VulkanGraphicsContext : public GraphicsContext {
public:
    struct Settings {
        PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
    };

    static Ref<VulkanGraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

private:
    VulkanGraphicsContext();
    ~VulkanGraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;

    Ref<detail::VulkanDevice> m_device;
};

} // namespace ln
