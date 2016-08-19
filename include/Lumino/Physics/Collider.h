
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class PlaneCollider;
class BoxCollider;
class SphereCollider;
class CapsuleCollider;
using PlaneColliderPtr = RefPtr<PlaneCollider>;
using BoxColliderPtr = RefPtr<BoxCollider>;
using SphereColliderPtr = RefPtr<SphereCollider>;
using CapsuleColliderPtr = RefPtr<CapsuleCollider>;

/**
	@brief	衝突判定のためのオブジェクト形状のベースクラスです。
*/
class Collider
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
protected:
	Collider();
	virtual ~Collider();
	void Initialize(btCollisionShape* shape);

LN_INTERNAL_ACCESS:
	btCollisionShape* GetBtCollisionShape() { return m_shape; }
		
private:
	btCollisionShape*	m_shape;
};

/**
	@brief	無限平面の衝突判定形状です。
*/
class PlaneCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		PlaneCollider オブジェクトを作成します。
		@param[in]	direction	: 面の正面方向 (省略した場合は Y+ 方向)
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static PlaneColliderPtr Create(const Vector3& direction = Vector3::UnitY);
	
LN_INTERNAL_ACCESS:
	PlaneCollider();
	virtual ~PlaneCollider();
	void Initialize(const Vector3& direction);
};

/**
	@brief	箱型の衝突判定形状です。
*/
class BoxCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		BoxCollider オブジェクトを作成します。
		@param[in]	size	: 各辺の幅
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static BoxColliderPtr Create(const Vector3& size);
	
	/**
		@brief		BoxCollider オブジェクトを作成します。
		@param[in]	x, y, z	: 各辺の幅
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static BoxColliderPtr Create(float x, float y, float z);

LN_INTERNAL_ACCESS:
	BoxCollider();
	virtual ~BoxCollider();
	void Initialize(const Vector3& size);
};

/**
	@brief	球型の衝突判定形状です。
*/
class SphereCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		CapsuleCollider オブジェクトを作成します。
		@param[in]	radius	: 半径
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static SphereColliderPtr Create(float radius);

LN_INTERNAL_ACCESS:
	SphereCollider();
	virtual ~SphereCollider();
	void Initialize(float radius);
};

/**
	@brief	カプセル型の衝突判定形状です。
*/
class CapsuleCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		CapsuleCollider オブジェクトを作成します。
		@param[in]	radius	: 半径
		@param[in]	height	: 高さ
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static CapsuleColliderPtr Create(float radius, float height);

LN_INTERNAL_ACCESS:
	CapsuleCollider();
	virtual ~CapsuleCollider();
	void Initialize(float radius, float height);
};

LN_NAMESPACE_END
