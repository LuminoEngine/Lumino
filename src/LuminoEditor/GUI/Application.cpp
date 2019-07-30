
#include "../../LuminoEngine/src/Engine/EngineManager.hpp"
#include "../../LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../LuminoEngine/src/Platform/PlatformManager.hpp"
#include "../../LuminoEngine/src/UI/UIManager.hpp"
#include <Workspace.hpp>
#include <Project.hpp>
#include <PluginManager.hpp>
#include "NavigatorManager.hpp"
#include "MainWindow.hpp"
#include "Application.hpp"

Ref<ln::UICommand> EditorApplication::NewCommand;
Ref<ln::UICommand> EditorApplication::OpenCommand;

static EditorApplication* s_app = nullptr;

EditorApplication* EditorApplication::instance()
{
    assert(s_app);
    return s_app;
}

EditorApplication::EditorApplication()
{
    assert(!s_app);
    s_app = this;

    NewCommand = ln::makeObject<ln::UICommand>();
    OpenCommand = ln::makeObject<ln::UICommand>();

    addAction(ln::makeObject<ln::UIAction>(NewCommand, [this](ln::UICommandEventArgs* x){ onNewProject(x); }));
    addAction(ln::makeObject<ln::UIAction>(OpenCommand, [this](ln::UICommandEventArgs* x) { onOpenProject(x); }));
}

EditorApplication::~EditorApplication()
{
    s_app = nullptr;
}

ln::Result EditorApplication::init()
{
	ln::EngineSettings::setMainWindowSize(1000, 600);
	ln::EngineSettings::setMainBackBufferSize(1000, 600);
    ln::detail::EngineDomain::engineManager()->settings().defaultObjectsCreation = false;
    ln::detail::EngineDomain::engineManager()->init();
    m_workspace = ln::makeObject<lna::Workspace>();
    onInit();


    return true;
}

void EditorApplication::dispose()
{
    ln::detail::EngineDomain::release();
}

void EditorApplication::run()
{
    while (!ln::detail::EngineDomain::engineManager()->isExitRequested())
    {
        ln::detail::EngineDomain::engineManager()->platformManager()->windowManager()->processSystemEventQueue(ln::detail::EventProcessingMode::Polling);
        ln::detail::EngineDomain::engineManager()->uiManager()->dispatchPostedEvents();
        ln::Thread::sleep(1);
    }
}

MainWindow* EditorApplication::mainWindow() const
{
    auto ptr = ln::static_pointer_cast<MainWindow>(ln::detail::EngineDomain::engineManager()->mainWindow());
    return ptr;
}

void EditorApplication::setMainWindow(ln::UIMainWindow* window)
{
	return ln::detail::EngineDomain::engineManager()->setMainWindow(window);
}

void EditorApplication::importFile(const ln::Path& filePath)
{
    if (m_workspace->project())
    {
        auto exts = m_workspace->project()->pluginManager()->getAssetImporterExtensions(filePath);
        if (exts.size() != 1) {
            LN_NOTIMPLEMENTED();
            return;
        }

        exts[0].second->import(filePath);
    }
}

void EditorApplication::onInit()
{
	setMainWindow(ln::makeObject<MainWindow>());
}

void EditorApplication::closeProject()
{
}

void EditorApplication::onNewProject(ln::UICommandEventArgs* e)
{
}

void EditorApplication::onOpenProject(ln::UICommandEventArgs* e)
{
    auto dlg = ln::PlatformFileOpenDialog::create();
    if (dlg->showDialog(mainWindow()->platformWindow())) {
        auto filePath = dlg->getFilePath();
        if (!filePath.isEmpty()) {
            closeProject();

            m_workspace->openProject2(filePath);

            mainWindow()->navigatorManager()->resetNavigators();

            //mainWindow()->mainHSplitter()->resetCellSize(0);

            //m_contentsViewManager->setup(m_workspace->project(), "ARPG-HC0");

            //m_assetBrowserContentsViewProvider = new AssetBrowserContentsViewProvider(m_workspace->project(), this);
            //m_contentsViewManager->addContentsViewProvider(m_assetBrowserContentsViewProvider);

            ////m_spritesetContentsViewProvider = new SpritesetContentsViewProvider(m_workspace->project(), this);
            ////m_contentsViewManager->addContentsViewProvider(m_spritesetContentsViewProvider);

            //m_audioContentsViewProvider->view()->setRootDir(
            //    LnToQt(ln::Path(m_workspace->project()->assetsDir(), u"Audio")));
        }
    }
}
