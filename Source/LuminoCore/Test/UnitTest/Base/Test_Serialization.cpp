#include <TestConfig.h>
#include <Lumino/Base/Serialization.h>
#include <Lumino/Json/JsonDocument.h>
#include <Lumino/Reflection/ReflectionObject.h>


struct TkMVSoundData
{
	ln::String		name;
	int				volume;

	void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("name", name);
		ar & ln::tr::MakeNVP("volume", volume);
	}
};

struct TkMVMapData1
{
	bool			autoplayBgm;
	TkMVSoundData	bgm;
	ln::String		displayName;

	void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("autoplayBgm", autoplayBgm);
		ar & ln::tr::MakeNVP("bgm", bgm);
		ar & ln::tr::MakeNVP("displayName", displayName);
	}
};

class TkMVSoundData3 : public ln::Object
{
public:
	ln::String		name;
	int				volume;

	virtual void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("name", name);
		ar & ln::tr::MakeNVP("volume", volume);
	}
};

class TkMVEventData3 : public ln::Object
{
public:
	ln::String		id;

	virtual void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("id", id);
	}
};

struct TkMVMapData3 : public ln::Object
{
	bool							autoplayBgm;
	RefPtr<TkMVSoundData3>			bgm;
	ln::String						displayName;
	List<RefPtr<TkMVEventData3>>	events;

	void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("autoplayBgm", autoplayBgm);
		ar & ln::tr::MakeNVP("bgm", bgm);
		ar & ln::tr::MakeNVP("displayName", displayName);
		ar & ln::tr::MakeNVP("events", events);
	}
};

struct TkMVEventCommand3 : public ln::Object
{
	int					code;
	List<tr::ScVariant>	params;

	virtual void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("code", code);
		ar & ln::tr::MakeNVP("params", params);
	}
};

struct TkMVMovementData3 : public ln::Object
{
	bool	ignore;
	List<RefPtr<TkMVEventCommand3>>	commands;

	virtual void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("ignore", ignore);
		ar & ln::tr::MakeNVP("commands", commands);
	}
};

