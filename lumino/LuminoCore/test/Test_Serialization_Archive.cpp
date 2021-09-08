#include "Common.hpp"
#include <LuminoCore/Json/JsonDocument.hpp>
#include <LuminoCore/Serialization/Serialization.hpp>
#include <LuminoCore/Math/Math.hpp>

//==============================================================================
//# シリアライズのテスト
class Test_Serialization2 : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
//## Minimal
TEST_F(Test_Serialization2, Minimal)
{
	//TODO: ルート Value は未対応
	////* [ ] Root primitive value
	//{
	//	int v1 = 100;
	//	int v2 = 0;
	//	String json = JsonSerializer::serialize(v1);
	//	JsonSerializer::deserialize(json, v2);
	//	ASSERT_EQ(100, v2);
	//}

	//* [ ] EmptyObject
	{
		struct MyData
		{
			int callCount = 0;
			void serialize(Archive& ar) { callCount++; }
		};

		MyData data1;
		String json = JsonSerializer::serialize(data1, JsonFormatting::None);
		ASSERT_EQ(json, _TT("{}"));

		JsonSerializer::deserialize(json, data1);

		ASSERT_EQ(2, data1.callCount);
	}
}

//------------------------------------------------------------------------------
//## String test
TEST_F(Test_Serialization2, String)
{
    struct MyData
    {
        String value;

        void serialize(Archive& ar)
        {
            ar & LN_NVP(value);
        }
    };

    MyData data1;
    data1.value = _TT("test");
    String json = JsonSerializer::serialize(data1, JsonFormatting::None);
    ASSERT_EQ(json, _TT("{\"value\":\"test\"}"));

    MyData data2;
    JsonSerializer::deserialize(json, data2);
    ASSERT_EQ(_TT("test"), data2.value);
}

//------------------------------------------------------------------------------
//## List<> test
TEST_F(Test_Serialization2, List)
{
	List<int> list1 = { 1, 2, 3 };
	String json = JsonSerializer::serialize(list1, JsonFormatting::None);
	ASSERT_EQ(json, _TT("[1,2,3]"));

	List<int> list2;
	JsonSerializer::deserialize(json, list2);
	ASSERT_EQ(3, list2.size());
	ASSERT_EQ(1, list2[0]);
	ASSERT_EQ(2, list2[1]);
	ASSERT_EQ(3, list2[2]);

    // empty list
    List<int> list3;
    json = JsonSerializer::serialize(list3, JsonFormatting::None);
    ASSERT_EQ(json, _TT("[]"));

    JsonSerializer::deserialize(json, list2);
    ASSERT_EQ(0, list2.size());

}

//------------------------------------------------------------------------------
//## ValueObject test
TEST_F(Test_Serialization2, ValueObject)
{
	struct MyData
	{
		int value;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(value);
		}
	};

	MyData data1;
	data1.value = 100;
	String json = JsonSerializer::serialize(data1, JsonFormatting::None);
	ASSERT_EQ(json, _TT("{\"value\":100}"));

	MyData data2;
	JsonSerializer::deserialize(json, data2);
	ASSERT_EQ(100, data2.value);
}

//------------------------------------------------------------------------------
//## Ref<> test
TEST_F(Test_Serialization2, RefObject)
{
	struct Class1 : public ln::RefObject
	{
		int value;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(value);
		}
	};

	struct MyData
	{
		Ref<Class1> ref;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(ref);
		}
	};

    //* [ ] null
    {
        MyData data1;
        data1.ref = nullptr;
        String json = JsonSerializer::serialize(data1, JsonFormatting::None);
        ASSERT_EQ(json, _TT("{\"ref\":null}"));

        MyData data2;
        data2.ref = makeRef<Class1>();
        JsonSerializer::deserialize(json, data2);
        ASSERT_EQ(nullptr, data2.ref);
    }

    //* [ ] has value
    {
        MyData data1;
        data1.ref = makeRef<Class1>();
        data1.ref->value = 100;
        String json = JsonSerializer::serialize(data1, JsonFormatting::None);
        ASSERT_EQ(json, _TT("{\"ref\":{\"value\":100}}"));

        MyData data2;
        JsonSerializer::deserialize(json, data2);
        ASSERT_EQ(100, data2.ref->value);
    }

    //* [ ] Ref<> nest and after other value
    {
        struct Data1 : public ln::RefObject
        {
            Ref<Data1> child;
            int value;

            void serialize(Archive& ar)
            {
                ar & LN_NVP(child);
                ar & LN_NVP(value);
            }
        };

        // save
        Data1 data1;
        data1.value = 1;
        data1.child = makeRef<Data1>();
        data1.child->value = 2;
        String json = JsonSerializer::serialize(data1);

        // load
        Data1 data2;
        JsonSerializer::deserialize(json, data2);
        ASSERT_EQ(1, data2.value);
        ASSERT_EQ(2, data2.child->value);
        ASSERT_EQ(nullptr, data2.child->child);
    }
}

