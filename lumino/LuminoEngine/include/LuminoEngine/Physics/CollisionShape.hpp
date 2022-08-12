
#pragma once
#include "Common.hpp"

namespace ln {
class MeshPrimitive;
class CollisionShape;
class PlaneCollisionShape;
class BoxCollisionShape;
class SphereCollisionShape;
class CapsuleCollisionShape;
class MeshCollisionShape;

/**
	@brief	衝突判定のためのオブジェクト形状のベースクラスです。
*/
LN_CLASS()
class CollisionShape
	: public Object
{
	LN_OBJECT;

public:

	/** この CollisionShape が衝突判定のためのトリガーであるかを設定します。初期値は false です。*/
	void setTrigger(bool enabled);

	/** この CollisionShape が衝突判定のためのトリガーであるかを取得します。*/
	bool isTrigger() const;

	/** この形状のローカルオフセットを設定します。 */
	void setPosition(const Vector3& value) { m_position = value; }

	/** この形状のローカルオフセットを取得します。 */
	const Vector3& position() const { return m_position; }

    void setRotation(const Quaternion& value) { m_rotation = value; }

    const Quaternion& rotation() const { return m_rotation; }

LN_CONSTRUCT_ACCESS:
	CollisionShape();
	virtual ~CollisionShape();
	Result init();
	bool init(btCollisionShape* shape);

LN_INTERNAL_ACCESS:
	btCollisionShape* getBtCollisionShape() const { return m_shape; }
		
private:
	btCollisionShape* m_shape;
	Vector3 m_position;
    Quaternion m_rotation;
	bool m_isTrigger;
};

/**
	@brief	無限平面の衝突判定形状です。
*/
class PlaneCollisionShape
	: public CollisionShape
{
	LN_OBJECT;
public:
	
	/**
		@brief		PlaneCollisionShape オブジェクトを作成します。
		@param[in]	direction	: 面の正面方向 (省略した場合は Y+ 方向)
	*/
	static Ref<PlaneCollisionShape> create(const Vector3& direction = Vector3::UnitY);
	
LN_CONSTRUCT_ACCESS:
	PlaneCollisionShape();
	virtual ~PlaneCollisionShape();
	bool init();
	bool init(const Vector3& direction);
};

/**
	@brief	箱型の衝突判定形状です。
*/
LN_CLASS()
class BoxCollisionShape
	: public CollisionShape
{
	LN_OBJECT;
public:
	
	/**
		@brief		BoxCollisionShape オブジェクトを作成します。
		@param[in]	size	: 各辺の幅
	*/
	static Ref<BoxCollisionShape> create(const Vector3& size);
	
	/**
		@brief		BoxCollisionShape オブジェクトを作成します。
		@param[in]	x, y, z	: 各辺の幅
	*/
	static Ref<BoxCollisionShape> create(float x, float y, float z);

LN_CONSTRUCT_ACCESS:
	BoxCollisionShape();
	virtual ~BoxCollisionShape();

	Result init();

	/** init */
	LN_METHOD()
	bool init(const Vector3& size);
	
	/** init */
	LN_METHOD(OverloadPostfix = "WHD")
	bool init(float width, float height, float depth);
};

/**
	@brief	球型の衝突判定形状です。
*/
class SphereCollisionShape
	: public CollisionShape
{
	LN_OBJECT;
public:
	
	/**
		@brief		CapsuleCollisionShape オブジェクトを作成します。
		@param[in]	radius	: 半径
	*/
	static Ref<SphereCollisionShape> create(float radius);

LN_CONSTRUCT_ACCESS:
	SphereCollisionShape();
	virtual ~SphereCollisionShape();
	Result init();
	bool init(float radius);
};

/**
	@brief	カプセル型の衝突判定形状です。
*/
class CapsuleCollisionShape
	: public CollisionShape
{
	LN_OBJECT;
public:
	
	/**
		@brief		CapsuleCollisionShape オブジェクトを作成します。
		@param[in]	radius	: 半径
		@param[in]	height	: 高さ
	*/
	static Ref<CapsuleCollisionShape> create(float radius, float height);

LN_CONSTRUCT_ACCESS:
	CapsuleCollisionShape();
	virtual ~CapsuleCollisionShape();
	Result init();
	bool init(float radius, float height);
};


/**
	@brief	
*/
class MeshCollisionShape
	: public CollisionShape
{
	LN_OBJECT;
public:
	
	/**
		@brief		MeshCollisionShape オブジェクトを作成します。
	*/
	static Ref<MeshCollisionShape> create(MeshPrimitive* mesh);
	static Ref<MeshCollisionShape> create(MeshPrimitive* mesh, const Matrix& transform);

LN_CONSTRUCT_ACCESS:
	MeshCollisionShape();
	virtual ~MeshCollisionShape();
	Result init();
	bool init(MeshPrimitive* mesh);
	bool init(MeshPrimitive* mesh, const Matrix& transform);

private:
	bool initInternal(MeshPrimitive* mesh, const Matrix* transform);

	btTriangleIndexVertexArray* m_btMeshData;
};


namespace detail {

class BtShapeManager
{
public:
    BtShapeManager();
    ~BtShapeManager();
    void addShape(CollisionShape* shape, const Matrix& localTransform = Matrix::Identity);
    bool isEmpty() const;
    btCollisionShape* getBtCollisionShape();

private:
    //struct ShapeEntry
    //{
    //    Ref<CollisionShape> shape;
    //    Matrix transform;
    //};

    void refresh();

   // List<ShapeEntry> m_collisionShapes;
    List<Ref<CollisionShape>> m_collisionShapes;
    btCompoundShape* m_btCompoundShape;
    btCollisionShape* m_activeShape;
    bool m_dirty;
};

} // namespace detail
} // namespace ln
