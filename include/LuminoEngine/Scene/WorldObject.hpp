
#pragma once
#include "Common.hpp"

namespace ln {
class World;
class Component;
namespace detail {

class WorldObjectTransform
    : public RefObject
{
public:
    Vector3		position;
    Quaternion	rotation;
    Vector3		scale;
    Vector3		center;

    WorldObjectTransform();

    Vector3 getFront() const
    {
        return Vector3::transform(Vector3::UnitZ, rotation);
    }

    void lookAt(const Vector3& target, const Vector3& up = Vector3::UnitY);

    Matrix getTransformMatrix() const;


    void updateWorldMatrix(const Matrix* parent);
    const Matrix& worldMatrix() const { return m_worldMatrix; }

private:
    Matrix		m_worldMatrix;
};

}

class WorldObject
	: public Object
{
    LN_OBJECT;
public:
	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(Property)
	void setPosition(const Vector3& pos) { m_transform->position = pos; }

	/** このオブジェクトの位置を設定します。 */
	LN_METHOD(OverloadPostfix = "XYZ")
	void setPosition(float x, float y, float z = 0.0f) { setPosition(Vector3(x, y, z)); }

	/** このオブジェクトの位置を位置を取得します。 */
	LN_METHOD(Property)
	const Vector3& position() const { return m_transform->position; }

	/** このオブジェクトの回転を設定します。 */
	LN_METHOD(Property)
	void setRotation(const Quaternion& rot) { m_transform->rotation = rot; }

	/** このオブジェクトの回転をオイラー角から設定します。(radian) */
	LN_METHOD()
	void setEulerAngles(float x, float y, float z) { m_transform->rotation = Quaternion::makeFromEulerAngles(Vector3(x, y, z)); }

	/** このオブジェクトの回転を取得します。 */
	LN_METHOD(Property)
	const Quaternion& rotation() const { return m_transform->rotation; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(Property)
	void setScale(const Vector3& scale) { m_transform->scale = scale; }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(OverloadPostfix = "S")
	void setScale(float xyz) { m_transform->scale = Vector3(xyz, xyz, xyz); }

	/** このオブジェクトの拡大率を設定します。 */
	LN_METHOD(OverloadPostfix = "XYZ")
	void setScale(float x, float y, float z = 1.0f) { m_transform->scale = Vector3(x, y, z); }

	/** このオブジェクトの拡大率を取得します。 */
	LN_METHOD(Property)
	const Vector3& scale() const { return m_transform->scale; }
    

    void lookAt(const Vector3& target, const Vector3& up = Vector3::UnitY);

    void addComponent(Component* component);


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
    detail::WorldObjectTransform* transform() const { return m_transform; }
    void preUpdateFrame();
    void updateFrame(float elapsedSeconds);
    void render();

    World* m_world;
    Ref<detail::WorldObjectTransform> m_transform;
    Ref<List<Ref<Component>>> m_components;

    friend class World;
};

} // namespace ln
