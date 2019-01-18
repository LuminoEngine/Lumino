#include "Common.hpp"
#include <LuminoCore/Base/Variant.hpp>
#include <LuminoCore/Math/Math.hpp>

class Test_Base_Variant : public ::testing::Test {};

class TestRefObject1 : public RefObject {};

TEST_F(Test_Base_Variant, SetAndGet)
{
	//* [ ] default
    Variant v0;
	ASSERT_EQ(VariantType::Null, v0.type());
	
	//* [ ] Bool
    {
        Variant var(true);
        ASSERT_EQ(VariantType::Bool, var.type());
        ASSERT_EQ(true, var.get<bool>());
    }
    //* [ ] Char
    {
        Variant var(u'A');
        ASSERT_EQ(VariantType::Char, var.type());
        ASSERT_EQ(u'A', var.get<Char>());
    }
    //* [ ] Int8
    {
        Variant var((int8_t)5);
        ASSERT_EQ(VariantType::Int8, var.type());
        ASSERT_EQ(5, var.get<int8_t>());
    }
    //* [ ] Int16
    {
        Variant var((int16_t)5);
        ASSERT_EQ(VariantType::Int16, var.type());
        ASSERT_EQ(5, var.get<int16_t>());
    }
    //* [ ] Int32
    {
        Variant var((int32_t)5);
        ASSERT_EQ(VariantType::Int32, var.type());
        ASSERT_EQ(5, var.get<int32_t>());
    }
    //* [ ] Int64
    {
        Variant var((int64_t)5);
        ASSERT_EQ(VariantType::Int64, var.type());
        ASSERT_EQ(5, var.get<int64_t>());
    }
    //* [ ] UInt8
    {
        Variant var((uint8_t)5);
        ASSERT_EQ(VariantType::UInt8, var.type());
        ASSERT_EQ(5, var.get<uint8_t>());
    }
    //* [ ] UInt16
    {
        Variant var((uint16_t)5);
        ASSERT_EQ(VariantType::UInt16, var.type());
        ASSERT_EQ(5, var.get<uint16_t>());
    }
    //* [ ] UInt32
    {
        Variant var((uint32_t)5);
        ASSERT_EQ(VariantType::UInt32, var.type());
        ASSERT_EQ(5, var.get<uint32_t>());
    }
    //* [ ] UInt64
    {
        Variant var((uint64_t)5);
        ASSERT_EQ(VariantType::UInt64, var.type());
        ASSERT_EQ(5, var.get<uint64_t>());
    }
    //* [ ] Float
    {
        Variant var((float)5);
        ASSERT_EQ(VariantType::Float, var.type());
        ASSERT_EQ(true, Math::nearEqual(5.0f, var.get<float>()));
    }
    //* [ ] Double
    {
        Variant var((double)5);
        ASSERT_EQ(VariantType::Double, var.type());
        ASSERT_EQ(true, Math::nearEqual(5.0, var.get<double>()));
    }
    //* [ ] String (Char*)
    {
        Variant var(u"str");
        ASSERT_EQ(VariantType::String, var.type());
        ASSERT_EQ(u"str", var.get<String>());
    }
    //* [ ] String (String)
    {
        Variant var(String(u"str"));
        ASSERT_EQ(VariantType::String, var.type());
        ASSERT_EQ(u"str", var.get<String>());
    }
    //* [ ] RefObject (RefObject*)
    {
        auto ref = makeRef<TestRefObject1>();
        Variant var(ref.get());
        ASSERT_EQ(VariantType::RefObject, var.type());
        ASSERT_EQ(ref.get(), var.getObject<TestRefObject1>());
    }
    //* [ ] RefObject (Ref<RefObject>)
    {
        auto ref = makeRef<TestRefObject1>();
        Variant var(ref);
        ASSERT_EQ(VariantType::RefObject, var.type());
        ASSERT_EQ(ref, var.getObject<TestRefObject1>());
    }
    //* [ ] List
    {
        auto ref = makeRef<List<Variant>>();
        ref->add(1);
        Variant var(ref);
        ASSERT_EQ(VariantType::List, var.type());
        ASSERT_EQ(1, var.list().size());
    }
}

