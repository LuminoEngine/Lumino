#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
struct WebGPUPipelineVertexLayout;

// NOTE: ShaderPass の情報が無いと WGPUVertexBufferLayout は確定できない。
// Vulkan など他も損か何時だったと思うので、 VertexLayout は Pipeline に統合してよいと思う。
class WebGPUVertexLayout : public IVertexDeclaration {
public:
    WebGPUVertexLayout();
    ~WebGPUVertexLayout() override;
    MaybeResult init(
        WebGPUDevice* device,
        const VertexElement* elements,
        int elementsCount);
    void onDestroy() override;
    MaybeResult createPipelineVertexLayout(
        const WebGPUShaderPass* shaderPass,
        WebGPUPipelineVertexLayout* outLayout);

private:
    std::vector<VertexElement> m_elements;
};

struct DerivedWGPUVertexBufferLayout : public WGPUVertexBufferLayout {
    std::vector<WGPUVertexAttribute> attributeInstances;
    int nextOffset = 0;
    VertexInputRate rate;
};

struct WebGPUPipelineVertexLayout {
    std::vector<DerivedWGPUVertexBufferLayout> bufferLayouts;
};

} // namespace detail
} // namespace ln
