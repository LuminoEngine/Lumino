#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

DrawCommandList::DrawCommandList(detail::RenderingManager* manager)
    : m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager())) {
}

void DrawCommandList::clear() {
    m_dataAllocator->cleanup();
    m_commandList.clear();
}

void DrawCommandList::addCommand(IDrawCommand* command) {
    m_commandList.push(command);
}

void DrawCommandList::submitMeshDrawCommands(GraphicsCommandList* commandList) {
    int count = m_commandList.length();
    for (int i = 0; i < count; i++) {
        submitMeshDrawCommand(commandList, m_commandList[i]);
    }
}

void DrawCommandList::submitMeshDrawCommand(GraphicsCommandList* commandList, const IDrawCommand* command) {

    commandList->setShaderDescriptor_deprecated(nullptr);

    switch (command->type) {
        case DrawCommandType::BeginRenderPass: {
            auto* cmd = static_cast<const BeginRenderPassCommand*>(command);
            commandList->beginRenderPass(cmd->renderPass);
            break;
        }
        case DrawCommandType::EndRenderPass: {
            auto* cmd = static_cast<const EndRenderPassCommand*>(command);
            commandList->endRenderPass();
            break;
        }
        case DrawCommandType::DrawPrimitive: {
            auto* cmd = static_cast<const DrawCommand*>(command);

            assert(cmd->shaderPass);
            if (cmd->stencilRef != 0) {
                LN_NOTIMPLEMENTED();
            }

            // PipelineState
            commandList->setBlendState(cmd->pipelineState.blendState);
            commandList->setRasterizerState(cmd->pipelineState.rasterizerState);
            commandList->setDepthStencilState(cmd->pipelineState.depthStencilState);
            commandList->setVertexLayout(cmd->pipelineState.vertexLayout);
            commandList->setPrimitiveTopology(cmd->pipelineState.primitiveTopology);
            commandList->setShaderPass(cmd->shaderPass);

            commandList->setShaderDescriptor(cmd->shaderDescriptor);
            commandList->setIndexBuffer(cmd->indexBuffer);

            for (int i = 0; i < cmd->vertexBuffers.size(); i++) {
                if (cmd->vertexBuffers[i]) {
                    commandList->setVertexBuffer(i, cmd->vertexBuffers[i]);
                }
                else {
                    break;
                }
            }

            if (cmd->indexBuffer) {
                commandList->drawPrimitiveIndexed(cmd->firstIndex, cmd->primitiveCount, cmd->instanceCount, cmd->firstVertex);
            }
            else {
                commandList->drawPrimitive(cmd->firstVertex, cmd->primitiveCount, cmd->instanceCount);
            }
            break;
        }
        default:
            LN_UNREACHABLE();
            break;
    }

    commandList->setShaderDescriptor(nullptr);
}

} // namespace kanata
} // namespace ln

