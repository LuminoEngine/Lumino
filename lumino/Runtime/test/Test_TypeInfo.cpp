#include <LuminoEngine/Engine/EngineContext2.hpp>

class Test_TypeInfo : public ::testing::Test {};

class MyObject1 : public Object {
    LN_OBJECT;
public:

};
LN_OBJECT_IMPLEMENT(MyObject1, Object) {}

TEST_F(Test_TypeInfo, Basic) {
    Runtime::initialize();

    auto* context = EngineContext2::instance();
    context->registerType<MyObject1>();

    auto* typeInfo1 = TypeInfo::getTypeInfo<MyObject1>();
    ASSERT_TRUE(typeInfo1 != nullptr);

    auto* typeInfo2 = context->findTypeInfo(U"MyObject1");
    ASSERT_TRUE(typeInfo1 != nullptr);

    Runtime::terminate();
}
