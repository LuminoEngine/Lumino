#include <TestConfig.h>
#include <Lumino/Base/Serialization.h>
#include <Lumino/Json/JsonDocument.h>
#include <Lumino/Reflection/ReflectionObject.h>

//
//class Archive2
//{
//public:
//
//};
//
////==============================================================================
//class Test_Serialization2 : public ::testing::Test
//{
//protected:
//	virtual void SetUp() {}
//	virtual void TearDown() {}
//};
//
//TEST_F(Test_Serialization, SimpleStruct)
//{
//}
//
//
//













//==============================================================================
struct TkMVSoundData
{
	ln::String		name;
	int				volume;

	void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("name"), name);
		ar & ln::tr::makeNVP(_TT("volume"), volume);
	}
};

struct TkMVMapData1
{
	bool			autoplayBgm;
	TkMVSoundData	bgm;
	ln::String		displayName;

	void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("autoplayBgm"), autoplayBgm);
		ar & ln::tr::makeNVP(_TT("bgm"), bgm);
		ar & ln::tr::makeNVP(_TT("displayName"), displayName);
	}
};

class TkMVSoundData3 : public ln::Object
{
public:
	ln::String		name;
	int				volume;

	virtual void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("name"), name);
		ar & ln::tr::makeNVP(_TT("volume"), volume);
	}
};

class TkMVEventData3 : public ln::Object
{
public:
	ln::String		id;

	virtual void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("id"), id);
	}
};

struct TkMVMapData3 : public ln::Object
{
	bool							autoplayBgm;
	Ref<TkMVSoundData3>			bgm;
	ln::String						displayName;
	List<Ref<TkMVEventData3>>	events;

	void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("autoplayBgm"), autoplayBgm);
		ar & ln::tr::makeNVP(_TT("bgm"), bgm);
		ar & ln::tr::makeNVP(_TT("displayName"), displayName);
		ar & ln::tr::makeNVP(_TT("events"), events);
	}
};

struct TkMVEventCommand3 : public ln::Object
{
	int					code;
	List<tr::ScVariant>	params;

	virtual void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("code"), code);
		ar & ln::tr::makeNVP(_TT("params"), params);
	}
};

struct TkMVMovementData3 : public ln::Object
{
	bool	ignore;
	List<Ref<TkMVEventCommand3>>	commands;

