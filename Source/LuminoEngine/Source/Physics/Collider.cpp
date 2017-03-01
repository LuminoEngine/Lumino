
#include "Internal.h"
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <Lumino/Physics/Collider.h>
#include "BulletUtils.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Collider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Collider, Object);

//------------------------------------------------------------------------------
Collider::Collider()
	: m_shape(nullptr)
	, m_offset()
	, m_isTrigger(false)
{
}

//------------------------------------------------------------------------------
Collider::~Collider()
{
	LN_SAFE_DELETE(m_shape);
}

//------------------------------------------------------------------------------
void Collider::Initialize(btCollisionShape* shape)
{
	m_shape = shape;
}

//------------------------------------------------------------------------------
void Collider::SetTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool Collider::IsTrigger() const
{
	return m_isTrigger;
}

//==============================================================================
// PlaneCollider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PlaneCollider, Collider);
	
//------------------------------------------------------------------------------
PlaneColliderPtr PlaneCollider::Create(const Vector3& direction)
{
	auto ptr = RefPtr<PlaneCollider>::MakeRef();
	ptr->Initialize(direction);
	return ptr;
}

//------------------------------------------------------------------------------
PlaneCollider::PlaneCollider()
{
}

//------------------------------------------------------------------------------
PlaneCollider::~PlaneCollider()
{
}

//------------------------------------------------------------------------------
void PlaneCollider::Initialize(const Vector3& direction)
{
	Collider::Initialize(new btStaticPlaneShape(detail::BulletUtil::LNVector3ToBtVector3(Vector3::Normalize(direction)), 0.0f));
}

//==============================================================================
// BoxCollider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BoxCollider, Collider);
	
//------------------------------------------------------------------------------
BoxColliderPtr BoxCollider::Create(const Vector3& size)
{
	auto ptr = RefPtr<BoxCollider>::MakeRef();
	ptr->Initialize(size);
	return ptr;
}

//------------------------------------------------------------------------------
BoxColliderPtr BoxCollider::Create(float x, float y, float z)
{
	return Create(Vector3(x, y, z));
}
	
//------------------------------------------------------------------------------
BoxCollider::BoxCollider()
{
}

//------------------------------------------------------------------------------
BoxCollider::~BoxCollider()
{
}

//------------------------------------------------------------------------------
void BoxCollider::Initialize(const Vector3& size)
{
	Collider::Initialize(new btBoxShape(detail::BulletUtil::LNVector3ToBtVector3(size * 0.5f)));
	// ※PMD の剛体サイズは bullet のと同じなので注意
}

//==============================================================================
// SphereCollider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SphereCollider, Collider);
	
//------------------------------------------------------------------------------
SphereColliderPtr SphereCollider::Create(float radius)
{
	auto ptr = RefPtr<SphereCollider>::MakeRef();
	ptr->Initialize(radius);
	return ptr;
}
	
//------------------------------------------------------------------------------
SphereCollider::SphereCollider()
{
}

//------------------------------------------------------------------------------
SphereCollider::~SphereCollider()
{
}

//------------------------------------------------------------------------------
void SphereCollider::Initialize(float radius)
{
	Collider::Initialize(new btSphereShape(radius));
}

//==============================================================================
// CapsuleCollider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CapsuleCollider, Collider);

//------------------------------------------------------------------------------
CapsuleColliderPtr CapsuleCollider::Create(float radius, float height)
{
	auto ptr = RefPtr<CapsuleCollider>::MakeRef();
	ptr->Initialize(radius, height);
	return ptr;
}

//------------------------------------------------------------------------------
CapsuleCollider::CapsuleCollider()
{
}

//------------------------------------------------------------------------------
CapsuleCollider::~CapsuleCollider()
{
}

//------------------------------------------------------------------------------
void CapsuleCollider::Initialize(float radius, float height)
{
	Collider::Initialize(new btCapsuleShape(radius, height));
}

LN_NAMESPACE_END
