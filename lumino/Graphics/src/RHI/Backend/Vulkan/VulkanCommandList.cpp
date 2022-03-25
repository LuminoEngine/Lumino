
#include "Internal.hpp"
#include "VulkanDeviceContext.hpp"
#include "VulkanBuffers.hpp"
#include "VulkanTextures.hpp"
#include "VulkanDescriptorPool.hpp"
#include "VulkanShaderPass.hpp"
#include "VulkanCommandList.hpp"

namespace ln {
namespace detail {

//==============================================================================
// VulkanGraphicsContext

VulkanGraphicsContext::VulkanGraphicsContext()
	: m_device(nullptr)
	, m_commandBuffer(nullptr)
{
}

bool VulkanGraphicsContext::init(VulkanDevice* owner)
{
	LN_CHECK(owner);
	ICommandList::init(owner);
	m_device = owner;

    m_commandBuffer = makeRef<VulkanCommandBuffer>();
	if (!m_commandBuffer->init(m_device)) {
		return false;
	}

	return true;
}

void VulkanGraphicsContext::dispose()
{
	if (m_commandBuffer) {
        m_commandBuffer->dispose();
        m_commandBuffer = nullptr;
	}

    ICommandList::dispose();
}

void VulkanGraphicsContext::onSaveExternalRenderState()
{
}

void VulkanGraphicsContext::onRestoreExternalRenderState()
{
}

void VulkanGraphicsContext::onBeginCommandRecoding()
{
    m_commandBuffer->beginRecording();
}

void VulkanGraphicsContext::onEndCommandRecoding()
{
    m_commandBuffer->endRecording();
}

void VulkanGraphicsContext::onBeginRenderPass(IRenderPass* renderPass_)
{
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
	//if (testFlag(renderPass->clearFlags(), ClearFlags::Color))
	{
		auto& color = renderPass->clearColor();
		for (uint32_t ii = 0; ii < framebuffer->renderTargets().size(); ii++) {
			if (framebuffer->renderTargets()[ii]) {
				clearValues[count].color = { color.r, color.g, color.b, color.a };
				count++;
			}
		}
	}
	//if ((testFlag(renderPass->clearFlags(), ClearFlags::Depth) || testFlag(renderPass->clearFlags(), ClearFlags::Stencil)))
    {
		if (framebuffer->depthBuffer()) {
			clearValues[count].depthStencil = { renderPass->clearDepth(), renderPass->clearStencil() };
			count++;
		}
	}

	renderPassInfo.clearValueCount = count;
	renderPassInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(m_commandBuffer->vulkanCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanGraphicsContext::onEndRenderPass(IRenderPass* renderPass)
{
	vkCmdEndRenderPass(m_commandBuffer->vulkanCommandBuffer());
}

void VulkanGraphicsContext::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline)
{
    if (stateDirtyFlags & GraphicsContextStateDirtyFlags_RegionRects) {
		VkViewport viewport;
		viewport.x = state.regionRects.viewportRect.x;
		viewport.y = state.regionRects.viewportRect.height + state.regionRects.viewportRect.y;
		viewport.width = state.regionRects.viewportRect.width;
		viewport.height = -state.regionRects.viewportRect.height;   // height マイナスで、DirectX や OpenGL と同じ座標系になる (1.1+, VK_KHR_maintenance1)
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_commandBuffer->vulkanCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor;
		scissor.offset.x = state.regionRects.scissorRect.x;
		scissor.offset.y = state.regionRects.scissorRect.y;
		scissor.extent.width = state.regionRects.scissorRect.width;
		scissor.extent.height = state.regionRects.scissorRect.height;
		vkCmdSetScissor(m_commandBuffer->vulkanCommandBuffer(), 0, 1, &scissor);
	}

	if (submitSource == GraphicsContextSubmitSource_Draw) {
		if (pipeline) {
			auto vulkanPipeline = static_cast<VulkanPipeline2*>(pipeline);
			vkCmdBindPipeline(m_commandBuffer->vulkanCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->nativePipeline());
		}


		{
			//VulkanVertexDeclaration* layout = static_cast<VulkanVertexDeclaration*>(state.pipelineState.vertexDeclaration);

			//std::array<VkBuffer, MaxVertexStreams> vertexBufferHandles;
			//std::array<VkDeviceSize, MaxVertexStreams> vertexBufferOffsets;
			//int i = 0;
			//for (; i < layout->elements>; i++) {
			//	if (state.primitive.vertexBuffers[i]) {
			//		auto* vertexBuffer = static_cast<VulkanVertexBuffer*>(state.primitive.vertexBuffers[i]);
			//		vertexBufferHandles[i] = vertexBuffer->vulkanBuffer();
			//		vertexBufferOffsets[i] = 0;
			//	}
			//	else {
			//		vertexBufferHandles[i] = VK_NULL_HANDLE;
			//		vertexBufferOffsets[i] = 0;
			//	}
			//}
			//vkCmdBindVertexBuffers(m_commandBuffer->vulkanCommandBuffer(), 0, MaxVertexStreams, vertexBufferHandles.data(), vertexBufferOffsets.data());


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
					vkCmdBindVertexBuffers(m_commandBuffer->vulkanCommandBuffer(), i, 1, &buffer, &offset);
				}
			}

			//VkBuffer buffer = VK_NULL_HANDLE;
			//VkDeviceSize offset = 0;
			//vkCmdBindVertexBuffers(m_commandBuffer->vulkanCommandBuffer(), 3, 1, &buffer, &offset);

			auto* indexBuffer = static_cast<VulkanIndexBuffer*>(state.primitive.indexBuffer);

			if (indexBuffer) {
				vkCmdBindIndexBuffer(m_commandBuffer->vulkanCommandBuffer(), indexBuffer->vulkanBuffer(), 0, indexBuffer->indexType());
			}
		}

		{

#if 1
            auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);
            auto* descriptor = static_cast<VulkanDescriptor2*>(state.descriptor);
            if (descriptor) {
                auto& sets = descriptor->descriptorSets();
                vkCmdBindDescriptorSets(
                    m_commandBuffer->vulkanCommandBuffer(),
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    shaderPass->vulkanPipelineLayout(), 0,
                    sets.size(), sets.data(), 0, nullptr);
            }
#else
			auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);

