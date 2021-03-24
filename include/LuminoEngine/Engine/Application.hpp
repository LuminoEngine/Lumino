#pragma once
#include "Common.hpp"
#include "Object.hpp"

namespace ln {
namespace detail {
class ApplicationHelper;
class UIManager;
}
class UICommand;
class UIAction;
class UIEventArgs;
class UIMainWindow;


/** グローバルなアプリケーション状態を扱うための基本クラスです。 */
LN_CLASS()
class LN_API Application
	: public Object
{
    LN_OBJECT;
public:
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

LN_CONSTRUCT_ACCESS:
	Application();
	virtual ~Application();

    /**  */
    LN_METHOD(RuntimeInitializer)
    void init();

protected:
	void setupMainWindow(UIMainWindow* window, bool createBasicObjects);	// onInit でのみ可

    virtual void onRoutedEvent(UIEventArgs* e);

private:
	void initInternal2();
	void updateInertnal2();
	void finalizeInternal2();

	detail::EngineManager* m_manager;
    List<Ref<UICommand>> m_commands;
    List<Ref<UIAction>> m_actions;
	bool m_initialized;

	friend class detail::ApplicationHelper;
	friend class detail::UIManager;
};

class AppData
{
public:
	static void setValue(const StringRef& key, Ref<Variant> value);
	static Ref<Variant> getValue(const StringRef& key);	// null = NotFound.

};

namespace detail {
class LN_API ApplicationHelper
{
public:
	// for external main loop (emscripten, android)
	static void init(Application* app);
	static bool processTick(Application* app);
	static void finalize(Application* app);

	// for internal main loop (win32, macOS...)
	static void run(Application* app);

    static void callOnRoutedEvent(Application* app, UIEventArgs* e) { app->onRoutedEvent(e); }
};

class AppDataInternal : public RefObject
{
public:
	void setValue(const StringRef& key, Ref<Variant> value);
	Ref<Variant> getValue(const StringRef& key) const;	// null = NotFound.
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

#define LUMINO_APP(appClass) \
    extern "C" ::ln::Application* LuminoCreateApplicationInstance() \
    { \
        return new appClass(); \
    }