//------------------------------------------------------------------------------
//## PrimitiveValues test
TEST_F(Test_Serialization2, PrimitiveValues)
{
    //- [ ] 各値を save/load できること

    struct Test
    {
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

        void serialize(Archive& ar)
        {
            ar & LN_NVP(v_s8l);
            ar & LN_NVP(v_s16l);
            ar & LN_NVP(v_s32l);
            ar & LN_NVP(v_s64l);

            ar & LN_NVP(v_s8u);
            ar & LN_NVP(v_s16u);
            ar & LN_NVP(v_s32u);
            ar & LN_NVP(v_s64u);

            ar & LN_NVP(v_floatl);
            ar & LN_NVP(v_doublel);

            ar & LN_NVP(v_floatu);
            ar & LN_NVP(v_doubleu);

            ar & LN_NVP(v_floatm);
            ar & LN_NVP(v_doublem);

            ar & LN_NVP(v_str);
        }
    };

    String json;

    Test obj1;
    obj1.v_s8l = INT8_MIN;
    obj1.v_s16l = INT16_MIN;
    obj1.v_s32l = INT32_MIN;
    obj1.v_s64l = INT64_MIN;
    obj1.v_s8u = INT8_MAX;
    obj1.v_s16u = INT16_MAX;
    obj1.v_s32u = INT32_MAX;
    obj1.v_s64u = INT64_MAX;
    obj1.v_floatl = -FLT_MAX;	// https://stackoverflow.com/questions/2528039/why-is-flt-min-equal-to-zero
    obj1.v_doublel = -DBL_MAX;	// Json にするとものすごくたくさん桁がでるけどそれでよい
    obj1.v_floatu = FLT_MAX;
    obj1.v_doubleu = DBL_MAX;
    obj1.v_floatm = 1.0f;
    obj1.v_doublem = 1.0f;
    obj1.v_str = _LT("text");

    // Save
    {
        JsonTextOutputArchive ar;
        ar.process(obj1);
        json = ar.toString(JsonFormatting::None);
    }

    // Load
    Test obj2;
    {
        JsonTextInputArchive ar(json);
        ar.process(obj2);
    }

    ASSERT_EQ(obj1.v_s8l, obj2.v_s8l);
    ASSERT_EQ(obj1.v_s16l, obj2.v_s16l);
    ASSERT_EQ(obj1.v_s32l, obj2.v_s32l);
    ASSERT_EQ(obj1.v_s64l, obj2.v_s64l);
    ASSERT_EQ(obj1.v_s8u, obj2.v_s8u);
    ASSERT_EQ(obj1.v_s16u, obj2.v_s16u);
    ASSERT_EQ(obj1.v_s32u, obj2.v_s32u);
    ASSERT_EQ(obj1.v_s64u, obj2.v_s64u);
    ASSERT_FLOAT_EQ(obj1.v_floatl, obj2.v_floatl);
    ASSERT_DOUBLE_EQ(obj1.v_doublel, obj2.v_doublel);
    ASSERT_FLOAT_EQ(obj1.v_floatu, obj2.v_floatu);
    ASSERT_DOUBLE_EQ(obj1.v_doubleu, obj2.v_doubleu);
    ASSERT_FLOAT_EQ(obj1.v_floatm, obj2.v_floatm);
    ASSERT_DOUBLE_EQ(obj1.v_doublem, obj2.v_doublem);
    ASSERT_EQ(obj1.v_str, obj2.v_str);
}

