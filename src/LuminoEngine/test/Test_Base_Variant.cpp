#include "Common.hpp"

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


//------------------------------------------------------------------------------
//## Variant serialization
TEST_F(Test_Base_Variant, Serialization)
{
	String json = _T("{\"v_Int32\":100}");

	struct Data
	{
		//Variant v_Null;	// TODO:
		Variant v_Bool;
		//Variant v_Char,	// TODO:
		Variant v_Int8;
		Variant v_Int16;
		Variant v_Int32;
		Variant v_Int64;
		Variant v_UInt8;
		Variant v_UInt16;
		Variant v_UInt32;
		Variant v_UInt64;
		Variant v_Float;
		Variant v_Double;
		Variant v_String;
		Variant v_List;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(v_Bool);
			ar & LN_NVP(v_Int8);
			ar & LN_NVP(v_Int16);
			ar & LN_NVP(v_Int32);
			ar & LN_NVP(v_Int64);
			ar & LN_NVP(v_UInt8);
			ar & LN_NVP(v_UInt16);
			ar & LN_NVP(v_UInt32);
			ar & LN_NVP(v_UInt64);
			ar & LN_NVP(v_Float);
			ar & LN_NVP(v_Double);
			ar & LN_NVP(v_String);
			ar & LN_NVP(v_List);
		}
	};

	//* [ ] Save
	{
		JsonTextOutputArchive ar;
		Data data;
		data.v_Bool = true;
		data.v_Int8 = 1;
		data.v_Int16 = 2;
		data.v_Int32 = 3;
		data.v_Int64 = 4;
		data.v_UInt8 = 5;
		data.v_UInt16 = 6;
		data.v_UInt32 = 7;
		data.v_UInt64 = 8;
		data.v_Float = 1;
		data.v_Double = 1;
		data.v_String = _T("11");
		data.v_List = Variant({ 1, 2, 3 });
		ar.process(data);
		json = ar.toString(JsonFormatting::None);
	}

	//* [ ] Load
	{
		JsonTextInputArchive ar(json);
		Data data;
		ar.process(data);
		ASSERT_EQ(true, data.v_Bool.get<bool>());
		ASSERT_EQ(1, data.v_Int8.get<int8_t>());
		ASSERT_EQ(2, data.v_Int16.get<int16_t>());
		ASSERT_EQ(3, data.v_Int32.get<int32_t>());
		ASSERT_EQ(4, data.v_Int64.get<int64_t>());
		ASSERT_EQ(5, data.v_UInt8.get<uint8_t>());
		ASSERT_EQ(6, data.v_UInt16.get<uint16_t>());
		ASSERT_EQ(7, data.v_UInt32.get<uint32_t>());
		ASSERT_EQ(8, data.v_UInt64.get<uint64_t>());
		ASSERT_EQ(true, Math::nearEqual(1, data.v_Float.get<float>()));
		ASSERT_EQ(true, Math::nearEqual(1, data.v_Double.get<double>()));
		ASSERT_EQ(_T("11"), data.v_String.get<String>());
		ASSERT_EQ(3, data.v_List.list().size());
		ASSERT_EQ(1, data.v_List.list()[0].get<int32_t>());
		ASSERT_EQ(2, data.v_List.list()[1].get<int32_t>());
		ASSERT_EQ(3, data.v_List.list()[2].get<int32_t>());
	}
}
