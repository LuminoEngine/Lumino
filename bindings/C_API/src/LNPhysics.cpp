
#include "LNInternal.h"
#include "../include/LNPhysics.h"

//=============================================================================
// LNPlaneCollider
//=============================================================================
LN_TYPE_INFO_IMPL(Physics::PlaneCollider, LNPlaneCollider);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNPlaneCollider_Create(LN_OUT LN_HANDLE(LNPlaneCollider)* outPlaneCollider)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNPlaneCollider_CreateVec3(const LNVector3* direction, LN_OUT LN_HANDLE(LNPlaneCollider)* outPlaneCollider)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}

//=============================================================================
// LNBoxCollider
//=============================================================================
LN_TYPE_INFO_IMPL(Physics::BoxCollider, LNBoxCollider);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNBoxCollider_Create(const LNVector3* size, LN_OUT LN_HANDLE(LNBoxCollider)* outBoxCollider)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNBoxCollider_CreateXYZ(float x, float y, float z, LN_OUT LN_HANDLE(LNBoxCollider)* outBoxCollider)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}

//=============================================================================
// LNSphereCollider
//=============================================================================
LN_TYPE_INFO_IMPL(Physics::SphereCollider, LNSphereCollider);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSphereCollider_Create(float radius, LN_OUT LN_HANDLE(LNSphereCollider)* outSphereCollider)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}

//=============================================================================
// LNCapsuleCollider
//=============================================================================
LN_TYPE_INFO_IMPL(Physics::CapsuleCollider, LNCapsuleCollider);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNCapsuleCollider_Create(float radius, float height, LN_OUT LN_HANDLE(LNCapsuleCollider)* outCapsuleCollider)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}

//=============================================================================
// LNRigidBody
//=============================================================================
LN_TYPE_INFO_IMPL(Physics::RigidBody, LNRigidBody);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNRigidBody_Create(LN_OUT LN_HANDLE(LNRigidBody)* outRigidBody)
{
	LN_FUNC_TRY_BEGIN;
	LN_FUNC_TRY_END_RETURN;
}
