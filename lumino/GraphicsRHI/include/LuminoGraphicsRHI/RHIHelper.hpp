#pragma once
#include "Common.hpp"

namespace ln {

class RHIHelper {
public:
    static size_t getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex);
    static size_t getVertexElementTypeSize(VertexElementType type);
    static size_t getPixelSize(TextureFormat format);
    static IndexBufferFormat selectIndexBufferFormat(int vertexCount) { return (vertexCount > 0xFFFF) ? IndexBufferFormat::UInt32 : IndexBufferFormat::UInt16; }
    static size_t getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat::UInt16) ? 2 : 4; }
    static IndexBufferFormat getIndexBufferFormat(size_t size) { return (size == 2) ? IndexBufferFormat::UInt16 : IndexBufferFormat::UInt32; }

};

} // namespace ln
