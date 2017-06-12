
#pragma once
#include "../Common.h"
#include "../Game/Component.h"

LN_NAMESPACE_BEGIN
class WorldObject;
namespace detail { class GameSceneManager; }

/**
	@brief		ゲームシーンのベースクラスです。
	@details	ゲームシーンはタイトル画面やゲームオーバー画面などの画面遷移の単位です。
*/
LN_CLASS()
class GameScene
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
protected:

	/** 開始処理 */
	LN_METHOD()
	virtual void OnStart();

	/// フレーム更新
	virtual void OnUpdate();

	/// 終了処理
	virtual void OnTerminate();

	

LN_CONSTRUCT_ACCESS:
	GameScene();
	virtual ~GameScene();

	/** Initialize */
	LN_METHOD()
	void initialize();

private:
	void AddGameObject(WorldObject* obj);
	void Update();

	List<RefPtr<WorldObject>>	m_gameObjectList;

	friend class WorldObject;
	friend class detail::GameSceneManager;
};

LN_NAMESPACE_END