			std::array<VkDescriptorSet, DescriptorType_Count> sets;
            m_commandBuffer->allocateDescriptorSets(shaderPass, &sets);
			vkCmdBindDescriptorSets(m_commandBuffer->vulkanCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPass->vulkanPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
#endif
		}
	}
}

void VulkanGraphicsContext::onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length)
{
    // データ転送に使う vkCmdCopyBuffer() は RenderPass inside では使えないので、開いていればここで End しておく。次の onSubmitState() で再開される。
    m_commandBuffer->endRenderPassInRecordingIfNeeded();

	VulkanBuffer* buffer = nullptr;
	switch (resource->resourceType())
	{
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

	//VulkanBuffer* stagingBuffer = recodingCommandBuffer()->cmdCopyBuffer(length, buffer);
	//stagingBuffer->setData(offset, data, length);
    VulkanSingleFrameBufferInfo stagingBuffer = recodingCommandBuffer()->cmdCopyBuffer(length, buffer);
    stagingBuffer.buffer->setData(stagingBuffer.offset + offset, data, length);
}

void VulkanGraphicsContext::onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
	// データ転送に使う vkCmdCopyBufferToImage() は RenderPass inside では使えないので、開いていればここで End しておく。次の onSubmitState() で再開される。
    m_commandBuffer->endRenderPassInRecordingIfNeeded();

	static_cast<VulkanTexture*>(resource)->setSubData(this, x, y, width, height, data, dataSize);
}

void VulkanGraphicsContext::onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
	// データ転送に使う vkCmdCopyBufferToImage() は RenderPass inside では使えないので、開いていればここで End しておく。次の onSubmitState() で再開される。
    m_commandBuffer->endRenderPassInRecordingIfNeeded();

	static_cast<VulkanTexture*>(resource)->setSubData3D(this, x, y, z, width, height, depth, data, dataSize);
}

void VulkanGraphicsContext::onDispatch(const GraphicsContextState& state, IPipeline* basePipeline, int groupCountX, int groupCountY, int groupCountZ)
{
	VkCommandBuffer commandBuffer = m_commandBuffer->vulkanCommandBuffer();
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
		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr);
	}


	VulkanPipeline2* pipeline = static_cast<VulkanPipeline2*>(basePipeline);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->nativePipeline());

	VkPipelineLayout pipelineLayout = shaderPass->vulkanPipelineLayout();
	if (descriptor) {
		auto& sets = descriptor->descriptorSets();
		vkCmdBindDescriptorSets(
			m_commandBuffer->vulkanCommandBuffer(),
			VK_PIPELINE_BIND_POINT_COMPUTE,
			pipelineLayout, 0,
			sets.size(), sets.data(), 0, nullptr);
	}
	//vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, compute.pipelineLayout, 0, 1, &compute.descriptorSets[readSet], 0, 0);

	vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);


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
		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr);
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

void VulkanGraphicsContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    auto* renderPass = static_cast<VulkanRenderPass2*>(currentRenderPass());
    auto& framebuffer = renderPass->framebuffer();
	const auto viewSize = framebuffer->renderTargets()[0]->extentSize();

	//const GraphicsContextState& state = stagingState();

	//SizeI size = state.framebufferState.renderTargets[0]->realSize();
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
		//uint32_t rtCount = 1;

		if (testFlag(flags, ClearFlags::Color))
		{
			float frgba[4] = { color.r, color.g, color.b, color.a, };

			//for (uint32_t ii = 0; ii < state.framebufferState.renderTargets.size(); ++ii)
            for (uint32_t ii = 0; ii < 1; ++ii)
			{
				if (framebuffer->renderTargets()[ii])
				{
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
            framebuffer->depthBuffer() != nullptr)
		{
			attachments[count].colorAttachment = count;
			attachments[count].aspectMask = 0;
			attachments[count].aspectMask |= (testFlag(flags, ClearFlags::Depth)) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
			attachments[count].aspectMask |= (testFlag(flags, ClearFlags::Stencil)) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
			attachments[count].clearValue.depthStencil.stencil = stencil;
			attachments[count].clearValue.depthStencil.depth = z;
			++count;
		}

		vkCmdClearAttachments(
            m_commandBuffer->vulkanCommandBuffer()
			, count
			, attachments
			, LN_ARRAY_SIZE_OF(rect)
			, rect
		);

	}
}

void VulkanGraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
	vkCmdDraw(m_commandBuffer->vulkanCommandBuffer(), VulkanHelper::getPrimitiveVertexCount(primitive, primitiveCount), 1, startVertex, 0);
}

void VulkanGraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset)
{
    int ic = (instanceCount == 0) ? 1 : instanceCount;
	vkCmdDrawIndexed(
        m_commandBuffer->vulkanCommandBuffer(), VulkanHelper::getPrimitiveVertexCount(primitive, primitiveCount),
        ic, startIndex, vertexOffset, 0);
}

void VulkanGraphicsContext::onDrawExtension(INativeGraphicsExtension* extension)
{
    auto i = m_device->vulkanNativeGraphicsInterface();
    i->setContext(this);
	extension->onRender(i);
    i->setContext(nullptr);
}

void VulkanGraphicsContext::wait()
{
    m_commandBuffer->wait();
}

} // namespace detail
} // namespace ln
