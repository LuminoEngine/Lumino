
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

void Editor::addPane(ImGuiDockPane* pane) {
    detail::EngineDomain::engineManager()->mainWindow()->dockManager()->addDockPane(pane);
}

void Editor::closeAllPanes() {
    const auto& dockManager = detail::EngineDomain::engineManager()->mainWindow()->dockManager();
    for (auto& dock : dockManager->dockPanes()) {
        dock->close();
    }
}

ImGuiDockPane* Editor::editorViewportToolPane() {
    return detail::EngineDomain::engineManager()->runtimeEditor()->editorViewportToolPane();
}

} // namespace ln
