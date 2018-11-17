
#include "Internal.hpp"
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// WorldObjectTransform

namespace detail {

WorldObjectTransform::WorldObjectTransform()
    : position(Vector3::Zero)
    , rotation(Quaternion::Identity)
    , scale(Vector3::Ones)
    , m_worldMatrix()
{}

void WorldObjectTransform::lookAt(const Vector3& target, const Vector3& up)
{
    if (target == position) return;

    // left-hand coord
    Vector3 f = Vector3::normalize(target - position);


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
    rotation = Quaternion::makeFromRotationMatrix(mat);
}

Matrix WorldObjectTransform::getTransformMatrix() const
{
    return Matrix::makeAffineTransformation(scale, center, rotation, position);
}

void WorldObjectTransform::updateWorldMatrix(const Matrix* parent)
{
    Matrix localMatrix = Matrix::makeAffineTransformation(scale, center, rotation, position);

    if (parent != nullptr)
        m_worldMatrix = localMatrix * (*parent);
    else
        m_worldMatrix = localMatrix;
}

} // namespace detail 

//==============================================================================
// WorldObject
//  Transform は Unity のように Component 扱いしない。
//  Component にしてしまうと、m_components リストを操作するときに混ざりこんでしまって危険かもしれない。
//  特に必要性が出てくるまではこのスタイルにしておく。

LN_OBJECT_IMPLEMENT(WorldObject, Object);

WorldObject::WorldObject()
    : m_world(nullptr)
    , m_transform(makeRef<detail::WorldObjectTransform>())
    , m_components(makeRef<List<Ref<Component>>>())
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
    component->m_object = this;
    m_components->add(component);
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

} // namespace ln

