
#include "Internal.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Graphics/RHIs/RHIProfiler.hpp"
#include "ProfilerToolPane.hpp"

namespace ln {
namespace detail {

//=============================================================================
// ProfilerToolPane
	
void ProfilerToolPane::onGui()
{
	const RHIProfiler* profiler = detail::EngineDomain::graphicsManager()->deviceContext()->profiler().get();

	if (ImGui::CollapsingHeader("RHI Objects:", ImGuiTreeNodeFlags_DefaultOpen)) {
		char num[32];

		ImGui::Columns(2);

		sprintf(num, "%d", profiler->vertexLayoutCount());
		ImGui::Text("VertexLayout"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->vertexBufferCount());
		ImGui::Text("VertexBuffer"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->indexBufferCount());
		ImGui::Text("IndexBuffer"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->texture2DCount());
		ImGui::Text("Texture2D"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->renderTargetCount());
		ImGui::Text("RenderTarget"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->depthBufferCount());
		ImGui::Text("DepthBuffer"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->samplerStateCount());
		ImGui::Text("SamplerState"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->shaderPassCount());
		ImGui::Text("ShaderPass"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->uniformBufferCount());
		ImGui::Text("UniformBuffer"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->descriptorPoolCount());
		ImGui::Text("DescriptorPool"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->renderPassCount());
		ImGui::Text("RenderPass"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->pipelineStateCount());
		ImGui::Text("PipelineState"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->commandListCount());
		ImGui::Text("CommandList"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		sprintf(num, "%d", profiler->swapChainCount());
		ImGui::Text("SwapChain"); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();

		ImGui::Columns(1);
	}
}

} // namespace detail
} // namespace ln

