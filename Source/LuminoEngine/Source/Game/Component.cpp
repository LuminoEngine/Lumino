
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
	return &m_owner->transform;
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
void Component::Attach(WorldObject* owner)
{
	m_owner = owner;
	transfotm = &m_owner->transform;
	OnAttached();
}

//------------------------------------------------------------------------------
void Component::Detach()
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
{
}

//------------------------------------------------------------------------------
Transform::~Transform()
{
}

//------------------------------------------------------------------------------
void Transform::Translate(const Vector3& translation)
{
	position = position.Get() + translation;
}

//------------------------------------------------------------------------------
void Transform::Translate(float x, float y, float z)
{
	Translate(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
Matrix Transform::GetTransformMatrix() const
{
	return Matrix::MakeAffineTransformation(scale, center, rotation, position);
}

LN_NAMESPACE_END