//==============================================================================
class Test_Serialization : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, SimpleStruct)
{
	ln::String json;
	// save
	{
		TkMVSoundData t1;
		t1.name = _T("test");
		t1.volume = 100;
		json = tr::JsonSerializer::Save(t1);
	}
	// load
	{
		TkMVSoundData t2 = tr::JsonSerializer::Load<TkMVSoundData>(json);
		ASSERT_EQ(_T("test"), t2.name);
		ASSERT_EQ(100, t2.volume);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, StructNested)
{
	ln::String json;
	// save
	{
		TkMVMapData1 t1;
		t1.autoplayBgm = true;
		t1.bgm.name = _T("test");
		t1.bgm.volume = 100;
		t1.displayName = _T("map1");
		json = tr::JsonSerializer::Save(t1);
	}
	// load
	{
		TkMVMapData1 t2 = tr::JsonSerializer::Load<TkMVMapData1>(json);
		ASSERT_EQ(true, t2.autoplayBgm);
		ASSERT_EQ(_T("test"), t2.bgm.name);
		ASSERT_EQ(100, t2.bgm.volume);
		ASSERT_EQ(_T("map1"), t2.displayName);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, SimpleObject)
{
	ln::String json;
	// save
	{
		auto t1 = NewObject<TkMVSoundData3>();
		t1->name = _T("test");
		t1->volume = 100;
		json = tr::JsonSerializer::Save(t1);
	}
	// load
	{
		auto t2 = tr::JsonSerializer::LoadObject<TkMVSoundData3>(json);
		ASSERT_EQ(_T("test"), t2->name);
		ASSERT_EQ(100, t2->volume);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, CommonObject)
{
	ln::String json;
	// save
	{
		auto t1 = NewObject<TkMVMapData3>();
		t1->autoplayBgm = true;
		t1->bgm = NewObject<TkMVSoundData3>();
		t1->bgm->name = _T("test");
		t1->bgm->volume = 100;
		t1->displayName = _T("map1");
		t1->events.Add(NewObject<TkMVEventData3>());
		t1->events.Add(nullptr);
		t1->events.Add(NewObject<TkMVEventData3>());
		t1->events[0]->id = _T("EV001");
		t1->events[2]->id = _T("EV003");
		json = tr::JsonSerializer::Save(t1);
	}
	// load
	{
		auto t2 = tr::JsonSerializer::LoadObject<TkMVMapData3>(json);
		ASSERT_EQ(true, t2->autoplayBgm);
		ASSERT_EQ(_T("test"), t2->bgm->name);
		ASSERT_EQ(100, t2->bgm->volume);
		ASSERT_EQ(_T("map1"), t2->displayName);
		ASSERT_EQ(_T("EV001"), t2->events[0]->id);
		ASSERT_EQ(nullptr, t2->events[1]);
		ASSERT_EQ(_T("EV003"), t2->events[2]->id);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Variant)
{
	ln::String json;
	// save
	{
		auto t1 = NewObject<TkMVMovementData3>();
		t1->ignore = true;
		{
			auto c1 = NewObject<TkMVEventCommand3>();
			c1->code = 100;
			tr::ScVariant v1;
			tr::ScVariant v2;
			v1.SetInt(20);
			v2.SetString(_T("test"));
			c1->params.Add(v1);
			c1->params.Add(v2);
			t1->commands.Add(c1);
		}
		{
			auto c1 = NewObject<TkMVEventCommand3>();
			c1->code = 200;
			tr::ScVariant v1;
			tr::ScVariant v2;
			v1.SetInt(120);
			v2.SetString(_T("test2"));
			c1->params.Add(v1);
			c1->params.Add(v2);
			t1->commands.Add(c1);
		}
		json = tr::JsonSerializer::Save(t1);
	}
	// load
	{
		auto t2 = tr::JsonSerializer::LoadObject<TkMVMovementData3>(json);
		ASSERT_EQ(true, t2->ignore);
		ASSERT_EQ(100, t2->commands[0]->code);
		ASSERT_EQ(20, t2->commands[0]->params[0].GetInt());
		ASSERT_EQ(_T("test"), t2->commands[0]->params[1].GetString());
		ASSERT_EQ(200, t2->commands[1]->code);
		ASSERT_EQ(120, t2->commands[1]->params[0].GetInt());
		ASSERT_EQ(_T("test2"), t2->commands[1]->params[1].GetString());
	}
}

#if 0
class TestObject1
{
public:

	//void lnsl_SerializeImpl(tr::Archive& ar)
	//LN_SERIALIZE(ar, version, 1)
	virtual void Serialize(tr::Archive& ar, int version)
	{
		ar & tr::MakeNVP(_T("value1"), m_value1);
	}

public:
	int		m_value1 = 100;
};

//==============================================================================
class Test_Serialization : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Save)
{
	TestObject1 t1;
	tr::JsonDocument2 doc;
	tr::Archive ar(&doc, tr::ArchiveMode::Save, false);

	ar & tr::MakeNVP(_T("obj1"), t1);
	doc.Save(TEMPFILE("json3.txt"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Load)
{
	TestObject1 t1;
	tr::JsonDocument2 doc;
	doc.Load(TEMPFILE("json3.txt"));
	tr::Archive ar(&doc, tr::ArchiveMode::Load, false);

	t1.m_value1 = 1;
	ar & tr::MakeNVP(_T("obj1"), t1);
}

//------------------------------------------------------------------------------
class TestObject2
{
public:
	//void lnsl_SerializeImpl(tr::Archive& ar)
	//LN_SERIALIZE(ar, version, 1)
	virtual void Serialize(tr::Archive& ar, int version)
	{
		ar & tr::MakeNVP(_T("value"), m_value);
	}

public:
	int m_value;
};

class TestObject3 : public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	void initialize() {}
	//void lnsl_SerializeImpl(tr::Archive& ar)
	//LN_SERIALIZE(ar, version, 1)
	virtual void Serialize(tr::Archive& ar, int version)
	{
		ver1 = version;
		ar & tr::MakeNVP(_T("value"), m_value);
	}

public:
	int m_value;
	int ver1;
};
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TestObject3, Object, tr::TypeInfo::ClassVersion<TestObject3>(1));

TEST_F(Test_Serialization, List)
{
	// <Test> write primitive type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Save, false);

		List<int>	m_list1 = { 1, 2, 3 };
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[1,2,3]}"), doc.ToString());
	}
	// <Test> write struct type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Save, false);

		List<TestObject2>	m_list1 = { TestObject2(), TestObject2(), TestObject2() };
		m_list1[0].m_value = 1;
		m_list1[1].m_value = 2;
		m_list1[2].m_value = 3;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[{\"value\":1},{\"value\":2},{\"value\":3}]}"), doc.ToString());
	}
	// <Test> write ln::Object type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Save, false);

