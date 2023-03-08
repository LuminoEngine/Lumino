#include <LuminoGraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include <LuminoGraphicsRHI/Vulkan/VulkanBuffers.hpp>
#include <LuminoGraphicsRHI/Vulkan/VulkanTextures.hpp>
#include <LuminoGraphicsRHI/Vulkan/VulkanDescriptorPool.hpp>
#include <LuminoGraphicsRHI/Vulkan/VulkanShaderPass.hpp>
#include <LuminoGraphicsRHI/Vulkan/VulkanCommandList.hpp>
#include "VulkanSingleFrameAllocator.hpp"

namespace ln {
namespace detail {

//==============================================================================
// VulkanCommandList

VulkanCommandList::VulkanCommandList()
    : m_device(nullptr)
    , m_commandBuffer(VK_NULL_HANDLE)
    , m_inFlightFence(VK_NULL_HANDLE)
    , m_transferBufferSingleFrameAllocator(nullptr) {
}

Result<> VulkanCommandList::init(VulkanDevice* owner) {
    LN_CHECK(owner);
    ICommandList::init(owner);
    m_device = owner;

    m_transferBufferSingleFrameAllocator = makeRef<VulkanSingleFrameAllocator>(m_device->transferBufferSingleFrameAllocator());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_device->vulkanCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    LN_VK_CHECK(vkAllocateCommandBuffers(m_device->vulkanDevice(), &allocInfo, &m_commandBuffer));

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    LN_VK_CHECK(vkCreateFence(m_device->vulkanDevice(), &fenceInfo, m_device->vulkanAllocator(), &m_inFlightFence));

    return ok();
}

void VulkanCommandList::onDestroy() {
    // Wait for execution to complete as it may be pending.
    vkWaitForFences(m_device->vulkanDevice(), 1, &m_inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());

    // CommandBuffer must be released before vkResetDescriptorPool.
    if (m_commandBuffer) {
        vkFreeCommandBuffers(m_device->vulkanDevice(), m_device->vulkanCommandPool(), 1, &m_commandBuffer);
        m_commandBuffer = VK_NULL_HANDLE;
    }

    if (m_inFlightFence) {
        vkDestroyFence(m_device->vulkanDevice(), m_inFlightFence, m_device->vulkanAllocator());
        m_inFlightFence = VK_NULL_HANDLE;
    }

    ICommandList::onDestroy();
}

Result<> VulkanCommandList::submit(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore) {
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // 実行を開始する前に待機するセマフォ
    VkSemaphore waitSemaphores[] = { waitSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = (waitSemaphore == VK_NULL_HANDLE) ? 0 : 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    // 実行するコマンド
    VkCommandBuffer commandBuffer = m_commandBuffer;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    // 実行を完了したときに通知されるセマフォ
    VkSemaphore signalSemaphores[] = { signalSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // unsignaled にしておく。vkQueueSubmit で発行した実行が完了したときに signaled になる。
    LN_VK_CHECK(vkResetFences(m_device->vulkanDevice(), 1, &m_inFlightFence));

    LN_VK_CHECK(vkQueueSubmit(m_device->m_graphicsQueue, 1, &submitInfo, m_inFlightFence));

    return ok();
}

VulkanSingleFrameBufferInfo VulkanCommandList::cmdCopyBuffer(size_t size, VulkanBuffer* destination) {
    VulkanSingleFrameBufferInfo bufferInfo = m_transferBufferSingleFrameAllocator->allocate(size);

    // コマンドバッファに乗せる
    VkBufferCopy copyRegion;
    copyRegion.srcOffset = bufferInfo.offset;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(m_commandBuffer, bufferInfo.buffer->nativeBuffer(), destination->nativeBuffer(), 1, &copyRegion);

    VkBufferMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;

    // barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    // barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;  // TODO: ?
    // barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    // barrier.dstAccessMask = VK_ACCESS_UNIFORM_READ_BIT;  // TODO: ?

    barrier.srcQueueFamilyIndex = m_device->graphicsQueueFamilyIndex();
    barrier.dstQueueFamilyIndex = m_device->graphicsQueueFamilyIndex();
    barrier.buffer = destination->nativeBuffer();
    // barrier.offset;
    barrier.size = size;

    vkCmdPipelineBarrier(
        m_commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,     // このパイプラインステージで、1セットのデータが完全に生成されたことを保証する
        VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, // このパイプラインステージがそれを消費することを許可する
        0,
        0,
        nullptr,
        1,
        &barrier, // どのデータをブロック/ブロック解除するかを定義します。
        0,
        nullptr);
    // http://web.engr.oregonstate.edu/~mjb/vulkan/Handouts/PipelineBarriers.2pp.pdf
    // https://stackoverflow.com/questions/48894573/how-to-synchronize-uniform-buffer-updates
    // https://stackoverflow.com/questions/40577047/vulkan-vkcmdpipelinebarrier-for-data-coherence
    // https://chromium.googlesource.com/chromium/src/+/master/gpu/vulkan/vulkan_command_buffer.cc

    // 戻り先で書いてもらう
    return bufferInfo;
}

VulkanSingleFrameBufferInfo VulkanCommandList::cmdCopyBufferToImage(size_t size, int width, int height, VulkanImage* destination) {
    VulkanSingleFrameBufferInfo bufferInfo = m_transferBufferSingleFrameAllocator->allocate(size);

    VkBufferImageCopy region = {};
    region.bufferOffset = bufferInfo.offset;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        1
    };

    vkCmdCopyBufferToImage(
        m_commandBuffer,
        bufferInfo.buffer->nativeBuffer(),
        destination->vulkanImage(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region);

    // 戻り先で書いてもらう
    return bufferInfo;
}

void VulkanCommandList::onSaveExternalRenderState() {
}

void VulkanCommandList::onRestoreExternalRenderState() {
}

void VulkanCommandList::onBeginCommandRecoding() {
    m_transferBufferSingleFrameAllocator->cleanup();

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    VkResult r = vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
    if (r != VK_SUCCESS) {
        LN_LOG_ERROR("vkBeginCommandBuffer failed.");
    }
}

void VulkanCommandList::onEndCommandRecoding() {
    VkResult r = vkEndCommandBuffer(m_commandBuffer);
    if (r != VK_SUCCESS) {
        LN_LOG_ERROR("vkBeginCommandBuffer failed.");
    }
}

void VulkanCommandList::onBeginRenderPass(IRenderPass* renderPass_) {
    auto* renderPass = static_cast<VulkanRenderPass2*>(renderPass_);
    auto& framebuffer = renderPass->framebuffer();
    const auto viewSize = renderPass->framebuffer()->renderTargets()[0]->extentSize();

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass->nativeRenderPass();
    renderPassInfo.framebuffer = renderPass->framebuffer()->nativeFramebuffer();
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent.width = viewSize.width;
    renderPassInfo.renderArea.extent.height = viewSize.height;

    VkClearValue clearValues[MaxMultiRenderTargets + 1] = {};
    uint32_t count = 0;
    // if (testFlag(renderPass->clearFlags(), ClearFlags::Color))
    {
        auto& color = renderPass->clearColor();
        for (uint32_t ii = 0; ii < framebuffer->renderTargets().size(); ii++) {
            if (framebuffer->renderTargets()[ii]) {
                clearValues[count].color = { color.r, color.g, color.b, color.a };
                count++;
            }
        }
    }
    // if ((testFlag(renderPass->clearFlags(), ClearFlags::Depth) || testFlag(renderPass->clearFlags(), ClearFlags::Stencil)))
    {
        if (framebuffer->depthBuffer()) {
            clearValues[count].depthStencil = { renderPass->clearDepth(), renderPass->clearStencil() };
            count++;
        }
    }

    renderPassInfo.clearValueCount = count;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandList::onEndRenderPass(IRenderPass* renderPass) {
    vkCmdEndRenderPass(m_commandBuffer);
}

void VulkanCommandList::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) {
    if (stateDirtyFlags & GraphicsContextStateDirtyFlags_RegionRects) {
        VkViewport viewport;
        viewport.x = state.regionRects.viewportRect.x;
        viewport.y = state.regionRects.viewportRect.height + state.regionRects.viewportRect.y;
        viewport.width = state.regionRects.viewportRect.width;
        viewport.height = -state.regionRects.viewportRect.height; // height マイナスで、DirectX や OpenGL と同じ座標系になる (1.1+, VK_KHR_maintenance1)
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset.x = state.regionRects.scissorRect.x;
        scissor.offset.y = state.regionRects.scissorRect.y;
        scissor.extent.width = state.regionRects.scissorRect.width;
        scissor.extent.height = state.regionRects.scissorRect.height;
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
    }

    if (submitSource == GraphicsContextSubmitSource_Draw) {
        if (pipeline) {
            auto vulkanPipeline = static_cast<VulkanPipeline2*>(pipeline);
            vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->nativePipeline());
        }

        {
            // VulkanVertexDeclaration* layout = static_cast<VulkanVertexDeclaration*>(state.pipelineState.vertexDeclaration);

            // std::array<VkBuffer, MaxVertexStreams> vertexBufferHandles;
            // std::array<VkDeviceSize, MaxVertexStreams> vertexBufferOffsets;
            // int i = 0;
            // for (; i < layout->elements>; i++) {
            //	if (state.primitive.vertexBuffers[i]) {
            //		auto* vertexBuffer = static_cast<VulkanVertexBuffer*>(state.primitive.vertexBuffers[i]);
            //		vertexBufferHandles[i] = vertexBuffer->vulkanBuffer();
            //		vertexBufferOffsets[i] = 0;
            //	}
            //	else {
            //		vertexBufferHandles[i] = VK_NULL_HANDLE;
            //		vertexBufferOffsets[i] = 0;
            //	}
            // }
            // vkCmdBindVertexBuffers(m_commandBuffer->vulkanCommandBuffer(), 0, MaxVertexStreams, vertexBufferHandles.data(), vertexBufferOffsets.data());

            // VertexLayout で要求されている StreamIndex へは必ず VertexBuffer を Bind する必要がある。
            // 例えば VertexLayout で [0, 1, 3] といったように途中抜けの StreamIndex を使うことは可能だが、
            // これらに VK_NULL_HANDLE を Bind することはできない。
            // 未使用を丁寧にリセットするつもりで VK_NULL_HANDLE を指定しても検証エラーとなるため注意。
            // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBindVertexBuffers.html
            int vbCount = 0;
            for (int i = 0; i < state.primitive.vertexBuffers.size(); i++) {
                if (state.primitive.vertexBuffers[i]) {
                    auto* vertexBuffer = static_cast<VulkanVertexBuffer*>(state.primitive.vertexBuffers[i]);
                    VkBuffer buffer = vertexBuffer->vulkanBuffer();
                    VkDeviceSize offset = 0;
                    vkCmdBindVertexBuffers(m_commandBuffer, i, 1, &buffer, &offset);
                }
            }

            // VkBuffer buffer = VK_NULL_HANDLE;
            // VkDeviceSize offset = 0;
            // vkCmdBindVertexBuffers(m_commandBuffer->vulkanCommandBuffer(), 3, 1, &buffer, &offset);

            auto* indexBuffer = static_cast<VulkanIndexBuffer*>(state.primitive.indexBuffer);

            if (indexBuffer) {
                vkCmdBindIndexBuffer(m_commandBuffer, indexBuffer->vulkanBuffer(), 0, indexBuffer->indexType());
            }
        }

        {

#if 1
            auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);
            auto* descriptor = static_cast<VulkanDescriptor2*>(state.descriptor);
            if (descriptor) {
                auto& sets = descriptor->descriptorSets();
                vkCmdBindDescriptorSets(
                    m_commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    shaderPass->vulkanPipelineLayout(),
                    0,
                    sets.size(),
                    sets.data(),
                    0,
                    nullptr);
            }
#else
            auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);

            std::array<VkDescriptorSet, DescriptorType_Count> sets;
            m_commandBuffer->allocateDescriptorSets(shaderPass, &sets);
            vkCmdBindDescriptorSets(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPass->vulkanPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
#endif
        }
    }
}

void VulkanCommandList::onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) {
    VulkanBuffer* buffer = nullptr;
    switch (resource->resourceType()) {
        case RHIResourceType::VertexBuffer:
            buffer = static_cast<VulkanVertexBuffer*>(resource)->buffer();
            break;
        case RHIResourceType::IndexBuffer:
            buffer = static_cast<VulkanIndexBuffer*>(resource)->buffer();
            break;
        default:
            LN_NOTIMPLEMENTED();
            break;
    }

    // VulkanBuffer* stagingBuffer = recodingCommandBuffer()->cmdCopyBuffer(length, buffer);
    // stagingBuffer->setData(offset, data, length);
    VulkanSingleFrameBufferInfo stagingBuffer = cmdCopyBuffer(length, buffer);
    stagingBuffer.buffer->setData(stagingBuffer.offset + offset, data, length);
}

void VulkanCommandList::onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) {
    static_cast<VulkanTexture*>(resource)->setSubData(this, x, y, width, height, data, dataSize);
}

