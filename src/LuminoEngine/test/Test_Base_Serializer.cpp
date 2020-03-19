#include "Common.hpp"
#include <LuminoEngine/Base/Promise.hpp>

class Test_Base_Serializer : public ::testing::Test {};

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
class Test_Base_Serializer_ObjectList1 : public Object
{
	LN_OBJECT;
public:
	List<Ref<TestObject1>> m_list;

	void onSerialize2(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(u"list");
			sr->beginWriteList();
			for (auto& v : m_list) sr->writeObject(v);
			sr->endWriteList();
		}
		else {
			if (sr->readName(u"list")) {
				int count = 0;
				if (sr->beginReadList(&count)) {
					m_list.reserve(count);
					for (int i = 0; i < count; i++) {
						m_list.add(dynamic_pointer_cast<TestObject1>(sr->readObject()));
					}
					sr->endReadList();
				}
			}
		}
	}
};
LN_OBJECT_IMPLEMENT(Test_Base_Serializer_ObjectList1, Object) {}

TEST_F(Test_Base_Serializer, ObjectList)
{
	auto obj1 = makeObject<Test_Base_Serializer_ObjectList1>();
	obj1->m_list = { makeObject<TestObject1>(), makeObject<TestObject1>(), makeObject<TestObject1>() };
	obj1->m_list[0]->m_value = 100;
	obj1->m_list[1]->m_value = 200;
	obj1->m_list[2]->m_value = 300;
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, u"");

	auto asset2 = Serializer2::deserialize(text, u"");
	auto obj2 = dynamic_cast<Test_Base_Serializer_ObjectList1*>(asset2->target());

	ASSERT_EQ(true, obj2 != nullptr);
	ASSERT_EQ(3, obj2->m_list.size());
	ASSERT_EQ(100, obj2->m_list[0]->m_value);
	ASSERT_EQ(200, obj2->m_list[1]->m_value);
	ASSERT_EQ(300, obj2->m_list[2]->m_value);
}


//-----------------------------------------------------------------------------
class Test_Base_Serializer_Values_TestObject1 : public Object
{
	LN_OBJECT;
public:
	bool m_bool = false;
	int8_t m_int8 = 0;
	int16_t m_int16 = 0;
	int32_t m_int32 = 0;
	int64_t m_int64 = 0;
	uint8_t m_uint8 = 0;
	uint16_t m_uint16 = 0;
	uint32_t m_uint32 = 0;
	uint64_t m_uint64 = 0;
	float m_float = 0;
	double m_double = 0;
	String m_string;

	void onSerialize2(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(u"m_bool"); sr->writeBool(m_bool);
			//sr->writeName(u"m_int8"); sr->writeInt(m_int8);
		}
		else {
			if (sr->readName(u"m_bool")) m_bool = sr->readBool();
			//if (sr->readName(u"m_int8")) m_int8 = sr->readInt();
		}
	}
};
LN_OBJECT_IMPLEMENT(Test_Base_Serializer_Values_TestObject1, Object) {}

TEST_F(Test_Base_Serializer, Values)
{
	auto obj1 = makeObject<Test_Base_Serializer_Values_TestObject1>();
	obj1->m_bool = true;
	obj1->m_int8 = 8;
	obj1->m_int16 = 16;
	obj1->m_int32 = 32;
	obj1->m_int64 = 64;
	obj1->m_uint8 = 1008;
	obj1->m_uint16 = 1016;
	obj1->m_uint32 = 1032;
	obj1->m_uint64 = 1064;
	obj1->m_float = 500;
	obj1->m_double = 1000;
	obj1->m_string = u"test";
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, u"");

	auto asset2 = Serializer2::deserialize(text, u"");
	auto obj2 = dynamic_cast<Test_Base_Serializer_Values_TestObject1*>(asset2->target());

	ASSERT_EQ(true, obj2 != nullptr);
	ASSERT_EQ(true, obj2->m_bool);
	ASSERT_EQ(8, obj2->m_int8);
	ASSERT_EQ(16, obj2->m_int16);
	ASSERT_EQ(32, obj2->m_int32);
	ASSERT_EQ(64, obj2->m_int64);
	ASSERT_EQ(1008, obj2->m_uint8);
	ASSERT_EQ(1016, obj2->m_uint16);
	ASSERT_EQ(1032, obj2->m_uint32);
	ASSERT_EQ(1064, obj2->m_uint64);
	ASSERT_EQ(true, Math::nearEqual(obj2->m_float, 500));
	ASSERT_EQ(true, Math::nearEqual(obj2->m_double, 1000));
	ASSERT_EQ(u"test", obj2->m_string);
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
