#pragma once

namespace ln {

/** 
 * Test summary.
 */
LN_STRUCT()
struct TestStruct1
{
public:
    /** Test field1. */
    LN_FIELD()
    float x;

    /** Test field2. */
    LN_FIELD()
    float y;

    /**
     * Constructor.
     */
    LN_METHOD()
    TestStruct1();
    
    /**
     * Test method - void(void)
     */
    LN_METHOD()
    void method1();

    /**
     * Test method. - primitive(void)
     */
    LN_METHOD()
    float method2();

    /**
     * Test method. - void(primitive)
     */
    LN_METHOD()
    void method3(float v1);

    /**
     * Test method. - primitive(primitive)
     */
    LN_METHOD()
    float method4(float v1);
	
};

} // namespace ln
