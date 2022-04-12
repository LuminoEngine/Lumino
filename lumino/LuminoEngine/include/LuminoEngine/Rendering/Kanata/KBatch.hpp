#pragma once
#include "Common.hpp"
#include "KPipelineState.hpp"

namespace ln {
namespace kanata {

struct BatchElement final {
    std::array<VertexBuffer*, detail::MaxVertexStreams> vertexBuffers;
    IndexBuffer* indexBuffer;
    int32_t firstIndex;  // Indexed only
    int32_t firstVertex; // No indexed only
    int32_t primitiveCount;
    int32_t instanceCount;

    BatchElement()
        : vertexBuffers({})
        , indexBuffer(nullptr)
        , firstIndex(0)
        , firstVertex(0)
        , primitiveCount(0)
        , instanceCount(0)
     {}
};

struct Batch {
public:
    Array<BatchElement> elemets;    // TODO: SSOArray
    VertexLayout* vertexLayout;
    PrimitiveTopology primitiveTopology;
    Material* material = nullptr;
    Matrix worldTransform;
    detail::SkeletonInstance* skeleton = nullptr;
    detail::BuiltinEffectData builtinEffectData;
    //subsetIndex
    //LODIndex
    //castshadow
    //Selectable

    // 今のところ blit 用。他ではあまり使ってほしくないところ。
    RenderPass* renderPass;

private:
};

} // namespace kanata
} // namespace ln

