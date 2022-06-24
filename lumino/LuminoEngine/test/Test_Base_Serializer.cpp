#include "Common.hpp"
#include <LuminoEngine/Reflection/Serializer3.hpp>

class Test_Base_Serializer : public ::testing::Test {};

//-----------------------------------------------------------------------------
class TestObject1 : public Object {
    LN_OBJECT;
public:
    int m_value = 0;

	void serialize3(Serializer3& ar) override {
        ar & LN_NVP(m_value);
	}
};
LN_OBJECT_IMPLEMENT(TestObject1, Object) {}

//TEST_F(Test_Base_Serializer, ErrorHandling) {
//
//    // Load
//    auto obj2 = JsonSerializer3::deserialize<TestObject1>(U"{a:");
//}

TEST_F(Test_Base_Serializer, EmptyObject) {
    class TestClass : public Object {
    };

    auto obj1 = makeObject<TestClass>();
    String text = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);
    ASSERT_EQ(UR"({})", text); 

    auto obj2 = JsonSerializer3::deserialize<TestClass>(text);
    ASSERT_TRUE(obj2 != obj1);
    ASSERT_TRUE(dynamic_cast<TestClass*>(obj2.get()) != nullptr);
}

TEST_F(Test_Base_Serializer, Basic) {
    auto obj1 = makeObject<TestObject1>();
    obj1->m_value = 500;
    String text = JsonSerializer3::serialize<TestObject1>(obj1, JsonFormatting::None);
    ASSERT_EQ(UR"({"m_value":500})", text);	// 型登録していないので、型情報は保存されない

	// 型情報は無くても、Ref<> の型引数に一致するオブジェクトを生成できる
    auto obj2 = JsonSerializer3::deserialize<TestObject1>(text);
    ASSERT_EQ(true, obj2 != obj1);
    ASSERT_EQ(500, obj2->m_value);
}

TEST_F(Test_Base_Serializer, DefaultValue) {
    class TestClass : public Object {
    public:
        int x = 0;
        int y = 0;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(x);
            ar& LN_NVP(y, 123);
        }
    };

    String text = UR"({"x":5})";
    auto obj2 = JsonSerializer3::deserialize<TestClass>(text);
    ASSERT_EQ(5, obj2->x);
    ASSERT_EQ(123, obj2->y);
}

TEST_F(Test_Base_Serializer, PrimitiveValues) {
    class TestClass : public Object {
    public:
        int8_t v_s8l = 0;
        int16_t v_s16l = 0;
        int32_t v_s32l = 0;
        int64_t v_s64l = 0;
        int8_t v_s8u = 0;
        int16_t v_s16u = 0;
        int32_t v_s32u = 0;
        int64_t v_s64u = 0;
        float v_floatl = 0;
        double v_doublel = 0;
        float v_floatu = 0;
        double v_doubleu = 0;
        float v_floatm = 0;
        double v_doublem = 0;
        String v_str;

        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(v_s8l);
            ar& LN_NVP(v_s16l);
            ar& LN_NVP(v_s32l);
            ar& LN_NVP(v_s64l);
            ar& LN_NVP(v_s8u);
            ar& LN_NVP(v_s16u);
            ar& LN_NVP(v_s32u);
            ar& LN_NVP(v_s64u);
            ar& LN_NVP(v_floatl);
            ar& LN_NVP(v_doublel);
            ar& LN_NVP(v_floatu);
            ar& LN_NVP(v_doubleu);
            ar& LN_NVP(v_floatm);
            ar& LN_NVP(v_doublem);
            ar& LN_NVP(v_str);
        }
    };

    auto obj1 = makeObject<TestClass>();
    obj1->v_s8l = INT8_MIN;
    obj1->v_s16l = INT16_MIN;
    obj1->v_s32l = INT32_MIN;
    obj1->v_s64l = INT64_MIN;
    obj1->v_s8u = INT8_MAX;
    obj1->v_s16u = INT16_MAX;
    obj1->v_s32u = INT32_MAX;
    obj1->v_s64u = INT64_MAX;
    obj1->v_floatl = -FLT_MAX;  // https://stackoverflow.com/questions/2528039/why-is-flt-min-equal-to-zero
    obj1->v_doublel = -DBL_MAX; // Json にするとものすごくたくさん桁がでるけどそれでよい
    obj1->v_floatu = FLT_MAX;
    obj1->v_doubleu = DBL_MAX;
    obj1->v_floatm = 1.0f;
    obj1->v_doublem = 1.0f;
    obj1->v_str = _LT("text");

    // Save
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
	ASSERT_EQ(obj1->v_s8l, obj2->v_s8l);
	ASSERT_EQ(obj1->v_s16l, obj2->v_s16l);
	ASSERT_EQ(obj1->v_s32l, obj2->v_s32l);
	ASSERT_EQ(obj1->v_s64l, obj2->v_s64l);
	ASSERT_EQ(obj1->v_s8u, obj2->v_s8u);
	ASSERT_EQ(obj1->v_s16u, obj2->v_s16u);
	ASSERT_EQ(obj1->v_s32u, obj2->v_s32u);
	ASSERT_EQ(obj1->v_s64u, obj2->v_s64u);
	ASSERT_FLOAT_EQ(obj1->v_floatl, obj2->v_floatl);
	ASSERT_DOUBLE_EQ(obj1->v_doublel, obj2->v_doublel);
	ASSERT_FLOAT_EQ(obj1->v_floatu, obj2->v_floatu);
	ASSERT_DOUBLE_EQ(obj1->v_doubleu, obj2->v_doubleu);
	ASSERT_FLOAT_EQ(obj1->v_floatm, obj2->v_floatm);
	ASSERT_DOUBLE_EQ(obj1->v_doublem, obj2->v_doublem);
	ASSERT_EQ(obj1->v_str, obj2->v_str);
}

