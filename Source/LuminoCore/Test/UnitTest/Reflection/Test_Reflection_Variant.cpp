#include <TestConfig.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/ReflectionArrayObject.h>
#include <Lumino/Reflection/ReflectionStruct.h>
#include <Lumino/Reflection/Variant.h>
using namespace ln::tr;

LN_NAMESPACE_BEGIN
namespace detail { class VariantHelper; }
class Variant;

enum class VariantType
{
	Null,
	Bool,
	Char,	// ln::Char
	Int8,
	Int16,
	Int32,
	Int64,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Float,
	Double,
	String,
};

class Variant
{
public:
	Variant();
	Variant(bool value);
	Variant(Char value);
	Variant(int8_t value);
	Variant(int16_t value);
	Variant(int32_t value);
	Variant(int64_t value);
	Variant(uint8_t value);
	Variant(uint16_t value);
	Variant(uint32_t value);
	Variant(uint64_t value);
	Variant(float value);
	Variant(double value);
	Variant(const Char* value);
	Variant(const String& value);
	Variant(const Variant& value);
	~Variant();

	void clear() LN_NOEXCEPT;

	VariantType type() const { return m_type; }

	template<typename TValue>
	TValue get() const;

	Variant& operator=(const Variant& rhs);

private:
	void assign(bool value);
	void assign(Char value);
	void assign(int8_t value);
	void assign(int16_t value);
	void assign(int32_t value);
	void assign(int64_t value);
	void assign(uint8_t value);
	void assign(uint16_t value);
	void assign(uint32_t value);
	void assign(uint64_t value);
	void assign(float value);
	void assign(double value);
	void assign(const String& value);
	void copy(const Variant& value);

	VariantType	m_type;

	union
	{
		bool v_Bool;
		Char v_Char;
		int8_t v_Int8;
		int16_t v_Int16;
		int32_t v_Int32;
		int64_t v_Int64;
		uint8_t v_UInt8;
		uint16_t v_UInt16;
		uint32_t v_UInt32;
		uint64_t v_UInt64;
		float v_Float;
		double v_Double;
		String v_String;
	};

	friend class detail::VariantHelper;
};

Variant::Variant()
	: m_type(VariantType::Null)
{}

Variant::Variant(bool value)
	: m_type(VariantType::Bool)
	, v_Bool(value)
{
}

Variant::Variant(Char value)
	: m_type(VariantType::Char)
	, v_Char(value)
{
}

Variant::Variant(int8_t value)
	: m_type(VariantType::Int8)
	, v_Int8(value)
{
}

Variant::Variant(int16_t value)
	: m_type(VariantType::Int64)
	, v_Int64(value)
{
}

Variant::Variant(int32_t value)
	: m_type(VariantType::Int32)
	, v_Int32(value)
{
}

Variant::Variant(int64_t value)
	: m_type(VariantType::Int64)
	, v_Int64(value)
{
}

Variant::Variant(uint8_t value)
	: m_type(VariantType::UInt8)
	, v_UInt8(value)
{
}

Variant::Variant(uint16_t value)
	: m_type(VariantType::UInt16)
	, v_UInt16(value)
{
}

Variant::Variant(uint32_t value)
	: m_type(VariantType::UInt32)
	, v_UInt32(value)
{
}

Variant::Variant(uint64_t value)
	: m_type(VariantType::UInt64)
	, v_UInt64(value)
{
}

Variant::Variant(float value)
	: m_type(VariantType::Float)
	, v_Float(value)
{
}

Variant::Variant(double value)
	: m_type(VariantType::Double)
	, v_Double(value)
{
}

Variant::Variant(const Char* value)
	: Variant(String(value))
{
}

Variant::Variant(const String& value)
	: m_type(VariantType::String)
	, v_String(value)
{
}

Variant::Variant(const Variant& value)
	: m_type(VariantType::Null)
{
	copy(value);
}

Variant::~Variant()
{
	clear();
}

