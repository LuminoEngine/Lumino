
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../Project/PluginManager.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "LevelEditorHierarchyPane.hpp"
#include "LevelEditorPropertyPane.hpp"
#include "LevelEditor.hpp"

#include "../../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"   // TODO:
#include "../../../../lumino/LuminoEngine/src/Scene/SceneManager.hpp"   // TODO:

namespace lna {
   
//==============================================================================
// LevelEditor

bool LevelEditor::init()
{
    AssetEditorModel::init();

    m_hierarchyPane = ln::makeObject<LevelEditorHierarchyPane>(this);
    m_propertyPane = ln::makeObject<LevelEditorPropertyPane>(this);

    //m_inspectorPane = ln::makeObject<ln::EditorPane>();
    //m_inspectorPane->setBackgroundColor(ln::Color::LightGray);

    m_modePanes = ln::makeList<Ref<ln::EditorPane>>({ ln::static_pointer_cast<ln::EditorPane>(m_hierarchyPane) });
    m_inspectorPanes = ln::makeList<Ref<ln::EditorPane>>({ m_propertyPane });
    //m_toolPanes = ln::makeList<Ref<ln::EditorPane>>();

    return true;
}

void LevelEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
    m_assetModel = asset;
    m_mainLayout = ln::makeObject<ln::UIVBoxLayout3>();
    m_mainLayout->setName(_TT("---"));
    frame->addElement(m_mainLayout);
    {
        auto t = ln::UIText::create(_TT("test"));
		t->getGridLayoutInfo()->layoutWeight = 0;
		//t->setVAlignment(ln::UIVAlignment::Center);
        m_mainLayout->addChild(t);
    }


	m_mainViewport = ln::makeObject<ln::UIViewport>();
    //m_mainViewport->setVAlignment(ln::UIVAlignment::Stretch);
	m_mainViewport->getGridLayoutInfo()->layoutWeight = 1;
    m_mainLayout->addChild(m_mainViewport);
    //frame->addElement(m_mainViewport);
    //m_mainViewport->setBackgroundColor(ln::Color::Blue);// ln::Color(ln::Random::randFloat(), ln::Random::randFloat(), ln::Random::randFloat(), 1));


    //auto m_mainWorld = dynamic_cast<ln::World*>(asset->target());//ln::makeObject<ln::World>(); //
    //if (LN_REQUIRE(m_mainWorld)) return;
    //auto m_mainWorldAsset = ln::AssetModel::create(m_mainWorld);

    // create world and view
    {
        m_mainWorld = ln::makeObject<ln::World>();
        m_mainCamera = ln::makeObject<ln::Camera>();
        m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
        m_mainWorldRenderView->setTargetWorld(m_mainWorld);
        m_mainWorldRenderView->setCamera(m_mainCamera);
        m_mainWorldRenderView->setClearMode(ln::SceneClearMode::SkyDome);
        //m_mainWorldRenderView->setClearMode(ln::SceneClearMode::ColorAndDepth);
        m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
        m_mainWorldRenderView->connectOnUIEvent(ln::bind(this, &LevelEditor::WorldRenderView_OnUIEvent));
		m_mainWorldRenderView->setPhysicsDebugDrawEnabled(true);
        m_mainWorldRenderView->setGizmoEnabled(true);
        m_mainViewport->addRenderView(m_mainWorldRenderView);

        m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());
    }

    // attach as main scene
    m_editingLevel = dynamic_cast<ln::Level*>(asset->target());
    m_mainWorld->addScene(m_editingLevel);

	m_timer = ln::makeObject<ln::UIActiveTimer>();
	m_timer->connectOnTick(ln::bind(this, &LevelEditor::handleTickEvent));
	m_mainViewport->registerActiveTimer(m_timer);


    frame->addAction(ln::makeObject<ln::UIAction>(EditorApplication::SaveCommand, [this](ln::UICommandEventArgs* x) {
        printf("Save");
        //this->editorContext()->assetDatabase()->saveAsset(this->m_assetModel);
        ln::Assets::save(m_editingLevel);
    }));


    //auto a = ln::Mesh::create(_TT("C:/Proj/LN/PrivateProjects/HC4/assets/Models/WARZONE/2-OverpassBroken_grp.gltf");
    //m_editingLevel->addObject(a);
}

void LevelEditor::onClosed()
{
}

Ref<ln::List<Ref<ln::EditorPane>>> LevelEditor::getEditorPanes(lna::EditorPaneKind kind)
{
    switch (kind)
    {
    case lna::EditorPaneKind::Mode:
        return m_modePanes;
    case lna::EditorPaneKind::Inspector:
        return m_inspectorPanes;
    case lna::EditorPaneKind::Tool:
        return m_toolPanes;
    }
    return nullptr;
}

void LevelEditor::tryInstantiateObjectFromAnyFile(const ln::Path& filePath)
{
    if (auto obj = ln::detail::EngineDomain::sceneManager()->instantiateObjectFromAnyFile(filePath)) {
        m_editingLevel->addObject(obj);
    }
}

void LevelEditor::WorldRenderView_OnUIEvent(ln::UIEventArgs* e)
{
    if (e->type() == ln::UIEvents::MouseDownEvent) {
        auto me = static_cast<ln::UIMouseEventArgs*>(e);
        if (me->getMouseButtons() == ln::MouseButtons::Left) {
            auto point = me->getPosition(m_mainViewport);
            if (auto obj = m_mainWorldRenderView->findObjectInPoint(point.x, point.y)) {
                printf("select\n");
                m_mainWorldRenderView->transformControls()->setTarget(obj);
            }
        }
    }
}

void LevelEditor::handleTickEvent(ln::UITimerEventArgs* e)
{
	m_mainWorld->updateFrame(e->elapsedSeconds());
}

} // namespace lna

