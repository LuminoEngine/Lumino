#pragma once
#include <LuminoEngine/Engine/CoreApplication.hpp>
#include "Common.hpp"

namespace ln {
namespace detail {
class ApplicationHelper;
class UIManager;
class EngineManager;
}
class GraphicsCommandList;
class RenderTargetTexture;
class UICommand;
class UIAction;
class UIEventArgs;
class UIMainWindow;
class ApplicationSetupSettings;
class StandaloneApplicationRunner;

/** グローバルなアプリケーション状態を扱うための基本クラスです。 */
LN_CLASS()
class LN_API Application : public CoreApplication {
    LN_OBJECT;

public:
    /**
     * アプリケーションの初期化中に呼び出されます。
     * 
     * この関数は、フレームワーク内部で作成される基本オブジェクトをオーバーライドするために実装できます。
     * 代表的なものでは、ユーザーが実装した UIMainWindow の派生クラスをアプリケーションに登録できます。
     */
    LN_METHOD()
    virtual void onSetup(ApplicationSetupSettings* settings);

    /** エンジンの初期化処理が完了した後に呼び出されます。 */
    LN_METHOD()
    virtual void onInit();

    //virtual void onStart();

    /** 毎フレーム呼び出されます。 */
    LN_METHOD()
    virtual void onUpdate();

    //virtual void onStop();

    virtual void onDestroy();

    void addApplicationCommand(UICommand* command);
    void addAction(UIAction* action);

    /** デフォルトで作成されるメインの World を取得します。 */
    LN_METHOD()
    World* world() const;

    /**
	 * Application の実行を開始します。
	 */
    LN_METHOD()
    void run();

    virtual ~Application();

LN_CONSTRUCT_ACCESS:
    Application();

    /**  */
    LN_METHOD(RuntimeInitializer)
    void init();

protected:
    void setupMainWindow(UIMainWindow* window, bool createBasicObjects); // onInit でのみ可

    virtual void onRoutedEvent(UIEventArgs* e);

    [[deprecated]]
    void initInternal();
    void updateInertnal();
    void finalizeInternal();

    detail::EngineManager* m_manager;
    List<Ref<UICommand>> m_commands;
    List<Ref<UIAction>> m_actions;
    bool m_initialized;

    friend class detail::ApplicationHelper;
    friend class detail::UIManager;
    friend class StandaloneApplicationRunner;
};

class AppData {
public:
    static void setValue(const StringView& key, Ref<Variant> value);
    static Ref<Variant> getValue(const StringView& key); // null = NotFound.
};

/**  */
LN_CLASS()
class ApplicationSetupSettings : public Object {
public:
    void setMainWindow(UIMainWindow* value) noexcept;

private:
    Ref<UIMainWindow> m_mainWindow;

    friend class StandaloneApplicationRunner;
};

#if 0
class AppIntegration {
public:
    typedef void (*ConfigureApp)();
    typedef Application* (*CreateAppInstance)();

    // for external main loop (emscripten, android)
    static Result initialize(ConfigureApp configureApp, CreateAppInstance createAppInstance);
    static bool update();
    static void render(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget);
    static void terminate();

    // for internal main loop (win32, macOS...)
    static void run(ConfigureApp configureApp, CreateAppInstance createAppInstance);

    // for internal
    static void preConfigure();

private:

    static Ref<Application> s_app;
};
#endif


namespace detail {
class LN_API ApplicationHelper {
public:
    static void preConfigure();

    // for external main loop (emscripten, android)
    static void init(Application* app);
    static bool processTick(Application* app);
    static void finalize(Application* app);

    // for internal main loop (win32, macOS...)
    static void run(Application* app);

    static void callOnRoutedEvent(Application* app, UIEventArgs* e) { app->onRoutedEvent(e); }
};

class AppDataInternal : public RefObject {
public:
    void setValue(const StringView& key, Ref<Variant> value);
    Ref<Variant> getValue(const StringView& key) const; // null = NotFound.
    void attemptSave();
    void attemptLoad();

private:
    Path makeFilePath() const;
    void save(const Path& filePath);
    void load(const Path& filePath);

    std::unordered_map<String, Ref<Variant>> m_values;
};

} // namespace detail
} // namespace ln

#define LUMINO_APP(appClass)                                          \
    extern "C" void LuminoConfigureApplication() {                    \
        ::ln::detail::ApplicationHelper::preConfigure();              \
        appClass::configure();                                        \
    }                                                                 \
    extern "C" ::ln::Application* LuminoCreateApplicationInstance() { \
        return new appClass();                                        \
    }
