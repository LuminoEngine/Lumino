
#pragma once
#include "../Common.h"
#include "../Game/Component.h"

LN_NAMESPACE_BEGIN
class DrawList;
class World;
class WorldObject;
using WorldObjectPtr = Ref<WorldObject>;
class Component;

enum class BuiltinLayers
{
	Default2D,
	Default3D,
	Background2D,
	Background3D,
	Layer4,
	Layer5,
	Layer6,
	Layer7,
};

struct LayerMask
{
	static const int MaxLayers = 32;
	static const uint32_t All = 0xFFFFFFFF;

	static int GetLayer(BuiltinLayers builtinLayer);

	static int GetRenderOrder(int layerId);

	static bool filterComponent(Component* obj, uint32_t layerMask);
};

/**
	@brief		
*/
class WorldObject
	: public Object
{
	LN_OBJECT;
public:
	Transform	transform;

public:
	static WorldObjectPtr create();

	WorldObject();
	virtual ~WorldObject();
	void initialize();

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(Property)
	void setPosition(const Vector3& pos) { transform.position = pos; }

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD()
	void setPosition(float x, float y, float z = 0.0f) { setPosition(Vector3(x, y, z)); }

	/** このオブジェクトの位置を位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& getPosition() const { return transform.position; }

	/** このオブジェクトの回転を設定します。 */
	LN_METHOD(Property)
	void setRotation(const Quaternion& rot) { transform.rotation = rot; }

	/** このオブジェクトの回転をオイラー角から設定します。(radian) */
	LN_METHOD()
	void SetEulerAngles(float x, float y, float z) { transform.rotation = Quaternion::makeFromEulerAngles(Vector3(x, y, z)); }

	/** このオブジェクトの回転を取得します。 */
	LN_METHOD(Property)
	const Quaternion& getRotation() const { return transform.rotation; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(Property)
	void setScale(const Vector3& scale) { transform.scale = scale; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD()
	void setScale(float xyz) { transform.scale = Vector3(xyz, xyz, xyz); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD()
	void setScale(float x, float y, float z = 1.0f) { transform.scale = Vector3(x, y, z); }

	/** このオブジェクトの拡大率を取得します。 */
	LN_METHOD(Property)
	const Vector3& getScale() const { return transform.scale; }

	/** このオブジェクトにタグ文字列を設定します。 */
	LN_METHOD()
	void setTag(const StringRef& tag) { m_tag = tag; }

	/** このオブジェクトのタグ文字列を取得します。 */
	LN_METHOD(Property)
	const String& getTag() const { return m_tag; }

	/** このオブジェクトが属するレイヤーを設定します。 */
	LN_METHOD()
	void setLayer(int layer) { m_layer = layer; }

	/** このオブジェクトが属するレイヤーを取得します。 */
	LN_METHOD(Property)
	int getLayer() const { return m_layer; }

	World* getWorld() const;


	void addComponent(Component* component);

	void removeFromWorld();


	/// 開始処理
	//virtual void onStart();

	// 物理演算・衝突判定の前
	virtual void onPreUpdate();

	/// フレーム更新
	virtual void onUpdate(float deltaSceonds);

	


	virtual void onRender(DrawList* context);

	/// 終了処理
	virtual void onDestroy();

	virtual void onAttachedWorld(World* world);
	virtual void onDetachedWorld(World* world);

protected:
	virtual void onUIEvent(UIEventArgs* e);

LN_INTERNAL_ACCESS:
	//const Matrix& GetCombinedGlobalMatrix() const { return m_combinedGlobalMatrix; }
	const List<Ref<Component>>& getComponents() const { return m_components; }
	void setSpecialObject(bool enalbed) { m_isSpecialObject = true; }
	bool isSpecialObject() const { return m_isSpecialObject; }
	void updateWorldMatrixHierarchical();

private:
	void updateFrame(float deltaSceonds);
	void render(DrawList* context);
	void releaseComponents();

	List<Ref<Component>>		m_components;

	World*						m_world;
	List<Ref<WorldObject>>	m_children;
	WorldObject*				m_parent;
	String						m_tag;
	int							m_layer;
	//Matrix						m_combinedGlobalMatrix;
	bool						m_isAutoRelease;
	bool						m_isSpecialObject;	// World 内で特殊な扱いをされるオブジェクト。今はデフォルトのカメラとライトで、RemoveObjects で除外されない。

	friend class World;
};

///**
//	@brief		
//	@detail		このクラスのインスタンスは作成されると、現在のアクティブな 2D シーンに追加されます。
//*/
//class WorldObject2D
//	: public WorldObject
//{
//	LN_OBJECT;
//public:
//
//LN_CONSTRUCT_ACCESS:
//	WorldObject2D();
//	virtual ~WorldObject2D();
//	void initialize();
//};
//
///**
//	@brief		
//	@detail		このクラスのインスタンスは作成されると、現在のアクティブな 3D シーンに追加されます。
//*/
//class WorldObject3D
//	: public WorldObject
//{
//	LN_OBJECT;
//public:
//
//LN_CONSTRUCT_ACCESS:
//	WorldObject3D();
//	virtual ~WorldObject3D();
//	void initialize();
//};

LN_NAMESPACE_END