	virtual void serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP(_TT("ignore"), ignore);
		ar & ln::tr::makeNVP(_TT("commands"), commands);
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
		t1.name = _TT("test");
		t1.volume = 100;
		json = tr::JsonSerializer::save(t1);
	}
	// load
	{
		TkMVSoundData t2 = tr::JsonSerializer::load<TkMVSoundData>(json);
		ASSERT_EQ(_TT("test"), t2.name);
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
		t1.bgm.name = _TT("test");
		t1.bgm.volume = 100;
		t1.displayName = _TT("map1");
		json = tr::JsonSerializer::save(t1);
	}
	// load
	{
		TkMVMapData1 t2 = tr::JsonSerializer::load<TkMVMapData1>(json);
		ASSERT_EQ(true, t2.autoplayBgm);
		ASSERT_EQ(_TT("test"), t2.bgm.name);
		ASSERT_EQ(100, t2.bgm.volume);
		ASSERT_EQ(_TT("map1"), t2.displayName);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, SimpleObject)
{
	ln::String json;
	// save
	{
		auto t1 = newObject<TkMVSoundData3>();
		t1->name = _TT("test");
		t1->volume = 100;
		json = tr::JsonSerializer::save(t1);
	}
	// load
	{
		auto t2 = tr::JsonSerializer::loadObject<TkMVSoundData3>(json);
		ASSERT_EQ(_TT("test"), t2->name);
		ASSERT_EQ(100, t2->volume);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, CommonObject)
{
	ln::String json;
	// save
	{
		auto t1 = newObject<TkMVMapData3>();
		t1->autoplayBgm = true;
		t1->bgm = newObject<TkMVSoundData3>();
		t1->bgm->name = _TT("test");
		t1->bgm->volume = 100;
		t1->displayName = _TT("map1");
		t1->events.add(newObject<TkMVEventData3>());
		t1->events.add(nullptr);
		t1->events.add(newObject<TkMVEventData3>());
		t1->events[0]->id = _TT("EV001");
		t1->events[2]->id = _TT("EV003");
		json = tr::JsonSerializer::save(t1);
	}
	// load
	{
		auto t2 = tr::JsonSerializer::loadObject<TkMVMapData3>(json);
		ASSERT_EQ(true, t2->autoplayBgm);
		ASSERT_EQ(_TT("test"), t2->bgm->name);
		ASSERT_EQ(100, t2->bgm->volume);
		ASSERT_EQ(_TT("map1"), t2->displayName);
		ASSERT_EQ(_TT("EV001"), t2->events[0]->id);
		ASSERT_EQ(nullptr, t2->events[1]);
		ASSERT_EQ(_TT("EV003"), t2->events[2]->id);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Variant)
{
	ln::String json;
	// save
	{
		auto t1 = newObject<TkMVMovementData3>();
		t1->ignore = true;
		{
			auto c1 = newObject<TkMVEventCommand3>();
			c1->code = 100;
			tr::ScVariant v1;
			tr::ScVariant v2;
			v1.setInt(20);
			v2.setString(_TT("test"));
			c1->params.add(v1);
			c1->params.add(v2);
			t1->commands.add(c1);
		}
		{
			auto c1 = newObject<TkMVEventCommand3>();
			c1->code = 200;
			tr::ScVariant v1;
			tr::ScVariant v2;
			v1.setInt(120);
			v2.setString(_TT("test2"));
			c1->params.add(v1);
			c1->params.add(v2);
			t1->commands.add(c1);
		}
		json = tr::JsonSerializer::save(t1);
	}
	// load
	{
		auto t2 = tr::JsonSerializer::loadObject<TkMVMovementData3>(json);
		ASSERT_EQ(true, t2->ignore);
		ASSERT_EQ(100, t2->commands[0]->code);
		ASSERT_EQ(20, t2->commands[0]->params[0].getInt());
		ASSERT_EQ(_TT("test"), t2->commands[0]->params[1].getString());
		ASSERT_EQ(200, t2->commands[1]->code);
		ASSERT_EQ(120, t2->commands[1]->params[0].getInt());
		ASSERT_EQ(_TT("test2"), t2->commands[1]->params[1].getString());
	}
}

TEST_F(Test_Serialization, PrimitiveValues)
{
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

		void serialize(tr::Archive& ar, int version)
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
		}

	} obj;

	obj.v_s8l = INT8_MIN;
	obj.v_s16l = INT16_MIN;
	obj.v_s32l = INT32_MIN;
	obj.v_s64l = INT64_MIN;
	obj.v_s8u = INT8_MAX;
	obj.v_s16u = INT16_MAX;
	obj.v_s32u = INT32_MAX;
	obj.v_s64u = INT64_MAX;
	obj.v_floatl = FLT_MIN;
	obj.v_doublel = DBL_MIN;
	obj.v_floatu = FLT_MAX;
	obj.v_doubleu = DBL_MAX;

	tr::JsonDocument2 doc;
	tr::Archive ar(&doc, tr::ArchiveMode::save, true);
	ar.save(obj);
	String text = doc.toString();

	printf("");
}

/*

```
{
	"lumino_archive_version" : 1,
	"lumino_root_object" : {
	}
}
```

*/

#if 0
class TestObject1
{
public:

