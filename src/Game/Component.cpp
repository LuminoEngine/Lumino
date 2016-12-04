
#include "../Internal.h"
#include <Lumino/Game/Component.h>
#include <Lumino/Foundation/GameScene.h>

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
GameObject* Component::GetOwner() const
{
	return m_owner;
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
void Component::Attach(GameObject* owner)
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

//==============================================================================
// Transform
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Transform, Component);
LN_TR_PROPERTY_IMPLEMENT(Transform, Vector3, position, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(Transform, Quaternion, rotation, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(Transform, Vector3, scale, tr::PropertyMetadata());

//------------------------------------------------------------------------------
Transform::Transform()
{
}

//------------------------------------------------------------------------------
Transform::~Transform()
{
}

//------------------------------------------------------------------------------
Matrix Transform::GetTransformMatrix() const
{
	return Matrix::MakeAffineTransformation(scale, center, rotation, position);
}

LN_NAMESPACE_END