TEST_F(Test_Base_Serializer, InnerObject) {
    class TestClass : public Object {
    public:
        Ref<TestObject1> m_inner1;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_inner1);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_inner1 = makeObject<TestObject1>();
    obj1->m_inner1->m_value = 123;
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_TRUE(obj2->m_inner1 != nullptr);
    ASSERT_TRUE(obj2->m_inner1 != obj1->m_inner1);
    ASSERT_EQ(123, obj2->m_inner1->m_value);
}

TEST_F(Test_Base_Serializer, Struct) {
    struct TestStruct {
        int m_value;
        void serialize3(Serializer3& ar) {
            ar& LN_NVP(m_value);
        }
    };

    class TestClass : public Object {
    public:
        TestStruct m_value;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_value);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_value.m_value = 123;
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_EQ(123, obj2->m_value.m_value);
}

TEST_F(Test_Base_Serializer, EmptyArray) {
    class TestClass : public Object {
    public:
        Array<int> m_array;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_array);
        }
    };

    auto obj1 = makeObject<TestClass>();
    String text = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    auto obj2 = JsonSerializer3::deserialize<TestClass>(text);
    ASSERT_TRUE(obj2->m_array.isEmpty());
}

TEST_F(Test_Base_Serializer, PrimitiveValueArray) {
    class TestClass : public Object {
    public:
        Array<int> m_array;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_array);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_array = { 1, 2, 3 };
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);

    ASSERT_EQ(true, obj2 != nullptr);
    ASSERT_EQ(3, obj2->m_array.size());
    ASSERT_EQ(1, obj2->m_array[0]);
    ASSERT_EQ(2, obj2->m_array[1]);
    ASSERT_EQ(3, obj2->m_array[2]);
}

TEST_F(Test_Base_Serializer, StructArray) {
    struct TestStruct {
        int m_value;
        void serialize3(Serializer3& ar) {
            ar& LN_NVP(m_value);
        }
    };

    class TestClass : public Object {
    public:
        Array<TestStruct> m_values;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_values);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_values.resize(3);
    obj1->m_values[0].m_value = 1;
    obj1->m_values[1].m_value = 2;
    obj1->m_values[2].m_value = 3;
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_EQ(3, obj2->m_values.size());
    ASSERT_EQ(1, obj2->m_values[0].m_value);
    ASSERT_EQ(2, obj2->m_values[1].m_value);
    ASSERT_EQ(3, obj2->m_values[2].m_value);
}

TEST_F(Test_Base_Serializer, ObjectArray) {
    struct TestClassB : public Object {
    public:
        int m_value;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_value);
        }
    };

    class TestClass : public Object {
    public:
        Array<Ref<TestClassB>> m_values;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_values);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_values.resize(3);
    obj1->m_values[0] = makeObject<TestClassB>();
    obj1->m_values[1] = makeObject<TestClassB>();
    obj1->m_values[2] = makeObject<TestClassB>();
    obj1->m_values[0]->m_value = 1;
    obj1->m_values[1]->m_value = 2;
    obj1->m_values[2]->m_value = 3;
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_EQ(3, obj2->m_values.size());
    ASSERT_EQ(1, obj2->m_values[0]->m_value);
    ASSERT_EQ(2, obj2->m_values[1]->m_value);
    ASSERT_EQ(3, obj2->m_values[2]->m_value);
}

