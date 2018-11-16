#include "Common.hpp"
#include <LuminoEngine/Engine/Property.hpp>

class Test_Engine_Object : public ::testing::Test {};

class PropertyTestObjectA
    : public Object
{
    LN_OBJECT2;
public:
    LN_PROPERTY_DECLARE(int, Prop1);
    //static PropertyInitializer iniii;
    //static const ::ln::Ref<::ln::PropertyInfo> ppp;

    void initialize();

    void setProp1(int value) { m_prop1 = value; }
    int prop1() const { return m_prop1; }

    int m_count = 0;

private:
    static void onProp1Changed(Object* obj);

    Property<int> m_prop1;
};

LN_OBJECT2_IMPLEMENT(PropertyTestObjectA);
//LN_TR_PROPERTY_IMPLEMENT(PropertyTestObjectA, Prop1, m_prop1, PropertyMetadata([](Object* obj) { static_cast<PropertyTestObjectA*>(obj)->onProp1Changed(); }));
LN_PROPERTY_IMPLEMENT(PropertyTestObjectA, Prop1, m_prop1, PropertyMetadata(PropertyTestObjectA::onProp1Changed));

//const ::ln::Ref<::ln::PropertyInfo> PropertyTestObjectA::ppp = ::ln::makeRef<::ln::PropertyInfo>(PropertyMetadata(PropertyTestObjectA::onProp1Changed));
//PropertyInitializer PropertyTestObjectA::iniii((PropertyMetadata(PropertyTestObjectA::onProp1Changed)));

void PropertyTestObjectA::initialize()
{
    Object::initialize();
}

void PropertyTestObjectA::onProp1Changed(Object* obj)
{
    static_cast<PropertyTestObjectA*>(obj)->m_count++;
}

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, Property)
{
    auto objA = newObject<PropertyTestObjectA>();
    objA->setProp1(5);
    ASSERT_EQ(5, objA->prop1());

    ASSERT_EQ(1, objA->m_count);


    //* [ ] PropertyInfo が取れる
    ASSERT_EQ(true, PropertyTestObjectA::Prop1Id != nullptr);
}
