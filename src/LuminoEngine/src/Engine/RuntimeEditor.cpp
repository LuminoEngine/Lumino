
#include "Internal.hpp"
#include <imgui.h>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/Controls/UISplitter.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include "EngineManager.hpp"
#include "RuntimeEditor.hpp"

namespace ln {
namespace detail {

//=============================================================================
// RuntimeEditor

RuntimeEditor::RuntimeEditor()
	: m_mode(Mode::Hidden)
{
}

void RuntimeEditor::init(EngineManager* manager, UIMainWindow* window)
{
	m_manager = manager;

	m_window = window;

	m_window->m_onImGuiLayer.connect(ln::bind(this, &RuntimeEditor::handleImGuiDebugLayer));

	m_splitter = makeObject<UISplitter>();
	m_splitter->setOrientation(Orientation::Horizontal);

	m_mainContentsPane = makeObject<UIControl>();
	m_splitter->addChild(m_mainContentsPane);

	m_toolPane = makeObject<UIElement>();
	m_toolPane->setBackgroundColor(Color::White);
	m_toolPane->setBorderColor(Color::LightGray);
	m_toolPane->setBorderThickness(1);
	m_splitter->addChild(m_toolPane);

	setMode(Mode::Activated);

	//m_toolWindow = makeObject<UIFrameWindow>();
}

void RuntimeEditor::toggleMode()
{
	switch (m_mode)
	{
	case Mode::Hidden:
		setMode(Mode::Activated);
		break;
	case Mode::Activated:
		setMode(Mode::Hidden);
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

void RuntimeEditor::setMode(Mode mode)
{
	if (m_mode != mode) {
		m_mode = mode;
		if (m_mode == Mode::Activated) {
			attach();
		}
		else {
			detach();
		}
	}
}

void RuntimeEditor::attach()
{
	// MainWindow の子要素を m_mainContentsPane へ移動する
	const auto mainChildren = m_window->logicalChildren()->toArray();
	for (const auto& child : mainChildren) {
		m_window->removeElement(child);
		m_mainContentsPane->addChild(child);
	}

	m_window->addChild(m_splitter);

	m_window->setImGuiLayerEnabled(true);
}

void RuntimeEditor::detach()
{
	m_window->setImGuiLayerEnabled(false);

	m_window->removeElement(m_splitter);

	const auto mainChildren = m_mainContentsPane->logicalChildren()->toArray();
	for (const auto& child : mainChildren) {
		m_mainContentsPane->removeElement(child);
		m_window->addChild(child);
	}
}

void RuntimeEditor::handleImGuiDebugLayer(UIEventArgs* e)
{
	const auto pos = m_toolPane->m_combinedFinalRenderTransform.position();
	const auto size = m_toolPane->actualSize();
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
	ImGui::SetNextWindowSize(ImVec2(size.width, size.height));


	ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);


	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Statistics"))
		{
			ImGui::Text("test");


			const auto& fps = m_manager->fpsController();

			ImGui::Text("FPS: %.2f, Ext: %.2f", fps.totalFps(), fps.externalFps());
			ImGui::Text("Min: %.2f, Max: %.2f", fps.minFrameMillisecondsPerSeconds() / 1000.0f, fps.maxFrameMillisecondsPerSeconds() / 1000.0f);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Details"))
		{
			ImGui::Text("ID: 0123456789");




			//const auto& world = m_manager->mainWorld();
			//const auto& objects = world->objects








			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}




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

