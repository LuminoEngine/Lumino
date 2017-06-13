
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
void CollisionShape::initialize(btCollisionShape* shape)
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
PlaneCollisionShapePtr PlaneCollisionShape::create(const Vector3& direction)
{
	auto ptr = RefPtr<PlaneCollisionShape>::makeRef();
	ptr->initialize(direction);
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
void PlaneCollisionShape::initialize(const Vector3& direction)
{
	CollisionShape::initialize(new btStaticPlaneShape(detail::BulletUtil::LNVector3ToBtVector3(Vector3::normalize(direction)), 0.0f));
}

//==============================================================================
// BoxCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BoxCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
BoxCollisionShapePtr BoxCollisionShape::create(const Vector3& size)
{
	auto ptr = RefPtr<BoxCollisionShape>::makeRef();
	ptr->initialize(size);
	return ptr;
}

//------------------------------------------------------------------------------
BoxCollisionShapePtr BoxCollisionShape::create(float x, float y, float z)
{
	return create(Vector3(x, y, z));
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
void BoxCollisionShape::initialize(const Vector3& size)
{
	CollisionShape::initialize(new btBoxShape(detail::BulletUtil::LNVector3ToBtVector3(size * 0.5f)));
	// Å¶PMD ÇÃçÑëÃÉTÉCÉYÇÕ bullet ÇÃÇ∆ìØÇ∂Ç»ÇÃÇ≈íçà”
}

//==============================================================================
// SphereCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SphereCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
SphereCollisionShapePtr SphereCollisionShape::create(float radius)
{
	auto ptr = RefPtr<SphereCollisionShape>::makeRef();
	ptr->initialize(radius);
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
void SphereCollisionShape::initialize(float radius)
{
	CollisionShape::initialize(new btSphereShape(radius));
}

//==============================================================================
// CapsuleCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CapsuleCollisionShape, CollisionShape);

//------------------------------------------------------------------------------
CapsuleCollisionShapePtr CapsuleCollisionShape::create(float radius, float height)
{
	auto ptr = RefPtr<CapsuleCollisionShape>::makeRef();
	ptr->initialize(radius, height);
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
void CapsuleCollisionShape::initialize(float radius, float height)
{
	CollisionShape::initialize(new btCapsuleShape(radius, height));
}

//==============================================================================
// MeshCollisionShape
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MeshCollisionShape, CollisionShape);

//------------------------------------------------------------------------------
MeshCollisionShapePtr MeshCollisionShape::create(MeshResource* mesh)
{
	auto ptr = RefPtr<MeshCollisionShape>::makeRef();
	ptr->initialize(mesh);
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
void MeshCollisionShape::initialize(MeshResource* mesh)
{
	if (LN_CHECK_ARG(mesh != nullptr)) return;
	if (LN_CHECK_STATE(m_btMeshData == nullptr)) return;

	IndexBuffer* indexBuffer = mesh->GetIndexBuffer();

	void* vb = mesh->getVertexBuffer(MeshResource::VB_BasicVertices)->getMappedData();
	void* ib = indexBuffer->getMappedData();

	btIndexedMesh btMesh;
	btMesh.m_numTriangles = mesh->GetTriangleCount();
	btMesh.m_triangleIndexBase = (const unsigned char *)ib;
	btMesh.m_triangleIndexStride = indexBuffer->getIndexStride() * 3;
	btMesh.m_numVertices = mesh->getVertexCount();
	btMesh.m_vertexBase = (const unsigned char *)vb;
	btMesh.m_vertexStride = sizeof(Vertex);

	m_btMeshData = new btTriangleIndexVertexArray();
	m_btMeshData->addIndexedMesh(btMesh, (indexBuffer->getIndexStride() == 2) ? PHY_SHORT : PHY_INTEGER);

	//m_btMeshData = new btTriangleIndexVertexArray(
	//	mesh->GetTriangleCount(), (int*)ib, mesh->getIndexStride(),
	//	mesh->getVertexCount(), (btScalar*)vb, sizeof(Vertex));
	
	CollisionShape::initialize(new btBvhTriangleMeshShape(m_btMeshData, true));
}

LN_NAMESPACE_END
