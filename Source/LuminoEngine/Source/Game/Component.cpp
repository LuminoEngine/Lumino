
#include "../Internal.h"
#include <Lumino/Rendering/RenderingContext.h>
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
	, m_layer(0)
	, m_orderInLayer(0)
{
}

//------------------------------------------------------------------------------
Component::~Component()
{
}

//------------------------------------------------------------------------------
WorldObject* Component::getOwnerObject() const
{
	return m_owner;
}

//------------------------------------------------------------------------------
transform* Component::getTransform() const
{
	return (m_owner != nullptr) ? &m_owner->transform : nullptr;
}

//------------------------------------------------------------------------------
void Component::onAttached()
{
}

//------------------------------------------------------------------------------
void Component::onDetaching()
{
}

//------------------------------------------------------------------------------
void Component::onUpdate()
{
}

//------------------------------------------------------------------------------
void Component::onRender(DrawList* context)
{
}

//------------------------------------------------------------------------------
void Component::onUIEvent(UIEventArgs* e)
{
}

//------------------------------------------------------------------------------
void Component::attach(WorldObject* owner)
{
	m_owner = owner;
	transfotm = &m_owner->transform;
	onAttached();
}

//------------------------------------------------------------------------------
void Component::detach()
{
	onDetaching();
	m_owner = nullptr;
	transfotm = nullptr;
}

//------------------------------------------------------------------------------
void Component::updateFrame()
{
	onUpdate();
}

//------------------------------------------------------------------------------
void Component::render(RenderingContext* context)
{
	onRender(context);
}

//==============================================================================
// transform
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(transform, Component);

//------------------------------------------------------------------------------
transform::transform()
	: position(Vector3::Zero)
	, rotation(Quaternion::Identity)
	, scale(Vector3::Ones)
	, m_parent(nullptr)
	, m_worldMatrix()
{
}

//------------------------------------------------------------------------------
transform::~transform()
{
}

//------------------------------------------------------------------------------
Vector3 transform::getFront() const
{
	return Vector3::transform(Vector3::UnitZ, rotation);
}

//------------------------------------------------------------------------------
void transform::translate(const Vector3& translation)
{
	position = position + translation;
}

//------------------------------------------------------------------------------
void transform::translate(float x, float y, float z)
{
	translate(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
void transform::lookAt(const Vector3& target, const Vector3& up)
{
	if (target == position) return;

	// left-hand coord
	Vector3 f = Vector3::normalize(target - position);
	Vector3 s = Vector3::normalize(Vector3::cross(up, f));
	Vector3 u = Vector3::cross(f, s);
	Matrix mat(
		s.x, s.y, s.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		f.x, f.y, f.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	rotation = Quaternion::makeFromRotationMatrix(mat);
}

//------------------------------------------------------------------------------
Matrix transform::getTransformMatrix() const
{
	return Matrix::makeAffineTransformation(scale, center, rotation, position);
}

//------------------------------------------------------------------------------
void transform::updateWorldMatrix()
{
	Matrix localMatrix = Matrix::makeAffineTransformation(scale, center, rotation, position);

	if (m_parent != nullptr)
		m_worldMatrix = localMatrix * m_parent->getWorldMatrix();
	else
		m_worldMatrix = localMatrix;
}

LN_NAMESPACE_END
