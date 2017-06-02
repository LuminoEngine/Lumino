
#include "Internal.h"
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>
#include <Lumino/Physics/CollisionShape.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Mesh/Mesh.h>
#include "BulletUtils.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// CollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CollisionShape, Object);

//------------------------------------------------------------------------------
CollisionShape::CollisionShape()
	: m_shape(nullptr)
	, m_offset()
	, m_isTrigger(false)
{
}

//------------------------------------------------------------------------------
CollisionShape::~CollisionShape()
{
	LN_SAFE_DELETE(m_shape);
}

//------------------------------------------------------------------------------
void CollisionShape::Initialize(btCollisionShape* shape)
{
	m_shape = shape;
}

//------------------------------------------------------------------------------
void CollisionShape::SetTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool CollisionShape::IsTrigger() const
{
	return m_isTrigger;
}

//==============================================================================
// PlaneCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PlaneCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
PlaneCollisionShapePtr PlaneCollisionShape::Create(const Vector3& direction)
{
	auto ptr = RefPtr<PlaneCollisionShape>::MakeRef();
	ptr->Initialize(direction);
	return ptr;
}

//------------------------------------------------------------------------------
PlaneCollisionShape::PlaneCollisionShape()
{
}

//------------------------------------------------------------------------------
PlaneCollisionShape::~PlaneCollisionShape()
{
}

//------------------------------------------------------------------------------
void PlaneCollisionShape::Initialize(const Vector3& direction)
{
	CollisionShape::Initialize(new btStaticPlaneShape(detail::BulletUtil::LNVector3ToBtVector3(Vector3::Normalize(direction)), 0.0f));
}

//==============================================================================
// BoxCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BoxCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
BoxCollisionShapePtr BoxCollisionShape::Create(const Vector3& size)
{
	auto ptr = RefPtr<BoxCollisionShape>::MakeRef();
	ptr->Initialize(size);
	return ptr;
}

//------------------------------------------------------------------------------
BoxCollisionShapePtr BoxCollisionShape::Create(float x, float y, float z)
{
	return Create(Vector3(x, y, z));
}
	
//------------------------------------------------------------------------------
BoxCollisionShape::BoxCollisionShape()
{
}

//------------------------------------------------------------------------------
BoxCollisionShape::~BoxCollisionShape()
{
}

//------------------------------------------------------------------------------
void BoxCollisionShape::Initialize(const Vector3& size)
{
	CollisionShape::Initialize(new btBoxShape(detail::BulletUtil::LNVector3ToBtVector3(size * 0.5f)));
	// Å¶PMD ÇÃçÑëÃÉTÉCÉYÇÕ bullet ÇÃÇ∆ìØÇ∂Ç»ÇÃÇ≈íçà”
}

//==============================================================================
// SphereCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SphereCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
SphereCollisionShapePtr SphereCollisionShape::Create(float radius)
{
	auto ptr = RefPtr<SphereCollisionShape>::MakeRef();
	ptr->Initialize(radius);
	return ptr;
}
	
//------------------------------------------------------------------------------
SphereCollisionShape::SphereCollisionShape()
{
}

//------------------------------------------------------------------------------
SphereCollisionShape::~SphereCollisionShape()
{
}

//------------------------------------------------------------------------------
void SphereCollisionShape::Initialize(float radius)
{
	CollisionShape::Initialize(new btSphereShape(radius));
}

//==============================================================================
// CapsuleCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CapsuleCollisionShape, CollisionShape);

//------------------------------------------------------------------------------
CapsuleCollisionShapePtr CapsuleCollisionShape::Create(float radius, float height)
{
	auto ptr = RefPtr<CapsuleCollisionShape>::MakeRef();
	ptr->Initialize(radius, height);
	return ptr;
}

//------------------------------------------------------------------------------
CapsuleCollisionShape::CapsuleCollisionShape()
{
}

//------------------------------------------------------------------------------
CapsuleCollisionShape::~CapsuleCollisionShape()
{
}

//------------------------------------------------------------------------------
void CapsuleCollisionShape::Initialize(float radius, float height)
{
	CollisionShape::Initialize(new btCapsuleShape(radius, height));
}

//==============================================================================
// MeshCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MeshCollisionShape, CollisionShape);

//------------------------------------------------------------------------------
MeshCollisionShapePtr MeshCollisionShape::Create(MeshResource* mesh)
{
	auto ptr = RefPtr<MeshCollisionShape>::MakeRef();
	ptr->Initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
MeshCollisionShape::MeshCollisionShape()
	: m_btMeshData(nullptr)
{
}

//------------------------------------------------------------------------------
MeshCollisionShape::~MeshCollisionShape()
{
	LN_SAFE_DELETE(m_btMeshData);
}

//------------------------------------------------------------------------------
void MeshCollisionShape::Initialize(MeshResource* mesh)
{
	if (LN_CHECK_ARG(mesh != nullptr)) return;
	if (LN_CHECK_STATE(m_btMeshData == nullptr)) return;

	IndexBuffer* indexBuffer = mesh->GetIndexBuffer();

	void* vb = mesh->GetVertexBuffer(MeshResource::VB_BasicVertices)->GetMappedData();
	void* ib = indexBuffer->GetMappedData();

	btIndexedMesh btMesh;
	btMesh.m_numTriangles = mesh->GetTriangleCount();
	btMesh.m_triangleIndexBase = (const unsigned char *)ib;
	btMesh.m_triangleIndexStride = indexBuffer->GetIndexStride() * 3;
	btMesh.m_numVertices = mesh->GetVertexCount();
	btMesh.m_vertexBase = (const unsigned char *)vb;
	btMesh.m_vertexStride = sizeof(Vertex);

	m_btMeshData = new btTriangleIndexVertexArray();
	m_btMeshData->addIndexedMesh(btMesh, (indexBuffer->GetIndexStride() == 2) ? PHY_SHORT : PHY_INTEGER);

	//m_btMeshData = new btTriangleIndexVertexArray(
	//	mesh->GetTriangleCount(), (int*)ib, mesh->GetIndexStride(),
	//	mesh->GetVertexCount(), (btScalar*)vb, sizeof(Vertex));
	
	CollisionShape::Initialize(new btBvhTriangleMeshShape(m_btMeshData, true));
}

LN_NAMESPACE_END
