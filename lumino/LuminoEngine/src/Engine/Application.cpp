
#include "Internal.hpp"
#include <yaml-cpp/yaml.h>
#include <optional>
#include "EngineManager.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/Engine/Application.hpp>
#include "UI/UIManager.hpp"

namespace ln {

//==============================================================================
// Application

LN_OBJECT_IMPLEMENT(Application, Object) {}

Application::Application()
    : m_manager(nullptr)
    , m_commands()
    , m_actions()
    , m_initialized(false) {
}

Application::~Application() {
}

Result Application::init() {
    return CoreApplication::init();
}

//void Application::setupMainWindow(ln::UIMainWindow* window, bool createBasicObjects) {
//}

// NOTE: なぜデフォルトでは UIMainWindow を内部で作るのか？
// ----------
// ゲームアプリやシングルページアプリを作るときはあまり縁が無いため。
// UI を作る際も、UIMainWindow ではなく UIRenderView をよく使う。
// UIMainWindow を使うのは、ImGui 等と一緒にエディタ系のアプリを作る場合がほとんどである。
//void Application::onSetup(ApplicationSetupSettings* settings) {
//}

void Application::onInit() {
}

//void Application::onStart()
//{
//}

void Application::onUpdate() {
}

//void Application::onStop()
//{
//}

void Application::onDestroy() {
}

void Application::addApplicationCommand(UICommand* command) {
    m_commands.add(command);
}

void Application::addAction(UIAction* action) {
    m_actions.add(action);
}

World* Application::world() const {
    return m_manager->mainWorld();
}

void Application::onRoutedEvent(UIEventArgs* e) {
    // ショートカットキーの実装
    for (auto& c : m_commands) {
        if (c->testInputEvent(e)) {
            e->handled = true;
            UICommandEventArgs::raiseExecute(detail::EngineDomain::uiManager()->forcusedElement(), c);
            return;
        }
    }

    if (detail::EngineDomain::uiManager()->handleCommonInputCommands(e)) {
        return;
    }

    if (detail::UICommandInternal::handleCommandRoutedEvent(e, m_actions)) {
        return;
    }
}

void Application::initInternal() {
    m_manager = detail::EngineDomain::engineManager();
    onInit();
}

void Application::updateInertnal() {
    if (!m_initialized) {
        initInternal();
        m_initialized = true;
    }

    onUpdate();
}

void Application::finalizeInternal() {
    onDestroy();
}

void Application::run() {
#ifdef __EMSCRIPTEN__
    LN_UNREACHABLE();
#endif
    //initInternal();
    EngineContext::current()->initializeEngineManager();
    EngineContext::current()->engineManager()->initializeAllManagers();

    do {

        detail::EngineDomain::engineManager()->updateFrame();
        detail::EngineDomain::engineManager()->presentFrame(nullptr, nullptr);
        //detail::EngineDomain::engineManager()->renderFrame();
    } while (!detail::EngineDomain::engineManager()->isExitRequested());

    detail::EngineDomain::release();
    //finalizeInternal();
}

//==============================================================================
// AppData

void AppData::setValue(const StringView& key, Ref<Variant> value) {
    detail::EngineDomain::engineManager()->appData()->setValue(key, value);
}

Ref<Variant> AppData::getValue(const StringView& key) {
    return detail::EngineDomain::engineManager()->appData()->getValue(key);
}

//==============================================================================
// ApplicationSetupSettings

void ApplicationSetupSettings::setMainWindow(UIMainWindow* value) noexcept {
    m_mainWindow = value;
}

#if 0
//==============================================================================
// AppIntegration

Ref<Application> AppIntegration::s_app;

Result AppIntegration::initialize(ConfigureApp configureApp, CreateAppInstance createAppInstance) {
    configureApp();

    if (!initializeEngine()) {
        return err();
    }

    s_app = Ref<Application>(createAppInstance(), false);

    return ok();
}

bool AppIntegration::update() {
    return updateEngine();
}

void AppIntegration::render(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget) {
    renderEngine(commandList, renderTarget);
}

void AppIntegration::terminate() {
    terminateEngine();
    s_app = nullptr;
}

void AppIntegration::run(ConfigureApp configureApp, CreateAppInstance createAppInstance) {
    if (!initialize(configureApp, createAppInstance)) {
        return;
    }

    while (update()) {
        render(nullptr, nullptr);
    }
    terminate();
}

void AppIntegration::preConfigure() {
    // v0.11.0 以降、ln::Enigne を使ったメインループよりも ln::Application 及び ln::AppIntegration を推奨する。
    // ln::Enigne はより低レイヤーのものと位置づけ、各種モジュールの初期化のみを行い、ウィンドウ等のオブジェクトは
    // デフォルトでは生成しないようにしたい。より細かく Lumino を制御したい人向けのクラスとする。
    detail::EngineManager::s_settings.defaultObjectsCreation = true;
}

Result AppIntegration::initializeEngine() {
    EngineContext::current()->initializeEngineManager();
    EngineContext::current()->engineManager()->initializeAllManagers();
    return ok();
}

bool AppIntegration::updateEngine() {
    detail::EngineDomain::engineManager()->updateFrame();
    return !detail::EngineDomain::engineManager()->isExitRequested();
}

void AppIntegration::renderEngine(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget) {
    detail::EngineDomain::engineManager()->presentFrame(commandList, renderTarget);
}

void AppIntegration::terminateEngine() {
    detail::EngineDomain::release();
}
#endif
 
//==============================================================================
// ApplicationHelper

namespace detail {

void ApplicationHelper::preConfigure() {
    // v0.11.0 以降、ln::Enigne を使ったメインループよりも ln::Application 及び ln::AppIntegration を推奨する。
    // ln::Enigne はより低レイヤーのものと位置づけ、各種モジュールの初期化のみを行い、ウィンドウ等のオブジェクトは
    // デフォルトでは生成しないようにしたい。より細かく Lumino を制御したい人向けのクラスとする。
    detail::EngineManager::s_settings.defaultObjectsCreation = true;
}

void ApplicationHelper::init(Application* app) {
    //app->initInternal();
}

bool ApplicationHelper::processTick(Application* app) {
    detail::EngineDomain::engineManager()->updateFrame();
    return !detail::EngineDomain::engineManager()->isExitRequested();
    //return app->updateInertnal();
}

void ApplicationHelper::finalize(Application* app) {
    detail::EngineDomain::release();
    //app->finalizeInternal();
}

void ApplicationHelper::run(Application* app) {
    app->run();
}

//==============================================================================
// AppDataInternal

void AppDataInternal::setValue(const StringView& key, Ref<Variant> value) {
    m_values[key] = value;
}

Ref<Variant> AppDataInternal::getValue(const StringView& key) const {
    auto itr = m_values.find(key);
    if (itr != m_values.end())
        return itr->second;
    else
        return nullptr;
}

void AppDataInternal::attemptSave() {
    if (!m_values.empty()) {
        save(makeFilePath());
    }
}

void AppDataInternal::attemptLoad() {
    const auto path = makeFilePath();
    if (FileSystem::existsFile(path)) {
        load(path);
    }
}

Path AppDataInternal::makeFilePath() const {
    return Path::combine(Environment::specialFolderPath(SpecialFolder::ApplicationData), _TT("Lumino"), _TT("CommonAppData.yml"));
}

void AppDataInternal::save(const Path& filePath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    for (const auto& pair : m_values) {
        out << YAML::Key;
        out << pair.first.toStdString();

        if (pair.second->type() == VariantType::Int) {
            out << pair.second->get<int>();
        }
        else if (pair.second->type() == VariantType::Float) {
            out << pair.second->get<float>();
        }
        else if (pair.second->type() == VariantType::String) {
            out << pair.second->get<String>().toStdString();
        }
    }
    out << YAML::EndMap;

    FileSystem::writeAllBytes(filePath, out.c_str(), strlen(out.c_str()));
}

// TODO: cpp-yaml は値の型を読み取ることができない。
// try で頑張る必要があるが、これも暗黙の型変換が働いたりするため float, int, string 以上に増やすと対応できなくなる可能性が高い。
// JSON にしたほうがよさそう。
void AppDataInternal::load(const Path& filePath) {
    const auto buffer = FileSystem::readAllBytes(filePath);
    const auto text = std::string(reinterpret_cast<const char*>(buffer.unwrap().data()), buffer.unwrap().size());
    YAML::Node doc = YAML::Load(text);

    for (auto itr = doc.begin(); itr != doc.end(); ++itr) {
        std::string key;
        key = itr->first.as<std::string>();

        const YAML::Node node = itr->second;
        Ref<Variant> value;
        {
            try {
                value = makeVariant(node.as<float>());
            }
            catch (const YAML::BadConversion& e) {
            }
        }
        if (!value) {
            try {
                value = makeVariant(node.as<int>());
            }
            catch (const YAML::BadConversion& e) {
            }
        }
        if (!value) {
            try {
                value = makeVariant(String::fromStdString(node.as<std::string>()));
            }
            catch (const YAML::BadConversion& e) {
            }
        }

        m_values[String::fromStdString(key)] = value;
    }
}

} // namespace detail
} // namespace ln