//------------------------------------------------------------------------------
//## List<> types test
TEST_F(Test_Serialization2, ListTypes)
{
	//* [ ] List<> in, int, string, object
	{
		struct MyData0 : public RefObject
		{
			int a;

			MyData0() : a(0) {}
			MyData0(int v) : a(v) {}

			void serialize(Archive& ar)
			{
				ar & LN_NVP(a);
			}
		};
		struct MyData1
		{
			int a;

			void serialize(Archive& ar)
			{
				ar & LN_NVP(a);
			}
		};
		struct MyData2
		{
			List<int> list1;
			List<String> list2;
			List<MyData1> list3;
			List<Ref<MyData0>> list4;

			void serialize(Archive& ar)
			{
				ar & LN_NVP(list1);
				ar & LN_NVP(list2);
				ar & LN_NVP(list3);
				ar & LN_NVP(list4);
			}
		};

		MyData2 data1;
		data1.list1 = { 1, 2, 3 };
		data1.list2 = { _T("a"), _T("b"), _T("c") };
		data1.list3 = { MyData1{1}, MyData1{2}, MyData1{3} };
		data1.list4 = { makeRef<MyData0>(4), makeRef<MyData0>(5), makeRef<MyData0>(6) };
		String json = JsonSerializer::serialize(data1);

		MyData2 data2;
		JsonSerializer::deserialize(json, data2);
		ASSERT_EQ(3, data2.list1.size());
		ASSERT_EQ(3, data2.list2.size());
		ASSERT_EQ(3, data2.list3.size());
		ASSERT_EQ(1, data2.list1[0]);
		ASSERT_EQ(2, data2.list1[1]);
		ASSERT_EQ(3, data2.list1[2]);
		ASSERT_EQ(_T("a"), data2.list2[0]);
		ASSERT_EQ(_T("b"), data2.list2[1]);
		ASSERT_EQ(_T("c"), data2.list2[2]);
		ASSERT_EQ(1, data2.list3[0].a);
		ASSERT_EQ(2, data2.list3[1].a);
		ASSERT_EQ(3, data2.list3[2].a);
		ASSERT_EQ(4, data2.list4[0]->a);
		ASSERT_EQ(5, data2.list4[1]->a);
		ASSERT_EQ(6, data2.list4[2]->a);
	}
}

//------------------------------------------------------------------------------
//## Optional 型のテスト
TEST_F(Test_Serialization2, Optional)
{
	struct MyData
	{
		Optional<int> value1;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(value1);
		}
	};

	//* [ ] has value
	{
		MyData data1;
		data1.value1 = 10;
		String json = JsonSerializer::serialize(data1, JsonFormatting::None);
		ASSERT_EQ(_TT("{\"value1\":10}"), json);

		MyData data2;
		JsonSerializer::deserialize(json, data2);
		ASSERT_EQ(10, data2.value1.value());
	}

	//* [ ] not has
	{
		MyData data1;
		data1.value1 = nullptr;
		String json = JsonSerializer::serialize(data1, JsonFormatting::None);
		ASSERT_EQ(_TT("{\"value1\":null}"), json);

		MyData data2;
		JsonSerializer::deserialize(json, data2);
		ASSERT_EQ(false, data2.value1.hasValue());
	}

    //* [ ] innter struct
    {
        struct DataB
        {
            int value1;
            void serialize(Archive& ar) { ar & LN_NVP(value1); }
        };
        struct DataA
        {
            Optional<DataB> value2;
            void serialize(Archive& ar) { ar & LN_NVP(value2); }
        };

        DataA data1;
        data1.value2 = DataB{100};
        String json = JsonSerializer::serialize(data1, JsonFormatting::None);
        ASSERT_EQ(_TT("{\"value2\":{\"value1\":100}}"), json);

        DataA data2;
        JsonSerializer::deserialize(json, data2);
        ASSERT_EQ(100, data2.value2.value().value1);
    }

    struct Test
    {
        ln::Optional<ln::List<ln::String>> includePaths;
        ln::Optional<ln::List<ln::String>> defines;
        ln::Optional<ln::String> precompiledHeaderFile;
        ln::Optional<ln::String> mscFullVer;

        void serialize(Archive& ar)
        {
            ar & LN_NVP(includePaths);
            ar & LN_NVP(defines);
            ar & LN_NVP(precompiledHeaderFile);
            ar & LN_NVP(mscFullVer);
        }
    };

    Test t1;
    Test t2;
    ln::String json;

    t1.includePaths = ln::List<ln::String>({ _TT("dir1"), _TT("dir2") });
    t1.defines = nullptr;
    t1.precompiledHeaderFile = ln::String(_TT("pch.h"));
    t1.mscFullVer = nullptr;

    // Save
    {
        JsonTextOutputArchive ar;
        ar.process(t1);
        json = ar.toString(JsonFormatting::None);
        ASSERT_EQ(_TT("{\"includePaths\":[\"dir1\",\"dir2\"],\"defines\":null,\"precompiledHeaderFile\":\"pch.h\",\"mscFullVer\":null}"), json);
    }

    // Load
    t2.mscFullVer = ln::String(_TT("123"));
    {
        JsonTextInputArchive ar(json);
        ar.process(t2);
        ASSERT_EQ(2, t2.includePaths.value().size());
        ASSERT_EQ(_TT("dir1"), t2.includePaths.value().at(0));
        ASSERT_EQ(_TT("dir2"), t2.includePaths.value().at(1));
        ASSERT_EQ(false, t2.defines.hasValue());
        ASSERT_EQ(_TT("pch.h"), t2.precompiledHeaderFile.value());
        ASSERT_EQ(false, t2.mscFullVer.hasValue());
    }


    //* [ ] データ構造バージョンアップ
    {
        struct TestV2
        {
            ln::Optional<ln::List<ln::String>> includePaths;
            ln::Optional<ln::String> compilerVer;

            void serialize(Archive& ar)
            {
                ar & LN_NVP(includePaths);
                ar & LN_NVP(compilerVer);
            }
        };

        TestV2 tv2;
        tv2.compilerVer = ln::String(_TT("567"));

        JsonTextInputArchive ar(json);
        ar.process(tv2);
        ASSERT_EQ(2, tv2.includePaths.value().size());
        ASSERT_EQ(_TT("dir1"), tv2.includePaths.value().at(0)); // 同じ名前は引き継ぐ
        ASSERT_EQ(_TT("dir2"), tv2.includePaths.value().at(1));
        ASSERT_EQ(_TT("567"), tv2.compilerVer.value());      // 新しい名前は serialize からは設定されない
    }
}