		List <RefPtr<TestObject3>> m_list1 = { NewObject<TestObject3>(), NewObject<TestObject3>(), NewObject<TestObject3>() };
		m_list1[0]->m_value = 1;
		m_list1[1]->m_value = 2;
		m_list1[2]->m_value = 3;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		auto ss = doc.ToString();
		ASSERT_EQ(_T("{\"list1\":[{\"value\":1},{\"value\":2},{\"value\":3}]}"), doc.ToString());
	}

	// <Test> read primitive type values.
	{
		tr::JsonDocument2 doc;
		doc.Load(LN_LOCALFILE("TestData/Test_Serialization_List1.json"));
		tr::Archive ar(&doc, tr::ArchiveMode::Load, false);

		List<int>	m_list1;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(3, m_list1.GetCount());
		ASSERT_EQ(1, m_list1[0]);
		ASSERT_EQ(2, m_list1[1]);
		ASSERT_EQ(3, m_list1[2]);
	}
	// <Test> read struct type values.
	{
		tr::JsonDocument2 doc;
		doc.Load(LN_LOCALFILE("TestData/Test_Serialization_List2.json"));
		tr::Archive ar(&doc, tr::ArchiveMode::Load, false);

		List<TestObject2>	m_list1;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(3, m_list1.GetCount());
		ASSERT_EQ(5, m_list1[0].m_value);
		ASSERT_EQ(6, m_list1[1].m_value);
		ASSERT_EQ(7, m_list1[2].m_value);
	}
	// <Test> read ln::Object type values.
	{
		tr::JsonDocument2 doc;
		doc.Load(LN_LOCALFILE("TestData/Test_Serialization_List2.json"));
		tr::Archive ar(&doc, tr::ArchiveMode::Load, false);

		List <RefPtr<TestObject3>> m_list1;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(3, m_list1.GetCount());
		ASSERT_EQ(5, m_list1[0]->m_value);
		ASSERT_EQ(6, m_list1[1]->m_value);
		ASSERT_EQ(7, m_list1[2]->m_value);
	}
}

//------------------------------------------------------------------------------
//using Test_Factory = RefPtr<Object>(*)();
//
//
////template<typename T, typename isAbstract>
////static Test_Factory GetFactory();
//
//template<class TObject, class TIsAbstract> struct Test_GetFactory {};
//
//
//template<class T>
//class Test_TypeInfo;
//
//class TestObject4 : public Object
//{
//public:
//	static Test_TypeInfo<TestObject4> info;
//
//	void initialize() {}
//};
//
//template<class TObject> struct Test_GetFactory<TObject, std::false_type>
//{
//	static Test_Factory GetFactory()
//	{
//		return []() { return RefPtr<Object>::StaticCast(NewObject<TObject>()); };
//	}
//};
//template<class TObject> struct Test_GetFactory<TObject, std::true_type>
//{
//	static Test_Factory GetFactory()
//	{
//		return nullptr;
//	}
//};
//
//
//template<class T>
//class Test_TypeInfo
//{
//public:
//	Test_Factory m_factory;
//
//	Test_TypeInfo()
//		: m_factory(Test_GetFactory<T, std::is_abstract<T>::type>::GetFactory())
//	{
//		printf("aa");
//	}
//
//};
//
//
//Test_TypeInfo<TestObject4> TestObject4::info;

//template<>
//static Test_Factory GetFactory<TestObject4, std::false_type::value>()
//{
//	return nullptr;
//}
//
//template<>
//static Test_Factory GetFactory<TestObject4, std::true_type::value>()
//{
//	return nullptr;
//}


class TestObject4 : public TestObject3
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	void initialize() {}
	//LN_SERIALIZE(ar, version, 2)
	virtual void Serialize(tr::Archive& ar, int version)
	{
		ver2 = version;
		ar & tr::MakeNVPBaseObject<TestObject3>(this);
		ar & tr::MakeNVP(_T("value2"), m_value2);
	}

public:
	int m_value2;
	int ver2;
};
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TestObject4, TestObject3, tr::TypeInfo::ClassVersion<TestObject4>(2));

