#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "Common.hpp"
#include "KPipelineState.hpp"

namespace ln {
namespace kanata {

enum class DrawCommandType {
    BeginRenderPass,
    EndRenderPass,
    DrawPrimitive,
};

struct IDrawCommand {
    DrawCommandType type;
};

struct BeginRenderPassCommand final : public IDrawCommand {
    RenderPass* renderPass;
};

struct EndRenderPassCommand final : public IDrawCommand {
    RenderPass* renderPass;
};

// RenderPass 間での使いまわしは無し。
// GeometryPath と ShaderPass では別々の DrawCommand を使う。
struct DrawCommand final : public IDrawCommand {
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

class DrawCommandList final : public URefObject {
public:
    DrawCommandList(detail::RenderingManager* manager);
    void clear();

    template<class TBatch, class... TArgs>
    TBatch* newCommand(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(TBatch));
        TBatch* data = new (buffer) TBatch(std::forward<TArgs>(args)...);
        addCommand(data);
        return data;
    }

    void submitMeshDrawCommands(GraphicsCommandList* commandList);

private:
    void addCommand(IDrawCommand* command);
    void submitMeshDrawCommand(GraphicsCommandList* commandList, const IDrawCommand* command);

    Ref<detail::LinearAllocator> m_dataAllocator;
    Array<IDrawCommand*> m_commandList; // TODO: LinkedList のほうがいいか？
};

} // namespace kanata
} // namespace ln