//------------------------------------------------------------------------------
//## unordered_map
TEST_F(Test_Serialization2, unordered_map)
{
	struct Test
	{
		ln::String name;
		std::unordered_map<ln::String, ln::String> values;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(name);
			ar & LN_NVP(values);
		}
	};

	Test t1;
	t1.name = _TT("test");
	t1.values = { {_TT("f1"), _TT("s1")}, {_TT("f2"), _TT("s2")} };
	String json = JsonSerializer::serialize(t1, JsonFormatting::None);
	ASSERT_EQ(_TT("{\"name\":\"test\",\"values\":{\"f1\":\"s1\",\"f2\":\"s2\"}}"), json);

	Test t2;
	JsonSerializer::deserialize(json, t2);
	ASSERT_EQ(_TT("test"), t2.name);
	ASSERT_EQ(2, t2.values.size());
	ASSERT_EQ(_TT("s1"), t2.values[_TT("f1")]);
	ASSERT_EQ(_TT("s2"), t2.values[_TT("f2")]);
}

//------------------------------------------------------------------------------
//## Examples
TEST_F(Test_Serialization2, Examples)
{
	//* [ ] Example 1
	{
		struct MyData
		{
			String name;
			int value;

			void serialize(Archive& ar)
			{
				ar & LN_NVP(name);
				ar & LN_NVP(value);
			}
		};

		MyData data1;
		data1.name = _TT("example");
		data1.value = 100;
		String json = JsonSerializer::serialize(data1);

		MyData data2;
		JsonSerializer::deserialize(json, data2);
		ASSERT_EQ(_T("example"), data2.name);
		ASSERT_EQ(100, data2.value);
	}

	//* [ ] Example 2
	{
		struct Documents
		{
            Path caption;
			List<Path> fileList;

			void serialize(Archive& ar)
			{
				ar & LN_NVP(caption);
				ar & LN_NVP(fileList);
			}
		};

		Documents docs1;
		docs1.caption = _TT("note");
		docs1.fileList.add(_TT("file1.md"));
		docs1.fileList.add(_TT("file2.md"));

		String json = JsonSerializer::serialize(docs1, JsonFormatting::None);
		ASSERT_EQ(_TT("{\"caption\":\"note\",\"fileList\":[\"file1.md\",\"file2.md\"]}"), json);


		//------ check
		Documents data2;
		JsonSerializer::deserialize(json, data2);
		ASSERT_EQ(_TT("note"), data2.caption.str());
		ASSERT_EQ(2, data2.fileList.size());
		ASSERT_EQ(_TT("file1.md"), data2.fileList[0].str());
		ASSERT_EQ(_TT("file2.md"), data2.fileList[1].str());
	}
}

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Serialization2, EmptyContainer)
{
	struct Test2
	{
		int x = 200;
		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
		}
	};
    struct EmptyTest1
	{
		void serialize(Archive& ar) { }
	};
    struct EmptyTest2
	{
		EmptyTest1 t1;
		List<int> t2;
		Test2 t3;
		void serialize(Archive& ar)
		{
			ar & LN_NVP(t1);
			ar & LN_NVP(t2);
			ar & LN_NVP(t3);
		}
	};

	String json;

	//- [ ] 空オブジェクトの Save
	{
		JsonTextOutputArchive ar;

		EmptyTest2 t;
		ar.process(t);

		json = ar.toString(JsonFormatting::None);
		ASSERT_EQ(_T("{\"t1\":{},\"t2\":[],\"t3\":{\"x\":200}}"), json);
	}

	//- [ ] 空オブジェクトの Load
	{
		JsonTextInputArchive ar(json);

		EmptyTest2 t;
		t.t3.x = 0;
		ar.process(t);

		ASSERT_EQ(true, t.t2.isEmpty());
		ASSERT_EQ(200, t.t3.x);
	}
}

