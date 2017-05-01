
#pragma once
#include "../Common.h"
#include "../Game/Component.h"

LN_NAMESPACE_BEGIN
class WorldObject;
using WorldObjectPtr = RefPtr<WorldObject>;
namespace detail { class GameSceneManager; }

/**
	@brief		
*/
class WorldObject
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	Transform	transform;

public:
	static WorldObjectPtr Create();

	WorldObject();
	virtual ~WorldObject();

	void AddComponent(Component* component);

	/// 開始処理
	//virtual void OnStart();

	/// フレーム更新
	virtual void OnUpdate();

	/// 終了処理
	virtual void OnDestroy();

private:
	void ReleaseComponents();
	List<RefPtr<Component>>	m_components;
};

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
	void Initialize();

private:
	void AddGameObject(WorldObject* obj);
	void Update();

	List<WorldObjectPtr>	m_gameObjectList;

	friend class WorldObject;
	friend class detail::GameSceneManager;
};

LN_NAMESPACE_END
