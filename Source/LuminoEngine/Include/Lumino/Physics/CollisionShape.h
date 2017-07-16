
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class MeshResource;
class CollisionShape;
class PlaneCollisionShape;
class BoxCollisionShape;
class SphereCollisionShape;
class CapsuleCollisionShape;
class MeshCollisionShape;
using CollisionShapePtr = Ref<CollisionShape>;
using PlaneCollisionShapePtr = Ref<PlaneCollisionShape>;
using BoxCollisionShapePtr = Ref<BoxCollisionShape>;
using SphereCollisionShapePtr = Ref<SphereCollisionShape>;
using CapsuleCollisionShapePtr = Ref<CapsuleCollisionShape>;
using MeshCollisionShapePtr = Ref<MeshCollisionShape>;

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

	void setCenter(const Vector3& offset) { m_offset = offset; }

	const Vector3& getCenter() const { return m_offset; }

LN_CONSTRUCT_ACCESS:
	CollisionShape();
	virtual ~CollisionShape();
	void initialize(btCollisionShape* shape);

LN_INTERNAL_ACCESS:
	btCollisionShape* getBtCollisionShape() const { return m_shape; }
		
private:
	btCollisionShape*	m_shape;
	Vector3				m_offset;
	bool				m_isTrigger;
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
	static PlaneCollisionShapePtr create(const Vector3& direction = Vector3::UnitY);
	
LN_INTERNAL_ACCESS:
	PlaneCollisionShape();
	virtual ~PlaneCollisionShape();
	void initialize(const Vector3& direction);
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
	static BoxCollisionShapePtr create(const Vector3& size);
	
	/**
		@brief		BoxCollisionShape オブジェクトを作成します。
		@param[in]	x, y, z	: 各辺の幅
	*/
	static BoxCollisionShapePtr create(float x, float y, float z);

LN_CONSTRUCT_ACCESS:
	BoxCollisionShape();
	virtual ~BoxCollisionShape();

	LN_METHOD()
	void initialize(const Vector3& size);
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
	static SphereCollisionShapePtr create(float radius);

LN_CONSTRUCT_ACCESS:
	SphereCollisionShape();
	virtual ~SphereCollisionShape();
	void initialize(float radius);
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
	static CapsuleCollisionShapePtr create(float radius, float height);

LN_CONSTRUCT_ACCESS:
	CapsuleCollisionShape();
	virtual ~CapsuleCollisionShape();
	void initialize(float radius, float height);
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
	static MeshCollisionShapePtr create(MeshResource* mesh);

LN_CONSTRUCT_ACCESS:
	MeshCollisionShape();
	virtual ~MeshCollisionShape();
	void initialize(MeshResource* mesh);

private:
	btTriangleIndexVertexArray* m_btMeshData;
};

LN_NAMESPACE_END
