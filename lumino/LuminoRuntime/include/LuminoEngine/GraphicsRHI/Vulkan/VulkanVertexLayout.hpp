
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {
	
class VulkanVertexDeclaration : public IVertexDeclaration {
public:
    // 実際の VkVertexInputAttributeDescription を作るための元情報。
    // location は、Pipleline 作成時に m_elements が持っているセマンティクス情報と、ShaderPass が持っているセマンティクス情報とつなげて確定させる。
    struct AttributeDescriptionSource {
        VertexElementUsage usage;
        uint32_t usageIndex;

        uint32_t binding; // DirectX でいうところの StreamIndex。頂点バッファを複数分けるときに使う。
        VkFormat format;
        uint32_t offset;
    };

    VulkanVertexDeclaration();
    Result<> init(const VertexElement* elements, int elementsCount);
    void onDestroy() override;

    const std::vector<VertexElement>& elements() const { return m_elements; }
    uint32_t maxStreamCount() const { return m_maxStreamCount; }
    const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions() const { return m_bindings; }
    const std::vector<AttributeDescriptionSource>& vertexAttributeDescriptionSources() const { return m_attributeSources; }
    const VulkanVertexDeclaration::AttributeDescriptionSource* findAttributeDescriptionSource(kokage::AttributeUsage usage, int usageIndex) const;

private:
    std::vector<VertexElement> m_elements;
    uint32_t m_maxStreamCount;
    std::vector<VkVertexInputBindingDescription> m_bindings;
    std::vector<AttributeDescriptionSource> m_attributeSources;
};

} // namespace detail
} // namespace ln