void VulkanCommandList::onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    static_cast<VulkanTexture*>(resource)->setSubData3D(this, x, y, z, width, height, depth, data, dataSize);
}

void VulkanCommandList::onDispatch(const GraphicsContextState& state, IPipeline* basePipeline, int groupCountX, int groupCountY, int groupCountZ) {
    auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);
    auto* descriptor = static_cast<VulkanDescriptor2*>(state.descriptor);

    // addGraphicsToComputeBarriers
    for (int32_t i = 0; i < descriptor->m_refarencedResourceCount; i++) {
        VkBufferMemoryBarrier bufferBarrier;
        bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        bufferBarrier.pNext = nullptr;
        bufferBarrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        bufferBarrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
        bufferBarrier.srcQueueFamilyIndex = m_device->graphicsQueueFamilyIndex();
        bufferBarrier.dstQueueFamilyIndex = m_device->graphicsQueueFamilyIndex();
        bufferBarrier.buffer = static_cast<VulkanVertexBuffer*>(descriptor->m_refarencedResources[i])->vulkanBuffer();
        bufferBarrier.offset = 0;
        bufferBarrier.size = VK_WHOLE_SIZE;
        vkCmdPipelineBarrier(m_commandBuffer, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr, 1, &bufferBarrier, 0, nullptr);
    }

    VulkanPipeline2* pipeline = static_cast<VulkanPipeline2*>(basePipeline);
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->nativePipeline());

    VkPipelineLayout pipelineLayout = shaderPass->vulkanPipelineLayout();
    if (descriptor) {
        auto& sets = descriptor->descriptorSets();
        vkCmdBindDescriptorSets(
            m_commandBuffer,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            pipelineLayout,
            0,
            sets.size(),
            sets.data(),
            0,
            nullptr);
    }
    // vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, compute.pipelineLayout, 0, 1, &compute.descriptorSets[readSet], 0, 0);

    vkCmdDispatch(m_commandBuffer, groupCountX, groupCountY, groupCountZ);

    // addComputeToGraphicsBarriers
    for (int32_t i = 0; i < descriptor->m_refarencedResourceCount; i++) {
        VkBufferMemoryBarrier bufferBarrier;
        bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        bufferBarrier.pNext = nullptr;
        bufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
        bufferBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        bufferBarrier.srcQueueFamilyIndex = m_device->graphicsQueueFamilyIndex();
        bufferBarrier.dstQueueFamilyIndex = m_device->graphicsQueueFamilyIndex();
        bufferBarrier.buffer = static_cast<VulkanVertexBuffer*>(descriptor->m_refarencedResources[i])->vulkanBuffer();
        bufferBarrier.offset = 0;
        bufferBarrier.size = VK_WHOLE_SIZE;
        vkCmdPipelineBarrier(m_commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, nullptr, 1, &bufferBarrier, 0, nullptr);
    }

    //{
    //	VkBufferMemoryBarrier bufferBarrier = vks::initializers::bufferMemoryBarrier();
    //	bufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    //	bufferBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    //	bufferBarrier.srcQueueFamilyIndex = vulkanDevice->queueFamilyIndices.compute;
    //	bufferBarrier.dstQueueFamilyIndex = vulkanDevice->queueFamilyIndices.graphics;
    //	bufferBarrier.size = VK_WHOLE_SIZE;
    //	std::vector<VkBufferMemoryBarrier> bufferBarriers;
    //	bufferBarrier.buffer = compute.storageBuffers.input.buffer;
    //	bufferBarriers.push_back(bufferBarrier);
    //	bufferBarrier.buffer = compute.storageBuffers.output.buffer;
    //	bufferBarriers.push_back(bufferBarrier);
    //	vkCmdPipelineBarrier(
    //		commandBuffer,
    //		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
    //		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
    //		VK_FLAGS_NONE,
    //		0, nullptr,
    //		static_cast<uint32_t>(bufferBarriers.size()), bufferBarriers.data(),
    //		0, nullptr);
    //}
}

