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

struct BatchMaterial {
    Material* material;

    // 以下、Material の継承を解決した、Batch 生成時点で確定しているステート。
    // SceneRenderPass で必要に応じてオーバーライドされたあと、最終的に確定する。 
    BlendMode blendMode;
    CullMode cullingMode;
    bool depthTestEnabled;
    bool depthWriteEnabled;
    ShadingModel shadingModel;
};

struct Batch {
public:
    BatchElement* elemets2;
    uint8_t elementsCount;
    VertexLayout* vertexLayout;
    PrimitiveTopology primitiveTopology;
    //Material* material = nullptr;
    BatchMaterial material;
    Matrix worldTransform;
    detail::SkeletonInstance* skeleton = nullptr;
    detail::BuiltinEffectData builtinEffectData;
    //subsetIndex
    //LODIndex
    //castshadow
    //Selectable

    // For TextRendering
    Texture* overrideTexture = nullptr;
    SamplerState* overrideSamplerState = nullptr;

    // 今のところ blit 用。他ではあまり使ってほしくないところ。
    RenderPass* renderPass;

private:
};

} // namespace kanata
} // namespace ln

