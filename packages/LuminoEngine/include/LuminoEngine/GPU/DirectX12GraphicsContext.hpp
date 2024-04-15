// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

/**
 */
class DirectX12GraphicsContext : public GraphicsContext {
public:
    struct Settings {
        PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
        ln::String priorityAdapterName;
    };
    
    static Ref<DirectX12GraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

private:
    DirectX12GraphicsContext();
    ~DirectX12GraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;

    Ref<detail::DX12Device> m_device;
};

} // namespace ln
