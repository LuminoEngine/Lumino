#include "Common.hpp"
#include <Lumino/Json/JsonDocument.h>
#include <Lumino/Serialization/Serialization.hpp>
#include <Lumino/Base/Variant.hpp>
#include <Lumino/Math/Math.hpp>

//==============================================================================
//# シリアライズのテスト
class Test_Serialization2 : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

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
		data1.name = "example";
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
			String caption;
			List<Path> fileList;

			void serialize(Archive& ar)
			{
				ar & LN_NVP(caption);
				ar & LN_NVP(fileList);
			}
		};

		Documents docs1;
		docs1.caption = "note";
		docs1.fileList.add("file1.md");
		docs1.fileList.add("file2.md");

		String json = JsonSerializer::serialize(docs1, JsonFormatting::None);
		ASSERT_EQ(_T("{\"caption\":\"note\",\"fileList\":[\"file1.md\",\"file2.md\"]}"), json);


		//------ check
		Documents data2;
		JsonSerializer::deserialize(json, data2);
		ASSERT_EQ(_T("note"), data2.caption);
		ASSERT_EQ(2, data2.fileList.size());
		ASSERT_EQ(_T("file1.md"), data2.fileList[0].str());
		ASSERT_EQ(_T("file2.md"), data2.fileList[1].str());
	}
}

//------------------------------------------------------------------------------
//## Extra types
TEST_F(Test_Serialization2, ExtraTypes)
{
	//* [ ] List<> in, int, string, object
	{
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

			void serialize(Archive& ar)
			{
				ar & LN_NVP(list1);
				ar & LN_NVP(list2);
				ar & LN_NVP(list3);
			}
		};

		MyData2 data1;
		data1.list1 = {1, 2, 3};
		data1.list2 = { _T("a"), _T("b"), _T("c") };
		data1.list3 = { MyData1{1}, MyData1{2}, MyData1{3} };
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
	}
}

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Serialization2, SimpleSave)
{
	class Test1
	{
		int x = 100;
	};
	class Test2
	{
	public:
		int x = 200;
		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
		}
	};

	class EmptyTest1
	{
	public:
		void serialize(Archive& ar)
		{
		}
	};

	//- [ ] Save
	tr::JsonDocument2 doc;
	JsonArchiveStore s(&doc);
	Archive ar(&s, ArchiveMode::Save);

	Test1 t1;
	Test2 t2;
	//ar.process(LN_NVP(t1));
	ar.process(LN_NVP(t2));

	auto json = doc.toString();

	//- [ ] Load
	{
		t2.x = 1;

		tr::JsonDocument2 doc;
		doc.parse(json);
		JsonArchiveStore s(&doc);
		Archive ar(&s, ArchiveMode::Load);


		ar.process(t2);
	}

	//- [ ] 空オブジェクトの Save
	{
		tr::JsonDocument2 doc;
		JsonArchiveStore s(&doc);
		Archive ar(&s, ArchiveMode::Save);

		EmptyTest1 t1;
		ar.process(t1);

		json = doc.toString(JsonFormatting::None);
		ASSERT_EQ(_T("{}"), json);
	}

	//- [ ] 空オブジェクトの Load
	{
		tr::JsonDocument2 doc;
		doc.parse(json);
		JsonArchiveStore s(&doc);
		Archive ar(&s, ArchiveMode::Load);

		EmptyTest1 t1;
		ar.process(t1);

		json = doc.toString(JsonFormatting::None);
	}
}


/*
- [ ] List<Ref<MyObject>>
- [ ] Variant (イベントコマンド引数)
*/


//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Serialization2, EmptyContainer)
{
	class Test2
	{
	public:
		int x = 200;
		void serialize(Archive& ar)
		{
			ar & LN_NVP(x);
		}
	};
	class EmptyTest1
	{
	public:
		void serialize(Archive& ar)
		{
		}
	};
	class EmptyTest2
	{
	public:
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
//## プリミティブ型のテスト
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


namespace ln {
void serialize(Archive& ar, Variant& value)
{
	ArchiveNodeType type;
	ar.makeVariantTag(&type);

	if (ar.isSaving())
	{
		switch (value.type())
		{
			case VariantType::Null:
			{
				LN_NOTIMPLEMENTED();
				break;
			}
			case VariantType::Bool:
			{
				auto v = value.get<bool>();
				ar.process(v);
				break;
			}
			case VariantType::Char:
			{
				LN_NOTIMPLEMENTED();
				break;
			}
			case VariantType::Int8:
			{
				auto v = value.get<int8_t>();
				ar.process(v);
				break;
			}
			case VariantType::Int16:
			{
				auto v = value.get<int16_t>();
				ar.process(v);
				break;
			}
			case VariantType::Int32:
			{
				auto v = value.get<int32_t>();
				ar.process(v);
				break;
			}
			case VariantType::Int64:
			{
				auto v = value.get<int64_t>();
				ar.process(v);
				break;
			}
			case VariantType::UInt8:
			{
				auto v = value.get<uint8_t>();
				ar.process(v);
				break;
			}
			case VariantType::UInt16:
			{
				auto v = value.get<uint16_t>();
				ar.process(v);
				break;
			}
			case VariantType::UInt32:
			{
				auto v = value.get<uint32_t>();
				ar.process(v);
				break;
			}
			case VariantType::UInt64:
			{
				auto v = value.get<uint64_t>();
				ar.process(v);
				break;
			}
			case VariantType::Float:
			{
				auto v = value.get<float>();
				ar.process(v);
				break;
			}
			case VariantType::Double:
			{
				auto v = value.get<double>();
				ar.process(v);
				break;
			}
			case VariantType::String:
			{
				auto v = value.get<String>();
				ar.process(v);
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
		}
	}
	else
	{
		switch (type)
		{
			case ln::ArchiveNodeType::Null:
			{
				value.clear();
				break;
			}
			case ln::ArchiveNodeType::Bool:
			{
				bool v;
				ar.process(v);
				value = v;
				break;
			}
			case ln::ArchiveNodeType::Int64:
			{
				int64_t v;
				ar.process(v);
				value = v;
				break;
			}
			case ln::ArchiveNodeType::Double:
			{
				double v;
				ar.process(v);
				value = v;
				break;
			}
			case ln::ArchiveNodeType::String:
			{
				String v;
				ar.process(v);
				value = v;
				break;
			}
			case ln::ArchiveNodeType::Object:
			{
				LN_NOTIMPLEMENTED();
				break;
			}
			case ln::ArchiveNodeType::Array:
			{
				LN_NOTIMPLEMENTED();
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
		}
	}
}
}


//------------------------------------------------------------------------------
//## Variant serialization
TEST_F(Test_Serialization2, VariantTest)
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
		}
	};

	//{
	//	JsonTextInputArchive ar(json);
	//	Data data;
	//	ar.process(data);
	//	//ASSERT_EQ(1, t.x);
	//	//ASSERT_EQ(55, t.z);
	//}

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
		data.v_Float = 9;
		data.v_Double = 10;
		data.v_String = _T("11");
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
		ASSERT_EQ(true, Math::nearEqual(9, data.v_Float.get<float>()));
		ASSERT_EQ(true, Math::nearEqual(10, data.v_Double.get<double>()));
		ASSERT_EQ(_T("11"), data.v_String.get<String>());
	}

}
