
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
public:
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