void Variant::clear() LN_NOEXCEPT
{
	switch (m_type)
	{
	case VariantType::Null:
	case VariantType::Bool:
	case VariantType::Char:
	case VariantType::Int8:
	case VariantType::Int16:
	case VariantType::Int32:
	case VariantType::Int64:
	case VariantType::UInt8:
	case VariantType::UInt16:
	case VariantType::UInt32:
	case VariantType::UInt64:
	case VariantType::Float:
	case VariantType::Double:
		break;
	case VariantType::String:
		v_String.~String();
		break;
	default:
		assert(0);
		break;
	}

	m_type = VariantType::Null;
}

Variant& Variant::operator=(const Variant& rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}

void Variant::assign(bool value)
{
	m_type = VariantType::Bool;
	v_Bool = value;
}

void Variant::assign(Char value)
{
	m_type = VariantType::Char;
	v_Char = value;
}

void Variant::assign(int8_t value)
{
	m_type = VariantType::Int8;
	v_Int8 = value;
}

void Variant::assign(int16_t value)
{
	m_type = VariantType::Int16;
	v_Int16 = value;
}

void Variant::assign(int32_t value)
{
	m_type = VariantType::Int32;
	v_Int32 = value;
}

void Variant::assign(int64_t value)
{
	m_type = VariantType::Int64;
	v_Int64 = value;
}

void Variant::assign(uint8_t value)
{
	m_type = VariantType::UInt8;
	v_UInt8 = value;
}

void Variant::assign(uint16_t value)
{
	m_type = VariantType::UInt16;
	v_UInt16 = value;
}

void Variant::assign(uint32_t value)
{
	m_type = VariantType::UInt32;
	v_UInt32 = value;
}

void Variant::assign(uint64_t value)
{
	m_type = VariantType::UInt64;
	v_UInt64 = value;
}

void Variant::assign(float value)
{
	m_type = VariantType::Float;
	v_Float = value;
}

void Variant::assign(double value)
{
	m_type = VariantType::Double;
	v_Double = value;
}

void Variant::assign(const String& value)
{
	m_type = VariantType::String;
	v_String = value;
}

void Variant::copy(const Variant& value)
{
	switch (value.m_type)
	{
	case VariantType::Null:
		break;
	case VariantType::Bool:
		assign(value.v_Bool);
		break;
	case VariantType::Char:
		assign(value.v_Char);
		break;
	case VariantType::Int8:
		assign(value.v_Int8);
		break;
	case VariantType::Int16:
		assign(value.v_Int16);
		break;
	case VariantType::Int32:
		assign(value.v_Int32);
		break;
	case VariantType::Int64:
		assign(value.v_Int64);
		break;
	case VariantType::UInt8:
		assign(value.v_UInt8);
		break;
	case VariantType::UInt16:
		assign(value.v_UInt16);
		break;
	case VariantType::UInt32:
		assign(value.v_UInt32);
		break;
	case VariantType::UInt64:
		assign(value.v_UInt64);
		break;
	case VariantType::Float:
		assign(value.v_Float);
		break;
	case VariantType::Double:
		assign(value.v_Double);
		break;
	case VariantType::String:
		assign(value.v_String);
		break;
	default:
		assert(0);
		break;
	}
}

namespace detail
{
class VariantHelper
{
public:
	static bool canConvertToNumeric(const Variant& value);

	template<typename T>
	static T convertToNumeric(const Variant& value);


