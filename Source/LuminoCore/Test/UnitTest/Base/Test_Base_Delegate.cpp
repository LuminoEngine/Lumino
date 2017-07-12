#include <TestConfig.h>
#include <functional>
#include <Lumino/Base/Delegate.h>

#ifdef LN_CPP11



static int g_Value = 0;

//==============================================================================
class Test_Base_Delegate : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
	}
	virtual void TearDown() {}

	// テスト用クラス
	class Class1
	{
	public:
		int	m_value = 0;

		static void Func1() { g_Value += 1; }
		void Func2() { m_value += 10; }
		void Func3() { m_value += 100; }
	};
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Delegate, Constructor)
{
	g_Value = 0;
	Class1 t;

	// static 関数
	Delegate<void()> d1(&Class1::Func1);
	d1();
	ASSERT_EQ(1, g_Value);

	// メンバ関数
	Delegate<void()> d2(&t, &Class1::Func2);
	d2();
	ASSERT_EQ(10, t.m_value);

	// ラムダ式 (static)
	Delegate<void()> d3([]() { g_Value++; });
	d3();
	ASSERT_EQ(2, g_Value);

	// ラムダ式 (func object)
	Delegate<void()> d4([&t]() { t.Func2(); });
	d4();
	ASSERT_EQ(20, t.m_value);

	// コピー
	Delegate<void()> d5 = d1;
	d5();
	ASSERT_EQ(3, g_Value);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Delegate, isEmpty)
{
	Delegate<void()> d1;
	ASSERT_TRUE(d1.isEmpty());

	Delegate<void()> d2(&Class1::Func1);
	ASSERT_FALSE(d2.isEmpty());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Delegate, Operator_Assign)
{
	g_Value = 0;
	Class1 t;

	// static 関数
	Delegate<void()> d1;
	d1 = Class1::Func1;
	d1();
	ASSERT_EQ(1, g_Value);

	// ラムダ式 (static)
	Delegate<void()> d2;
	d2 = []() { g_Value++; };
	d2();
	ASSERT_EQ(2, g_Value);

	// ラムダ式 (func object)
	Delegate<void()> d3;
	d3 = [&t]() { t.Func2(); };
	d3();
	ASSERT_EQ(10, t.m_value);

	// コピー
	Delegate<void()> d4;
	d4 = d1;
	d4();
	ASSERT_EQ(3, g_Value);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Delegate, Operator_Eq)
{
	Class1 class1, class2;
	Delegate<void()> d1(&class1, &Class1::Func2);
	Delegate<void()> d2(&class1, &Class1::Func2);
	Delegate<void()> d3(&class2, &Class1::Func2);
	Delegate<void()> d4(&class1, &Class1::Func3);
	Delegate<void()> d5, d6;
	ASSERT_TRUE(d1 == d2);		// 同一
	ASSERT_FALSE(d1 == d3);		// インスタンスが違う
	ASSERT_FALSE(d1 == d4);		// メンバ関数が違う
	ASSERT_FALSE(d1 == d5);		// 片方が空
	ASSERT_TRUE(d5 == d6);		// 空で同一
	ASSERT_TRUE(d5 == nullptr);	// nullptr
}


//------------------------------------------------------------------------------
TEST_F(Test_Base_Delegate, createDelegate)
{
	g_Value = 0;
	Class1 t;
	Delegate<void()> d1 = createDelegate(&Class1::Func1);
	Delegate<void()> d2 = createDelegate(&t, &Class1::Func2);
	//Delegate<void()> d3 = CreateDelegate([&t](){ t.m_value = 100; });
	//Delegate<void()> d4 = CreateDelegate([](){ g_Value = 100; });
}

#else

static int g_Value = 0;

class Test_Base_Delegate : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

	// テスト用クラス
	class Class1
	{
	public:

		void Callback01_1(int v) { g_Value += v; }
		void Callback01_2(int v) { g_Value += v * 10; }

		void Callback02_1(int v1, int v2) { g_Value += v1; }
		void Callback02_2(int v1, int v2) { g_Value += v1 * 10; }

		void Callback03_1(int v1, int v2, int v3) { g_Value += v1; }
		void Callback03_2(int v1, int v2, int v3) { g_Value += v1 * 10; }

		void Callback04_1(int v1, int v2, int v3, int v4) { g_Value += v1; }
		void Callback04_2(int v1, int v2, int v3, int v4) { g_Value += v1 * 10; }

	};

};




