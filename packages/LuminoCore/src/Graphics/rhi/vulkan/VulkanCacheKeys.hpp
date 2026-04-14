#pragma once

#include <LuminoBase/Types.hpp>
#include <LuminoBase/SmallVector.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <vulkan/vulkan.h>

namespace ln::rhi::vulkan {

// ------ RenderPass Cache --------------------------------------------------------------------------------------------------------

struct RenderPassKey {
    struct ColorAttachment {
        VkFormat format;
        VkAttachmentLoadOp loadOp;
        bool isSwapchainBackbuffer = false;
        bool operator==(const ColorAttachment& other) const;
    };

    SmallVector<ColorAttachment, kMaxMultiRenderTargets> colorAttachments;
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;
    VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    bool operator==(const RenderPassKey& other) const;
};

struct RenderPassKeyHash {
    size_t operator()(const RenderPassKey& key) const;
};

// ------ Framebuffer Cache ------------------------------------------------------------------------------------------------------

struct FramebufferKey {
    VkRenderPass renderPass = VK_NULL_HANDLE;
    SmallVector<VkImageView, kMaxMultiRenderTargets + 1> attachments;
    uint32_t width = 0, height = 0;

    bool operator==(const FramebufferKey& o) const;
};

struct FramebufferKeyHash {
    size_t operator()(const FramebufferKey& key) const;
};

} // namespace ln::rhi::vulkan
