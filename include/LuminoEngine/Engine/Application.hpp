#pragma once
#include "Object.hpp"

namespace ln {
namespace detail {
class ApplicationHelper;
}
class UIAction;
class UIEventArgs;



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

	virtual void onStart();

    /** 毎フレーム呼び出されます。 */
    LN_METHOD()
	virtual void onUpdate();

	virtual void onStop();

	virtual void onDestroy();

    void addAction(UIAction* action);

LN_CONSTRUCT_ACCESS:
	Application();
	virtual ~Application();

    /**  */
    LN_METHOD(RuntimeInitializer)
    void init();

protected:
    virtual void onRoutedEvent(UIEventArgs* e);

private:
	void initInternal();
	bool updateInertnal();
	void finalizeInternal();
	void run();

    Ref<List<Ref<UIAction>>> m_actions;

	friend class detail::ApplicationHelper;
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
} // namespace detail
} // namespace ln

#define LUMINO_APP(appClass) \
    extern "C" ::ln::Application* LuminoCreateApplicationInstance() \
    { \
        return new appClass(); \
    }

