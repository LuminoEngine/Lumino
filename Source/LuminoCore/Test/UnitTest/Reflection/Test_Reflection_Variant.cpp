#include <TestConfig.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/ReflectionArrayObject.h>
#include <Lumino/Reflection/ReflectionStruct.h>
#include <Lumino/Reflection/Variant.h>
using namespace ln::tr;



//# Variant のテスト
class Test_Variant : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//## プリミティブ数値型を扱うテスト
TEST_F(Test_Variant, PrimitiveNumericTypes)
{
	ASSERT_EQ(10, Variant((int8_t)10).get<int8_t>());
	ASSERT_EQ(10, Variant((int16_t)10).get<int16_t>());
	ASSERT_EQ(10, Variant((int32_t)10).get<int32_t>());
	ASSERT_EQ(10, Variant((int64_t)10).get<int64_t>());
	ASSERT_EQ(10, Variant((uint8_t)10).get<uint8_t>());
	ASSERT_EQ(10, Variant((uint16_t)10).get<uint16_t>());
	ASSERT_EQ(10, Variant((uint32_t)10).get<uint32_t>());
	ASSERT_EQ(10, Variant((uint64_t)10).get<uint64_t>());
	ASSERT_FLOAT_EQ(10.0f, Variant((float)10).get<float>());
	ASSERT_DOUBLE_EQ(10.0, Variant((double)10).get<double>());
}

//## String 型を扱うテスト
TEST_F(Test_Variant, StringType)
{
	Variant v1(_T("s1"));

	auto s = v1.get<String>();
	ASSERT_EQ(_T("s1"), v1.get<String>());

	v1 = _T("s2");
	ASSERT_EQ(_T("s2"), v1.get<String>());
}

////## List 型を扱うテスト
//TEST_F(Test_Variant, StringType)
//{
//}

#ifdef LN_LEGACY_VARIANT_ENABLED

// テスト用
class Point
{
public:
	int x, y;

	Point(int x_, int y_)
		: x(x_), y(y_)
	{}
};

class IntegrationTest_Reflection_Variant : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

LN_ENUM(VariantTestEnum1)
{
	Value0,
	Value1,
};
LN_ENUM_DECLARE(VariantTestEnum1);

class RefTest1 : public Object
{
};
class ArrayTest1 : public ReflectionArrayObject
{
};
struct StructTest1// : public ReflectionStruct
{
	int a, b;
};


