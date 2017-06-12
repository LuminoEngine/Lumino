
#include "../Internal.h"
#include <Lumino/Game/Component.h>
#include <Lumino/Framework/GameScene.h>
#include <Lumino/Scene/WorldObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Component
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Component, Object);

//------------------------------------------------------------------------------
Component::Component()
	: transfotm(nullptr)
	, m_owner(nullptr)
	, m_specialComponentType(SpecialComponentType::None)
{
}

//------------------------------------------------------------------------------
Component::~Component()
{
}

//------------------------------------------------------------------------------
WorldObject* Component::GetOwnerObject() const
{
	return m_owner;
}

//------------------------------------------------------------------------------
Transform* Component::GetTransform() const
{
	return (m_owner != nullptr) ? &m_owner->transform : nullptr;
}

//------------------------------------------------------------------------------
void Component::OnAttached()
{
}

//------------------------------------------------------------------------------
void Component::OnDetaching()
{
}

//------------------------------------------------------------------------------
void Component::OnUpdate()
{
}

//------------------------------------------------------------------------------
void Component::OnRender(DrawList* context)
{
}

//------------------------------------------------------------------------------
void Component::OnUIEvent(UIEventArgs* e)
{
}

//------------------------------------------------------------------------------
void Component::attach(WorldObject* owner)
{
	m_owner = owner;
	transfotm = &m_owner->transform;
	OnAttached();
}

//------------------------------------------------------------------------------
void Component::detach()
{
	OnDetaching();
	m_owner = nullptr;
	transfotm = nullptr;
}

//------------------------------------------------------------------------------
void Component::UpdateFrame()
{
	OnUpdate();
}

//------------------------------------------------------------------------------
void Component::Render(DrawList* context)
{
	OnRender(context);
}

//==============================================================================
// Transform
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Transform, Component);
LN_TR_PROPERTY_IMPLEMENT(Transform, Vector3, position, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(Transform, Quaternion, rotation, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(Transform, Vector3, scale, tr::PropertyMetadata());

//------------------------------------------------------------------------------
Transform::Transform()
	: position(Vector3::Zero)
	, rotation(Quaternion::Identity)
	, scale(Vector3::Ones)
	, m_parent(nullptr)
	, m_worldMatrix()
{
}

//------------------------------------------------------------------------------
Transform::~Transform()
{
}

//------------------------------------------------------------------------------
Vector3 Transform::getFront() const
{
	return Vector3::Transform(Vector3::UnitZ, rotation.get());
}

//------------------------------------------------------------------------------
void Transform::translate(const Vector3& translation)
{
	position = position.get() + translation;
}

//------------------------------------------------------------------------------
void Transform::translate(float x, float y, float z)
{
	translate(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
void Transform::LookAt(const Vector3& target, const Vector3& up)
{
	if (target == position.get()) return;

	// left-hand coord
	Vector3 f = Vector3::Normalize(target - position.get());
	Vector3 s = Vector3::Normalize(Vector3::Cross(up, f));
	Vector3 u = Vector3::Cross(f, s);
	Matrix mat(
		s.x, s.y, s.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		f.x, f.y, f.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	rotation = Quaternion::MakeFromRotationMatrix(mat);
}

//------------------------------------------------------------------------------
Matrix Transform::GetTransformMatrix() const
{
	return Matrix::MakeAffineTransformation(scale, center, rotation, position);
}

//------------------------------------------------------------------------------
void Transform::UpdateWorldMatrix()
{
	Matrix localMatrix = Matrix::MakeAffineTransformation(scale, center, rotation, position);

	if (m_parent != nullptr)
		m_worldMatrix = localMatrix * m_parent->GetWorldMatrix();
	else
		m_worldMatrix = localMatrix;
}

LN_NAMESPACE_END