	//void lnsl_SerializeImpl(tr::Archive& ar)
	//LN_SERIALIZE(ar, version, 1)
	virtual void Serialize(tr::Archive& ar, int version)
	{
		ar & tr::makeNVP(_T("value1"), m_value1);
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
TEST_F(Test_Serialization, save)
{
	TestObject1 t1;
	tr::JsonDocument2 doc;
	tr::Archive ar(&doc, tr::ArchiveMode::save, false);

	ar & tr::makeNVP(_T("obj1"), t1);
	doc.save(TEMPFILE("json3.txt"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Load)
{
	TestObject1 t1;
	tr::JsonDocument2 doc;
	doc.Load(TEMPFILE("json3.txt"));
	tr::Archive ar(&doc, tr::ArchiveMode::Load, false);

	t1.m_value1 = 1;
	ar & tr::makeNVP(_T("obj1"), t1);
}

//------------------------------------------------------------------------------
class TestObject2
{
public:
	//void lnsl_SerializeImpl(tr::Archive& ar)
	//LN_SERIALIZE(ar, version, 1)
	virtual void Serialize(tr::Archive& ar, int version)
	{
		ar & tr::makeNVP(_T("value"), m_value);
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
		ar & tr::makeNVP(_T("value"), m_value);
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
		tr::Archive ar(&doc, tr::ArchiveMode::save, false);

		List<int>	m_list1 = { 1, 2, 3 };
		ar & tr::makeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[1,2,3]}"), doc.ToString());
	}
	// <Test> write struct type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::save, false);

		List<TestObject2>	m_list1 = { TestObject2(), TestObject2(), TestObject2() };
		m_list1[0].m_value = 1;
		m_list1[1].m_value = 2;
		m_list1[2].m_value = 3;
		ar & tr::makeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[{\"value\":1},{\"value\":2},{\"value\":3}]}"), doc.ToString());
	}
	// <Test> write ln::Object type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::save, false);

		List <Ref<TestObject3>> m_list1 = { NewObject<TestObject3>(), NewObject<TestObject3>(), NewObject<TestObject3>() };
		m_list1[0]->m_value = 1;
		m_list1[1]->m_value = 2;
		m_list1[2]->m_value = 3;
		ar & tr::makeNVP(_T("list1"), m_list1);

		auto ss = doc.ToString();
		ASSERT_EQ(_T("{\"list1\":[{\"value\":1},{\"value\":2},{\"value\":3}]}"), doc.ToString());
	}

	// <Test> read primitive type values.
	{
		tr::JsonDocument2 doc;
		doc.Load(LN_LOCALFILE("TestData/Test_Serialization_List1.json"));
		tr::Archive ar(&doc, tr::ArchiveMode::Load, false);

		List<int>	m_list1;
		ar & tr::makeNVP(_T("list1"), m_list1);

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
		ar & tr::makeNVP(_T("list1"), m_list1);

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

		List <Ref<TestObject3>> m_list1;
		ar & tr::makeNVP(_T("list1"), m_list1);

		ASSERT_EQ(3, m_list1.GetCount());
		ASSERT_EQ(5, m_list1[0]->m_value);
		ASSERT_EQ(6, m_list1[1]->m_value);
		ASSERT_EQ(7, m_list1[2]->m_value);
	}
}

//------------------------------------------------------------------------------
//using Test_Factory = Ref<Object>(*)();
//
//
////template<typename T, typename isAbstract>
////static Test_Factory getFactory();
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
//	static Test_Factory getFactory()
//	{
//		return []() { return Ref<Object>::StaticCast(newObject<TObject>()); };
//	}
//};
//template<class TObject> struct Test_GetFactory<TObject, std::true_type>
//{
//	static Test_Factory getFactory()
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
//		: m_factory(Test_GetFactory<T, std::is_abstract<T>::type>::getFactory())
//	{
//		printf("aa");
//	}
//
//};
//
//
//Test_TypeInfo<TestObject4> TestObject4::info;

//template<>
//static Test_Factory getFactory<TestObject4, std::false_type::value>()
//{
//	return nullptr;
//}
//
//template<>
//static Test_Factory getFactory<TestObject4, std::true_type::value>()
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
		ar & tr::makeNVPBaseObject<TestObject3>(this);
		ar & tr::makeNVP(_T("value2"), m_value2);
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
			tr::Archive ar(&doc, tr::ArchiveMode::save, true);

			List <Ref<TestObject3>> m_list1 = { NewObject<TestObject3>(), NewObject<TestObject3>(), NewObject<TestObject3>() };
			m_list1[0]->m_value = 1;
			m_list1[1]->m_value = 2;
			m_list1[2]->m_value = 3;
			ar & tr::makeNVP(_T("list1"), m_list1);

			json = doc.ToString();
		}
		{
			tr::JsonDocument2 doc;
			doc.Parse(json);
			tr::Archive ar(&doc, tr::ArchiveMode::Load, true);

			List <Ref<TestObject3>> m_list1;
			ar & tr::makeNVP(_T("list1"), m_list1);

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
			tr::Archive ar(&doc, tr::ArchiveMode::save, true);

			auto obj1 = NewObject<TestObject4>();
			auto obj2 = NewObject<TestObject4>();
			auto obj3 = NewObject<TestObject4>();
			obj1->m_value = 1; obj1->m_value2 = 4;
			obj2->m_value = 2; obj2->m_value2 = 5;
			obj3->m_value = 3; obj3->m_value2 = 6;
			List <Ref<TestObject3>> m_list1 = { obj1.get(), obj2.get(), obj3.get() };
			ar & tr::makeNVP(_T("list1"), m_list1);

			json = doc.ToString(tr::JsonFormatting::Indented);
		}
		{
			tr::JsonDocument2 doc;
			doc.Parse(json);
			tr::Archive ar(&doc, tr::ArchiveMode::Load, true);

			List <Ref<TestObject3>> m_list1;
			ar & tr::makeNVP(_T("list1"), m_list1);

			auto* obj1 = dynamic_cast<TestObject4*>(m_list1[0].get());
			auto* obj2 = dynamic_cast<TestObject4*>(m_list1[1].get());
			auto* obj3 = dynamic_cast<TestObject4*>(m_list1[2].get());
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
		ar & ln::tr::makeNVP("name", name);
		ar & ln::tr::makeNVP("pitch", pitch);
		ar & ln::tr::makeNVP("volume", volume);
	}
};

struct TkMVMapData
{
	bool			autoplayBgm;
	TkMVSoundData	bgm;
	ln::String		displayName;

	void Serialize(ln::tr::Archive& ar, int version)
	{
		ar & ln::tr::makeNVP("autoplayBgm", autoplayBgm);
		ar & ln::tr::makeNVP("bgm", bgm);
		ar & ln::tr::makeNVP("displayName", displayName);
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
