
#pragma once
#include <Lumino/FlatCCommon.h>

extern "C"
{

/**
    @brief Test summary.
*/
struct LnTestStruct1
{
    float x;
    float y;
};


%%Enums%%

%%Delegates%%

/**
    @brief Constructor.
*/
LN_API LnResult LnTestStruct1_Teststruct1(LnTestStruct1* teststruct1);

/**
    @brief Test method - void(void)
*/
LN_API LnResult LnTestStruct1_Method1(LnTestStruct1* teststruct1);

/**
    @brief Test method. - primitive(void)
*/
LN_API LnResult LnTestStruct1_Method2(LnTestStruct1* teststruct1, float* outReturn);

/**
    @brief Test method. - void(primitive)
*/
LN_API LnResult LnTestStruct1_Method3(LnTestStruct1* teststruct1, float v1);

/**
    @brief Test method. - primitive(primitive)
*/
LN_API LnResult LnTestStruct1_Method4(LnTestStruct1* teststruct1, float v1, float* outReturn);



%%ClassMemberFuncDecls%%

} // extern "C"

