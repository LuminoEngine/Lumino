#include "Internal.h"
#include "Lumino.Wrapper.generated.h"

extern "C"
{

LN_API LnResult LnTestStruct1_Teststruct1(LnTestStruct1* teststruct1)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::TestStruct1*>(teststruct1)) TestStruct1();
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnTestStruct1_Method1(LnTestStruct1* teststruct1)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::TestStruct1*>(teststruct1)->method1());
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnTestStruct1_Method2(LnTestStruct1* teststruct1, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::TestStruct1*>(teststruct1)->method2());
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnTestStruct1_Method3(LnTestStruct1* teststruct1, float v1)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::TestStruct1*>(teststruct1)->method3(v1));
    LNI_FUNC_TRY_END_RETURN;
}

LN_API LnResult LnTestStruct1_Method4(LnTestStruct1* teststruct1, float v1, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::TestStruct1*>(teststruct1)->method4(v1));
    LNI_FUNC_TRY_END_RETURN;
}



%%ClassMemberFuncImpls%%
	
} // extern "C"
