
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
	LN_OBJECT;
protected:

	/** 開始処理 */
	LN_METHOD()
	virtual void onStart();

	/// フレーム更新
	virtual void onUpdate();

	/// 終了処理
	virtual void onTerminate();

	

LN_CONSTRUCT_ACCESS:
	GameScene();
	virtual ~GameScene();

	/** Initialize */
	LN_METHOD()
	bool initialize();

private:
	void addGameObject(WorldObject* obj);
	void update();

	List<Ref<WorldObject>>	m_gameObjectList;

	friend class WorldObject;
	friend class detail::GameSceneManager;
};

LN_NAMESPACE_END
