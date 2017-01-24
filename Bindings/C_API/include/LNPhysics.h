
#pragma once
#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_physics 物理演算
	@brief		物理演算
	@{
*/
LN_MODULE(Physics)

//==============================================================================
/**
	@brief	衝突判定のためのオブジェクト形状のベースクラスです。
*/
LN_CLASS(LNCollider, LNObject)
	
LN_CLASS_END

//==============================================================================
/**
	@brief	無限平面の衝突判定形状です。
*/
LN_CLASS(LNPlaneCollider, LNCollider)

	/**
		@brief		PlaneCollider オブジェクトを作成します。
		@param[out]	outPlaneCollider	: 作成された BoxCollider オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNPlaneCollider_Create(LN_OUT LN_HANDLE(LNPlaneCollider)* outPlaneCollider);

	/**
		@brief		PlaneCollider オブジェクトを作成します。
		@param[in]	direction	: 面の正面方向
		@param[out]	outPlaneCollider	: 作成された BoxCollider オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LN_ATTR_OVERLOAD(LNPlaneCollider_Create)
	LNResult LNPlaneCollider_CreateVec3(const LNVector3* direction, LN_OUT LN_HANDLE(LNPlaneCollider)* outPlaneCollider);

LN_CLASS_END

//==============================================================================
/**
	@brief	箱型の衝突判定形状です。
*/
LN_CLASS(LNBoxCollider, LNCollider)

	/**
		@brief		BoxCollider オブジェクトを作成します。
		@param[in]	size	: 各辺の幅
		@param[out]	outBoxCollider	: 作成された BoxCollider オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNBoxCollider_Create(const LNVector3* size, LN_OUT LN_HANDLE(LNBoxCollider)* outBoxCollider);

	/**
		@brief		BoxCollider オブジェクトを作成します。
		@param[in]	x		: 横幅
		@param[in]	y		: 高さ
		@param[in]	z		: 奥行き
		@param[out]	outBoxCollider	: 作成された BoxCollider オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LN_ATTR_OVERLOAD(LNBoxCollider_Create)
	LNResult LNBoxCollider_CreateXYZ(float x, float y, float z, LN_OUT LN_HANDLE(LNBoxCollider)* outBoxCollider);

LN_CLASS_END

//==============================================================================
/**
	@brief	球型の衝突判定形状です。
*/
LN_CLASS(LNSphereCollider, LNCollider)

	/**
		@brief		SphereCollider オブジェクトを作成します。
		@param[in]	radius	: 半径
		@param[out]	outSphereCollider	: 作成された SphereCollider オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNSphereCollider_Create(float radius, LN_OUT LN_HANDLE(LNSphereCollider)* outSphereCollider);

LN_CLASS_END

//==============================================================================
/**
	@brief	カプセル型の衝突判定形状です。
*/
LN_CLASS(LNCapsuleCollider, LNCollider)

	/**
		@brief		CapsuleCollider オブジェクトを作成します。
		@param[in]	radius	: 半径
		@param[in]	height	: 高さ
		@param[out]	outCapsuleCollider	: 作成された CapsuleCollider オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNCapsuleCollider_Create(float radius, float height, LN_OUT LN_HANDLE(LNCapsuleCollider)* outCapsuleCollider);

LN_CLASS_END

//==============================================================================
/**
	@brief	剛体
*/
LN_CLASS(LNRigidBody, LNObject)

	/**
		@brief		剛体オブジェクトを作成します。
		@param[out]	outRigidBody	: 作成された剛体オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNRigidBody_Create(LN_OUT LN_HANDLE(LNRigidBody)* outRigidBody);
	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
