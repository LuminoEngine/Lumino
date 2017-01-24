#include <TestConfig.h>
#include <Lumino/Base/Serialization.h>
#include <Lumino/Json/JsonDocument.h>


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

	printf("");
}

