
#pragma once
#include "../Common.h"
#include "../Game/Component.h"

LN_NAMESPACE_BEGIN
class DrawList;
class World;
class WorldObject;
using WorldObjectPtr = RefPtr<WorldObject>;

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
	void Initialize();

	void AddComponent(Component* component);

	/// 開始処理
	//virtual void OnStart();

	/// フレーム更新
	virtual void OnUpdate();

	/// 終了処理
	virtual void OnDestroy();

	virtual void OnRender(DrawList* context);

private:
	void Render(DrawList* context);
	void ReleaseComponents();
	List<RefPtr<Component>>	m_components;

	friend class World;
};

LN_NAMESPACE_END
