#include "Common.hpp"
#include <LuminoEngine/Base/Promise.hpp>

class Test_Base_Serializer : public ::testing::Test {};

class TestObject1 : public Object
{
	LN_OBJECT;
public:
	int m_value = 0;

	void onSerialize2(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(u"value1");
			sr->writeInt(m_value);
		}
		else {
			if (sr->readName(u"value1")) m_value = sr->readInt();
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

	ASSERT_EQ(true, obj2 != nullptr);
	ASSERT_EQ(500, obj2->m_value);
}

class Test_Base_Serializer_TestObject1 : public Object
{
	LN_OBJECT;
public:
	List<int> m_list;

	void onSerialize2(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(u"list");
			sr->beginWriteList();
			for (auto& v : m_list) sr->writeInt(v);
			sr->endWriteList();
		}
		else {
			if (sr->readName(u"list")) {
				int count = 0;
				if (sr->beginReadList(&count)) {
					m_list.reserve(count);
					for (int i = 0; i < count; i++) m_list.add(sr->readInt());
					sr->endReadList();
				}
			}
		}
	}
};
LN_OBJECT_IMPLEMENT(Test_Base_Serializer_TestObject1, Object) {}

TEST_F(Test_Base_Serializer, List)
{
	auto obj1 = makeObject<Test_Base_Serializer_TestObject1>();
	obj1->m_list = { 1, 2, 3 };
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, u"");

	auto asset2 = Serializer2::deserialize(text, u"");
	auto obj2 = dynamic_cast<Test_Base_Serializer_TestObject1*>(asset2->target());

	ASSERT_EQ(true, obj2 != nullptr);
	ASSERT_EQ(3, obj2->m_list.size());
	ASSERT_EQ(1, obj2->m_list[0]);
	ASSERT_EQ(2, obj2->m_list[1]);
	ASSERT_EQ(3, obj2->m_list[2]);
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