//------------------------------------------------------------------------------
//## ルートノードの生成を確認する (JSON)
TEST_F(Test_Serialization2, RootNode_Json)
{
	class Test1
	{
	public:
		int x = 0;
		int y = 0;
		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
			ar & LN_NVP(y);
		}
	};

	String json;

	//- [ ] root に名前無しで Object を Save
	{
		JsonTextOutputArchive ar;

		Test1 t1;
		t1.x = 200;
		t1.y = 500;
		ar.save(t1);

		json = ar.toString(JsonFormatting::None);
		ASSERT_EQ(_T("{\"x\":200,\"y\":500}"), json);
	}

	//- [ ] root の名前無し Object を Load
	{
		JsonTextInputArchive ar(json);

		Test1 t1;
		ar.load(t1);

		ASSERT_EQ(200, t1.x);
		ASSERT_EQ(500, t1.y);
	}


	class ArrayTest1
	{
	public:
		int ary[3];
		void serialize(Archive& ar)
		{
			ar.makeArrayTag(nullptr);
			ar.process(ary[0]);
			ar.process(ary[1]);
			ar.process(ary[2]);
		}
	};

#if 0	// TODO: JsonDocument が root の Array 未対応
	//- [ ] root に名前無しで Array を Save
	{
		JsonTextOutputArchive ar;

		ArrayTest1 t1;
		t1.ary[0] = 100; t1.ary[1] = 200; t1.ary[2] = 300;
		ar.process(t1);

		// これは無し。必ず serialize を介す。
		// こんなことあまりすることないし、Load が大変。
		//ar.makeArrayTag();
		//ar.process(ary[0]);
		//ar.process(ary[1]);
		//ar.process(ary[2]);

		json = ar.toString(JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_archive_version\":1,\"lumino_archive_root\":[100,200,300]}"), json);
	}

	//- [ ] root の名前無し Array を Load
	{
		JsonTextInputArchive ar(json);

		ArrayTest1 t1;
		t1.ary[0] = 0; t1.ary[1] = 0; t1.ary[2] = 0;
		ar.process(t1);

		ASSERT_EQ(100, t1.ary[0]);
		ASSERT_EQ(200, t1.ary[1]);
		ASSERT_EQ(300, t1.ary[2]);
	}
#endif
}



//------------------------------------------------------------------------------
//## クラスバージョンを付けたい
//## Save と Load で異なる処理をしたい

class ClassVersionTest1
{
public:
	int x = 0;
	bool flag = false;
	void serialize(Archive& ar)
	{
		ar & LN_NVP(x);
		ar & LN_NVP(flag);
	}
};
LN_SERIALIZE_CLASS_VERSION_NI(ClassVersionTest1, 1);

class ClassVersionTest2
{
public:
	int x = -1;
	int flags = 0;
	void serialize(Archive& ar)
	{
		ar & LN_NVP(x);

		if (ar.isSaving())
		{
			ar & LN_NVP(flags);
		}
		else
		{
			bool oldFlag = true;
			ar & ln::makeNVP(_T("flag"), oldFlag);
			if (!oldFlag) flags = 0xFF;
		}
	}
};
LN_SERIALIZE_CLASS_VERSION_NI(ClassVersionTest2, 2);

