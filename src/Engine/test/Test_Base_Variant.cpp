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
		Ref<Variant> var = makeVariant(u'A');
        ASSERT_EQ(VariantType::Char, var->type());
        ASSERT_EQ(u'A', var->get<Char>());
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

//------------------------------------------------------------------------------
//## Variant serialization
TEST_F(Test_Base_Variant, Serialization)
{
	String json = _T("{\"v_Int32\":100}");

	struct Data
	{
		Ref<Variant> v_VarNull;
		//Ref<Variant> v_Null;	// TODO:
		Ref<Variant> v_Bool;
		//Ref<Variant> v_Char,	// TODO:
		Ref<Variant> v_Int8;
		Ref<Variant> v_Int16;
		Ref<Variant> v_Int32;
		Ref<Variant> v_Int64;
		Ref<Variant> v_UInt8;
		Ref<Variant> v_UInt16;
		Ref<Variant> v_UInt32;
		Ref<Variant> v_UInt64;
		Ref<Variant> v_Float;
		Ref<Variant> v_Double;
		Ref<Variant> v_String;
		Ref<Variant> v_List;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(v_VarNull);
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
		data.v_VarNull = nullptr;
		data.v_Bool = makeVariant(true);
		data.v_Int8 = makeVariant(1);
		data.v_Int16 = makeVariant(2);
		data.v_Int32 = makeVariant(3);
		data.v_Int64 = makeVariant(4);
		data.v_UInt8 = makeVariant(5);
		data.v_UInt16 = makeVariant(6);
		data.v_UInt32 = makeVariant(7);
		data.v_UInt64 = makeVariant(8);
		data.v_Float = makeVariant(1);
		data.v_Double = makeVariant(1);
		data.v_String = makeVariant(_T("11"));
		data.v_List = makeVariant(List<int>({ 1, 2, 3 }));
		ar.process(data);
		json = ar.toString(JsonFormatting::None);
	}

	//* [ ] Load
	{
		JsonTextInputArchive ar(json);
		Data data;
		ar.process(data);
		ASSERT_EQ(data.v_VarNull, nullptr);
		ASSERT_EQ(true, data.v_Bool->get<bool>());
		ASSERT_EQ(1, data.v_Int8->get<int8_t>());
		ASSERT_EQ(2, data.v_Int16->get<int16_t>());
		ASSERT_EQ(3, data.v_Int32->get<int32_t>());
		ASSERT_EQ(4, data.v_Int64->get<int64_t>());
		ASSERT_EQ(5, data.v_UInt8->get<uint8_t>());
		ASSERT_EQ(6, data.v_UInt16->get<uint16_t>());
		ASSERT_EQ(7, data.v_UInt32->get<uint32_t>());
		ASSERT_EQ(8, data.v_UInt64->get<uint64_t>());
		ASSERT_EQ(true, Math::nearEqual(1, data.v_Float->get<float>()));
		ASSERT_EQ(true, Math::nearEqual(1, data.v_Double->get<double>()));
		ASSERT_EQ(_T("11"), data.v_String->get<String>());
		ASSERT_EQ(3, data.v_List->list().size());
		ASSERT_EQ(1, data.v_List->list()[0]->get<int32_t>());
		ASSERT_EQ(2, data.v_List->list()[1]->get<int32_t>());
		ASSERT_EQ(3, data.v_List->list()[2]->get<int32_t>());
	}
}

TEST_F(Test_Base_Variant, UseCase1)
{
	class Command : public Object
	{
	public:
		String code;
		Ref<List<Ref<Variant>>> params;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(code);
			ar & LN_NVP(params);
		}
	};

	class Script : public Object
	{
	public:
		String name;
		Ref<List<Ref<Command>>> commandList;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(name);
			ar & LN_NVP(commandList);
		}
	};

	String json;

	//- [ ]  Save
	{
		auto script1 = makeObject<Script>();
		script1->name = _TT("script1");
		script1->commandList = makeList<Ref<Command>>();

		auto c1 = makeObject<Command>();
		c1->code = _TT("c1");
		c1->params = makeList<Ref<Variant>>({ makeVariant(1), makeVariant(_TT("test1")) });
		script1->commandList->add(c1);
		auto c2 = makeObject<Command>();
		c2->code = _TT("c2");
		c2->params = makeList<Ref<Variant>>({ makeVariant(2), makeVariant(_TT("test2")) });
		script1->commandList->add(c2);

		JsonTextOutputArchive ar;
		ar.process(script1);
		json = ar.toString(JsonFormatting::None);
	}

	//- [ ] Load
	{
		auto script2 = makeObject<Script>();

		JsonTextInputArchive ar(json);
		ar.process(*script2);

		ASSERT_EQ(_TT("script1"), script2->name);
		ASSERT_EQ(2, script2->commandList->size());

		ASSERT_EQ(_TT("c1"), script2->commandList[0]->code);
		ASSERT_EQ(1, script2->commandList[0]->params[0]->get<int>());
		ASSERT_EQ(_TT("test1"), script2->commandList[0]->params[1]->get<String>());

		ASSERT_EQ(_TT("c2"), script2->commandList[1]->code);
		ASSERT_EQ(2, script2->commandList[1]->params[0]->get<int>());
		ASSERT_EQ(_TT("test2"), script2->commandList[1]->params[1]->get<String>());
	}
}


TEST_F(Test_Base_Variant, Value_Rect)
{
	struct S { Ref<Variant> v; void serialize(Archive& ar) { ar & LN_NVP(v); } };
	S s1 = { makeVariant(Rect(1, 2, 3, 4)) };
	S s2;

	// Note: struct は型情報を保存しないので、Rect の復元はできない。
	//auto json = JsonSerializer::serialize(s1, JsonFormatting::None);
	//JsonSerializer::deserialize(json, s2);
}
