
#pragma once
#include "../Platform/Common.h"
#include "../EngineSettings.h"

LN_NAMESPACE_BEGIN
class EngineManager;
class GameScene;
namespace detail { class GameSceneManager; }
class UIMainWindow;

/**
	@brief		アプリケーションを表します。
*/
class Application
	: public RefObject
{
public:

	/** アプリケーションのメインウィンドウを取得します。 */
	static UIMainWindow* getMainWindow();


	PlatformWindow* getNativeMainWindow();

protected:
	Application();
	virtual ~Application();

LN_INTERNAL_ACCESS:
	void initialize(EngineManager* engineManager);

private:
	EngineManager*	m_engineManager;
};

namespace detail
{
class InternalApplicationImpl
	: public Application
{
public:
	InternalApplicationImpl() = default;
	virtual ~InternalApplicationImpl() = default;
};
} // namespace detail


/**
	@brief		ゲームアプリケーションを表します。
*/
LN_CLASS()
class GameApplication
	: public Object
{
	LN_OBJECT;
public:
	GameApplication();
	virtual ~GameApplication();

	/**
		@brief		アプリケーションの初期化設定を構築する際に呼び出されます。
	*/
	virtual void OnConfigure();

	/**
		@brief		アプリケーションを実行します。
	*/
	LN_METHOD()
	void run(GameScene* initialScene = nullptr);

LN_INTERNAL_ACCESS:
	LN_METHOD()
	void initialize();

private:
	Ref<detail::GameSceneManager>	m_gameSceneManager;
	
};

LN_NAMESPACE_END
