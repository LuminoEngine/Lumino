// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"

namespace ln {

enum class NativeGraphicsExtensionRenderPassPreCondition
{
	EnsureInside,

	EnsureOutside,
};

class INativeGraphicsInterface
{
public:
	virtual GraphicsAPI getGraphicsAPI() const = 0;
	//virtual NativeGraphicsExtensionRenderPassPreCondition getRenderPassPreCondition() const = 0;

	//virtual void configureExtension(NativeGraphicsExtensionRenderPassPreCondition renderPassPreCondition) = 0;	// onLoaded で実行する


private:
};

class INativeGraphicsExtension
{
public:
    // カレントの Scene rendering のデフォルトターゲット
    // TODO: INativeGraphicsInterface に移動したい
    GraphicsCommandList* graphicsContext = nullptr;
    RenderTargetTexture* renderTarget = nullptr;
    DepthBuffer* depthBuffer = nullptr;

	virtual NativeGraphicsExtensionRenderPassPreCondition getRenderPassPreCondition() const = 0;

	// 初回ロード時やDeviceReset時に呼び出される。
	virtual void onLoaded(INativeGraphicsInterface* nativeInterface) = 0;
	virtual void onUnloaded(INativeGraphicsInterface* nativeInterface) = 0;

    /*
        Vulkan:
            VkCommandBuffer は記録状態であり、RenderPass も開始されている。
			Viewport と Scissor は vkCmdSetViewport, vkCmdSetScissor により設定済み。
    */
    virtual void onRender(INativeGraphicsInterface* nativeInterface) = 0;
};

} // namespace ln
