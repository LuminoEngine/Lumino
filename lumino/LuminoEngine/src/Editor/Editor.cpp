
#include "Internal.hpp"
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include <LuminoEngine/Editor/Editor.hpp>
#include "../Engine/EngineManager.hpp"
#include "DevelopmentTool.hpp"
#include "EditorViewportToolPane.hpp"

namespace ln {

//==============================================================================
// Editor

void Editor::addPane(ImGuiDockPane* pane)
{
	const auto& imgui = detail::EngineDomain::engineManager()->mainWindow()->m_imguiContext;
	imgui->addDock(pane);
}

void Editor::closeAllPanes()
{
	const auto& imgui = detail::EngineDomain::engineManager()->mainWindow()->m_imguiContext;
	for (auto& dock : imgui->dockPanes()) {
		dock->close();
	}
}

ImGuiDockPane* Editor::editorViewportToolPane()
{
	return detail::EngineDomain::engineManager()->runtimeEditor()->editorViewportToolPane();
}

} // namespace ln

