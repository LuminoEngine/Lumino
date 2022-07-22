
#pragma once
#include "Common.hpp"
#include <LuminoEngine/Base/Builder.hpp>
#include <LuminoEngine/Base/Collection.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include "Component.hpp"

namespace ln {
class World;
class Level;
class WorldObject;
class Component;
class RenderingContext;

// TODO: Delegate 等と同じように using ComponentList = Collection<Ref<Component>>; にしたい
/** ComponentList */
LN_CLASS()
class ComponentList : public Collection<Ref<Component>>
{
public:
    void serialize_deprecated(Serializer2_deprecated& ar) override { Collection<Ref<Component>>::serialize_deprecated(ar); }

};

namespace detail {

class WorldObjectTransform
    : public RefObject
{
public:

    WorldObjectTransform(WorldObject* parent);

    /** 位置を設定します。 */
    void setPosition(const Vector3& pos);
    
	/** 位置を取得します。 */
	const Vector3& position() const { return m_position; }
    
	/** このオブジェクトの回転を設定します。 */
	void setRotation(const Quaternion& rot);
    
	/** このオブジェクトの回転を取得します。 */
	const Quaternion& rotation() const { return m_rotation; }

	/** このオブジェクトの拡大率を設定します。 */
	void setScale(const Vector3& scale);
    
	/** このオブジェクトの拡大率を取得します。 */
	const Vector3& scale() const { return m_scale; }
    
	/** このオブジェクトのローカルの中心位置を設定します。 */
	void setCenterPoint(const Vector3& value);
    
	/** このオブジェクトのローカルの中心位置を取得します。 */
	const Vector3& centerPoint() const { return m_center; }

    // TODO:Forward?
    Vector3 getFront() const
    {
        return Vector3::transform(Vector3::UnitZ, m_rotation);
    }

    Vector3 up() const
    {
        return Vector3::transform(Vector3::UnitY, m_rotation);
    }

    void lookAt(const Vector3& target, const Vector3& up = Vector3::UnitY);

    Matrix getLocalMatrix() const;




    //void updateWorldMatrix(const Matrix* parent);
    //const Matrix& worldMatrix() const { return m_worldMatrix; }

private:
    WorldObject* m_parent;
    Vector3 m_position;
    Quaternion m_rotation;
    Vector3 m_scale;
    Vector3 m_center;

	friend class ::ln::WorldObject;
};

class IWorldObjectVisitor
{
public:
    // 継続する場合は true, やめる場合は false
    virtual bool visit(WorldObject* obj) = 0;
};

} // namespace detail


/** World 内の全エンティティのベースクラスです。 */
LN_CLASS()
class WorldObject
	: public Object
	, public IWorldRenderingElement
{
    LN_OBJECT;
	LN_BUILDER;
public:

	const String& name() const { return m_name; }
	void setName(const String& value) { m_name = value; }

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(Property)
	void setPosition(const Vector3& pos) { m_transform->setPosition(pos); }

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(OverloadPostfix = "XYZ")
	void setPosition(float x, float y, float z = 0.0f) { setPosition(Vector3(x, y, z)); }

	/** このオブジェクトの位置を位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& position() const { return m_transform->position(); }

	/** このオブジェクトの回転を設定します。 */
	LN_METHOD()
	void setRotation(const Quaternion& rot) { m_transform->setRotation(rot); }

	/** このオブジェクトの回転をオイラー角から設定します(radian単位) 。回転順序は Z(Roll) > X(Pich) > Y(Yaw) です。 */
	LN_METHOD(OverloadPostfix = "XYZ")
	void setRotation(float x, float y, float z) { setRotation(Quaternion::makeFromYawPitchRoll(y, x, z)); }

	/** このオブジェクトの回転を取得します。 */
	LN_METHOD(Property)
	const Quaternion& rotation() const { return m_transform->rotation(); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(Property)
	void setScale(const Vector3& scale) { m_transform->setScale(scale); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(OverloadPostfix = "S")
	void setScale(float xyz) { setScale(Vector3(xyz, xyz, xyz)); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(OverloadPostfix = "XYZ")
	void setScale(float x, float y, float z = 1.0f) { setScale(Vector3(x, y, z)); }

	/** このオブジェクトの拡大率を取得します。 */
	LN_METHOD(Property)
	const Vector3& scale() const { return m_transform->scale(); }
    
	/** このオブジェクトのローカルの中心位置を設定します。 */
	LN_METHOD(Property)
	void setCenterPoint(const Vector3& value) { m_transform->setCenterPoint(value); }
    
	/** このオブジェクトのローカルの中心位置を設定します。 */
    LN_METHOD(OverloadPostfix = "XYZ")
	void setCenterPoint(float x, float y, float z = 0.0f) { setCenterPoint(Vector3(x, y, z)); }

	/** このオブジェクトのローカルの中心位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& centerPoint() const { return m_transform->centerPoint(); }

	/** 指定した座標を向くように、オブジェクトを回転させます。 */
	LN_METHOD()
	void lookAt(const Vector3& target) { lookAt(target, Vector3::UnitY); }

	/** 指定した座標を向くように、オブジェクトを回転させます。 */
	LN_METHOD(OverloadPostfix = "XYZ")
    void lookAt(float x, float y, float z) { return lookAt(Vector3(x, y, z), Vector3::UnitY); }

	/** 指定した座標を向くように、オブジェクトを回転させます。 */
	void lookAt(const Vector3& target, const Vector3& up);

	/** 指定した座標を向くように、オブジェクトを回転させます。 */
    void lookAt(float x, float y, float z, const Vector3& up) { return lookAt(Vector3(x, y, z), up); }

	/** Component を追加します。 */
	LN_METHOD()
    void addComponent(Component* component);

	/** Component を除外します。 */
	LN_METHOD()
	void removeComponent(Component* component);

	void removeAllComponents();

	/** タグを追加します。 */
	LN_METHOD()
    void addTag(const StringView& tag) { m_tags->add(tag); }

	/** タグを除外します。 */
	LN_METHOD()
    void removeTag(const StringView& tag) { m_tags->remove(tag); }

	/** 指定したタグを持っているかを確認します。 */
	LN_METHOD()
    bool hasTag(const StringView& tag) const { return m_tags->contains(tag); }

	/**
	 * WorldObject を破棄します。
	 *
	 * 実際の削除は、現在のフレームのアップデート処理後に行われます。
	 * 削除された WorldObject は、親の World, Level, WorldObject からも除外されます。
	 */
	LN_METHOD()
	void destroy();

	/** destroy() が呼び出され、オブジェクトが破棄されようとしているか、または破棄されたかを確認します。実際の削除は、現在のフレームのアップデート処理後に行われます。 */
	bool destroyed() const { return m_destroyed; }

	/** このオブジェクトを直ちに World から除外します。このメソッドは World のアップデートシーケンス中に呼び出してはなりません。 */
	void removeFromParent();

	//void addToWorld();

    const Matrix& worldMatrix();

	ln::Vector3 front() const { return Vector3::transform(Vector3::UnitZ, rotation()); }

	//Level* scene() const { return m_scene; }


    /** この WorldObject に含まれている Component のうち、指定した型である最初の Component を返します。 */
    Component* findComponentByType(const TypeInfo* type) const;

	template<class TComponent>
	TComponent* findComponent() const { return static_cast<TComponent*>(findComponentByType(TypeInfo::getTypeInfo<TComponent>())); }

	template<class TFunc>
	void forEachComponents(TFunc func) const { std::for_each(m_components->begin(), m_components->end(), func); }

    bool traverse(detail::IWorldObjectVisitor* visitor);

	LN_METHOD(Property)
	ComponentList* components() const { return m_components; }

	/** この WorldObject を指定した World へ追加します。省略した場合はデフォルトの World へ追加します。 */
	LN_METHOD()
	void addInto(World* world = nullptr);

protected:
	virtual void onInit();

	/** 物理演算・衝突判定の前 (onCollisionStay() などはこの後) */
	LN_METHOD()
    virtual void onPreUpdate();

	/** フレーム更新 */
	LN_METHOD()
    virtual void onUpdate(float elapsedSeconds);

    virtual void onRender(RenderingContext* context);

	bool traverseRefrection(ReflectionObjectVisitor* visitor) override;


	void serialize_deprecated(Serializer2_deprecated& ar) override;

LN_CONSTRUCT_ACCESS:
	WorldObject();
	virtual ~WorldObject();

	/** WorldObject を作成します。 */
	LN_METHOD()
	bool init();

	virtual void onDispose(bool explicitDisposing) override;

public: // TODO:
    enum class DirtyFlags
    {
        Transform = 0x01,
    };

    void setSpecialObject(bool enalbed) { m_isSpecialObject = true; }
    bool isSpecialObject() const { return m_isSpecialObject; }
    detail::WorldObjectTransform* transform() const { return m_transform; }
	void start();
    void preUpdateFrame();
    void updateFrame(float elapsedSeconds);
    void render(RenderingContext* context) override;
    void notifyTransformChanged();
    void resolveWorldMatrix();
    void updateWorldMatrixHierarchical();


    Level* m_parentLevel;
    WorldObject* m_parent;

	String m_name;
    Ref<detail::WorldObjectTransform> m_transform;
    Ref<List<String>> m_tags;
    //Ref<List<Ref<Component>>> m_components;
	Ref<ComponentList> m_components;
    Ref<List<Ref<WorldObject>>> m_children;
    Flags<DirtyFlags> m_dirtyFlags;
	int m_id = 0;
    Matrix m_worldMatrix;
    bool m_isSpecialObject;
	bool m_destroyed;

    friend class World;
    friend class Level;
};

//==============================================================================
// WorldObject::Builder

struct WorldObject::BuilderDetails : public AbstractBuilderDetails
{
	LN_BUILDER_DETAILS(WorldObject);

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	std::vector<BuilderVariant<Component>> m_components;

	BuilderDetails();
	void apply(WorldObject* p) const;
};

template<class T, class B, class D>
struct WorldObject::BuilderCore : public AbstractBuilder<T, B, D>
{
	LN_BUILDER_CORE(AbstractBuilder);

	///** width property */
	//B& width(float value) { d()->width = value; return self(); }

	///** height property */
	//B& height(float value) { d()->height = value; return self(); }

	///** height property */
	//B& backgroundColor(const Color& value) { d()->backgroundColor = value; return self(); }

	B& position(float x, float y, float z = 0.0f) { d()->position = Vector3(x, y, z); return self(); }
	
	B& rotation(float x, float y, float z) { d()->rotation = Quaternion::makeFromYawPitchRoll(y, x, z); return self(); }

	B& scale(float x, float y, float z = 1.0f) { d()->scale = Vector3(x, y, z); return self(); }
	
	B& scale(float xyz) { d()->scale = Vector3(xyz, xyz, xyz); return self(); }


	template<typename... TArgs>
	B& components(TArgs&&... args) { foreach_args<BuilderVariant<Component>>([this](auto& x) { d()->m_components.push_back(x); }, std::forward<TArgs>(args)...); return *static_cast<B*>(this); }

	Ref<T> buildInto(World* world = nullptr) { auto p = AbstractBuilder<T, B, D>::build(); p->addInto(world); return p; }
};

LN_BUILDER_IMPLEMENT(WorldObject);

namespace ed {

class WorldObjectAsset
	: public AssetModel
{
	LN_OBJECT;
public:

protected:

LN_CONSTRUCT_ACCESS:
	WorldObjectAsset();
	void init();

private:
	Vector3 m_position;
	Vector3 m_angles;
	Vector3 m_scale;
};

} // namespace ed
} // namespace ln