void VulkanCommandList::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
    auto* renderPass = static_cast<VulkanRenderPass2*>(currentRenderPass());
    auto& framebuffer = renderPass->framebuffer();
    const auto viewSize = framebuffer->renderTargets()[0]->extentSize();

    // const GraphicsContextState& state = stagingState();

    // SizeI size = state.framebufferState.renderTargets[0]->realSize();
    {
        VkClearRect rect[1];
        rect[0].rect.offset.x = 0;
        rect[0].rect.offset.y = 0;
        rect[0].rect.extent.width = viewSize.width;
        rect[0].rect.extent.height = viewSize.height;
        rect[0].baseArrayLayer = 0;
        rect[0].layerCount = 1;

        VkClearAttachment attachments[MaxMultiRenderTargets + 1] = {};

        uint32_t count = 0;
        // uint32_t rtCount = 1;

        if (testFlag(flags, ClearFlags::Color)) {
            float frgba[4] = {
                color.r,
                color.g,
                color.b,
                color.a,
            };

            // for (uint32_t ii = 0; ii < state.framebufferState.renderTargets.size(); ++ii)
            for (uint32_t ii = 0; ii < 1; ++ii) {
                if (framebuffer->renderTargets()[ii]) {
                    attachments[count].colorAttachment = count;
                    attachments[count].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    attachments[count].clearValue.color.float32[0] = color.r;
                    attachments[count].clearValue.color.float32[1] = color.g;
                    attachments[count].clearValue.color.float32[2] = color.b;
                    attachments[count].clearValue.color.float32[3] = color.a;
                    ++count;
                }
            }
        }

        if ((testFlag(flags, ClearFlags::Depth) || testFlag(flags, ClearFlags::Stencil)) &&
            framebuffer->depthBuffer() != nullptr) {
            attachments[count].colorAttachment = count;
            attachments[count].aspectMask = 0;
            attachments[count].aspectMask |= (testFlag(flags, ClearFlags::Depth)) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
            attachments[count].aspectMask |= (testFlag(flags, ClearFlags::Stencil)) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
            attachments[count].clearValue.depthStencil.stencil = stencil;
            attachments[count].clearValue.depthStencil.depth = z;
            ++count;
        }

        vkCmdClearAttachments(
            m_commandBuffer, count, attachments, LN_ARRAY_SIZE_OF(rect), rect);
    }
}

void VulkanCommandList::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) {
    vkCmdDraw(
        m_commandBuffer,
        VulkanHelper::getPrimitiveVertexCount(primitive, primitiveCount),
        std::max(instanceCount, 1),
        startVertex,
        0);
}

void VulkanCommandList::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) {
    int ic = (instanceCount == 0) ? 1 : instanceCount;
    vkCmdDrawIndexed(
        m_commandBuffer,
        VulkanHelper::getPrimitiveVertexCount(primitive, primitiveCount),
        ic,
        startIndex,
        vertexOffset,
        0);
}

void VulkanCommandList::onDrawExtension(INativeGraphicsExtension* extension) {
    auto i = m_device->vulkanNativeGraphicsInterface();
    i->setContext(this);
    extension->onRender(i);
    i->setContext(nullptr);
}

void VulkanCommandList::wait() {
    // もし前回 vkQueueSubmit したコマンドバッファが完了していなければ待つ
    vkWaitForFences(
        m_device->vulkanDevice(),
        1,
        &m_inFlightFence,
        VK_TRUE,
        std::numeric_limits<uint64_t>::max());

    vkResetCommandBuffer(
        m_commandBuffer,
        VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

} // namespace detail
} // namespace ln
