#include <TestConfig.h>
#include <Lumino/Base/Event.h>

#ifdef LN_CPP11

static int g_Value = 0;

class Test_Base_Event : public ::testing::Test
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
TEST_F(Test_Base_Event, AddHandler)
{
	Class1 c1;

	Event<void(int)> ev01;
	ev01.AddHandler(CreateDelegate(&c1, &Class1::Callback01_1));

	Event<void(int, int)> ev02;
	ev02.AddHandler(CreateDelegate(&c1, &Class1::Callback02_1));

	Event<void(int, int, int)> ev03;
	ev03.AddHandler(CreateDelegate(&c1, &Class1::Callback03_1));

	Event<void(int, int, int, int)> ev04;
	ev04.AddHandler(CreateDelegate(&c1, &Class1::Callback04_1));
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Event, RemoveHandler)
{
	// <TEST> 同一 delegate だけ除外できること。
	{
		Class1 c1;
		Event<void(int)> ev01;
		ev01.AddHandler(CreateDelegate(&c1, &Class1::Callback01_1));
		ASSERT_FALSE(ev01.IsEmpty());
		ev01.RemoveHandler(CreateDelegate(&c1, &Class1::Callback01_2));
		ASSERT_FALSE(ev01.IsEmpty());	// 違う関数なので remove できない
		ev01.RemoveHandler(CreateDelegate(&c1, &Class1::Callback01_1));
		ASSERT_TRUE(ev01.IsEmpty());	// 同じ関数なので remove できる
	}

	// <TEST> 同一 delegate だけ除外できること。
	{
		Class1 c1;
		Class1 c2;
		Event<void(int, int)> ev02;
		ev02.AddHandler(CreateDelegate(&c1, &Class1::Callback02_1));
		ASSERT_FALSE(ev02.IsEmpty());
		ev02.RemoveHandler(CreateDelegate(&c2, &Class1::Callback02_1));
		ASSERT_FALSE(ev02.IsEmpty());	// 違うインスタンスなので remove できない
		ev02.RemoveHandler(CreateDelegate(&c1, &Class1::Callback02_1));
		ASSERT_TRUE(ev02.IsEmpty());	// 同じインスタンスなので remove できる
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Event, Raise)
{
	g_Value = 0;
	Class1 c1;
	Event<void(int)> ev01;

	// <TEST> 1つのイベントハンドラを呼び出せること。
	{
		ev01 += CreateDelegate(&c1, &Class1::Callback01_1);
		ev01.Raise(2);
		ASSERT_EQ(2, g_Value);
	}

	// <TEST> 複数のイベントハンドラを呼び出せること。
	{
		ev01 += CreateDelegate(&c1, &Class1::Callback01_2);
		ev01(2);	// operator() で呼んでみる。
		ASSERT_EQ(24, g_Value);
	}

	// <TEST> イベントハンドラを1つ除外した後、残りを呼び出せること。
	{
		ev01 -= CreateDelegate(&c1, &Class1::Callback01_1);
		ev01.Raise(2);
		ASSERT_EQ(44, g_Value);
	}
}
#else
static int g_Value = 0;

class Test_Base_Event : public ::testing::Test
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
TEST_F(Test_Base_Event, AddHandler)
{
	Class1 c1;

	Event01<int> ev01;
	ev01.AddHandler(LN_CreateDelegate(&c1, &Class1::Callback01_1));

	Event02<int, int> ev02;
	ev02.AddHandler(LN_CreateDelegate(&c1, &Class1::Callback02_1));

	Event03<int, int, int> ev03;
	ev03.AddHandler(LN_CreateDelegate(&c1, &Class1::Callback03_1));

	Event04<int, int, int, int> ev04;
	ev04.AddHandler(LN_CreateDelegate(&c1, &Class1::Callback04_1));
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Event, RemoveHandler)
{
	// <TEST> 同一 delegate だけ除外できること。
	{
		Class1 c1;
		Event01<int> ev01;
		ev01.AddHandler(LN_CreateDelegate(&c1, &Class1::Callback01_1));
		ASSERT_FALSE(ev01.IsEmpty());
		ev01.RemoveHandler(LN_CreateDelegate(&c1, &Class1::Callback01_2));
		ASSERT_FALSE(ev01.IsEmpty());	// 違う関数なので remove できない
		ev01.RemoveHandler(LN_CreateDelegate(&c1, &Class1::Callback01_1));
		ASSERT_TRUE(ev01.IsEmpty());	// 同じ関数なので remove できる
	}

	// <TEST> 同一 delegate だけ除外できること。
	{
		Class1 c1;
		Class1 c2;
		Event02<int, int> ev02;
		ev02.AddHandler(LN_CreateDelegate(&c1, &Class1::Callback02_1));
		ASSERT_FALSE(ev02.IsEmpty());
		ev02.RemoveHandler(LN_CreateDelegate(&c2, &Class1::Callback02_1));
		ASSERT_FALSE(ev02.IsEmpty());	// 違うインスタンスなので remove できない
		ev02.RemoveHandler(LN_CreateDelegate(&c1, &Class1::Callback02_1));
		ASSERT_TRUE(ev02.IsEmpty());	// 同じインスタンスなので remove できる
	}
}

//---------------------------------------------------------------------
TEST_F(Test_Base_Event, Raise)
{
	g_Value = 0;
	Class1 c1;
	Event01<int> ev01;

	// <TEST> 1つのイベントハンドラを呼び出せること。
	{
		ev01 += LN_CreateDelegate(&c1, &Class1::Callback01_1);
		ev01.Raise(2);
		ASSERT_EQ(2, g_Value);
	}

	// <TEST> 複数のイベントハンドラを呼び出せること。
	{
		ev01 += LN_CreateDelegate(&c1, &Class1::Callback01_2);
		ev01(2);	// operator() で呼んでみる。
		ASSERT_EQ(24, g_Value);
	}

	// <TEST> イベントハンドラを1つ除外した後、残りを呼び出せること。
	{
		ev01 -= LN_CreateDelegate(&c1, &Class1::Callback01_1);
		ev01.Raise(2);
		ASSERT_EQ(44, g_Value);
	}
}
#endif