	//static bool getRawBool(const Variant& value) { return value.v_Bool; }
	//static Char getRawBool(const Variant& value) { return value.v_Char; }
	//static int8_t getRawBool(const Variant& value) { return value.v_Int8; }
	//static int16_t getRawBool(const Variant& value) { return value.v_Int16; }
	//static int32_t getRawBool(const Variant& value) { return value.v_Int32; }
	//static int64_t getRawBool(const Variant& value) { return value.v_Int64; }
	//static uint8_t getRawBool(const Variant& value) { return value.v_UInt8; }
	//static uint16_t getRawBool(const Variant& value) { return value.v_UInt16; }
	//static uint32_t getRawBool(const Variant& value) { return value.v_UInt32; }
	//static uint64_t getRawBool(const Variant& value) { return value.v_UInt64; }
	//static float getRawBool(const Variant& value) { return value.v_Float; }
	//static double getRawBool(const Variant& value) { return value.v_Double; }
	static const String& getRawString(const Variant& value) { return value.v_String; }
};

bool VariantHelper::canConvertToNumeric(const Variant& value)
{
	switch (value.type())
	{
	case VariantType::Int8:
	case VariantType::Int16:
	case VariantType::Int32:
	case VariantType::Int64:
	case VariantType::UInt8:
	case VariantType::UInt16:
	case VariantType::UInt32:
	case VariantType::UInt64:
	case VariantType::Float:
	case VariantType::Double:
		return true;
	default:
		return false;
	}
}

template<typename T>
T VariantHelper::convertToNumeric(const Variant& value)
{
	switch (value.type())
	{
	case VariantType::Int8:
		return static_cast<T>(value.v_Int8);
	case VariantType::Int16:
		return static_cast<T>(value.v_Int16);
	case VariantType::Int32:
		return static_cast<T>(value.v_Int32);
	case VariantType::Int64:
		return static_cast<T>(value.v_Int64);
	case VariantType::UInt8:
		return static_cast<T>(value.v_UInt8);
	case VariantType::UInt16:
		return static_cast<T>(value.v_UInt16);
	case VariantType::UInt32:
		return static_cast<T>(value.v_UInt32);
	case VariantType::UInt64:
		return static_cast<T>(value.v_UInt64);
	case VariantType::Float:
		return static_cast<T>(value.v_Float);
	case VariantType::Double:
		return static_cast<T>(value.v_Double);
	default:
		assert(0);
		return 0;
	}
}

} // namespace detail

template<typename TValue>
struct VariantValueTraits
{
	static bool canConvertFrom(const Variant& value) { return false; }
	static TValue convert(const Variant& value) { return TValue(); }
};

template<>
struct VariantValueTraits<int8_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int8_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int8_t>(value); }
};
template<>
struct VariantValueTraits<int16_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int16_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int16_t>(value); }
};
template<>
struct VariantValueTraits<int32_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int32_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int32_t>(value); }
};
template<>
struct VariantValueTraits<int64_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int64_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int64_t>(value); }
};
template<>
struct VariantValueTraits<uint8_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint8_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint8_t>(value); }
};
template<>
struct VariantValueTraits<uint16_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint16_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint16_t>(value); }
};
template<>
struct VariantValueTraits<uint32_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint32_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint32_t>(value); }
};
template<>
struct VariantValueTraits<uint64_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint64_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint64_t>(value); }
};
template<>
struct VariantValueTraits<float>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static float convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<float>(value); }
};
template<>
struct VariantValueTraits<double>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static double convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<double>(value); }
};
template<>
struct VariantValueTraits<String>
{
	static bool canConvertFrom(const Variant& value) { return value.type() == VariantType::String; }
	static String convert(const Variant& value) { return detail::VariantHelper::getRawString(value); }
};

template<typename TValue>
TValue Variant::get() const
{
	return VariantValueTraits<TValue>::convert(*this);
}

LN_NAMESPACE_END

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
	EXPECT_FLOAT_EQ(10.0f, Variant((float)10).get<float>());
	EXPECT_DOUBLE_EQ(10.0, Variant((double)10).get<double>());
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

class RefTest1 : public ReflectionObject
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
		ASSERT_EQ(nullptr, Variant::cast<RefTest1*>(v2));	// ReflectionObject で cast すると nullptr が返ってくる
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
	// <Test> ReflectionObject 型
	{
		RefTest1* obj1 = new RefTest1();
		Variant v1(obj1);
		Variant v2 = v1;	// copy
		ASSERT_EQ(VariantType::Object, v1.getType());
		ASSERT_EQ(VariantType::Object, v2.getType());
		ASSERT_EQ(obj1, Variant::cast<RefTest1*>(v1));
		ASSERT_EQ(obj1, Variant::cast<ReflectionObject*>(v1));
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
		ASSERT_EQ(obj1.get(), Variant::cast<ReflectionObject*>(v1));
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
		ASSERT_EQ(obj1, Variant::cast<ReflectionObject*>(v1));
		ASSERT_EQ(obj1, Variant::cast<ArrayTest1*>(v2));
		obj1->release();
	}
}

#endif

