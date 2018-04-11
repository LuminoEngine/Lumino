#include "Common.hpp"
#include <Lumino/Base/Event.hpp>

static int g_Value = 0;

//==============================================================================
//# Event
class Test_Base_EventConnection : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

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

static void StaticCallback01_1(int v)
{
	g_Value += v;
}

//## static function callback
TEST_F(Test_Base_EventConnection, StaticFuncCallback)
{
	//* [ ] can add static function by connect()
	{
		g_Value = 0;
		Event<void(int)> event1;
		event1.connect(StaticCallback01_1);
		event1.raise(5);
		ASSERT_EQ(5, g_Value);
	}
	//* [ ] can add static function by +=
	{
		g_Value = 0;
		Event<void(int)> event1;
		event1 += StaticCallback01_1;
		event1.raise(5);
		ASSERT_EQ(5, g_Value);
	}
	//* [ ] can disconnect static function
	{
		g_Value = 0;
		Event<void(int)> event1;
		auto conn = event1.connect(StaticCallback01_1);
		event1.raise(5);
		conn.disconnect();
		event1.raise(5);
		ASSERT_EQ(5, g_Value);
	}
}

//## member function callback
TEST_F(Test_Base_EventConnection, Basic)
{
	Class1 c1, c2;
	Event<void(int)> ev01;

	//* [ ] can add member function by connect()
	{
		g_Value = 0;

		ev01.connect(ln::bind(&c1, &Class1::Callback01_1));
		ev01.raise(5);
		ASSERT_EQ(5, g_Value);
	}

	//* [ ] can disconnect member function
	EventConnection conn = ev01.connect(ln::bind(&c2, &Class1::Callback01_2));
	{
		ev01.raise(5);
		ASSERT_EQ(60, g_Value);

		conn.disconnect();

		ev01.raise(5);
		ASSERT_EQ(65, g_Value);
	}

	//* [ ] can disconnect member function after clear()
	{
		ev01.clear();
		ev01.raise(5);
		ASSERT_EQ(65, g_Value);
		conn.disconnect();	// 特に何も起こらない
	}
}

//## multi arguments callback
TEST_F(Test_Base_EventConnection, MultiArgs)
{
	Class1 c1;
	g_Value = 0;

	Event<void(int)> ev01;
	ev01.connect(ln::bind(&c1, &Class1::Callback01_1));
	ev01.raise(5);
	ASSERT_EQ(5, g_Value);

	Event<void(int, int)> ev02;
	ev02.connect(ln::bind(&c1, &Class1::Callback02_1));
	ev02.raise(6, 6);
	ASSERT_EQ(11, g_Value);

	Event<void(int, int, int)> ev03;
	ev03.connect(ln::bind(&c1, &Class1::Callback03_1));
	ev03.raise(7, 7, 7);
	ASSERT_EQ(18, g_Value);

	Event<void(int, int, int, int)> ev04;
	ev04.connect(ln::bind(&c1, &Class1::Callback04_1));
	ev04.raise(8, 8, 8, 8);
	ASSERT_EQ(26, g_Value);
}

//---------------------------------------------------------------------
TEST_F(Test_Base_EventConnection, raise)
{
	g_Value = 0;
	Class1 c1;
	Event<void(int)> ev01;
	EventConnection conn;

	// <Test> 1つのイベントハンドラを呼び出せること。
	{
		conn = ev01.connect(ln::bind(&c1, &Class1::Callback01_1));
		ev01.raise(2);
		ASSERT_EQ(2, g_Value);
	}

	// <Test> 複数のイベントハンドラを呼び出せること。
	{
		ev01 += ln::bind(&c1, &Class1::Callback01_2);	// operator += で追加してみる
		ev01(2);	// ln::bind() で呼んでみる。
		ASSERT_EQ(24, g_Value);
	}

	// <Test> イベントハンドラを1つ除外した後、残りを呼び出せること。
	{
		conn.disconnect();
		ev01.raise(2);
		ASSERT_EQ(44, g_Value);
	}
}
