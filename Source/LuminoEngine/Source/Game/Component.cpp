
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
Transform* Component::getTransform() const
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
void Component::onUpdate(float deltaSceonds)
{
}

//------------------------------------------------------------------------------
void Component::onPreRender(DrawList* context)
{
}

//------------------------------------------------------------------------------
void Component::onRender(DrawList* context)
{
}

//------------------------------------------------------------------------------
void Component::onAttachedWorld(World* world)
{
}

//------------------------------------------------------------------------------
void Component::onDetachedWorld(World* world)
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
void Component::updateFrame(float deltaSceonds)
{
	onUpdate(deltaSceonds);
}

//------------------------------------------------------------------------------
void Component::render(RenderingContext* context)
{
	onRender(context);
}

//==============================================================================
// Behavior
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Behavior, Component);

//------------------------------------------------------------------------------
Behavior::Behavior()
{
}

//------------------------------------------------------------------------------
Behavior::~Behavior()
{
}

//------------------------------------------------------------------------------
void Behavior::initialize()
{
	Component::initialize();
}

//==============================================================================
// Transform
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Transform, Component);

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
	return Vector3::transform(Vector3::UnitZ, rotation);
}

//------------------------------------------------------------------------------
void Transform::translate(const Vector3& translation)
{
	position = position + translation;
}

//------------------------------------------------------------------------------
void Transform::translate(float x, float y, float z)
{
	translate(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
void Transform::lookAt(const Vector3& target, const Vector3& up)
{
	if (target == position) return;

	// left-hand coord
	Vector3 f = Vector3::normalize(target - position);


	Vector3 s = Vector3::cross(up, f);
	if (Vector3::nearEqual(s, Vector3::Zero))
	{
		// TODO: https://jp.mathworks.com/help/matlab/ref/circshift.html?requestedDomain=www.mathworks.com
		// f Ç∆ up Ç™ìØàÍé≤è„Ç…Ç†ÇÈ
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

//------------------------------------------------------------------------------
Matrix Transform::getTransformMatrix() const
{
	return Matrix::makeAffineTransformation(scale, center, rotation, position);
}

//------------------------------------------------------------------------------
void Transform::updateWorldMatrix()
{
	Matrix localMatrix = Matrix::makeAffineTransformation(scale, center, rotation, position);

	if (m_parent != nullptr)
		m_worldMatrix = localMatrix * m_parent->getWorldMatrix();
	else
		m_worldMatrix = localMatrix;
}

LN_NAMESPACE_END
