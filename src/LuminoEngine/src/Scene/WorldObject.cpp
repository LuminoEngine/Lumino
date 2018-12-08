
#include "Internal.hpp"
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// WorldObjectTransform

namespace detail {

WorldObjectTransform::WorldObjectTransform(WorldObject* parent)
    : m_parent(parent)
    , m_position(Vector3::Zero)
    , m_rotation(Quaternion::Identity)
    , m_scale(Vector3::Ones)
    //, m_worldMatrix()
{}

void WorldObjectTransform::setPosition(const Vector3& pos)
{
    if (m_position != pos)
    {
        m_position = pos;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::setRotation(const Quaternion& rot)
{
    if (m_rotation != rot)
    {
        m_rotation = rot;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::setScale(const Vector3& scale)
{
    if (m_scale != scale)
    {
        m_scale = scale;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::setCenterPoint(const Vector3& value)
{
    if (m_center != value)
    {
        m_center = value;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::lookAt(const Vector3& target, const Vector3& up)
{
    if (target == m_position) return;

    // left-hand coord
    Vector3 f = Vector3::normalize(target - m_position);


    Vector3 s = Vector3::cross(up, f);
    if (Vector3::nearEqual(s, Vector3::Zero))
    {
        // TODO: https://jp.mathworks.com/help/matlab/ref/circshift.html?requestedDomain=www.mathworks.com
        Vector3 u2 = Vector3::UnitZ;
        //std::rotate<float*>(&u2.x, (&u2.x)+2, &u2.z);
        s = Vector3::cross(u2, f);
    }

    s.normalize();

    Vector3 u = Vector3::cross(f, s);
    Matrix mat(
        s.x, s.y, s.z, 0.0f,
        u.x, u.y, u.z, 0.0f,
        f.x, f.y, f.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    m_rotation = Quaternion::makeFromRotationMatrix(mat);
}

Matrix WorldObjectTransform::getLocalMatrix() const
{
    return Matrix::makeAffineTransformation(m_scale, Vector3::Zero, m_rotation, m_position - m_center);
    //return Matrix::makeAffineTransformation(m_scale, m_center, m_rotation, m_position);
}

//void WorldObjectTransform::updateWorldMatrix(const Matrix* parent)
//{
//    Matrix localMatrix = Matrix::makeAffineTransformation(scale, center, rotation, position);
//
//    if (parent != nullptr)
//        m_worldMatrix = localMatrix * (*parent);
//    else
//        m_worldMatrix = localMatrix;
//}

} // namespace detail 

//==============================================================================
// WorldObject
//  Transform は Unity のように Component 扱いしない。
//  Component にしてしまうと、m_components リストを操作するときに混ざりこんでしまって危険かもしれない。
//  特に必要性が出てくるまではこのスタイルにしておく。

LN_OBJECT_IMPLEMENT(WorldObject, Object);

WorldObject::WorldObject()
    : m_world(nullptr)
    , m_parent(nullptr)
    , m_transform(makeRef<detail::WorldObjectTransform>(this))
    , m_components(makeRef<List<Ref<Component>>>())
    , m_children(makeRef<List<Ref<WorldObject>>>())
    , m_isSpecialObject(false)
{
}

WorldObject::~WorldObject()
{
}

void WorldObject::initialize()
{
    Object::initialize();

	World* activeWorld = detail::EngineDomain::sceneManager()->activeWorld();
	if (activeWorld) {
		activeWorld->addObject(this);
	}
}

void WorldObject::lookAt(const Vector3& target, const Vector3& up)
{
    m_transform->lookAt(target, up);
}

void WorldObject::addComponent(Component* component)
{
    if (LN_REQUIRE(component)) return;
    if (LN_REQUIRE(!component->m_object)) return;

    component->m_object = this;
    m_components->add(component);
}

const Matrix& WorldObject::worldMatrix()
{
    resolveWorldMatrix();
    return m_worldMatrix;
}

void WorldObject::onPreUpdate()
{
}

void WorldObject::onUpdate(float elapsedSeconds)
{

}

void WorldObject::onRender()
{

}

void WorldObject::updateFrame(float elapsedSeconds)
{
    onUpdate(elapsedSeconds);
}

void WorldObject::render()
{
    onRender();
}

void WorldObject::notifyTransformChanged()
{
    if (!m_dirtyFlags.hasFlag(DirtyFlags::Transform))
    {
        m_dirtyFlags.set(DirtyFlags::Transform);

        for (auto& obj : m_children) {
            obj->notifyTransformChanged();
        }
    }
}

void WorldObject::resolveWorldMatrix()
{
    if (m_dirtyFlags.hasFlag(DirtyFlags::Transform)) {

        if (m_parent) {
            m_worldMatrix = m_transform->getLocalMatrix() * m_parent->worldMatrix();
        }
        else
            m_worldMatrix = m_transform->getLocalMatrix();

        m_dirtyFlags.unset(DirtyFlags::Transform);
    }
}

} // namespace ln

