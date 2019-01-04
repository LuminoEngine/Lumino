
#include "Internal.hpp"
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <LuminoEngine/Physics/CollisionShape.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include "BulletUtils.hpp"

namespace ln {

//==============================================================================
// CollisionShape
//   なぜローカルの姿勢を RigidBody ではなく CollitionShape に持っているのか？
//   - 複合形状を作るときにオフセットを指定するのが Body 側だと、GUI 作りづらいから。
//   参考：
//   https://github.com/lriki/Lumino/issues/32
//==============================================================================
LN_OBJECT_IMPLEMENT(CollisionShape, Object);

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
void CollisionShape::setTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool CollisionShape::isTrigger() const
{
	return m_isTrigger;
}

//==============================================================================
// PlaneCollisionShape
//==============================================================================
LN_OBJECT_IMPLEMENT(PlaneCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
Ref<PlaneCollisionShape> PlaneCollisionShape::create(const Vector3& direction)
{
    return newObject<PlaneCollisionShape>(direction);
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
LN_OBJECT_IMPLEMENT(BoxCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
Ref<BoxCollisionShape> BoxCollisionShape::create(const Vector3& size)
{
    return newObject<BoxCollisionShape>(size);
}

//------------------------------------------------------------------------------
Ref<BoxCollisionShape> BoxCollisionShape::create(float x, float y, float z)
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
	// ※PMD の剛体サイズは bullet のと同じなので注意
}

//==============================================================================
// SphereCollisionShape
//==============================================================================
LN_OBJECT_IMPLEMENT(SphereCollisionShape, CollisionShape);
	
//------------------------------------------------------------------------------
Ref<SphereCollisionShape> SphereCollisionShape::create(float radius)
{
    return newObject<SphereCollisionShape>(radius);
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
LN_OBJECT_IMPLEMENT(CapsuleCollisionShape, CollisionShape);

//------------------------------------------------------------------------------
Ref<CapsuleCollisionShape> CapsuleCollisionShape::create(float radius, float height)
{
    return newObject<CapsuleCollisionShape>(radius, height);
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
LN_OBJECT_IMPLEMENT(MeshCollisionShape, CollisionShape);

//------------------------------------------------------------------------------
Ref<MeshCollisionShape> MeshCollisionShape::create(MeshResource* mesh)
{
    return newObject<MeshCollisionShape>(mesh);
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
	if (LN_REQUIRE(mesh != nullptr)) return;
	if (LN_REQUIRE(m_btMeshData == nullptr)) return;

#if 1
    LN_NOTIMPLEMENTED();
#else
	IndexBuffer* indexBuffer = mesh->getIndexBuffer();

	void* vb = mesh->getVertexBuffer(MeshResource::VB_BasicVertices)->getMappedData();
	void* ib = indexBuffer->getMappedData();

	btIndexedMesh btMesh;
	btMesh.m_numTriangles = mesh->getTriangleCount();
	btMesh.m_triangleIndexBase = (const unsigned char *)ib;
	btMesh.m_triangleIndexStride = indexBuffer->getIndexStride() * 3;
	btMesh.m_numVertices = mesh->getVertexCount();
	btMesh.m_vertexBase = (const unsigned char *)vb;
	btMesh.m_vertexStride = sizeof(Vertex);

	m_btMeshData = new btTriangleIndexVertexArray();
	m_btMeshData->addIndexedMesh(btMesh, (indexBuffer->getIndexStride() == 2) ? PHY_SHORT : PHY_INTEGER);

	//m_btMeshData = new btTriangleIndexVertexArray(
	//	mesh->getTriangleCount(), (int*)ib, mesh->getIndexStride(),
	//	mesh->getVertexCount(), (btScalar*)vb, sizeof(Vertex));
	
	CollisionShape::initialize(new btBvhTriangleMeshShape(m_btMeshData, true));
#endif
}

//==============================================================================
// BtShapeManager
//		各種 Body にアタッチする btCollisionShape を効率よく管理するためのクラス。
//		Bullet は、Shape が姿勢のオフセットを持つことはできない。
//		持たせたければ btCompoundShape を経なければならない。
//		常に btCompoundShape を作ってしまえばいいのだがそれだとメモリや演算の効率が悪い。
//		そこで、姿勢のオフセットを持たない単一の Shape しか使わなければそれを、
//		そうでなければ　btCompoundShape を作る。
namespace detail {

BtShapeManager::BtShapeManager()
	: m_collisionShapes()
	, m_btCompoundShape(nullptr)
	, m_activeShape(nullptr)
	, m_dirty(false)
{
}

BtShapeManager::~BtShapeManager()
{
	LN_SAFE_DELETE(m_btCompoundShape);
}

void BtShapeManager::addShape(CollisionShape* shape, const Matrix& localTransform)
{
	if (LN_REQUIRE(shape != nullptr)) return;

    m_collisionShapes.add(shape);

	//if (m_collisionShape != nullptr)
	//{
	//	// TODO: 今は1つのみ許可
	//	LN_NOTIMPLEMENTED();
	//}
	//else
	//{
	//	m_collisionShape = shape;
	//}

	m_dirty = true;
}

bool BtShapeManager::isEmpty() const
{
    return m_collisionShapes.isEmpty();
}

btCollisionShape* BtShapeManager::getBtCollisionShape()
{
	if (m_dirty)
	{
		refresh();
		m_dirty = false;
	}
	return m_activeShape;
}

void BtShapeManager::refresh()
{
	btCollisionShape* shape;
    if (m_collisionShapes.size() == 1 &&
        m_collisionShapes.front()->getCenter() == Vector3::Zero &&
        m_collisionShapes.front()->rotation() == Quaternion::Identity) {
        // Shape が1つだけ & ローカル姿勢も定義していない場合は btCompoundShape を作る必要はない。メモリ削減。
        shape = m_collisionShapes.front()->getBtCollisionShape();
    }
    else
	{
		if (m_btCompoundShape == nullptr)
		{
			m_btCompoundShape = new btCompoundShape();
		}
		else
		{
			for (int i = m_btCompoundShape->getNumChildShapes() - 1; i >= 0; i--)
			{
				m_btCompoundShape->removeChildShapeByIndex(i);
			}
		}

        for (auto& shape : m_collisionShapes)
        {
            btTransform t;
            t.setBasis(btMatrix3x3::getIdentity());
            t.setRotation(BulletUtil::LNQuaternionToBtQuaternion(shape->rotation()));
            t.setOrigin(BulletUtil::LNVector3ToBtVector3(shape->getCenter()));
            m_btCompoundShape->addChildShape(t, shape->getBtCollisionShape());
        }

        m_btCompoundShape->recalculateLocalAabb();

		shape = m_btCompoundShape;
	}

	m_activeShape = shape;
}

} // namespace detail
} // namespace ln
