﻿
#include "Internal.hpp"
#include <LuminoEngine/UI/UIColors.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../Graphics/RHIs/RHIProfiler.hpp"
#include "../Graphics/GraphicsProfiler.hpp"
#include "../Graphics/RenderTargetTextureCache.hpp"
#include "../Engine/EngineManager.hpp"
#include "../Engine/EngineProfiler.hpp"
#include "ProfilerToolPane.hpp"

namespace ln {
namespace detail {

//=============================================================================
// ProfilerToolPane
	
static ImU32 toImU32(const Color& c)
{
	return ImColor(ImVec4(c.r, c.g, c.b, c.a));
}

void ProfilerToolPane::onGui()
{

	//

	//return;
	{
		const ImVec2 p = ImGui::GetCursorScreenPos();
		const ImVec2 size = ImGui::GetContentRegionAvail();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		EngineManager* manager = EngineDomain::engineManager();
		EngineProfiler* profiler = manager->engineProfiler().get();

		const double div = 1.0f / static_cast<double>(profiler->frameRate());
		const double updatingRatio = profiler->updateTime() / div;
		const double renderingRatio = profiler->renderingTime() / div;
		const double totalRatio = profiler->totalFrameTime() / div;




		const float safeW = size.x * 0.9f;
		const float failW = size.x * 0.1f;
		const float HEIGHT = 20.0f;
		const float TOTAL_HEIGHT = 10.0f;
		const float totalW = safeW * totalRatio;
		const float updatingW = safeW * updatingRatio;
		const float renderingW = safeW * renderingRatio;
		const ImU32 totalC = toImU32(UIColors::indigo());
		const ImU32 updatingC = toImU32(UIColors::green());
		const ImU32 renderingC = toImU32(UIColors::orange());
		const ImU32 failC = toImU32(UIColors::red());
		const ImU32 lineC = toImU32(UIColors::grey());

		draw_list->AddLine(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y), lineC);
		{
			float x = p.x;
			float y = p.y;
			draw_list->AddRectFilled(ImVec2(x, p.y), ImVec2(x + updatingW, p.y + HEIGHT), updatingC); x += updatingW;
			draw_list->AddRectFilled(ImVec2(x, p.y), ImVec2(x + renderingW, p.y + HEIGHT), renderingC); x += renderingW;
		}

		{
			float x = p.x;
			float y = p.y + HEIGHT;
			draw_list->AddRectFilled(ImVec2(p.x, y), ImVec2(p.x + totalW, y + TOTAL_HEIGHT), totalC);
		}

		{
			draw_list->AddRectFilled(ImVec2(p.x + safeW, p.y), ImVec2(p.x + size.x, p.y + HEIGHT + TOTAL_HEIGHT), failC);
		}

		const float bottom = p.y + HEIGHT + TOTAL_HEIGHT;
		draw_list->AddLine(ImVec2(p.x, bottom), ImVec2(p.x + size.x, bottom), lineC);

		ImGui::Dummy(ImVec2(0, HEIGHT + TOTAL_HEIGHT));

		{
			ImGui::Columns(3);

			ImGui::PushStyleColor(ImGuiCol_Text, updatingC);
			ImGui::Bullet();
			ImGui::PopStyleColor();
			ImGui::Text("Update");
			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Text, renderingC);
			ImGui::Bullet();
			ImGui::PopStyleColor();
			ImGui::Text("Render");
			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Text, totalC);
			ImGui::Bullet();
			ImGui::PopStyleColor();
			ImGui::Text("Total");
			ImGui::NextColumn();

			ImGui::Columns(1);
		}

	}

	char num[32];
	const auto addTextValue = [&](const char* label, int32_t number) {
		sprintf(num, "%d", number);
		ImGui::Text(label); ImGui::NextColumn(); ImGui::Text(num); ImGui::NextColumn();
	};

	if (ImGui::CollapsingHeader("RHI Objects:", ImGuiTreeNodeFlags_DefaultOpen)) {
		const RHIProfiler* profiler = detail::EngineDomain::graphicsManager()->deviceContext()->profiler().get();
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

	if (ImGui::CollapsingHeader("Graphics Objects:", ImGuiTreeNodeFlags_DefaultOpen)) {
		const GraphicsManager* manager = detail::EngineDomain::graphicsManager();
		const GraphicsProfiler* profiler = manager->profiler().get();
		ImGui::Columns(2);
		addTextValue("ConstantBuffer", profiler->constantBufferCount());
		addTextValue("RenderTarget", profiler->renderTargetCount());
		addTextValue("DepthBuffer", profiler->depthBufferCount());
		addTextValue("RenderPass", profiler->renderPassCount());
		addTextValue("RenderTarget(Cache)", manager->renderTargetTextureCacheManager()->count());
		addTextValue("DepthBuffer(Cache)", manager->depthBufferCacheManager()->count());

		

		ImGui::Columns(1);
	}
}

} // namespace detail
} // namespace ln

