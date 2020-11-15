
#include "ViewModel.hpp"
#include "MainWindow.hpp"
#include "Application.hpp"

Application::Application()
{
    ln::EngineSettings::setSceneIntegrationEnabled(false);
}

    /*
bool Application::init()
{
#ifdef LN_DEBUG
    lna::Workspace::developMode = true;
    ln::EngineSettings::setGraphicsDebugEnabled(true);
#endif
    lna::AppData::current()->load();

	ln::EngineSettings::setMainWindowSize(1600, 800);
    ln::EngineSettings::setUITheme(u"Chocotelier");
	//ln::EngineSettings::setMainBackBufferSize(1600, 800);
    //ln::EngineSettings::setAssetStorageAccessPriority(ln::AssetStorageAccessPriority::AllowLocalDirectory);
    ln::EngineSettings::setGraphicsAPI(ln::GraphicsAPI::Vulkan);
    ln::detail::EngineManager::s_settings.defaultObjectsCreation = false;
	ln::detail::EngineDomain::engineContext()->initializeEngineManager();
    ln::detail::EngineDomain::engineContext()->engineManager()->initializeAllManagers();
    ln::detail::EngineDomain::sceneManager()->m_editorMode = true;

	auto root = ln::detail::EngineManager::findRepositoryRootForTesting();
    ln::Font::registerFontFromFile(ln::Path(root, u"tools/mplus-font/mplus-1c-regular.ttf"));

    m_workspace = ln::makeObject<lna::Workspace>();
    onInit();

    m_editorContext = ln::makeObject<lna::EditorContext>();
    m_editorContext->m_application = this;
    m_editorContext->m_mainWindow = mainWindow();

    // TODO: test
    //openProject(u"C:/Proj/LN/PrivateProjects/HC4/HC4.lnproj");
    return true;
}
*/

//void EditorApplication::run()
//{
//	uint64_t lastTime = ln::Environment::getTickCount();
//
//    while (!ln::detail::EngineDomain::engineManager()->isExitRequested())
//    {
//        ln::detail::EngineDomain::engineManager()->platformManager()->windowManager()->processSystemEventQueue(ln::detail::EventProcessingMode::Polling);
//        ln::detail::EngineDomain::engineManager()->uiManager()->dispatchPostedEvents();
//
//		float elapsedSeconds = static_cast<float>(ln::Environment::getTickCount() - lastTime) / 1000.0f;
//		ln::detail::EngineDomain::engineManager()->uiManager()->tickGlobal(elapsedSeconds);
//
//        ln::Thread::sleep(1);
//    }
//}

void Application::onInit()
{
	setupMainWindow(ln::makeObject<MainWindow>(), true);
    ln::Engine::renderView()->setGuideGridEnabled(true);
    ln::Engine::camera()->addComponent(ln::CameraOrbitControlComponent::create());
    ln::Engine::renderView()->setBackgroundColor(ln::Color::Gray);
    ln::Engine::renderView()->setGizmoEnabled(true);
    
    ViewModel::instance()->load();

    //m_mesh = ln::StaticMesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.glb");

}
