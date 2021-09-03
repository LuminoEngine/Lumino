
#include "Internal.hpp"
#pragma warning(disable: 5033)	// disable warning in bullet headers
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
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include "BulletUtils.hpp"

namespace ln {

//==============================================================================
// CollisionShape
//   なぜローカルの姿勢を RigidBody ではなく CollitionShape に持っているのか？
//   - 複合形状を作るときにオフセットを指定するのが Body 側だと、GUI 作りづらいから。
//   参考：
//   https://github.com/lriki/Lumino/issues/32
//==============================================================================
LN_OBJECT_IMPLEMENT(CollisionShape, Object) {}

//------------------------------------------------------------------------------
CollisionShape::CollisionShape()
	: m_shape(nullptr)
	, m_position()
	, m_isTrigger(false)
{
}

//------------------------------------------------------------------------------
CollisionShape::~CollisionShape()
{
	LN_SAFE_DELETE(m_shape);
}

bool CollisionShape::init()
{
	return Object::init();
}

//------------------------------------------------------------------------------
bool CollisionShape::init(btCollisionShape* shape)
{
	if (!init()) return false;
	m_shape = shape;
	return true;
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
LN_OBJECT_IMPLEMENT(PlaneCollisionShape, CollisionShape) {}
	
//------------------------------------------------------------------------------
Ref<PlaneCollisionShape> PlaneCollisionShape::create(const Vector3& direction)
{
    return makeObject<PlaneCollisionShape>(direction);
}

//------------------------------------------------------------------------------
PlaneCollisionShape::PlaneCollisionShape()
{
}

//------------------------------------------------------------------------------
PlaneCollisionShape::~PlaneCollisionShape()
{
}

bool PlaneCollisionShape::init()
{
	return init(Vector3::UnitY);
}

//------------------------------------------------------------------------------
bool PlaneCollisionShape::init(const Vector3& direction)
{
	return CollisionShape::init(new btStaticPlaneShape(detail::BulletUtil::LNVector3ToBtVector3(Vector3::normalize(direction)), 0.0f));
}

//==============================================================================
// BoxCollisionShape
//==============================================================================
LN_OBJECT_IMPLEMENT(BoxCollisionShape, CollisionShape) {}
	
//------------------------------------------------------------------------------
Ref<BoxCollisionShape> BoxCollisionShape::create(const Vector3& size)
{
    return makeObject<BoxCollisionShape>(size);
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

bool BoxCollisionShape::init()
{
	return CollisionShape::init();
}

//------------------------------------------------------------------------------
bool BoxCollisionShape::init(const Vector3& size)
{
	return CollisionShape::init(new btBoxShape(detail::BulletUtil::LNVector3ToBtVector3(size * 0.5f)));
	// ※PMD の剛体サイズは bullet のと同じなので注意
}

bool BoxCollisionShape::init(float width, float height, float depth)
{
	return init(Vector3(width, height, depth));
}

//==============================================================================
// SphereCollisionShape
//==============================================================================
LN_OBJECT_IMPLEMENT(SphereCollisionShape, CollisionShape) {}
	
//------------------------------------------------------------------------------
Ref<SphereCollisionShape> SphereCollisionShape::create(float radius)
{
    return makeObject<SphereCollisionShape>(radius);
}
	
//------------------------------------------------------------------------------
SphereCollisionShape::SphereCollisionShape()
{
}

//------------------------------------------------------------------------------
SphereCollisionShape::~SphereCollisionShape()
{
}

bool SphereCollisionShape::init()
{
	return CollisionShape::init();
}

//------------------------------------------------------------------------------
bool SphereCollisionShape::init(float radius)
{
	return CollisionShape::init(new btSphereShape(radius));
}

//==============================================================================
// CapsuleCollisionShape
//==============================================================================
LN_OBJECT_IMPLEMENT(CapsuleCollisionShape, CollisionShape) {}

//------------------------------------------------------------------------------
Ref<CapsuleCollisionShape> CapsuleCollisionShape::create(float radius, float height)
{
    return makeObject<CapsuleCollisionShape>(radius, height);
}

//------------------------------------------------------------------------------
CapsuleCollisionShape::CapsuleCollisionShape()
{
}

//------------------------------------------------------------------------------
CapsuleCollisionShape::~CapsuleCollisionShape()
{
}

bool CapsuleCollisionShape::init()
{
	return CollisionShape::init();
}

//------------------------------------------------------------------------------
bool CapsuleCollisionShape::init(float radius, float height)
{
	// Bullet の Capsule の height は 円筒部分のみを示すので、Unity とかの動作と合わせて、全体が height になるようにする
	return CollisionShape::init(new btCapsuleShape(radius, height - (radius * 2.0f)));
}

//==============================================================================
// MeshCollisionShape
//==============================================================================
LN_OBJECT_IMPLEMENT(MeshCollisionShape, CollisionShape) {}

//------------------------------------------------------------------------------
Ref<MeshCollisionShape> MeshCollisionShape::create(MeshPrimitive* mesh)
{
    return makeObject<MeshCollisionShape>(mesh);
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

bool MeshCollisionShape::init()
{
	return CollisionShape::init();
}

//------------------------------------------------------------------------------
bool MeshCollisionShape::init(MeshPrimitive* mesh)
{
	if (!init()) return false;
	return initInternal(mesh, nullptr);
}

bool MeshCollisionShape::init(MeshPrimitive* mesh, const Matrix& transform)
{
	if (!init()) return false;

	// TODO: VertexBuffer と IndexBuffer のコピーが必要。
	LN_NOTIMPLEMENTED();

	return initInternal(mesh, &transform);
}

bool MeshCollisionShape::initInternal(MeshPrimitive* mesh, const Matrix* transform)
{
	if (LN_REQUIRE(mesh)) return false;
	if (LN_REQUIRE(!m_btMeshData)) return false;
	if (LN_REQUIRE(mesh->sections().size() > 0)) return false;

	const MeshSection2& section = mesh->sections()[0];
	if (LN_REQUIRE(section.topology == PrimitiveTopology::TriangleList)) return false;



	VertexBuffer* vertexBuffer = mesh->vertexBuffer(InterleavedVertexGroup::Main);
	IndexBuffer* indexBuffer = mesh->indexBuffer();

	const void* vb = vertexBuffer->data();
	void* ib = indexBuffer->map(MapMode::Read);


	btIndexedMesh btMesh;
	btMesh.m_numTriangles = section.primitiveCount;
	btMesh.m_triangleIndexBase = (const unsigned char*)ib;
	btMesh.m_triangleIndexStride = indexBuffer->stride() * 3;
	btMesh.m_numVertices = mesh->vertexCount();
	btMesh.m_vertexBase = (const unsigned char*)vb;
	btMesh.m_vertexStride = sizeof(Vertex);

	PHY_ScalarType indexFormat;
	switch (indexBuffer->format())
	{
	case IndexBufferFormat::UInt16:
		indexFormat = PHY_SHORT;
		break;
	case IndexBufferFormat::UInt32:
		indexFormat = PHY_INTEGER;
		break;
	default:
		LN_UNREACHABLE();
		return false;
	}

	m_btMeshData = new btTriangleIndexVertexArray();
	m_btMeshData->addIndexedMesh(btMesh, indexFormat);

	//m_btMeshData = new btTriangleIndexVertexArray(
	//	mesh->getTriangleCount(), (int*)ib, mesh->getIndexStride(),
	//	mesh->getVertexCount(), (btScalar*)vb, sizeof(Vertex));

	CollisionShape::init(new btBvhTriangleMeshShape(m_btMeshData, true));

	// TODO: scoped
	indexBuffer->unmap();
	//vertexBuffer->unmap();

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
        m_collisionShapes.front()->position() == Vector3::Zero &&
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
            t.setOrigin(BulletUtil::LNVector3ToBtVector3(shape->position()));
            m_btCompoundShape->addChildShape(t, shape->getBtCollisionShape());
        }

        m_btCompoundShape->recalculateLocalAabb();

		shape = m_btCompoundShape;
	}

	m_activeShape = shape;
}

} // namespace detail
} // namespace ln
