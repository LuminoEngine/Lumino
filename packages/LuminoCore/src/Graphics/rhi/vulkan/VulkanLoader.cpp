// volk の実装をこの翻訳単位に取り込む。VOLK_IMPLEMENTATION は
// プロジェクト全体でこの 1 か所だけで定義すること。
#define VOLK_IMPLEMENTATION
#include "VulkanLoader.hpp"

#include <LuminoBase/Logger.hpp>

namespace ln::rhi::vulkan {

bool loadVulkanLoader() {
    // マジックスタティックにより、初回呼び出し時に 1 度だけロードされる。
    static const bool loaded = [] {
        const VkResult r = volkInitialize();
        if (r != VK_SUCCESS) {
            // ローダーが存在しない、または壊れている。異常終了ではなく
            // 「この環境では Vulkan を使えない」という通常の分岐として扱う。
            LN_LOG_INFO("Vulkan loader is not available. (volkInitialize: %d)", static_cast<int>(r));
        }
        return r == VK_SUCCESS;
    }();
    return loaded;
}

} // namespace ln::rhi::vulkan
