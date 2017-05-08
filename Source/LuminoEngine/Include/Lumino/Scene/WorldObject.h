
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


	virtual void OnRender(DrawList* context);

	/// 終了処理
	virtual void OnDestroy();

LN_INTERNAL_ACCESS:
	const Matrix& GetCombinedGlobalMatrix() const { return m_combinedGlobalMatrix; }

private:
	void UpdateFrame();
	void Render(DrawList* context);
	void ReleaseComponents();

	List<RefPtr<Component>>		m_components;

	List<RefPtr<WorldObject>>	m_children;
	WorldObject*				m_parent;
	Matrix						m_combinedGlobalMatrix;
	bool						m_isAutoRelease;

	friend class World;
};

/**
	@brief		
	@detail		このクラスのインスタンスは作成されると、現在のアクティブな 2D シーンに追加されます。
*/
class WorldObject2D
	: public WorldObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

LN_CONSTRUCT_ACCESS:
	WorldObject2D();
	virtual ~WorldObject2D();
	void Initialize();
};

/**
	@brief		
	@detail		このクラスのインスタンスは作成されると、現在のアクティブな 3D シーンに追加されます。
*/
class WorldObject3D
	: public WorldObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

LN_CONSTRUCT_ACCESS:
	WorldObject3D();
	virtual ~WorldObject3D();
	void Initialize();
};

LN_NAMESPACE_END
