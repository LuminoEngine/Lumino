
#include "ViewModel.hpp"
#include "MainWindow.hpp"
#include "Application.hpp"

Application::Application()
{
    ln::EngineSettings::setSceneIntegrationEnabled(false);
    ln::EngineSettings::setMainWindowSize(1280, 960);
    //ln::EngineSettings::setMainWindowSize(0.75 * 640, 0.75 * 480);
    ln::EngineSettings::setDevelopmentToolsEnabled(true);
    //ln::EngineSettings::setGraphicsDebugEnabled(true);
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



class TestNode
{
public:
    virtual ~TestNode() = default;
    TestNode* m_next = nullptr;
};

struct TestNode2 : public TestNode
{
public:
    int value;
    //Ref<ln::Sprite> sprite;
};

struct List
{
    TestNode* m_head = nullptr;
    TestNode* m_tail = nullptr;

    void add(TestNode* node)
    {
        if (!m_head) {
            m_head = m_tail = node;
        }
        else {
            m_tail->m_next = node;
            m_tail = node;
        }
        node->m_next = nullptr;
    }

    void clear()
    {
        m_head = nullptr;
        m_tail = nullptr;
        // node はこの後は使わない想定なのでクリアの必要は無し
    }
};

void Application::onInit()
{
	setupMainWindow(ln::makeObject<MainWindow>(false), true);
    ln::Engine::renderView()->setGuideGridEnabled(true);
    ln::Engine::mainCamera()->addComponent(ln::CameraOrbitControlComponent::create());
    ln::Engine::renderView()->setBackgroundColor(ln::Color::Gray);
    ln::Engine::renderView()->setGizmoEnabled(true);
    
    ViewModel::instance()->load();

    //m_mesh = ln::Mesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.glb");


#if 0
    Ref<ln::Sprite> sprite = ln::Sprite::create();

#define SEQUENTIAL 1
    

    static const size_t count = 1000000;
#if SEQUENTIAL
    std::vector<TestNode2> instances(count);
    for (size_t i = 0; i < count; i++) {
        instances[i].value = rand();
        //instances[i].sprite = sprite;
    }
#else
    std::vector<std::unique_ptr<TestNode2>> instances;
    for (size_t i = 0; i < count; i++) {
        auto p = std::make_unique<TestNode2>();
        p->value = rand();
        //p->sprite = sprite;
        instances.push_back(std::move(p));
    }
#endif

    for (int i = 0; i < 20; i++)
    {
        ln::ElapsedTimer t;
        t.start();

        std::vector<TestNode2*> list;
        list.reserve(count);
        int64_t dummy = 0;

        // 追加
        for (size_t i = 0; i < count; i++) {
#if SEQUENTIAL
            list.push_back(&instances[i]);
#else
            list.push_back(instances[i].get());
#endif
        }

        uint64_t t1 = t.elapsedNanoseconds();

        // 列挙
        const size_t count = list.size();
        for (size_t i = 0; i < count; i++) {
            dummy += list[i]->value;
        }

        uint64_t t2 = t.elapsedNanoseconds();

        // 削除
        list.clear();

        std::cout << dummy << "\t" << t1 << "[ns], " << t2 << "[ns], " << t.elapsedNanoseconds() << "[ns]" << std::endl;
    }


    for (int i = 0; i < 20; i++)
    {
        ln::ElapsedTimer t;
        t.start();

        List list;
        int64_t dummy = 0;

        // 追加
        for (int i = 0; i < count; i++) {
#if SEQUENTIAL
            list.add(&instances[i]);
#else
            list.add(instances[i].get());
#endif
        }

        uint64_t t1 = t.elapsedNanoseconds();

        // 列挙
        TestNode* n = list.m_head;
        while (n) {
            dummy += static_cast<TestNode2*>(n)->value;
            n = n->m_next;
        }

        uint64_t t2 = t.elapsedNanoseconds();

        // 削除
        list.clear();

        std::cout << dummy << "\t" << t1 << "[ns], " << t2 << "[ns], " << t.elapsedNanoseconds() << "[ns]" << std::endl;
    }
#endif
}
