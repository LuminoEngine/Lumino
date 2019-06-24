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
     * Tet method1.
     */
    LN_METHOD()
    float method1() const;
};

} // namespace ln
