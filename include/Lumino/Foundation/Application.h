
#pragma once
#include "../Platform/Common.h"
#include "../EngineSettings.h"

LN_NAMESPACE_BEGIN
class GameScene;
namespace detail { class GameSceneManager; }

/**
	@brief		アプリケーションを表します。
*/
class Application
	: public RefObject
{
public:
	PlatformWindow* GetNativeMainWindow();

protected:
	Application();
	virtual ~Application();

LN_INTERNAL_ACCESS:
	void Initialize(EngineManager* engineManager);

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
class GameApplication
{
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
	void Run(GameScene* initialScene = nullptr);

protected:

private:
	RefPtr<detail::GameSceneManager>	m_gameSceneManager;
	
};

LN_NAMESPACE_END
