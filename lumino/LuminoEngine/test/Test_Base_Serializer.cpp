#include "Common.hpp"
#include <LuminoEngine/Base/Promise.hpp>

#if 0

class Test_Base_Serializer : public ::testing::Test {};

//-----------------------------------------------------------------------------
class TestObject1 : public Object
{
	LN_OBJECT;
public:
	int m_value = 0;

	void onSerialize(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(_TT("value1"));
			sr->writeInt(m_value);
		}
		else {
			if (sr->readName(_TT("value1"))) m_value = sr->readInt();
		}
	}
};
LN_OBJECT_IMPLEMENT(TestObject1, Object) {}

TEST_F(Test_Base_Serializer, Basic)
{
	auto obj1 = makeObject<TestObject1>();
	obj1->m_value = 500;
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, detail::AssetPath::Null);

	auto asset2 = Serializer2::deserialize(text, detail::AssetPath::Null);
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

	void onSerialize(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(_TT("list"));
			sr->beginWriteList();
			for (auto& v : m_list) sr->writeInt(v);
			sr->endWriteList();
		}
		else {
			if (sr->readName(_TT("list"))) {
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
	String text = Serializer2::serialize(asset1, detail::AssetPath::Null);

	auto asset2 = Serializer2::deserialize(text, detail::AssetPath::Null);
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

	void onSerialize(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(_TT("list"));
			sr->beginWriteList();
			for (auto& v : m_list) sr->writeObject(v);
			sr->endWriteList();
		}
		else {
			if (sr->readName(_TT("list"))) {
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
	String text = Serializer2::serialize(asset1, detail::AssetPath::Null);

	auto asset2 = Serializer2::deserialize(text, detail::AssetPath::Null);
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

	void onSerialize(Serializer2* sr) override
	{
		if (sr->isSaving()) {
			sr->writeName(_TT("m_bool")); sr->writeBool(m_bool);
			sr->writeName(_TT("m_int8")); sr->writeInt8(m_int8);
			sr->writeName(_TT("m_int16")); sr->writeInt16(m_int16);
			sr->writeName(_TT("m_int32")); sr->writeInt32(m_int32);
			sr->writeName(_TT("m_int64")); sr->writeInt64(m_int64);
			sr->writeName(_TT("m_uint8")); sr->writeUInt8(m_uint8);
			//sr->writeName(_TT("m_uint16")); sr->writeUInt16(m_uint16);
			//sr->writeName(_TT("m_uint32")); sr->writeUInt32(m_uint32);
			//sr->writeName(_TT("m_uint64")); sr->writeUInt64(m_uint64);
			sr->writeName(_TT("m_float")); sr->writeFloat(m_float);
			sr->writeName(_TT("m_double")); sr->writeDouble(m_double);
			sr->writeName(_TT("m_string")); sr->writeString(m_string);
		}
		else {
			if (sr->readName(_TT("m_bool"))) m_bool = sr->readBool();
			if (sr->readName(_TT("m_int8"))) m_int8 = sr->readInt8();
			if (sr->readName(_TT("m_int16"))) m_int16 = sr->readInt16();
			if (sr->readName(_TT("m_int32"))) m_int32 = sr->readInt32();
			if (sr->readName(_TT("m_int64"))) m_int64 = sr->readInt64();
			if (sr->readName(_TT("m_uint8"))) m_uint8 = sr->readUInt8();
			//if (sr->readName(_TT("m_uint16"))) m_uint16 = sr->readUInt16();
			//if (sr->readName(_TT("m_uint32"))) m_uint32 = sr->readUInt32();
			//if (sr->readName(_TT("m_uint64"))) m_uint64 = sr->readUInt64();
			if (sr->readName(_TT("m_float"))) m_float = sr->readFloat();
			if (sr->readName(_TT("m_double"))) m_double = sr->readDouble();
			if (sr->readName(_TT("m_string"))) m_string = sr->readString();
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
	obj1->m_uint8 = 108;
	obj1->m_uint16 = 1016;
	obj1->m_uint32 = 1032;
	obj1->m_uint64 = 1064;
	obj1->m_float = 500;
	obj1->m_double = 1000;
	obj1->m_string = _TT("test");
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, detail::AssetPath::Null);

	auto asset2 = Serializer2::deserialize(text, detail::AssetPath::Null);
	auto obj2 = dynamic_cast<Test_Base_Serializer_Values_TestObject1*>(asset2->target());

	ASSERT_EQ(true, obj2 != nullptr);
	ASSERT_EQ(true, obj2->m_bool);
	ASSERT_EQ(8, obj2->m_int8);
	ASSERT_EQ(16, obj2->m_int16);
	ASSERT_EQ(32, obj2->m_int32);
	ASSERT_EQ(64, obj2->m_int64);
	ASSERT_EQ(108, obj2->m_uint8);
	//ASSERT_EQ(1016, obj2->m_uint16);
	//ASSERT_EQ(1032, obj2->m_uint32);
	//ASSERT_EQ(1064, obj2->m_uint64);
	ASSERT_EQ(true, Math::nearEqual(obj2->m_float, 500));
	ASSERT_EQ(true, Math::nearEqual(obj2->m_double, 1000));
	ASSERT_EQ(_TT("test"), obj2->m_string);
}

//-----------------------------------------------------------------------------
class Test_Base_Serializer_CppUtils_TestObject1 : public Object
{
	LN_OBJECT;
public:
	int m_int = 0;
	String m_string;
	Ref<TestObject1> m_obj1;
	List<int> m_list1;
	List<Ref<TestObject1>> m_list2;

	void serialize(Serializer2& sr) override
	{
		sr & makeNVP(_TT("m_int"), m_int);
		sr & makeNVP(_TT("m_string"), m_string);
		sr & makeNVP(_TT("m_obj1"), m_obj1);
		sr & makeNVP(_TT("m_list1"), m_list1);
		sr & makeNVP(_TT("m_list2"), m_list2);
	}
};
LN_OBJECT_IMPLEMENT(Test_Base_Serializer_CppUtils_TestObject1, Object) {}

TEST_F(Test_Base_Serializer, CppUtils)
{
	auto obj1 = makeObject<Test_Base_Serializer_CppUtils_TestObject1>();
	obj1->m_int = 8;
	obj1->m_string = _TT("test");
	obj1->m_obj1 = makeObject<TestObject1>();
	obj1->m_obj1->m_value = 256;
	obj1->m_list1 = { 1, 2, 3 };
	obj1->m_list2 = { makeObject<TestObject1>(), makeObject<TestObject1>(), makeObject<TestObject1>() };
	obj1->m_list2[0]->m_value = 100;
	obj1->m_list2[1]->m_value = 200;
	obj1->m_list2[2]->m_value = 300;
	auto asset1 = makeObject<AssetModel>(obj1);
	String text = Serializer2::serialize(asset1, detail::AssetPath::Null);

	auto asset2 = Serializer2::deserialize(text, detail::AssetPath::Null);
	auto obj2 = dynamic_cast<Test_Base_Serializer_CppUtils_TestObject1*>(asset2->target());

	ASSERT_EQ(true, obj2 != nullptr);
	ASSERT_EQ(8, obj2->m_int);
	ASSERT_EQ(_TT("test"), obj2->m_string);
	ASSERT_EQ(256, obj2->m_obj1->m_value);
	ASSERT_EQ(3, obj2->m_list1.size());
	ASSERT_EQ(1, obj2->m_list1[0]);
	ASSERT_EQ(2, obj2->m_list1[1]);
	ASSERT_EQ(3, obj2->m_list1[2]);
	ASSERT_EQ(3, obj2->m_list2.size());
	ASSERT_EQ(100, obj2->m_list2[0]->m_value);
	ASSERT_EQ(200, obj2->m_list2[1]->m_value);
	ASSERT_EQ(300, obj2->m_list2[2]->m_value);
}

#endif

