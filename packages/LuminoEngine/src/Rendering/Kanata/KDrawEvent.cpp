#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

DrawEventList::DrawEventList(detail::RenderingManager* manager)
    : m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager())) {
}

void DrawEventList::clear() {
    m_dataAllocator->cleanup();
    m_drawEventList.clear();
}

void DrawEventList::addDrawEvent(IDrawEvent* drawEvent) {
    m_drawEventList.push(drawEvent);
}

void DrawEventList::submitDrawEvents(GraphicsCommandList* commandList) {
    int count = m_drawEventList.length();
    for (int i = 0; i < count; i++) {
        submitDrawEvent(commandList, m_drawEventList[i]);
    }
}

void DrawEventList::submitDrawEvent(GraphicsCommandList* commandList, const IDrawEvent* drawEvent) {

    commandList->setShaderDescriptor_deprecated(nullptr);

    switch (drawEvent->type) {
        case DrawEventType::BeginRenderPass: {
            auto* cmd = static_cast<const BeginRenderPassDrawEvent*>(drawEvent);
            commandList->beginRenderPass(cmd->renderPass);
            break;
        }
        case DrawEventType::EndRenderPass: {
            auto* cmd = static_cast<const EndRenderPassDrawEvent*>(drawEvent);
            commandList->endRenderPass();
            break;
        }
        case DrawEventType::DrawPrimitive: {
            auto* cmd = static_cast<const DrawEvent*>(drawEvent);

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
                commandList->drawPrimitiveIndexed(
                    cmd->firstIndex,
                    cmd->primitiveCount,
                    cmd->instanceCount,
                    cmd->firstVertex);
            }
            else {
                commandList->drawPrimitive(
                    cmd->firstVertex,
                    cmd->primitiveCount,
                    cmd->instanceCount);
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

