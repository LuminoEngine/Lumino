
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace Physics
{

/**
	@brief	衝突判定のためのオブジェクト形状のベースクラスです。
*/
class Collider
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
protected:
	Collider();
	virtual ~Collider();
	void Initialize(PhysicsManager* manager, btCollisionShape* shape);
		
private:
	PhysicsManager*		m_manager;
	btCollisionShape*	m_shape;
};

/**
	@brief	静的な無限平面の衝突判定形状です。
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
	static PlaneCollider* Create(const Vector3& direction = Vector3::UnitY);
	
protected:
	PlaneCollider();
	virtual ~PlaneCollider();
	void Initialize(PhysicsManager* manager, const Vector3& direction);
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
	static BoxCollider* Create(const Vector3& size);
	
	/**
		@brief		BoxCollider オブジェクトを作成します。
		@param[in]	x, y, z	: 各辺の幅
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static BoxCollider* Create(float x, float y, float z);
	
protected:
	BoxCollider();
	virtual ~BoxCollider();
	void Initialize(PhysicsManager* manager, const Vector3& size);
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
	static SphereCollider* Create(float radius);
	
protected:
	SphereCollider();
	virtual ~SphereCollider();
	void Initialize(PhysicsManager* manager, float radius);
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
	static CapsuleCollider* Create(float radius, float height);
	
protected:
	CapsuleCollider();
	virtual ~CapsuleCollider();
	void Initialize(PhysicsManager* manager, float radius, float height);
};


} // namespace Physics
LN_NAMESPACE_END
