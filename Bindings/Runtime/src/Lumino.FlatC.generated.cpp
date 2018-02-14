#include "Internal.h"
#include "WrapperIF.generated.h"

extern "C" {

LN_API LNResultCode LNVector3_GetLength(const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Vector3*>(vector3)->getLength());
    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_GetLengthSquared(const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Vector3*>(vector3)->getLengthSquared());
    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_Normalize(LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize());
    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_Normalize(LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize());
    LWIG_FUNC_TRY_END_RETURN;
}



} // extern "C"