TEST_F(Test_Serialization, Reflection)
{
	// <Test> simply
	{
		String json;
		{
			tr::JsonDocument2 doc;
			tr::Archive ar(&doc, tr::ArchiveMode::Save, true);

			List <RefPtr<TestObject3>> m_list1 = { NewObject<TestObject3>(), NewObject<TestObject3>(), NewObject<TestObject3>() };
			m_list1[0]->m_value = 1;
			m_list1[1]->m_value = 2;
			m_list1[2]->m_value = 3;
			ar & tr::MakeNVP(_T("list1"), m_list1);

			json = doc.ToString();
		}
		{
			tr::JsonDocument2 doc;
			doc.Parse(json);
			tr::Archive ar(&doc, tr::ArchiveMode::Load, true);

			List <RefPtr<TestObject3>> m_list1;
			ar & tr::MakeNVP(_T("list1"), m_list1);

			ASSERT_EQ(3, m_list1.GetCount());
			ASSERT_EQ(1, m_list1[0]->m_value);
			ASSERT_EQ(2, m_list1[1]->m_value);
			ASSERT_EQ(3, m_list1[2]->m_value);
		}
	}

	// <Test>
	{
		String json;
		{
			tr::JsonDocument2 doc;
			tr::Archive ar(&doc, tr::ArchiveMode::Save, true);

			auto obj1 = NewObject<TestObject4>();
			auto obj2 = NewObject<TestObject4>();
			auto obj3 = NewObject<TestObject4>();
			obj1->m_value = 1; obj1->m_value2 = 4;
			obj2->m_value = 2; obj2->m_value2 = 5;
			obj3->m_value = 3; obj3->m_value2 = 6;
			List <RefPtr<TestObject3>> m_list1 = { obj1.Get(), obj2.Get(), obj3.Get() };
			ar & tr::MakeNVP(_T("list1"), m_list1);

			json = doc.ToString(tr::JsonFormatting::Indented);
		}
		{
			tr::JsonDocument2 doc;
			doc.Parse(json);
			tr::Archive ar(&doc, tr::ArchiveMode::Load, true);

			List <RefPtr<TestObject3>> m_list1;
			ar & tr::MakeNVP(_T("list1"), m_list1);

			auto* obj1 = dynamic_cast<TestObject4*>(m_list1[0].Get());
			auto* obj2 = dynamic_cast<TestObject4*>(m_list1[1].Get());
			auto* obj3 = dynamic_cast<TestObject4*>(m_list1[2].Get());
			ASSERT_EQ(3, m_list1.GetCount());
			ASSERT_EQ(1, obj1->m_value); ASSERT_EQ(1, obj1->ver1);
			ASSERT_EQ(4, obj1->m_value2); ASSERT_EQ(2, obj1->ver2);
			ASSERT_EQ(2, obj2->m_value); ASSERT_EQ(1, obj2->ver1);
			ASSERT_EQ(5, obj2->m_value2); ASSERT_EQ(2, obj2->ver2);
			ASSERT_EQ(3, obj3->m_value); ASSERT_EQ(1, obj3->ver1);
			ASSERT_EQ(6, obj3->m_value2); ASSERT_EQ(2, obj3->ver2);
		}
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Variant)
{
	String json;
	//{
	//	tr::JsonDocument2 doc;
	//	tr::Archive ar(&doc, tr::ArchiveMode::Save, true);

	//	tr::ScVariant v;
	//	v.SetInt(10);
	//	ar.Save(v);

	//	json = doc.ToString();
	//}
}


//------------------------------------------------------------------------------

struct TkMVSoundData
{
	ln::String		name;
	int				pitch;
	int				volume;

	void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("name", name);
		ar & ln::tr::MakeNVP("pitch", pitch);
		ar & ln::tr::MakeNVP("volume", volume);
	}
};

struct TkMVMapData
{
	bool			autoplayBgm;
	TkMVSoundData	bgm;
	ln::String		displayName;

	void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::MakeNVP("autoplayBgm", autoplayBgm);
		ar & ln::tr::MakeNVP("bgm", bgm);
		ar & ln::tr::MakeNVP("displayName", displayName);
	}
};

TEST_F(Test_Serialization, Raw)
{
	ln::String json;
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Load, true);

		TkMVMapData data;
		ar.Load(data);

		json = doc.ToString(tr::JsonFormatting::Indented);
	}
}

#endif
