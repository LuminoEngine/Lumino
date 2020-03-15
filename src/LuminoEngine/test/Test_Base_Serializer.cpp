#include "Common.hpp"
#include <LuminoEngine/Base/Promise.hpp>

class Test_Base_Serializer : public ::testing::Test {};

TEST_F(Test_Base_Serializer, Basic)
{
	class TestObject1 : public Object
	{
		int m_value = 100;

		void onSerialize2(Serializer2* sr) override
		{
			if (sr->isSaving()) {
				sr->writeName(u"value1");
				sr->writeInt(m_value);
			}
			else {
				m_value = sr->readInt(u"value1");
			}
		}
	};

	auto obj1 = makeObject<TestObject1>();
	String text = Serializer2::serialize(obj1, u"");

	auto obj2 = dynamic_pointer_cast<TestObject1>(Serializer2::deserialize(text, u""));

	printf("");
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

	auto obj1 = makeObject<TestObject1>();
	String text = Serializer2::serialize(obj1, u"");

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
