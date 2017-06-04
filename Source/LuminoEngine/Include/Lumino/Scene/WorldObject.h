
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

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(Property)
	void SetPosition(const Vector3& pos) { transform.position = pos; }

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD()
	void SetPosition(float x, float y, float z = 0.0f) { SetPosition(Vector3(x, y, z)); }

	/** このオブジェクトの位置を位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& GetPosition() const { return transform.position; }

	/** このオブジェクトの回転を設定します。 */
	LN_METHOD(Property)
	void SetRotation(const Quaternion& rot) { transform.rotation = rot; }

	/** このオブジェクトの回転をオイラー角から設定します。(radian) */
	LN_METHOD()
	void SetEulerAngles(float x, float y, float z) { transform.rotation = Quaternion::MakeFromEulerAngles(Vector3(x, y, z)); }

	/** このオブジェクトの回転を取得します。 */
	LN_METHOD(Property)
	const Quaternion& GetRotation() const { return transform.rotation; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(Property)
	void SetScale(const Vector3& scale) { transform.scale = scale; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD()
	void SetScale(float xyz) { transform.scale = Vector3(xyz, xyz, xyz); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD()
	void SetScale(float x, float y, float z = 1.0f) { transform.scale = Vector3(x, y, z); }

	/** このオブジェクトの拡大率を取得します。 */
	LN_METHOD(Property)
	const Vector3& GetScale() const { return transform.scale; }

	/** このオブジェクトにタグ文字列を設定します。 */
	LN_METHOD()
	void SetTag(const StringRef& tag) { m_tag = tag; }

	/** このオブジェクトのタグ文字列を取得します。 */
	LN_METHOD(Property)
	const String& GetTag() const { return m_tag; }


	World* GetWorld() const;


	void AddComponent(Component* component);

	void RemoveFromWorld();


	/// 開始処理
	//virtual void OnStart();

	/// フレーム更新
	virtual void OnUpdate();


	virtual void OnRender(DrawList* context);

	/// 終了処理
	virtual void OnDestroy();

protected:
	virtual void OnUIEvent(UIEventArgs* e);

LN_INTERNAL_ACCESS:
	//const Matrix& GetCombinedGlobalMatrix() const { return m_combinedGlobalMatrix; }
	void SetSpecialObject(bool enalbed) { m_isSpecialObject = true; }
	bool IsSpecialObject() const { return m_isSpecialObject; }

private:
	void UpdateFrame();
	void Render(DrawList* context);
	void ReleaseComponents();

	List<RefPtr<Component>>		m_components;

	World*						m_world;
	List<RefPtr<WorldObject>>	m_children;
	WorldObject*				m_parent;
	String						m_tag;
	//Matrix						m_combinedGlobalMatrix;
	bool						m_isAutoRelease;
	bool						m_isSpecialObject;	// World 内で特殊な扱いをされるオブジェクト。今はデフォルトのカメラとライトで、RemoveObjects で除外されない。

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
