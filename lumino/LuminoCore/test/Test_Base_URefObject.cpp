#include "Common.hpp"
#include <LuminoCore/Base/URefObject.hpp>

class Test_Base_URefObject : public ::testing::Test {};

class UTestObjA : public URefObject {
public:
    UTestObjA() { s_counter++; }
    UTestObjA(int v) : a(v) { s_counter++; }
    ~UTestObjA() { s_counter--; }
    static int s_counter;
    int a = 100;
};

int UTestObjA::s_counter = 0;

class UTestObjB : public UTestObjA {
public:
};

class TestObjC;

static void TestFunc1(const URef<UTestObjA>& ptr) {
    URef<TestObjC> p1;  // Forward declaration only.
}
static void TestFunc2(UTestObjA* ptr) {
}

class TestObjC : public URefObject {
public:
};

TEST_F(Test_Base_URefObject, Construct) {
    URef<UTestObjA> ref1;
    URef<UTestObjA> ref2(nullptr);
    ASSERT_FALSE(ref1);
    ASSERT_FALSE(ref2);

    {
        URef<UTestObjA> ref3(new UTestObjA());
        ASSERT_TRUE(ref3);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    // move
    {
        URef<UTestObjA> ref4(new UTestObjA());
        URef<UTestObjA> ref5(std::move(ref4));
        ASSERT_FALSE(ref4);
        ASSERT_TRUE(ref5);
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    // up-cast and move
    {
        URef<UTestObjB> ref4(new UTestObjB());
        URef<UTestObjA> ref5(std::move(ref4));
        ASSERT_FALSE(ref4);
        ASSERT_TRUE(ref5);
        ASSERT_EQ(1, UTestObjA::s_counter);

        TestFunc1(std::move(ref4)); // can implicit cast
        TestFunc1(std::move(ref5)); // can implicit cast
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    {
        // Can create instances of URefObject on the stack. Copying is not allowed.
        TestObjC obj1;
    }
}

TEST_F(Test_Base_URefObject, Assign) {
    // nullptr
    {
        URef<UTestObjA> ref1(new UTestObjA());
        ref1 = nullptr;
        ASSERT_FALSE(ref1);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    // empty
    {
        URef<UTestObjA> ref1(new UTestObjA());
        ref1 = URef<UTestObjA>();
        ASSERT_FALSE(ref1);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    // up-cast
    {
        URef<UTestObjB> ref1(new UTestObjB());
        URef<UTestObjA> ref2(new UTestObjA());
        ref2 = std::move(ref1);
        ASSERT_FALSE(ref1);
        ASSERT_TRUE(ref2);
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    // self assignment
    {
        URef<UTestObjA> ref1(new UTestObjA());
        ref1 = std::move(ref1);
        ASSERT_TRUE(ref1);
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);
}

TEST_F(Test_Base_URefObject, ImplicitGetPointer) {
    {
        URef<UTestObjA> ref1(new UTestObjA());
        UTestObjA* ptr = ref1;
        TestFunc2(ref1);
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);
}


TEST_F(Test_Base_URefObject, reset) {
    // empty -> empty
    {
        URef<UTestObjA> ref1;
        ref1.reset();
        ASSERT_EQ(0, UTestObjA::s_counter);
    }

    // empty -> any
    {
        URef<UTestObjA> ref1;
        ref1.reset(new UTestObjA());
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    // any -> empty
    {
        URef<UTestObjA> ref1(new UTestObjA());
        ref1.reset();
        ASSERT_EQ(0, UTestObjA::s_counter);
    }

    // any -> any
    {
        URef<UTestObjA> ref1(new UTestObjA());
        ref1.reset(new UTestObjA());
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);

    
    // self assignment
    {
        URef<UTestObjA> ref1(new UTestObjA());
        ref1.reset(ref1.get());
        ASSERT_TRUE(ref1);
        ASSERT_EQ(1, UTestObjA::s_counter);
    }
    ASSERT_EQ(0, UTestObjA::s_counter);
}

TEST_F(Test_Base_URefObject, Operators) {
    URef<UTestObjA> ref1(new UTestObjA());
    URef<UTestObjA> ref2;

    ASSERT_EQ(100, (*ref1).a);
    ASSERT_EQ(100, ref1->a);
    ASSERT_FALSE(ref1 == ref2);
    ASSERT_TRUE(ref1 != ref2);
    ASSERT_FALSE(!ref1);
    ASSERT_TRUE(!ref2);

    bool r1 = ref1;
    ASSERT_EQ(true, r1);

    ref1 = nullptr;
    bool r2 = ref1;
    ASSERT_EQ(false, r2);

    ASSERT_TRUE(ref1 == ref2);
    ASSERT_FALSE(ref1 != ref2);

    ASSERT_FALSE(ref1 < nullptr);
}

TEST_F(Test_Base_URefObject, makeURef) {
    auto ref1 = makeURef<UTestObjA>();
    auto ref2 = makeURef<UTestObjA>(200);
    ASSERT_EQ(100, ref1->a);
    ASSERT_EQ(200, ref2->a);
}
