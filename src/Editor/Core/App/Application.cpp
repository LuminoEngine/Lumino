
#include "../../../lumino/LuminoEngine/src/Engine/EngineManager.hpp"
#include "../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoPlatform/detail/PlatformWindowManager.hpp>
#include "../../../lumino/LuminoEngine/src/UI/UIManager.hpp"
#include "../../../lumino/LuminoEngine/src/Scene/SceneManager.hpp"
#include <LuminoPlatform/PlatformDialogs.hpp>
#include <AppData.hpp>
#include <EnvironmentSettings.hpp>
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../Project/PluginManager.hpp"
#include "AssetEditor/AssetEditorModel.hpp"
#include "NavigatorManager.hpp"
#include "DocumentManager.hpp"
#include "MainWindow.hpp"
#include "Application.hpp"
#include "NewProjectDialog.hpp"

Ref<ln::UICommand> EditorApplication::NewCommand;
Ref<ln::UICommand> EditorApplication::OpenCommand;
Ref<ln::UICommand> EditorApplication::SaveCommand;

static EditorApplication* s_app = nullptr;

EditorApplication* EditorApplication::instance()
{
    assert(s_app);
    return s_app;
}

void EditorApplication::configure() {
#ifdef LN_DEBUG
    lna::Workspace::developMode = true;
    ln::EngineSettings::setGraphicsDebugEnabled(true);
#endif
    lna::AppData::current()->load();

    ln::EngineSettings::setMainWindowSize(1600, 800);
    ln::EngineSettings::setUITheme(_TT("Chocotelier"));
    // ln::EngineSettings::setMainBackBufferSize(1600, 800);
    // ln::EngineSettings::setAssetStorageAccessPriority(ln::AssetStorageAccessPriority::AllowLocalDirectory);
    ln::EngineSettings::setGraphicsAPI(ln::GraphicsAPI::Vulkan);
    ln::detail::EngineManager::s_settings.defaultObjectsCreation = false;
}

EditorApplication::EditorApplication()
    : ln::Application() {
    assert(!s_app);
    s_app = this;

    NewCommand = ln::makeObject_deprecated<ln::UICommand>();
    OpenCommand = ln::makeObject_deprecated<ln::UICommand>();

    SaveCommand = ln::makeObject_deprecated<ln::UICommand>();
    SaveCommand->addInputGesture(ln::KeyGesture::create(ln::Keys::S, ln::ModifierKeys::Control));
    addApplicationCommand(SaveCommand);

    addAction(ln::makeObject_deprecated<ln::UIAction>(NewCommand, [this](ln::UICommandEventArgs* x){ handleNewProject(x); }));
    addAction(ln::makeObject_deprecated<ln::UIAction>(OpenCommand, [this](ln::UICommandEventArgs* x) { handleOpenProject(x); }));
}

EditorApplication::~EditorApplication()
{
    s_app = nullptr;
}

ln::Result EditorApplication::init()
{
	ln::detail::EngineDomain::engineContext()->initializeEngineManager();
    ln::detail::EngineDomain::engineContext()->engineManager()->initializeAllManagers();
    ln::detail::EngineDomain::sceneManager()->m_editorMode = true;

	auto root = ln::detail::EngineManager::findRepositoryRootForTesting();
    ln::Font::registerFontFromFile(ln::Path(root, _TT("tools/mplus-font/mplus-1c-regular.ttf")));

    m_workspace = ln::makeObject_deprecated<lna::Workspace>();
    onInit();

    m_editorContext = ln::makeObject_deprecated<lna::EditorContext>();
    m_editorContext->m_application = this;
    m_editorContext->m_mainWindow = mainWindow();

    // TODO: test
    //openProject(_TT("C:/Proj/LN/PrivateProjects/HC4/HC4.lnproj");

    return ln::ok();
}

void EditorApplication::dispose()
{
    lna::AppData::current()->save();
    ln::detail::EngineDomain::release();
}

void EditorApplication::run()
{
	uint64_t lastTime = ln::Environment::getTickCount();

    while (!ln::detail::EngineDomain::engineManager()->isExitRequested())
    {
        ln::detail::PlatformManager::instance()->windowManager()->processSystemEventQueue(ln::detail::EventProcessingMode::Polling);
        ln::detail::EngineDomain::engineManager()->uiManager()->dispatchPostedEvents();

		float elapsedSeconds = static_cast<float>(ln::Environment::getTickCount() - lastTime) / 1000.0f;
		ln::detail::EngineDomain::engineManager()->uiManager()->updateFrame(elapsedSeconds);

        ln::Thread::sleep(1);
    }
}

MainWindow* EditorApplication::mainWindow() const
{
    auto ptr = ln::static_pointer_cast<MainWindow>(ln::detail::EngineDomain::engineManager()->mainWindow());
    return ptr;
}

lna::Project* EditorApplication::mainProject() const
{
    return m_workspace->mainProject();
}

#if 0
// deprecated
void EditorApplication::importFile(const ln::Path& filePath)
{
    assert(0);
    if (m_workspace->mainProject())
    {
        auto exts = m_workspace->mainPluginManager()->getAssetImporterExtensions(filePath);
        if (exts.size() != 1) {
            LN_NOTIMPLEMENTED();
            return;
        }

        exts[0].second->import(filePath);
    }
}
#endif

