
#include "Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/Controls/UISplitter.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/PostEffect/FilmicPostEffect.hpp>
#include "../../../Graphics/src/GraphicsManager.hpp"
#include "../Rendering/RenderingPipeline.hpp"
#include "../Engine/EngineManager.hpp"
#include "MainViewportToolPane.hpp"
#include "EditorViewportToolPane.hpp"
#include "ProfilerToolPane.hpp"
#include "DevelopmentTool.hpp"

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

	//m_window->m_onImGuiLayer.connect(ln::bind(this, &RuntimeEditor::handleImGuiDebugLayer));

	//m_splitter = makeObject<UISplitter>();
	//m_splitter->setOrientation(UILayoutOrientation::Horizontal);

	//m_mainContentsPane = makeObject<UIControl>();
	//m_splitter->addChild(m_mainContentsPane);

	//m_toolPane = makeObject<UIElement>();
	//m_toolPane->setBackgroundColor(Color::White);
	//m_toolPane->setBorderColor(Color::LightGray);
	//m_toolPane->setBorderThickness(1);
	//m_splitter->addChild(m_toolPane);

	m_window->m_renderView->setClearMode(SceneClearMode::ColorAndDepth);
	m_window->m_onImGuiLayer.connect(ln::bind(this, &RuntimeEditor::handleImGuiDebugLayer));

	if (auto v = AppData::getValue(_TT("LN.Tools.ToolModeWindowWidth"))) {
		m_toolModeWindowSize.width = v->get<float>();
	}
	if (auto v = AppData::getValue(_TT("LN.Tools.ToolModeWindowHeight"))) {
		m_toolModeWindowSize.height = v->get<float>();
	}

	setMode(Mode::Activated);
}

void RuntimeEditor::dispose()
{
	if (m_window) {
		AppData::setValue(_TT("LN.Tools.ToolModeWindowWidth"), makeVariant(m_toolModeWindowSize.width));
		AppData::setValue(_TT("LN.Tools.ToolModeWindowHeight"), makeVariant(m_toolModeWindowSize.height));

		m_window->dispose();
		m_window = nullptr;
	}
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
#if 1
	m_window->setImGuiLayerEnabled(true);
	m_window->invalidateVisual();
	//m_toolWindow->invalidateVisual();

	if (m_toolModeWindowSize.isAnyZero()) {
		m_gameModeWindowSize = m_window->actualSize();
		m_toolModeWindowSize.width = m_gameModeWindowSize.width + 500;
		m_toolModeWindowSize.height = m_gameModeWindowSize.height + 200;
	}


	m_window->setSize(m_toolModeWindowSize);

	ImGuiIntegration* imgui = m_window->m_imguiContext.get();

	{
		m_mainViewportToolPane = makeObject<MainViewportToolPane>();
		m_mainViewportToolPane->setInitialPlacement(ImGuiDockPlacement::MainView);
		imgui->addDock(m_mainViewportToolPane);
	}
	{
		m_pane2 = makeObject<ImGuiDockPane>();
		m_pane2->setInitialPlacement(ImGuiDockPlacement::Left);
		imgui->addDock(m_pane2);
	}
	{
		m_pane3 = makeObject<ImGuiDockPane>();
		m_pane3->setInitialPlacement(ImGuiDockPlacement::Right);
		imgui->addDock(m_pane3);
	}
	{
		m_profilerToolPane = makeObject<ProfilerToolPane>();
		m_profilerToolPane->setInitialPlacement(ImGuiDockPlacement::Right);
		imgui->addDock(m_profilerToolPane);
	}
	{
		m_pane5 = makeObject<ImGuiDockPane>();
		m_pane5->setInitialPlacement(ImGuiDockPlacement::Bottom);
		imgui->addDock(m_pane5);
	}
	{
		m_pane6 = makeObject<ImGuiDockPane>();
		m_pane6->setInitialPlacement(ImGuiDockPlacement::InnerLeft);
		imgui->addDock(m_pane6);
	}
	{
		m_editorViewportToolPane = makeObject<EditorViewportToolPane>(m_window);
		m_editorViewportToolPane->setInitialPlacement(ImGuiDockPlacement::DebugView);
		imgui->addDock(m_editorViewportToolPane);
	}


#else
	// MainWindow の子要素を m_mainContentsPane へ移動する
	const auto mainChildren = m_window->logicalChildren()->toArray();
	for (const auto& child : mainChildren) {
		m_window->removeElement(child);
		m_mainContentsPane->addChild(child);
	}

	m_window->addChild(m_splitter);

	m_window->setImGuiLayerEnabled(true);
#endif
}

