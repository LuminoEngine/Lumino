
#pragma once
#include "../Common.h"
#include "../Game/Component.h"

LN_NAMESPACE_BEGIN
class GameObject;
using GameObjectPtr = RefPtr<GameObject>;

/**
	@brief		
*/
class GameObject
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	Transform	transform;

public:
	static GameObjectPtr Create();

	GameObject();
	virtual ~GameObject();

	void AddComponent(Component* component);

	/// 開始処理
	virtual void OnStart();

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
class GameScene
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	GameScene();
	virtual ~GameScene();

	/// 開始処理
	virtual void OnStart();

	/// フレーム更新
	virtual void OnUpdate();

	/// 終了処理
	virtual void OnTerminate();

private:
	void AddGameObject(GameObject* obj);

	List<GameObjectPtr>	m_gameObjectList;

	friend class GameObject;
};

LN_NAMESPACE_END
