#include "Common.hpp"
#include <LuminoEngine/Engine/Property.hpp>

class Test_Engine_Object : public ::testing::Test {};

class TestObjectA
    : public Object
{
    LN_OBJECT2;
public:
    LN_PROPERTY_DECLARE(int, Prop1);

    void initialize();

    void setProp1(int value) { m_prop1 = value; }
    int prop1() const { return m_prop1; }

    int m_count = 0;

private:
    static void onProp1Changed(Object* obj);

    Property<int> m_prop1;
};

LN_OBJECT2_IMPLEMENT(TestObjectA);
LN_PROPERTY_IMPLEMENT(TestObjectA, Prop1, m_prop1, PropertyMetadata(TestObjectA::onProp1Changed));

void TestObjectA::initialize()
{
    Object::initialize();
}

void TestObjectA::onProp1Changed(Object* obj)
{
    static_cast<TestObjectA*>(obj)->m_count++;
}


class TestObjectB
    : public Object
{
    LN_OBJECT2;
public:
};

LN_OBJECT2_IMPLEMENT(TestObjectB);

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, TypeInfo)
{
    auto obj1 = newObject<TestObjectA>();
    auto obj2 = newObject<TestObjectB>();

    //* [ ] can get by object pointer
    TypeInfo* type1 = TypeInfo::getTypeInfo(obj1);
    ASSERT_EQ(true, type1 != nullptr);
    ASSERT_EQ(u"TestObjectA", type1->name());

    //* [ ] can get by template
    TypeInfo* type2 = TypeInfo::getTypeInfo<TestObjectB>();
    ASSERT_EQ(true, type2 != nullptr);
    ASSERT_EQ(u"TestObjectB", type2->name());
}

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, Property)
{
    auto objA = newObject<TestObjectA>();
    objA->setProp1(5);
    ASSERT_EQ(5, objA->prop1());

    //* [ ] PropertyChanged コールバックが呼ばれている
    ASSERT_EQ(1, objA->m_count);

    //* [ ] PropertyInfo が取れる
    ASSERT_EQ(true, TestObjectA::Prop1Id != nullptr);
}