void RuntimeEditor::detach()
{
#if 1
	m_window->setImGuiLayerEnabled(false);
#else
	m_window->setImGuiLayerEnabled(false);

	m_window->removeElement(m_splitter);

	const auto mainChildren = m_mainContentsPane->logicalChildren()->toArray();
	for (const auto& child : mainChildren) {
		m_mainContentsPane->removeElement(child);
		m_window->addChild(child);
	}
#endif
}

void RuntimeEditor::updateFrame()
{
	m_mainViewportToolPane->prepare(m_window);




	if (m_mode == Mode::Activated) {
		m_window->invalidateVisual();
	}
}

void RuntimeEditor::handleImGuiDebugLayer(UIEventArgs* e)
{
	//const auto pos = m_toolPane->m_combinedFinalRenderTransform.position();
	//const auto size = m_toolPane->actualSize();
	const auto pos = ln::Vector2(0, 0);
	const auto size = m_window->actualSize();

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

			ImGui::Separator();

			ImGui::Text("GraphicsResources: %d", detail::GraphicsManager::instance()->graphicsResources().size());
			

			const auto& tex = m_manager->mainRenderView()->sceneRenderingPipeline()->viweNormalAndDepthBuffer();
			if (tex) {
				ImGui::Image((ImTextureID)tex.get(), ImVec2(tex->width(), tex->height()));
			}


			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Scene"))
		{
			ImGui::Text("ID: 0123456789");




			//const auto& world = m_manager->mainWorld();
			//const auto& objects = world->objects



			bool v = Scene::isGammaEnabled();
			ImGui::Checkbox("Gamma", &v);
			Scene::setGammaEnabled(v);


			auto& pp = m_manager->mainRenderView()->finishingProcess();
			v = pp->isTonemapEnabled();
			ImGui::Checkbox("Tonemap", &v);
			pp->setTonemapEnabled(v);

			if (pp->isTonemapEnabled()) {
				ImGui::Indent(20);
				ImGui::SliderFloat("LinearWhite", &(pp->m_linearWhite), 0.0f, 10.0f);
				ImGui::SliderFloat("ShoulderStrength", &(pp->m_shoulderStrength), 0.0f, 10.0f);
				ImGui::SliderFloat("LinearStrength", &(pp->m_linearStrength), 0.0f, 10.0f);
				ImGui::SliderFloat("LinearAngle", &(pp->m_linearAngle), 0.0f, 10.0f);

				ImGui::SliderFloat("ToeStrength", &(pp->m_toeStrength), 0.0f, 10.0f);
				ImGui::SliderFloat("ToeNumerator", &(pp->m_toeNumerator), 0.0f, 10.0f);
				ImGui::SliderFloat("ToeDenominator", &(pp->m_toeDenominator), 0.0f, 10.0f);
				ImGui::SliderFloat("Exposure", &(pp->m_exposure), 0.0f, 10.0f);
				ImGui::Indent(0);
			}

			v = pp->isBloomEnabled();
			ImGui::Checkbox("Bloom", &v);
			pp->setBloomEnabled(v);

			v = pp->isDOFEnabled();
			ImGui::Checkbox("Depth Of Field", &v);
			pp->setDOFEnabled(v);
			if (pp->isDOFEnabled()) {
				ImGui::Indent(20);
				float v = pp->focusedLinearDepth();
				ImGui::SliderFloat("Focus Distance", &v, 0.0f, 1.0f);
				pp->setFocusedLinearDepth(v);
				ImGui::Indent(0);

			}

			v = pp->isVignetteEnabled();
			ImGui::Checkbox("Vignette", &v);
			pp->setVignetteEnabled(v);

			//float m_linearWhite = 5.0f;
			//float m_shoulderStrength = 0.15f;
			//float m_linearStrength = 0.5;
			//float m_linearAngle = 0.05;

			//float  = 0.02f;
			//float  = 0.02;
			//float  = 0.3;
			//float  = 5.0f;



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