void EditorApplication::openAssetFile(const ln::Path& filePath)
{
    if (m_workspace->mainProject())
    {
        auto asset = m_workspace->mainAssetDatabase()->openAsset(filePath);
        auto factories = m_workspace->mainPluginManager()->findAssetEditorModelFactory(asset->assetType());
        if (factories.size() != 1) {
            LN_NOTIMPLEMENTED();
            return;
        }

        auto editor = factories[0]->createAssetEditorModel();
        editor->m_editorContext = m_editorContext;
        mainWindow()->documentManager()->addDocument(ln::makeObject_deprecated<AssetEditorDocument>(asset, editor));
    }
}

void EditorApplication::onInit()
{

    auto sheet = ln::makeObject_deprecated<ln::UIStyleSheet>();
	if (auto s = sheet->obtainStyle(_TT("NavigationBarItem"))) {
		s->borderThickness = ln::Thickness(4, 0, 0, 0);
		s->borderInset = true;
	}
    if (auto s = sheet->obtainStyle(_TT("NavigationBarItem:Unselected"))) {
        s->setBorderColor(ln::Color::Transparency);
        s->textColor = ln::UIColors::get(ln::UIColorHues::Grey, 4);
    }
    if (auto s = sheet->obtainStyle(_TT("NavigationBarItem:Selected"))) {
        s->setBorderColor(ln::UIColors::get(ln::UIColorHues::LightGreen));
        s->textColor = ln::Color::White;
    }


    ln::detail::EngineDomain::uiManager()->styleContext()->addStyleSheet(sheet);
}

void EditorApplication::onRoutedEvent(ln::UIEventArgs* e)
{
    if (e->type() == ln::UIEvents::RequestNavigate) {
        openProject(static_cast<ln::UIRequestNavigateEventArgs*>(e)->url());
        e->handled = true;
        return;
    }

    Application::onRoutedEvent(e);
}

void EditorApplication::newProject(const ln::Path& dirPath, const ln::String& projectName)
{
	if (!closeProject()) {
		return;
	}

	m_workspace->newMainProject(ln::Path(dirPath, projectName), projectName);
	postProjectLoaded();
}

void EditorApplication::openProject(const ln::Path& filePath)
{
    //closeProject();

    lna::AppData::current()->addRecentProjectFile(filePath);

    m_workspace->openMainProject(filePath);
	postProjectLoaded();
    //mainWindow()->mainHSplitter()->resetCellSize(0);

    //m_contentsViewManager->setup(m_workspace->project(), "ARPG-HC0");

    //m_assetBrowserContentsViewProvider = new AssetBrowserContentsViewProvider(m_workspace->project(), this);
    //m_contentsViewManager->addContentsViewProvider(m_assetBrowserContentsViewProvider);

    ////m_spritesetContentsViewProvider = new SpritesetContentsViewProvider(m_workspace->project(), this);
    ////m_contentsViewManager->addContentsViewProvider(m_spritesetContentsViewProvider);

    //m_audioContentsViewProvider->view()->setRootDir(
    //    LnToQt(ln::Path(m_workspace->project()->assetsDir(), _TT("Audio")));
}

bool EditorApplication::closeProject()
{
	bool result = mainWindow()->documentManager()->closeAllTabs();
	if (!result) {
		return false;
	}

	mainWindow()->navigatorManager()->unloadAdditionalNavigators();
    m_workspace->mainPluginManager()->deactivateAllExtensions(m_editorContext);
	m_workspace->closeMainProject();
	return true;
}

void EditorApplication::postProjectLoaded()
{

	mainWindow()->navigatorManager()->resetNavigators();

    m_workspace->mainPluginManager()->activateAllExtensions(m_editorContext);
}

void EditorApplication::handleNewProject(ln::UICommandEventArgs* e)
{
	auto dlg = ln::makeObject_deprecated<NewProjectDialog>();
	mainWindow()->addElement(dlg);
	dlg->open();

#if 0
	auto popupContent = ln::makeObject_deprecated<ln::UIText>();
	popupContent->setText(_TT("POP");

    m_dialog = ln::makeObject_deprecated<ln::UIDialog>();
	m_dialog->addElement(popupContent);
    m_dialog->setWidth(100);
    m_dialog->setHeight(200);
    m_dialog->setBackgroundColor(ln::Color::Green);
	mainWindow()->addElement(m_dialog);
	m_dialog->open();
    //mainWindow()->m_renderView->setDialog(m_dialog);

    //auto dlg = ln::PlatformSelectFolderDialog::create();
    //if (dlg->showDialog(mainWindow()->platformWindow())) {
    //}
#endif
}

void EditorApplication::handleOpenProject(ln::UICommandEventArgs* e)
{
    auto dlg = ln::PlatformOpenFileDialog::create();
    if (dlg->showDialog(mainWindow()->platformWindow())) {
        auto filePath = dlg->getPath();
        if (!filePath.isEmpty()) {
            openProject(filePath);
        }
    }
}