TEST_F(Test_Serialization2, ClassVersion)
{
	String json;

	//- [ ] Save
	{
		JsonTextOutputArchive ar;

		ClassVersionTest1 t1;
		ar.process(t1);

		json = ar.toString(JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_class_version\":1,\"x\":0,\"flag\":false}"), json);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);

		ClassVersionTest2 t2;
		ar.process(t2);

		ASSERT_EQ(0, t2.x);
		ASSERT_EQ(0xFF, t2.flags);
	}
}


//------------------------------------------------------------------------------
//## クラスバージョンを class スコープに書きたい (serialize() に近いところに書きたい)

class ClassVersionTest3
{
public:
	int x = 0;
	int ver = 0;

	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(Archive& ar)
	{
		ar & LN_NVP(x);
		ver = ar.classVersion();
	}
};

TEST_F(Test_Serialization2, ClassVersionInner)
{
	//- [ ] Save
	{
		JsonTextOutputArchive ar;

		ClassVersionTest3 t;
		ar.process(t);

		ASSERT_EQ(1, t.ver);
	}
}

//------------------------------------------------------------------------------
//## ベースクラスをシリアライズしたい

class BaseTest1
{
public:
	int x = 0;

	void serialize(Archive& ar)
	{
		ar & LN_NVP(x);
	}
};

class DerivedTest1
	: public BaseTest1
{
public:
	int y = 0;

	void serialize(Archive& ar)
	{
		ar & Archive::BaseClass<BaseTest1>(this);
		ar & LN_NVP(y);
	}
};

TEST_F(Test_Serialization2, BaseClass)
{
	String json;

	//- [ ]  Save
	{
		JsonTextOutputArchive ar;

		DerivedTest1 t;
		t.x = 55;
		t.y = 77;
		ar.process(t);

		json = ar.toString(JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_base_class\":{\"x\":55},\"y\":77}"), json);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);

		DerivedTest1 t;
		ar.process(t);

		ASSERT_EQ(55, t.x);
		ASSERT_EQ(77, t.y);
	}
}


//------------------------------------------------------------------------------
//## デフォルト値を指定して Load する
// ユーザーオブジェクトのデフォルト値は対応しない。というか、RefObject のサブクラスでそれやろうとすると
// コピー禁止 (コピーコンストラクタ delete) が邪魔する。
#if 0
TEST_F(Test_Serialization2, DefaultValue)
{
	class DefaultTest1
	{
	public:
		int x = 1;
		int y = 2;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
			ar & LN_NVP(y);
		}
	};
	class DefaultTest2
	{
	public:
		int x = 10;
		int z = 20;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
			ar & LN_NVP(z, 55);
		}
	};

	String json;

	//- [ ]  Save
	{
		JsonTextOutputArchive ar;
		DefaultTest1 t;
		ar.process(t);
		json = ar.toString(JsonFormatting::None);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);
		DefaultTest2 t;
		ar.process(t);
		ASSERT_EQ(1, t.x);
		ASSERT_EQ(55, t.z);
	}
}
#endif

//------------------------------------------------------------------------------
//## [Issue] 初期状態 reserve で確保されるメモリを超えると serialize 中に落ちる問題
TEST_F(Test_Serialization2, ManyData)
{
	struct Test2
	{
		int x, y, z;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
			ar & LN_NVP(y);
			ar & LN_NVP(z);
		}
	};
	struct Test1
	{
		List<Test2> list;

		void serialize(Archive& ar)
		{
			ar & LN_NVP(list);
		}
	};
	Test1 t1;
	for (int i = 0; i < 100; i++) {
		t1.list.add(Test2{i, i * 10, i * 100});
	}

	String json = JsonSerializer::serialize(t1, JsonFormatting::None);
	ASSERT_EQ(true, json.contains(_TT("\"x\":99")));	// 最後の値が出ていればOKとする
}


//------------------------------------------------------------------------------
//## NaN and Inf
TEST_F(Test_Serialization2, NaN_Inf)
{
    struct Test
    {
        float x, y;

        void serialize(Archive& ar)
        {
            ar & LN_NVP(x);
            ar & LN_NVP(y);
        }
    };

    Test t1;
    t1.x = Math::NaN;
    t1.y = Math::Inf;

    String json = JsonSerializer::serialize(t1, JsonFormatting::None);

    Test t2;
    JsonSerializer::deserialize(json, t2);

    ASSERT_EQ(true, Math::isNaN(t2.x));
    ASSERT_EQ(true, Math::isInf(t2.y));
}
