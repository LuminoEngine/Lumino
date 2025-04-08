#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUShaderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUVertexLayout.hpp>

namespace ln {
namespace detail {

WGPUVertexFormat toWGPUVertexFormat(VertexElementType format) {
    switch (format) {
        case VertexElementType::Float1:
            return WGPUVertexFormat_Float32;
        case VertexElementType::Float2:
            return WGPUVertexFormat_Float32x2;
        case VertexElementType::Float3:
            return WGPUVertexFormat_Float32x3;
        case VertexElementType::Float4:
            return WGPUVertexFormat_Float32x4;
        case VertexElementType::Ubyte4:
            return WGPUVertexFormat_Uint8x4;
        case VertexElementType::Short2:
            return WGPUVertexFormat_Sint16x2;
        case VertexElementType::Short4:
            return WGPUVertexFormat_Sint16x4;
        default:
            LN_NOTIMPLEMENTED();
            return WGPUVertexFormat_Uint8;
    }
}

WGPUVertexStepMode toWGPUVertexStepMode(VertexInputRate rate) {
    switch (rate) {
        case VertexInputRate::Vertex:
            return WGPUVertexStepMode_Vertex;
        case VertexInputRate::Instance:
            return WGPUVertexStepMode_Instance;
        default:
            LN_NOTIMPLEMENTED();
            return WGPUVertexStepMode_Vertex;
    }
}

WebGPUVertexLayout::WebGPUVertexLayout() {
}

WebGPUVertexLayout::~WebGPUVertexLayout() {
}

MaybeResult_deprecated WebGPUVertexLayout::init(
    WebGPUDevice* device,
    const VertexElement* elements,
    int elementsCount) {
    IVertexDeclaration::init(elements, elementsCount);
    for (int i = 0; i < elementsCount; i++) {
        const VertexElement& element = elements[i];
        m_elements.push_back(element);
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUVertexLayout::onDestroy() {
    IVertexDeclaration::onDestroy();
}

MaybeResult_deprecated WebGPUVertexLayout::createPipelineVertexLayout(
    const WebGPUShaderPass* shaderPass,
    WebGPUPipelineVertexLayout* outLayout) {
    const std::vector<kokage::VertexInputAttribute>& inputAttributes = shaderPass->attributes();

    // まずは最大の StreamIndex 分だけ BufferLayout を確保する
    // (std::vector の allocate 時にポインタが変わらないように)
    for (const VertexElement& attr : m_elements) {
        DerivedWGPUVertexBufferLayout* bufferLayout = nullptr;
        if (outLayout->bufferLayouts.size() <= attr.StreamIndex) {
            outLayout->bufferLayouts.resize(attr.StreamIndex + 1);
            bufferLayout = &outLayout->bufferLayouts.back();
            bufferLayout->rate = attr.rate;
        }
        else {
            bufferLayout = &outLayout->bufferLayouts[attr.StreamIndex];
            if (bufferLayout->rate != attr.rate) {
                // Rate は Stream で一致させなければならない。
                return LN_MAKE_ERROR_deprecated("VertexInputRate mismatch");
            }
        }
    }

    // Build WGPUVertexAttribute
    for (const VertexElement& attr : m_elements) {
        DerivedWGPUVertexBufferLayout* bufferLayout = &outLayout->bufferLayouts[attr.StreamIndex];

        auto itr = std::find_if(
            inputAttributes.begin(),
            inputAttributes.end(),
            [&attr](const kokage::VertexInputAttribute& x) {
                return x.usage == static_cast<kokage::AttributeUsage>(attr.Usage) &&
                    x.index == attr.UsageIndex;
            });
        if (itr != inputAttributes.end()) {
            WGPUVertexAttribute attribute = WGPU_VERTEX_ATTRIBUTE_INIT;
            attribute.format = toWGPUVertexFormat(attr.Type);
            attribute.offset = bufferLayout->nextOffset;
            attribute.shaderLocation = itr->layoutLocation;
            bufferLayout->attributeInstances.push_back(attribute);
        }
        else {
            // VertexLayout の指定にはあるけど、 Shader 側で使われていない場合は無視してOK.
        }

        bufferLayout->nextOffset += RHIHelper::getVertexElementTypeSize(attr.Type);
    }

    // Build WGPUVertexBufferLayout
    for (DerivedWGPUVertexBufferLayout& bufferLayout : outLayout->bufferLayouts) {
        bufferLayout.stepMode = toWGPUVertexStepMode(bufferLayout.rate);
        bufferLayout.arrayStride = bufferLayout.nextOffset;
        bufferLayout.attributeCount = bufferLayout.attributeInstances.size();
        bufferLayout.attributes = bufferLayout.attributeInstances.data();
    }

    return LN_MAKE_SUCCESS();
}

} // namespace detail
} // namespace ln
