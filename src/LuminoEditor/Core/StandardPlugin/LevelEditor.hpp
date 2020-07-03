#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/ToolPanesArea.hpp"
#include "../App/NavigatorManager.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"

namespace lna {

class LevelEditor
    : public lna::AssetEditorModel
{
public:
    bool init();
    void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    void onClosed() override;
    Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(lna::EditorPaneKind kind) override;

    void tryInstantiateObjectFromAnyFile(const ln::Path& filePath);

private:
	void WorldRenderView_OnUIEvent(ln::UIEventArgs* e);
	void handleTickEvent(ln::UITimerEventArgs* e);

    Ref<ln::AssetModel> m_assetModel;

    Ref<ln::List<Ref<ln::EditorPane>>> m_modePanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_inspectorPanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_toolPanes;
    Ref<ln::UIVBoxLayout3> m_mainLayout;

    // Scene
	Ref<ln::UIActiveTimer> m_timer;
    Ref<ln::World> m_mainWorld;
	Ref<ln::UIViewport> m_mainViewport;
	Ref<ln::WorldRenderView> m_mainWorldRenderView;
	Ref<ln::Camera> m_mainCamera;

    Ref<ln::Level> m_editingLevel;
};

} // namespace lna 
