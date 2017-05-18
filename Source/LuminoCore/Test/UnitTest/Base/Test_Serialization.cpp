#include <TestConfig.h>
#include <Lumino/Base/Serialization.h>
#include <Lumino/Json/JsonDocument.h>
#include <Lumino/Reflection/ReflectionObject.h>


class TestObject1
{
public:

	void lnsl_SerializeImpl(tr::Archive& ar)
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
	tr::Archive ar(&doc, tr::ArchiveMode::Save);

	ar & tr::MakeNVP(_T("obj1"), t1);
	doc.Save(TEMPFILE("json3.txt"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Serialization, Load)
{
	TestObject1 t1;
	tr::JsonDocument2 doc;
	doc.Load(TEMPFILE("json3.txt"));
	tr::Archive ar(&doc, tr::ArchiveMode::Load);

	t1.m_value1 = 1;
	ar & tr::MakeNVP(_T("obj1"), t1);
}

//------------------------------------------------------------------------------
class TestObject2
{
public:
	void lnsl_SerializeImpl(tr::Archive& ar)
	{
		ar & tr::MakeNVP(_T("value"), m_value);
	}

public:
	int m_value;
};
class TestObject3 : public Object
{
public:
	void Initialize() {}
	void lnsl_SerializeImpl(tr::Archive& ar)
	{
		ar & tr::MakeNVP(_T("value"), m_value);
	}

public:
	int m_value;
};
TEST_F(Test_Serialization, List)
{
	// <Test> write primitive type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Save);

		List<int>	m_list1 = { 1, 2, 3 };
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[1,2,3]}"), doc.ToString());
	}
	// <Test> write struct type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Save);

		List<TestObject2>	m_list1 = { TestObject2{ 1 }, TestObject2{ 2 }, TestObject2{ 3 } };
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[{\"value\":1},{\"value\":2},{\"value\":3}]}"), doc.ToString());
	}
	// <Test> write ln::Object type values.
	{
		tr::JsonDocument2 doc;
		tr::Archive ar(&doc, tr::ArchiveMode::Save);

		List <RefPtr<TestObject3>> m_list1 = { NewObject<TestObject3>(), NewObject<TestObject3>(), NewObject<TestObject3>() };
		m_list1[0]->m_value = 1;
		m_list1[1]->m_value = 2;
		m_list1[2]->m_value = 3;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(_T("{\"list1\":[{\"value\":1},{\"value\":2},{\"value\":3}]}"), doc.ToString());
	}

	// <Test> read primitive type values.
	{
		tr::JsonDocument2 doc;
		doc.Load(LN_LOCALFILE("TestData/Test_Serialization_List1.json"));
		tr::Archive ar(&doc, tr::ArchiveMode::Load);

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
		tr::Archive ar(&doc, tr::ArchiveMode::Load);

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
		tr::Archive ar(&doc, tr::ArchiveMode::Load);

		List <RefPtr<TestObject3>> m_list1;
		ar & tr::MakeNVP(_T("list1"), m_list1);

		ASSERT_EQ(3, m_list1.GetCount());
		ASSERT_EQ(5, m_list1[0]->m_value);
		ASSERT_EQ(6, m_list1[1]->m_value);
		ASSERT_EQ(7, m_list1[2]->m_value);
	}
}

