#include <TestConfig.h>
#include <Lumino/Base/Serialization.h>
#include <Lumino/Json/JsonDocument.h>
#include <Lumino/Reflection/ReflectionObject.h>


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
	void Initialize() {}
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
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TestObject3, Object, tr::TypeInfo::ClassVersion(1));

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
//	void Initialize() {}
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
	void Initialize() {}
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
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TestObject4, TestObject3, ClassVersion(2));

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
