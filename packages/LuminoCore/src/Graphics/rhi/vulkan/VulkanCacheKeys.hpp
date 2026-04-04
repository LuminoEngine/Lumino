#pragma once

#include <LuminoBase/Types.hpp>
#include <vulkan/vulkan.h>
#include <vector>

namespace ln::rhi::vulkan {

// ------ RenderPass Cache --------------------------------------------------------------------------------------------------------

struct RenderPassKey {
    struct ColorAttachment {
        VkFormat format;
        VkAttachmentLoadOp loadOp;
        bool operator==(const ColorAttachment& other) const;
    };

    std::vector<ColorAttachment> colorAttachments;
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
    std::vector<VkImageView> attachments;
    u32 width = 0, height = 0;

    bool operator==(const FramebufferKey& o) const;
};

struct FramebufferKeyHash {
    size_t operator()(const FramebufferKey& key) const;
};

} // namespace ln::rhi::vulkan