//---------------------------------------------------------------------

class Test1		// GCC (Cygwin?) では関数内ローカルクラスのメンバ関数ポインタを取ろうとするとコンパイルエラーになるので、外に置いておく
{
public:

	int* mValue;

	void Callback1()
	{
		*mValue = 10;
	}
	void Callback2()
	{
		*mValue = 20;
	}
	static void LN_STDCALL Callback4()
	{
		g_Value = 40;
	}
	static void Callback5()
	{
		printf("5\n");
	}
} ;
	
TEST_F(Test_Base_Delegate, Delegate00)
{

	int value = 0;

	Test1 t1;

	t1.mValue = &value;


	Delegate<void()> dd1(&Test1::Callback5);
	Delegate<void()> dd2(&t1, &Test1::Callback1);
	Delegate<void()> dd3([&t1]() { g_Value = 500; });
	dd1();
	dd2();
	dd3();



	// IsEmpty
	Delegate00 delegate0;
	ASSERT_TRUE(delegate0.IsEmpty());

	// Call
	Delegate00 delegate1(&t1, &Test1::Callback1);
	delegate1.Call();
	ASSERT_EQ(10, value);
	ASSERT_FALSE(delegate1.IsEmpty());

	// oeprator ()()
	Delegate00 delegate2(&t1, &Test1::Callback2);
	delegate2();
	ASSERT_EQ(20, value);

	// oeprator =
	Delegate00 delegate3;
	delegate3 = delegate1;
	delegate3();
	ASSERT_EQ(10, value);

	// static method
	Delegate00 delegate4(&Test1::Callback4);
	delegate4();
	ASSERT_EQ(40, g_Value);

	// copy
	Delegate00 delegate5(delegate2);
	delegate5();
	ASSERT_EQ(20, value);
}

//---------------------------------------------------------------------

class Test_Delegate01
{
public:
	int* mValue;

	void Callback1(int a)
	{
		*mValue = a;
	}
	static void LN_STDCALL Callback2(int a)
	{
		g_Value = a + 10;
	}
};
	
//---------------------------------------------------------------------
TEST_F(Test_Base_Delegate, Delegate01)
{
	int value = 0;

	Test_Delegate01 t1;
	t1.mValue = &value;

	// デフォルトコンストラクタ
	// IsEmpty
	Delegate01<int> d1;
	ASSERT_TRUE(d1.IsEmpty());

	// コンストラクタ (インスタンスメソッド)
	// Call
	Delegate01<int> d2(&t1, &Test_Delegate01::Callback1);
	d2.Call(1);
	ASSERT_FALSE(d2.IsEmpty());
	ASSERT_EQ(1, value);

	// コンストラクタ (static メソッド)
	// oeprator ()()
	Delegate01<int> d3(&Test_Delegate01::Callback2);
	d3(2);
	ASSERT_EQ(12, g_Value);

	// copy
	Delegate01<int> d4(d2);
	d4(5);
	ASSERT_EQ(5, value);

	// oeprator =
	Delegate01<int> d5;
	d5 = d3;
	d5(6);
	ASSERT_EQ(16, g_Value);
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Delegate, Equals)
{
	Class1 class1, class2;
	Delegate01<int>	d1 = LN_CreateDelegate(&class1, &Class1::Callback01_1);
	Delegate01<int>	d2 = LN_CreateDelegate(&class1, &Class1::Callback01_1);
	Delegate01<int>	d3 = LN_CreateDelegate(&class2, &Class1::Callback01_1);
	Delegate01<int>	d4 = LN_CreateDelegate(&class1, &Class1::Callback01_2);
	ASSERT_TRUE(d1 == d2);	// true
	ASSERT_FALSE(d1 == d3);	// false
	ASSERT_FALSE(d1 == d4);	// false
}

#endif
