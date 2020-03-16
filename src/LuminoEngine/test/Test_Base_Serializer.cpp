#include "Common.hpp"
#include <LuminoEngine/Base/Promise.hpp>

class Test_Base_Serializer : public ::testing::Test {};

class TestObject1 : public Object
{
	LN_OBJECT;
public:

	int m_value;

	void onSerialize2(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(u"value1");
			sr->writeInt(m_value);
		}
		else {
			if (sr->setName(u"value1")) m_value = sr->readInt();
		}
	}
};
LN_OBJECT_IMPLEMENT(TestObject1, Object) {}

TEST_F(Test_Base_Serializer, Basic)
{
	auto obj1 = makeObject<TestObject1>();
	obj1->m_value = 500;
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, u"");

	auto asset2 = Serializer2::deserialize(text, u"");
	auto obj2 = dynamic_cast<TestObject1*>(asset2->target());

	ASSERT_EQ(500, obj2->m_value);
}

TEST_F(Test_Base_Serializer, List)
{
	class TestObject1 : public Object
	{
		List<int> m_list = { 1, 2, 3 };

		void onSerialize2(Serializer2* sr) override
		{
			sr->writeName(u"list");
			sr->beginWriteList();
			for (auto& v : m_list) sr->writeInt(v);
			sr->endWriteList();
		}
	};

	//auto obj1 = makeObject<TestObject1>();
	//String text = Serializer2::serialize(obj1, u"");

	printf("");
}

//TEST_F(Test_Base_Serializer, BasicSceneDirectly)
//{
//	class TestModel : public Object
//	{
//		void onSerialize2(Serializer2* sr) override
//		{
//			sr->writeInt(u"value1", 100);
//		}
//	};
//
//	class TestComponent : public Object
//	{
//	public:
//		Ref<TestModel> m_model;
//
//		void onSerialize2(Serializer2* sr) override
//		{
//			sr->writeName(u"model");
//			sr->writeObject(m_model);
//		}
//	};
//
//	class TestWorldObject : public Object
//	{
//	public:
//		List<Ref<TestComponent>> m_components;
//
//		void onSerialize2(Serializer2* sr) override
//		{
//			sr->beginWriteList(u"components");
//			for (auto& v : m_components) sr->writeObject(v);
//			sr->endWriteList();
//		}
//	};
//
//	class TestScene : public Object
//	{
//	public:
//		List<Ref<TestComponent>> m_objects;
//
//		void onSerialize2(Serializer2* sr) override
//		{
//			sr->beginWriteList(u"objects");
//			for (auto& v : m_objects) sr->writeObject(v);
//			sr->endWriteList();
//		}
//	};
//
//	auto scene1 = makeObject<TestScene>();
//	String text = Serializer2::serialize(scene1, u"");
//
//	printf("");
//}