TEST_F(Test_Base_Serializer, ArrayArray) {
    class TestClass : public Object {
    public:
        Array<Array<int>> m_array;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(m_array);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_array = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);

    ASSERT_EQ(true, obj2 != nullptr);
    ASSERT_EQ(3, obj2->m_array.size());
    ASSERT_EQ(Array<int>({ 1, 2, 3 }), obj2->m_array[0]);
    ASSERT_EQ(Array<int>({ 4, 5, 6 }), obj2->m_array[1]);
    ASSERT_EQ(Array<int>({ 7, 8, 9 }), obj2->m_array[2]);
}

TEST_F(Test_Base_Serializer, MixedTypes) {
    class TestClass : public Object {
    public:
        int m_int = 0;
        String m_string;
        Ref<TestObject1> m_obj1;
        Array<int> m_list1;
        Array<Ref<TestObject1>> m_list2;
        void serialize3(Serializer3& ar) override {
            ar& makeNVP(_TT("m_int"), m_int);
            ar& makeNVP(_TT("m_string"), m_string);
            ar& makeNVP(_TT("m_obj1"), m_obj1);
            ar& makeNVP(_TT("m_list1"), m_list1);
            ar& makeNVP(_TT("m_list2"), m_list2);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->m_int = 8;
    obj1->m_string = _TT("test");
    obj1->m_obj1 = makeObject<TestObject1>();
    obj1->m_obj1->m_value = 256;
    obj1->m_list1 = { 1, 2, 3 };
    obj1->m_list2 = { makeObject<TestObject1>(), makeObject<TestObject1>(), makeObject<TestObject1>() };
    obj1->m_list2[0]->m_value = 100;
    obj1->m_list2[1]->m_value = 200;
    obj1->m_list2[2]->m_value = 300;
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_EQ(true, obj2 != nullptr);
    ASSERT_EQ(8, obj2->m_int);
    ASSERT_EQ(_TT("test"), obj2->m_string);
    ASSERT_EQ(256, obj2->m_obj1->m_value);
    ASSERT_EQ(3, obj2->m_list1.size());
    ASSERT_EQ(1, obj2->m_list1[0]);
    ASSERT_EQ(2, obj2->m_list1[1]);
    ASSERT_EQ(3, obj2->m_list1[2]);
    ASSERT_EQ(3, obj2->m_list2.size());
    ASSERT_EQ(100, obj2->m_list2[0]->m_value);
    ASSERT_EQ(200, obj2->m_list2[1]->m_value);
    ASSERT_EQ(300, obj2->m_list2[2]->m_value);
}

TEST_F(Test_Base_Serializer, Optional) {
    class TestClass : public Object {
    public:
        Optional<int> value1;
        Optional<int> value2;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(value1);
            ar& LN_NVP(value2);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->value2 = 100;
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_FALSE(obj2->value1.hasValue());
    ASSERT_TRUE(obj2->value2.hasValue());
    ASSERT_EQ(100, obj2->value2.value());
}

TEST_F(Test_Base_Serializer, OptionalStruct) {
    struct TestStruct {
        int m_value;
        void serialize3(Serializer3& ar) {
            ar& LN_NVP(m_value);
        }
    };

    class TestClass : public Object {
    public:
        Optional<TestStruct> value1;
        Optional<TestStruct> value2;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(value1);
            ar& LN_NVP(value2);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->value2 = TestStruct{ 100 };
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_FALSE(obj2->value1.hasValue());
    ASSERT_TRUE(obj2->value2.hasValue());
    ASSERT_EQ(100, obj2->value2->m_value);
}

TEST_F(Test_Base_Serializer, OptionalArray) {
    class TestClass : public Object {
    public:
        Optional<Array<int>> value1;
        Optional<Array<int>> value2;
        Array<Optional<int>> value3;
        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(value1);
            ar& LN_NVP(value2);
            ar& LN_NVP(value3);
        }
    };

    // Save
    auto obj1 = makeObject<TestClass>();
    obj1->value2 = { 1, 2, 3 };
    obj1->value3.push(std::nullopt);
    obj1->value3.push(10);
    String json = JsonSerializer3::serialize<TestClass>(obj1, JsonFormatting::None);

    // Load
    auto obj2 = JsonSerializer3::deserialize<TestClass>(json);
    ASSERT_FALSE(obj2->value1.hasValue());
    ASSERT_TRUE(obj2->value2.hasValue());
    ASSERT_EQ(Array<int>({ 1, 2, 3 }), obj2->value2.value());
    ASSERT_FALSE(obj2->value3[0].hasValue());
    ASSERT_TRUE(obj2->value3[1].hasValue());
    ASSERT_EQ(10, obj2->value3[1].value());
}

class ClassVersionTestClass1 : public Object {
public:
    int x = 0;
    int _ver = 0;
    LN_SERIALIZE_CLASS_VERSION3(5)
    void serialize3(Serializer3& ar) override {
        ar& LN_NVP(x);
        _ver = ar.classVersion();
    }
};

class ClassVersionTestClass2 : public Object {
public:
    Ref<ClassVersionTestClass1> value;
    int _ver = 0;
    LN_SERIALIZE_CLASS_VERSION3(7)
    void serialize3(Serializer3& ar) override {
        ar& LN_NVP(value);
        _ver = ar.classVersion();
    }
};

TEST_F(Test_Base_Serializer, ClassVersion) {
    auto obj1 = makeObject<ClassVersionTestClass2>();
    obj1->value = makeObject<ClassVersionTestClass1>();
    obj1->value->x = 10;
    String json = JsonSerializer3::serialize<ClassVersionTestClass2>(obj1, JsonFormatting::None);

    auto obj2 = JsonSerializer3::deserialize<ClassVersionTestClass2>(json);
    ASSERT_EQ(10, obj2->value->x);
    ASSERT_EQ(7, obj2->_ver);
    ASSERT_EQ(5, obj2->value->_ver);
}

TEST_F(Test_Base_Serializer, Variant) {
    class Data : public Object {
    public:
        Ref<Variant> v_VarNull;
        // Ref<Variant> v_Null;	// TODO:
        Ref<Variant> v_Bool;
        // Ref<Variant> v_Char,	// TODO:
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
        //Ref<Variant> v_List;

        void serialize3(Serializer3& ar) override {
            ar& LN_NVP(v_VarNull);
            ar& LN_NVP(v_Bool);
            ar& LN_NVP(v_Int8);
            ar& LN_NVP(v_Int16);
            ar& LN_NVP(v_Int32);
            ar& LN_NVP(v_Int64);
            ar& LN_NVP(v_UInt8);
            ar& LN_NVP(v_UInt16);
            ar& LN_NVP(v_UInt32);
            ar& LN_NVP(v_UInt64);
            ar& LN_NVP(v_Float);
            ar& LN_NVP(v_Double);
            ar& LN_NVP(v_String);
            //ar& LN_NVP(v_List);
        }
    };

    // Save
    String json;
    {
        auto data = makeObject<Data>();
        data->v_VarNull = nullptr;
        data->v_Bool = makeVariant(true);
        data->v_Int8 = makeVariant(1);
        data->v_Int16 = makeVariant(2);
        data->v_Int32 = makeVariant(3);
        data->v_Int64 = makeVariant(4);
        data->v_UInt8 = makeVariant(5);
        data->v_UInt16 = makeVariant(6);
        data->v_UInt32 = makeVariant(7);
        data->v_UInt64 = makeVariant(8);
        data->v_Float = makeVariant(1);
        data->v_Double = makeVariant(1);
        data->v_String = makeVariant(_T("11"));
        //data->v_List = makeVariant(List<int>({ 1, 2, 3 }));
        json = JsonSerializer3::serialize<Data>(data, JsonFormatting::None);
    }

    // Load
    {
        auto data = JsonSerializer3::deserialize<Data>(json);
        ASSERT_EQ(data->v_VarNull, nullptr);
        ASSERT_EQ(true, data->v_Bool->get<bool>());
        ASSERT_EQ(1, data->v_Int8->get<int8_t>());
        ASSERT_EQ(2, data->v_Int16->get<int16_t>());
        ASSERT_EQ(3, data->v_Int32->get<int32_t>());
        ASSERT_EQ(4, data->v_Int64->get<int64_t>());
        ASSERT_EQ(5, data->v_UInt8->get<uint8_t>());
        ASSERT_EQ(6, data->v_UInt16->get<uint16_t>());
        ASSERT_EQ(7, data->v_UInt32->get<uint32_t>());
        ASSERT_EQ(8, data->v_UInt64->get<uint64_t>());
        ASSERT_EQ(true, Math::nearEqual(1, data->v_Float->get<float>()));
        ASSERT_EQ(true, Math::nearEqual(1, data->v_Double->get<double>()));
        ASSERT_EQ(_T("11"), data->v_String->get<String>());
        //ASSERT_EQ(3, data->v_List->list().size());
        //ASSERT_EQ(1, data->v_List->list()[0]->get<int32_t>());
        //ASSERT_EQ(2, data->v_List->list()[1]->get<int32_t>());
        //ASSERT_EQ(3, data->v_List->list()[2]->get<int32_t>());
    }
}
