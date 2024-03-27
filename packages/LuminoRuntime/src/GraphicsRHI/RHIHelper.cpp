#include <LuminoEngine/GraphicsRHI/RHIHelper.hpp>

namespace ln {

//==============================================================================
// RHIHelper
	
size_t RHIHelper::getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex) {
    int size = 0;
    for (int i = 0; i < elementsCount; ++i) {
        if (vertexElements[i].StreamIndex == streamIndex) {
            size += getVertexElementTypeSize(vertexElements[i].Type);
        }
    }
    return size;
}

size_t RHIHelper::getVertexElementTypeSize(VertexElementType type) {
    switch (type) {
        case VertexElementType::Float1:
            return sizeof(float);
        case VertexElementType::Float2:
            return sizeof(float) * 2;
        case VertexElementType::Float3:
            return sizeof(float) * 3;
        case VertexElementType::Float4:
            return sizeof(float) * 4;
        case VertexElementType::Ubyte4:
            return sizeof(unsigned char) * 4;
        case VertexElementType::Color4:
            return sizeof(unsigned char) * 4;
        case VertexElementType::Short2:
            return sizeof(short) * 2;
        case VertexElementType::Short4:
            return sizeof(short) * 4;
        default:
            LN_UNREACHABLE();
            break;
    }
    return 0;
}

size_t RHIHelper::getPixelSize(TextureFormat format) {
    switch (format) {
        case TextureFormat::Unknown:
            return 0;
        case TextureFormat::RGBA8:
            return 4;
        case TextureFormat::RGB8:
            return 3;
        case TextureFormat::RGBA16F:
            return 8;
        case TextureFormat::RGBA32F:
            return 16;
        case TextureFormat::R16F:
            return 2;
        case TextureFormat::R32F:
            return 4;
        case TextureFormat::R32S:
            return 4;
        default:
            LN_UNREACHABLE();
            return 0;
    }
}

} // namespace ln
