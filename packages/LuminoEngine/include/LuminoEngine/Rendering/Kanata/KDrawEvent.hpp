#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "Common.hpp"
#include "KPipelineState.hpp"

namespace ln {
namespace kanata {

enum class DrawEventType {
    BeginRenderPass,
    EndRenderPass,
    DrawPrimitive,
};

struct IDrawEvent {
    DrawEventType type;
};

struct BeginRenderPassDrawEvent final : public IDrawEvent {
    RenderPass* renderPass;
};

struct EndRenderPassDrawEvent final : public IDrawEvent {
    RenderPass* renderPass;
};

// RenderPass 間での使いまわしは無し。
// GeometryPath と ShaderPass では別々の DrawCommand を使う。
struct DrawEvent final : public IDrawEvent {
    std::array<VertexBuffer*, detail::MaxVertexStreams> vertexBuffers;
    IndexBuffer* indexBuffer;

    // Material によるユーザー指定のステートや、RenderPass 固有のステートを統合したもの。
    //   UE4 では FGraphicsMinimalPipelineStateId として持っていて
    //   ApplyViewOverridesToMeshDrawCommands で ID の発行が行われているようだが、
    //   その時点ではまだ RenderPass は確定していないため RHIPipelineState を作ることはできない。
    //   単にメモリ量削減のためのキャッシュと思われる
    PipelineState pipelineState;
    ShaderPass* shaderPass;

    ShaderDescriptor* shaderDescriptor;
    int32_t firstIndex;     // Indexed only
    int32_t firstVertex;    // No indexed only
    int32_t primitiveCount;
    int32_t instanceCount;
    uint8_t stencilRef;
};

class DrawEventList final : public URefObject {
public:
    DrawEventList(detail::RenderingManager* manager);
    void clear();

    template<class TBatch, class... TArgs>
    TBatch* newDrawEvent(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(TBatch));
        TBatch* data = new (buffer) TBatch(std::forward<TArgs>(args)...);
        addDrawEvent(data);
        return data;
    }

    void submitDrawEvents(GraphicsCommandList* commandList);

private:
    void addDrawEvent(IDrawEvent* drawEvent);
    void submitDrawEvent(GraphicsCommandList* commandList, const IDrawEvent* drawEvent);

    Ref<detail::LinearAllocator> m_dataAllocator;
    Array<IDrawEvent*> m_drawEventList;
};

} // namespace kanata
} // namespace ln

