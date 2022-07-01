#include "Common.hpp"
#include <LuminoCore/Json/JsonDocument.hpp>
#include <LuminoCore/Serialization/Serialization.hpp>
//#include <LuminoCore/Math/MathSerialization.hpp>
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
