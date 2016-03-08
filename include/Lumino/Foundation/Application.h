
#pragma once

#include "../EngineSettings.h"

LN_NAMESPACE_BEGIN

/**
	@brief		ゲームアプリケーションを表します。
*/
class GameApplication
{
public:

	/**
		@brief		アプリケーションの初期化設定を構築する際に呼び出されます。
	*/
	virtual void OnConfigure(EngineSettings* settings);

	/**
		@brief		アプリケーションを実行します。
	*/
	void Run();

protected:
	GameApplication();
	virtual ~GameApplication();
};

LN_NAMESPACE_END
