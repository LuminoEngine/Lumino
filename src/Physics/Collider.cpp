
#include "Internal.h"
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <Lumino/Physics/Collider.h>
#include "BulletUtils.h"

LN_NAMESPACE_BEGIN
namespace Physics
{

//=============================================================================
// Collider
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Collider, tr::ReflectionObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Collider::Collider()
	: m_manager(nullptr)
	, m_shape(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Collider::~Collider()
{
	LN_SAFE_DELETE(m_shape);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Collider::Initialize(PhysicsManager* manager, btCollisionShape* shape)
{
	m_manager = manager;
	m_shape = shape;
}

//=============================================================================
// PlaneCollider
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PlaneCollider, Collider);
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PlaneCollider* PlaneCollider::Create(const Vector3& direction)
{
	RefPtr<PlaneCollider> obj(LN_NEW PlaneCollider(), false);
	obj->Initialize(GetPhysicsManager(nullptr), direction);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PlaneCollider::PlaneCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PlaneCollider::~PlaneCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PlaneCollider::Initialize(PhysicsManager* manager, const Vector3& direction)
{
	Collider::Initialize(manager, new btStaticPlaneShape(BulletUtil::LNVector3ToBtVector3(Vector3::Normalize(direction)), 0.0f));
}

//=============================================================================
// BoxCollider
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BoxCollider, Collider);
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BoxCollider* BoxCollider::Create(const Vector3& size)
{
	RefPtr<BoxCollider> obj(LN_NEW BoxCollider(), false);
	obj->Initialize(GetPhysicsManager(nullptr), size);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BoxCollider* BoxCollider::Create(float x, float y, float z)
{
	return Create(Vector3(x, y, z));
}
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BoxCollider::BoxCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BoxCollider::~BoxCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BoxCollider::Initialize(PhysicsManager* manager, const Vector3& size)
{
	Collider::Initialize(manager, new btBoxShape(BulletUtil::LNVector3ToBtVector3(size * 0.5f)));
	// ※PMD の剛体サイズは bullet のと同じなので注意
}

//=============================================================================
// SphereCollider
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SphereCollider, Collider);
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SphereCollider* SphereCollider::Create(float radius)
{
	RefPtr<SphereCollider> obj(LN_NEW SphereCollider(), false);
	obj->Initialize(GetPhysicsManager(nullptr), radius);
	obj.SafeAddRef();
	return obj;
}
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SphereCollider::SphereCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SphereCollider::~SphereCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SphereCollider::Initialize(PhysicsManager* manager, float radius)
{
	Collider::Initialize(manager, new btSphereShape(radius));
}

//=============================================================================
// CapsuleCollider
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CapsuleCollider, Collider);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CapsuleCollider* CapsuleCollider::Create(float radius, float height)
{
	RefPtr<CapsuleCollider> obj(LN_NEW CapsuleCollider(), false);
	obj->Initialize(GetPhysicsManager(nullptr), radius, height);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CapsuleCollider::CapsuleCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CapsuleCollider::~CapsuleCollider()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CapsuleCollider::Initialize(PhysicsManager* manager, float radius, float height)
{
	Collider::Initialize(manager, new btCapsuleShape(radius, height));
}
	
} // namespace Physics
LN_NAMESPACE_END
