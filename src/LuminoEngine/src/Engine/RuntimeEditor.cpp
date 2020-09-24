
#include "Internal.hpp"
#include <imgui.h>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/Controls/UISplitter.hpp>
#include "EngineManager.hpp"
#include "RuntimeEditor.hpp"

namespace ln {
namespace detail {

//=============================================================================
// RuntimeEditor

RuntimeEditor::RuntimeEditor()
{
}

void RuntimeEditor::init(EngineManager* manager, UIMainWindow* window)
{
	m_manager = manager;
	m_window = window;

	m_splitter = makeObject<UISplitter>();
	m_splitter->setOrientation(Orientation::Horizontal);

	const auto mainChildren = m_window->logicalChildren()->toArray();
	for (const auto& child : mainChildren) {
		m_window->removeElement(child);
		m_splitter->addChild(child);
	}
	m_window->addChild(m_splitter);

	m_toolPane = makeObject<UIElement>();
	m_toolPane->setBackgroundColor(Color::White);
	m_splitter->addChild(m_toolPane);

	m_window->m_onImGuiLayer.connect(ln::bind(this, &RuntimeEditor::handleImGuiDebugLayer));
	m_window->setImGuiLayerEnabled(true);
}

void RuntimeEditor::handleImGuiDebugLayer(UIEventArgs* e)
{
	const auto pos = m_toolPane->m_combinedFinalRenderTransform.position();
	const auto size = m_toolPane->actualSize();
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
	ImGui::SetNextWindowSize(ImVec2(size.width, size.height));
	ImGui::Begin("Statistics");
	ImGui::Text("test");


	const auto& fps = m_manager->fpsController();
	
	ImGui::Text("FPS: %.2f, Ext: %.2f", fps.totalFps(), fps.externalFps());
	ImGui::Text("Min: %.2f, Max: %.2f", fps.minFrameMillisecondsPerSeconds() / 1000.0f, fps.maxFrameMillisecondsPerSeconds() / 1000.0f);
	//ImGui::Separator();

	//if (ImGui::CollapsingHeader("RenderView debug"))
	//{
	//	{
	//		bool check = m_mainWorldRenderView->guideGridEnabled();
	//		ImGui::Checkbox("Grid", &check);
	//		m_mainWorldRenderView->setGuideGridEnabled(check);
	//	}
	//	{
	//		bool check = m_mainWorldRenderView->physicsDebugDrawEnabled();
	//		ImGui::Checkbox("Physics", &check);
	//		m_mainWorldRenderView->setPhysicsDebugDrawEnabled(check);
	//	}
	//}


	//if (m_mainWorld) {
	//	//ImGui::BeginChild("Levels");
	//	Level* level = m_mainWorld->sceneConductor()->activeScene();
	//	ImGui::Text("ActiveScene"); ImGui::SameLine(150);
	//	if (ImGui::Button("Reload")) {
	//		level->reloadAsset();
	//	}
	//	if (ImGui::Button("Save")) {
	//		m_assetManager->saveAssetModelToLocalFile(makeObject<AssetModel>(level));
	//	}

	//	//ImGui::EndChild();
	//}

	ImGui::End();



}

} // namespace detail
} // namespace ln

