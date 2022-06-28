#include "Common.hpp"

class Test_Base_Variant : public ::testing::Test {};

class TestRefObject1 : public RefObject {};

TEST_F(Test_Base_Variant, SetAndGet)
{
	//* [ ] default
    Ref<Variant> v0 = makeVariant();
	ASSERT_EQ(VariantType::Null, v0->type());
	
	//* [ ] Bool
    {
		Ref<Variant> var = makeVariant(true);
        ASSERT_EQ(VariantType::Bool, var->type());
        ASSERT_EQ(true, var->get<bool>());
    }
    //* [ ] Char
    {
		Ref<Variant> var = makeVariant(_TT('A'));
        ASSERT_EQ(VariantType::Char, var->type());
        ASSERT_EQ(_TT('A'), var->get<Char>());
    }
    //* [ ] Int8
    {
		Ref<Variant> var = makeVariant((int8_t)5);
        ASSERT_EQ(VariantType::Int8, var->type());
        ASSERT_EQ(5, var->get<int8_t>());
    }
    //* [ ] Int16
    {
		Ref<Variant> var = makeVariant((int16_t)5);
        ASSERT_EQ(VariantType::Int16, var->type());
        ASSERT_EQ(5, var->get<int16_t>());
    }
    //* [ ] Int32
    {
		Ref<Variant> var = makeVariant((int32_t)5);
        ASSERT_EQ(VariantType::Int32, var->type());
        ASSERT_EQ(5, var->get<int32_t>());
    }
    //* [ ] Int64
    {
		Ref<Variant> var = makeVariant((int64_t)5);
        ASSERT_EQ(VariantType::Int64, var->type());
        ASSERT_EQ(5, var->get<int64_t>());
    }
    //* [ ] UInt8
    {
		Ref<Variant> var = makeVariant((uint8_t)5);
        ASSERT_EQ(VariantType::UInt8, var->type());
        ASSERT_EQ(5, var->get<uint8_t>());
    }
    //* [ ] UInt16
    {
		Ref<Variant> var = makeVariant((uint16_t)5);
        ASSERT_EQ(VariantType::UInt16, var->type());
        ASSERT_EQ(5, var->get<uint16_t>());
    }
    //* [ ] UInt32
    {
		Ref<Variant> var = makeVariant((uint32_t)5);
        ASSERT_EQ(VariantType::UInt32, var->type());
        ASSERT_EQ(5, var->get<uint32_t>());
    }
    //* [ ] UInt64
    {
		Ref<Variant> var = makeVariant((uint64_t)5);
        ASSERT_EQ(VariantType::UInt64, var->type());
        ASSERT_EQ(5, var->get<uint64_t>());
    }
    //* [ ] Float
    {
		Ref<Variant> var = makeVariant((float)5);
        ASSERT_EQ(VariantType::Float, var->type());
        ASSERT_EQ(true, Math::nearEqual(5.0f, var->get<float>()));
    }
    //* [ ] Double
    {
		Ref<Variant> var = makeVariant((double)5);
        ASSERT_EQ(VariantType::Double, var->type());
        ASSERT_EQ(true, Math::nearEqual(5.0, var->get<double>()));
    }
    //* [ ] String (Char*)
    {
		Ref<Variant> var = makeVariant(_TT("str"));
        ASSERT_EQ(VariantType::String, var->type());
        ASSERT_EQ(_TT("str"), var->get<String>());
    }
    //* [ ] String (String)
    {
		Ref<Variant> var = makeVariant(String(_TT("str")));
        ASSERT_EQ(VariantType::String, var->type());
        ASSERT_EQ(_TT("str"), var->get<String>());
    }
    //* [ ] RefObject (RefObject*)
    {
        auto ref = makeRef<TestRefObject1>();
		Ref<Variant> var = makeVariant(ref.get());
        ASSERT_EQ(VariantType::RefObject, var->type());
        ASSERT_EQ(ref.get(), var->getObject<TestRefObject1>());
    }
    //* [ ] RefObject (Ref<RefObject>)
    {
        auto ref = makeRef<TestRefObject1>();
		Ref<Variant> var = makeVariant(ref);
        ASSERT_EQ(VariantType::RefObject, var->type());
        ASSERT_EQ(ref, var->getObject<TestRefObject1>());
    }
    //* [ ] List
    {
        auto ref = makeRef<List<Ref<Variant>>>();
        ref->add(makeVariant(1));
		Ref<Variant> var = makeVariant(ref);
        ASSERT_EQ(VariantType::List, var->type());
        ASSERT_EQ(1, var->list().size());
    }
}
