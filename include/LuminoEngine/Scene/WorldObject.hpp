
#pragma once
#include "Common.hpp"

namespace ln {
class World;
class WorldObject;
class Component;
namespace detail {

class WorldObjectTransform
    : public RefObject
{
public:

    WorldObjectTransform(WorldObject* parent);

    /** 位置を設定します。 */
    LN_METHOD(Property)
    void setPosition(const Vector3& pos);
    
	/** 位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& position() const { return m_position; }
    
	/** このオブジェクトの回転を設定します。 */
	LN_METHOD(Property)
	void setRotation(const Quaternion& rot);
    
	/** このオブジェクトの回転を取得します。 */
	LN_METHOD(Property)
	const Quaternion& rotation() const { return m_rotation; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(Property)
	void setScale(const Vector3& scale);
    
	/** このオブジェクトの拡大率を取得します。 */
	LN_METHOD(Property)
	const Vector3& scale() const { return m_scale; }
    
	/** このオブジェクトのローカルの中心位置を設定します。 */
	LN_METHOD(Property)
	void setCenterPoint(const Vector3& value);
    
	/** このオブジェクトのローカルの中心位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& centerPoint() const { return m_center; }

    // TODO:Forward?
    Vector3 getFront() const
    {
        return Vector3::transform(Vector3::UnitZ, m_rotation);
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
};

}

class WorldObject
	: public Object
{
    LN_OBJECT;
public:
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
	LN_METHOD(Property)
	void setRotation(const Quaternion& rot) { m_transform->setRotation(rot); }

	/** このオブジェクトの回転をオイラー角から設定します。(radian) */
	LN_METHOD()
	void setEulerAngles(float x, float y, float z) { setRotation(Quaternion::makeFromEulerAngles(Vector3(x, y, z))); }

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

    void lookAt(const Vector3& target, const Vector3& up = Vector3::UnitY);

    void addComponent(Component* component);

    const Matrix& worldMatrix();

protected:
    // 物理演算・衝突判定の前
    virtual void onPreUpdate();

    // フレーム更新
    virtual void onUpdate(float elapsedSeconds);

    virtual void onRender();

LN_CONSTRUCT_ACCESS:
	WorldObject();
	virtual ~WorldObject();
	void initialize();

public: // TODO:
    enum class DirtyFlags
    {
        Transform = 0x01,
    };

    void setSpecialObject(bool enalbed) { m_isSpecialObject = true; }
    bool isSpecialObject() const { return m_isSpecialObject; }
    detail::WorldObjectTransform* transform() const { return m_transform; }
    void preUpdateFrame();
    void updateFrame(float elapsedSeconds);
    void render();
    void notifyTransformChanged();
    void resolveWorldMatrix();

    World* m_world;
    WorldObject* m_parent;
    Ref<detail::WorldObjectTransform> m_transform;
    Ref<List<Ref<Component>>> m_components;
    Ref<List<Ref<WorldObject>>> m_children;
    Flags<DirtyFlags> m_dirtyFlags;
    Matrix m_worldMatrix;
    bool m_isSpecialObject;

    friend class World;
};

} // namespace ln
