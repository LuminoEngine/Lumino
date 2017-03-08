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






//==============================================================================
class Test_Base_EventConnection : public ::testing::Test
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


namespace ln
{
namespace detail
{
	struct EventConnectionDataBase
	{
		bool	m_active;
	};

	struct EventInternalDataBase
	{
		virtual void OnDisconnect(EventConnectionDataBase* conn) = 0;
	};
};

class EventConnection
{
public:
	EventConnection()
		: m_internalData(nullptr)
		, m_connectionData(nullptr)
	{}

	void Disconnect()
	{
		if (m_internalData != nullptr)
		{
			m_internalData->OnDisconnect(m_connectionData);
		}
	}

private:
	EventConnection(const std::shared_ptr<detail::EventInternalDataBase>& internalData, detail::EventConnectionDataBase* connectionData)
		: m_internalData(internalData)
		, m_connectionData(connectionData)
	{
		m_connectionData->m_active = true;
	}

	std::shared_ptr<detail::EventInternalDataBase>	m_internalData;
	detail::EventConnectionDataBase*				m_connectionData;

	template<typename>
	friend class Event2;
};




template<typename>
class Event2 {};
template<typename TRet, typename... TArgs>
class Event2<TRet(TArgs...)>
{
public:
	typedef Delegate<TRet(TArgs...)> DelegateType;

	Event2()
		: m_internalData(std::make_shared<EventInternalData>())
	{}

	~Event2()
	{
		Clear();
	}

	EventConnection Connect(const DelegateType& handler)
	{
		return ConnectInternal(handler);
	}

	//EventConnection Connect(const std::function<TRet(TArgs...)>& handler)
	//{
	//	return ConnectInternal(handler);
	//}

	void Clear() LN_NOEXCEPT
	{
		m_internalData->connectionDataList.Clear();
	}

	bool IsEmpty() const
	{
		return m_internalData->connectionDataList.IsEmpty();
	}

	void Raise(TArgs... args)
	{
		if (!IsEmpty())
		{
			for (auto& data : m_internalData->connectionDataList)
			{
				if (data->m_active) data->handler.Call(args...);
			}
		}
	}


	EventConnection operator += (const DelegateType& handler)
	{
		return ConnectInternal(handler);
	}

	EventConnection operator += (const std::function<TRet(TArgs...)>& handler)
	{
		return ConnectInternal(handler);
	}

	void operator () (TArgs... args)
	{
		Raise(args...);
	}

private:
	struct EventConnectionData : public detail::EventConnectionDataBase
	{
		DelegateType	handler;
	};

	struct EventInternalData : public detail::EventInternalDataBase
	{
		List<std::shared_ptr<EventConnectionData>>	connectionDataList;

		virtual void OnDisconnect(detail::EventConnectionDataBase* conn)
		{
			// check memory not released
			bool released = true;
			for (auto& data : connectionDataList)
			{
				if (data.get() == conn) released = false;
			}
			if (!released)
			{
				conn->m_active = false;
			}
		}
	};

	EventConnection ConnectInternal(const DelegateType& handler)
	{
		auto connectionData = RequestFreeConnectionData();
		connectionData->handler = handler;
		return EventConnection(m_internalData, connectionData.get());
	}

	std::shared_ptr<EventConnectionData> RequestFreeConnectionData()
	{
		for (auto& data : m_internalData->connectionDataList)
		{
			if (!data->m_active) return data;
		}

		auto newData = std::make_shared<EventConnectionData>();
		m_internalData->connectionDataList.Add(newData);
		return newData;
	}

	std::shared_ptr<EventInternalData>	m_internalData;
};



}

//---------------------------------------------------------------------
TEST_F(Test_Base_EventConnection, Basic)
{
	Class1 c1, c2;
	g_Value = 0;

	Event2<void(int)> ev01;
	ev01.Connect(CreateDelegate(&c1, &Class1::Callback01_1));
	ev01.Raise(5);
	ASSERT_EQ(5, g_Value);

	EventConnection conn = ev01.Connect(CreateDelegate(&c2, &Class1::Callback01_2));
	ev01.Raise(5);
	ASSERT_EQ(60, g_Value);

	conn.Disconnect();

	ev01.Raise(5);
	ASSERT_EQ(65, g_Value);

	// <Test> Clear 後に Dissconnetct してみる → 特に何も起こらない
	ev01.Clear();
	ev01.Raise(5);
	ASSERT_EQ(65, g_Value);
	conn.Disconnect();
}

//---------------------------------------------------------------------
TEST_F(Test_Base_EventConnection, MultiArgs)
{
	Class1 c1;
	g_Value = 0;

	Event2<void(int)> ev01;
	ev01.Connect(CreateDelegate(&c1, &Class1::Callback01_1));
	ev01.Raise(5);
	ASSERT_EQ(5, g_Value);

	Event2<void(int, int)> ev02;
	ev02.Connect(CreateDelegate(&c1, &Class1::Callback02_1));
	ev02.Raise(6, 6);
	ASSERT_EQ(11, g_Value);

	Event2<void(int, int, int)> ev03;
	ev03.Connect(CreateDelegate(&c1, &Class1::Callback03_1));
	ev03.Raise(7, 7, 7);
	ASSERT_EQ(18, g_Value);

	Event2<void(int, int, int, int)> ev04;
	ev04.Connect(CreateDelegate(&c1, &Class1::Callback04_1));
	ev04.Raise(8, 8, 8, 8);
	ASSERT_EQ(26, g_Value);
}

//---------------------------------------------------------------------
TEST_F(Test_Base_EventConnection, Raise)
{
	g_Value = 0;
	Class1 c1;
	Event2<void(int)> ev01;
	EventConnection conn;

	// <Test> 1つのイベントハンドラを呼び出せること。
	{
		conn = ev01.Connect(CreateDelegate(&c1, &Class1::Callback01_1));
		ev01.Raise(2);
		ASSERT_EQ(2, g_Value);
	}

	// <Test> 複数のイベントハンドラを呼び出せること。
	{
		ev01 += CreateDelegate(&c1, &Class1::Callback01_2);	// operator += で追加してみる
		ev01(2);	// operator() で呼んでみる。
		ASSERT_EQ(24, g_Value);
	}

	// <Test> イベントハンドラを1つ除外した後、残りを呼び出せること。
	{
		conn.Disconnect();
		ev01.Raise(2);
		ASSERT_EQ(44, g_Value);
	}
}

using TestHandler1 = Delegate<void(int)>;

//---------------------------------------------------------------------
//TEST_F(Test_Base_EventConnection, DelegateTypedef)
//{
//	g_Value = 0;
//	Event2<TestHandler1> ev01;
//	Class1 c1;
//	{
//		ev01.Connect(CreateDelegate(&c1, &Class1::Callback01_1));
//		ev01.Raise(2);
//		ASSERT_EQ(2, g_Value);
//	}
//
//}

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
