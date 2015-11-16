
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		ゲームシーンのベースクラスです。
	@details	ゲームシーンはタイトル画面やゲームオーバー画面などの画面遷移の単位です。
*/
class GameScene
{
public:

	/// 開始処理
	virtual void OnStart();

	/// フレーム更新
	virtual void OnUpdate();

	/// 終了処理
	virtual void OnTerminate();

protected:
	GameScene();
	virtual ~GameScene();
};

LN_NAMESPACE_END