struct TestPoint
{
	//TestPoint(int ix = 0, int iy = 0) : x(ix), y(iy) {}
	int x, y;
};
//---------------------------------------------------------------------
TEST_F(IntegrationTest_Reflection_Variant, Basic)
{
#if 0	// 速度テスト
	for (int ffi = 0; ffi < 10; ++ffi)
	{
		ln::ElapsedTimer timer;
		timer.start();
		int f = 0;
		for (int i = 0; i < 1000000; ++i)
		{
			TestPoint st;
			st.x = 10;
			st.y = 20;
			Variant a(st);
			f += Variant::Cast<TestPoint>(a).x;
		}
		printf("%d : %lld\n", f, timer.GetElapsedTimeNS());
	}
#endif
#if 0
	for (int ffi = 0; ffi < 10; ++ffi)
	{
		ln::ElapsedTimer timer;
		timer.start();
		int f = 0;
		for (int i = 0; i < 1000000; ++i)
		{
			Point st;
			st.x = 10;
			st.y = 20;
			boost::any a(st);
			f += boost::any_cast<Point>(a).x;
		}
		printf("%d : %lld\n", f, timer.GetElapsedTimeNS());
	}
#endif

	// <Test> nullptr 型
	{
		Variant v1(nullptr);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Null, v1.getType());
		ASSERT_EQ(VariantType::Null, v2.getType());
        ASSERT_EQ(nullptr, Variant::cast<std::nullptr_t>(v1));
		ASSERT_EQ(nullptr, Variant::cast<RefTest1*>(v2));	// Object で cast すると nullptr が返ってくる
		ASSERT_EQ(nullptr, Variant::cast<ArrayTest1*>(v2));	// ReflectionArrayObject で cast すると nullptr が返ってくる
		ASSERT_EQ(NULL, Variant::cast<RefTest1*>(v2));
	}
	// <Test> Bool 型
	{
		Variant v1(true);
		Variant v2(false);
		Variant v3 = v2;	// copy
		ASSERT_EQ(VariantType::Bool, v1.getType());
		ASSERT_EQ(VariantType::Bool, v2.getType());
		ASSERT_EQ(VariantType::Bool, v3.getType());
		ASSERT_EQ(true, Variant::cast<bool>(v1));
		ASSERT_EQ(false, Variant::cast<bool>(v2));
		ASSERT_EQ(false, Variant::cast<bool>(v3));
	}
	// <Test> int 型
	{
		int a = 100;
		Variant v1(a);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Int32, v1.getType());
		ASSERT_EQ(VariantType::Int32, v2.getType());
		ASSERT_EQ(100, Variant::cast<int>(v1));
		ASSERT_EQ(100, Variant::cast<int32_t>(v2));		// 算術型ならキャストできる
	}
	// <Test> uint32_t 型
	{
		uint32_t a = 100;
		Variant v1(a);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::UInt32, v1.getType());
		ASSERT_EQ(VariantType::UInt32, v2.getType());
		ASSERT_EQ(100, Variant::cast<uint32_t>(v1));
		ASSERT_EQ(100, Variant::cast<int32_t>(v2));		// 算術型ならキャストできる
	}
	// <Test> float 型
	{
		float a = 100.0f;
		Variant v1(a);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Float, v1.getType());
		ASSERT_EQ(VariantType::Float, v2.getType());
		ASSERT_EQ(100, Variant::cast<float>(v1));
		ASSERT_EQ(100, Variant::cast<int>(v2));			// 算術型ならキャストできる
	}
	// <Test> double 型
	{
		double a = 100.0;
		Variant v1(a);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Double, v1.getType());
		ASSERT_EQ(VariantType::Double, v2.getType());
		ASSERT_EQ(100, Variant::cast<double>(v1));
		ASSERT_EQ(100, Variant::cast<float>(v2));		// 算術型ならキャストできる
	}
	// <Test> Char* 型
	{
		Variant v1(_T("str"));
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::String, v1.getType());
		ASSERT_EQ(VariantType::String, v2.getType());
		ASSERT_EQ(_T("str"), Variant::cast<String>(v1));
		ASSERT_EQ(_T("str"), Variant::cast<String>(v2));
	}
	// <Test> String 型
	{
		Variant v1(String(_T("str")));
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::String, v1.getType());
		ASSERT_EQ(VariantType::String, v2.getType());
		ASSERT_EQ(_T("str"), Variant::cast<String>(v1));
		ASSERT_EQ(_T("str"), Variant::cast<String>(v2));
		//ASSERT_EQ(_T("str"), Variant::cast<const String&>(v2));	// 参照文字列型はコンパイルエラーにする (static_assert)
	}
	// <Test> Enum 型
	{
		VariantTestEnum1 e1 = VariantTestEnum1::Value1;
		Variant v1(e1);							// Enum クラス派生
		Variant v2(VariantTestEnum1::Value0);	// enum 直値指定
		Variant v3 = v2;	// copy
		ASSERT_EQ(VariantType::Enum, v1.getType());
		ASSERT_EQ(VariantType::Enum, v2.getType());
		ASSERT_EQ(VariantType::Enum, v3.getType());
		ASSERT_EQ(VariantTestEnum1::Value1, Variant::cast<VariantTestEnum1>(v1));
		ASSERT_EQ(VariantTestEnum1::Value0, Variant::cast<VariantTestEnum1>(v2));
		ASSERT_EQ(VariantTestEnum1::Value0, Variant::cast<VariantTestEnum1>(v3));
	}
	// <Test> 構造体 (POD)
	{
		StructTest1 st;
		st.a = 10; st.b = 20;
		Variant v1(st);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Struct, v1.getType());
		ASSERT_EQ(VariantType::Struct, v2.getType());
		ASSERT_EQ(10, Variant::cast<StructTest1>(v1).a);
		ASSERT_EQ(20, Variant::cast<StructTest1>(v2).b);
		ASSERT_EQ(10, Variant::cast<const StructTest1&>(v2).a);	// 参照で取り出せる
	}
	// <Test> 構造体 (非POD)
	{
		Point st(10, 20);
		Variant v1(st);
		Variant v2 = v1;	// copy
		Variant v3(Point(30, 40));
		ASSERT_EQ(VariantType::Struct, v1.getType());
		ASSERT_EQ(VariantType::Struct, v2.getType());
		ASSERT_EQ(VariantType::Struct, v3.getType());
		ASSERT_EQ(10, Variant::cast<Point>(v1).x);
		ASSERT_EQ(20, Variant::cast<Point>(v2).y);
		ASSERT_EQ(10, Variant::cast<const Point&>(v2).x);	// 参照で取り出せる
		ASSERT_EQ(30, Variant::cast<Point>(v3).x);
	}
	// <Test> Object 型
	{
		RefTest1* obj1 = new RefTest1();
		Variant v1(obj1);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Object, v1.getType());
		ASSERT_EQ(VariantType::Object, v2.getType());
		ASSERT_EQ(obj1, Variant::cast<RefTest1*>(v1));
		ASSERT_EQ(obj1, Variant::cast<Object*>(v1));
		ASSERT_EQ(obj1, Variant::cast<RefTest1*>(v2));
		obj1->release();
	}
	// <Test> Ref 型
	{
		Ref<RefTest1> obj1(new RefTest1(), false);
		Variant v1(obj1);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Object, v1.getType());
		ASSERT_EQ(VariantType::Object, v2.getType());
		ASSERT_EQ(obj1.get(), Variant::cast<RefTest1*>(v1));
		ASSERT_EQ(obj1.get(), Variant::cast<Object*>(v1));
		ASSERT_EQ(obj1.get(), Variant::cast<RefTest1*>(v2));
		ASSERT_EQ(obj1, Variant::cast<Ref<RefTest1>>(v2));
	}
	// <Test> ReflectionArrayObject 型
	{
		ReflectionArrayObject* obj1 = new ReflectionArrayObject();
		Variant v1(obj1);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::ArrayObject, v1.getType());
		ASSERT_EQ(VariantType::ArrayObject, v2.getType());
		ASSERT_EQ(obj1, Variant::cast<ArrayTest1*>(v1));
		ASSERT_EQ(obj1, Variant::cast<ReflectionArrayObject*>(v1));
		ASSERT_EQ(obj1, Variant::cast<Object*>(v1));
		ASSERT_EQ(obj1, Variant::cast<ArrayTest1*>(v2));
		obj1->release();
	}
}

#endif

