#include <LuminoEngine/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/GraphicsRHI/Vulkan/VulkanVertexLayout.hpp>

namespace ln {
namespace detail {

//==============================================================================
// VulkanVertexDeclaration

VulkanVertexDeclaration::VulkanVertexDeclaration() {
}

// https://gist.github.com/SaschaWillems/428d15ed4b5d71ead462bc63adffa93a
Result<> VulkanVertexDeclaration::init(const VertexElement* elements, int elementsCount) {
    LN_DCHECK(elements);

    // 事前に binding がどれだけあるのか調べる
    m_maxStreamCount = 0;
    for (int i = 0; i < elementsCount; i++) {
        m_maxStreamCount = std::max(m_maxStreamCount, elements[i].StreamIndex);
        m_elements.push_back(elements[i]);
    }
    m_maxStreamCount++;
    m_bindings.resize(m_maxStreamCount);

    for (int i = 0; i < m_maxStreamCount; i++) {
        m_bindings[i].binding = i; // VkVertexInputAttributeDescription が参照する binding 番号。ひとまず連番

        for (int j = 0; j < elementsCount; j++) {
            if (elements[j].StreamIndex == i) {
                m_bindings[i].inputRate = (elements[j].rate == VertexInputRate::Vertex) ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;
            }
        }

        // m_bindings[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }

    // 実際に値を計算する
    for (int i = 0; i < elementsCount; i++) {
        auto& element = m_elements[i];

        AttributeDescriptionSource attr;
        attr.usage = element.Usage;
        attr.usageIndex = element.UsageIndex;
        attr.binding = element.StreamIndex;
        attr.format = VulkanHelper::LNVertexElementTypeToVkFormat(elements[i].Type);
        attr.offset = m_bindings[attr.binding].stride;
        m_attributeSources.push_back(attr);

        m_bindings[element.StreamIndex].stride += RHIHelper::getVertexElementTypeSize(elements[i].Type);
    }

    return ok();
}

void VulkanVertexDeclaration::onDestroy() {
    IVertexDeclaration::onDestroy();
}

const VulkanVertexDeclaration::AttributeDescriptionSource* VulkanVertexDeclaration::findAttributeDescriptionSource(kokage::AttributeUsage usage, int usageIndex) const {
    // TODO: これ線形探索じゃなくて、map 作った方がいいかも。
    // usage の種類は固定だし、usageIndex も最大 16 あれば十分だし、byte 型 8x16 くらいの Matrix で足りる。
    auto u = IGraphicsHelper::AttributeUsageToElementUsage(usage);
    for (auto& e : m_attributeSources) {
        if (e.usage == u && e.usageIndex == usageIndex) {
            return &e;
        }
    }
    return nullptr;
}

} // namespace detail
} // namespace ln
