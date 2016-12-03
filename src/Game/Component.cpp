
#include "../Internal.h"
#include <Lumino/Game/Component.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Component
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Component, Object);

//------------------------------------------------------------------------------
Component::Component()
	: m_owner(nullptr)
{
}

//------------------------------------------------------------------------------
Component::~Component()
{
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

LN_NAMESPACE_END
