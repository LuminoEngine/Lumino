#include "Common.hpp"

#if 0
//#include <Lumino/Base/Serialization.h>
#include <LuminoCore/Json/JsonDocument.h>
//#include <Lumino/Reflection/ReflectionObject.h>
//#include <Lumino/Base/Uuid.h>
#include <LuminoCore/Serialization/Serialization2.h>

//------------------------------------------------------------------------------
//## Ref<> を通した Object の serialize をしてほしい
// ダムポインタ（つまり、生ポインタなどint *）や参照はサポートしない。cereal と同じ。
// https://uscilab.github.io/cereal/pointers.html
// Lumino としては、「強参照」はシリアライズする。「弱参照(生ポインタやweak)」はシリアライズしない。
// また、オブジェクトを new したりしない。（今後要検討。ただ、デフォルトではしない方向で）
// 内部で new する場合、リフレクションの仕組みが必要。また、Ref<SceneNode> をシリアライズしようとしても、オブジェクトの実態は Sprite かもしれない。
// うっかりリフレクション登録を忘れたまま load で new しようとすると、間違えて SceneNode を作ってしまう。ちょっと危険が多い。

class ObjectTest1
	: public ln::Object
{
public:
	int x = 10;
		
	void serialize(Archive2& ar)
	{
		ar & LN_NVP2(x);
	}
};

class ObjectTestRoot1
	: public ln::Object
{
public:
	ObjectTest1 * t1 = nullptr;
	Ref<ObjectTest1> t2;

	void serialize(Archive2& ar)
	{
		//ar & LN_NVP2(t1);	// Un supported
		ar & LN_NVP2(t2);
	}
};

TEST_F(Test_Serialization2, ObjectTest)
{
	String json;

	//- [ ] Save
	{
		JsonTextOutputArchive ar;

		ObjectTestRoot1 t;
		t.t2 = newObject<ObjectTest1>();
		ar.process(t);

		json = ar.toString(tr::JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_archive_version\":1,\"lumino_archive_root\":{\"t2\":{\"x\":10}}}"), json);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);

		ObjectTestRoot1 t;
		t.t2 = newObject<ObjectTest1>();
		ar.process(t);

		ASSERT_EQ(10, t.t2->x);
	}
}

//------------------------------------------------------------------------------
//## List<> をシリアライズする

class ListTest1
	: public ln::Object
{
public:
	int x = 0;

	void serialize(Archive2& ar)
	{
		ar & LN_NVP2(x);
	}
};

TEST_F(Test_Serialization2, List)
{
	String json;

	//- [ ] プリミティブ型 List の Save
	{
		JsonTextOutputArchive ar;

		List<int> t = { 1, 2, 3 };
		ar.process(t);

		json = ar.toString(tr::JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_archive_version\":1,\"lumino_archive_root\":[1,2,3]}"), json);
	}

	//- [ ] プリミティブ型 List のLoad
	{
		JsonTextInputArchive ar(json);

		List<int> t;
		ar.process(t);

		ASSERT_EQ(3, t.getCount());
		ASSERT_EQ(1, t[0]);
		ASSERT_EQ(2, t[1]);
		ASSERT_EQ(3, t[2]);
	}

	//- [ ] Ref<> 型 List の Save
	{
		JsonTextOutputArchive ar;

		List<Ref<ListTest1>> t;
		t.add(newObject<ListTest1>()); t.getLast()->x = 10;
		t.add(newObject<ListTest1>()); t.getLast()->x = 20;
		t.add(newObject<ListTest1>()); t.getLast()->x = 30;
		ar.process(t);

		json = ar.toString(tr::JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_archive_version\":1,\"lumino_archive_root\":[{\"x\":10},{\"x\":20},{\"x\":30}]}"), json);
	}

	//- [ ] プリミティブ型 List のLoad (Load 先のオブジェクトをあらかじめ作っておく場合)
	{
		JsonTextInputArchive ar(json);

		List<Ref<ListTest1>> t;
		t.add(newObject<ListTest1>());
		t.add(newObject<ListTest1>());
		t.add(newObject<ListTest1>());
		ar.process(t);

		ASSERT_EQ(3, t.getCount());
		ASSERT_EQ(10, t[0]->x);
		ASSERT_EQ(20, t[1]->x);
		ASSERT_EQ(30, t[2]->x);
	}

	//- [ ] プリミティブ型 List のLoad (Load 先が null の場合はオブジェクトが作られる)
	{
		JsonTextInputArchive ar(json);

		List<Ref<ListTest1>> t;
		ar.process(t);

		ASSERT_EQ(3, t.getCount());
		ASSERT_EQ(10, t[0]->x);
		ASSERT_EQ(20, t[1]->x);
		ASSERT_EQ(30, t[2]->x);
	}
}

//------------------------------------------------------------------------------
//## Uuid をシリアライズする

class UuidTest1
	: public ln::Object
{
public:
	Uuid id;

	void serialize(Archive2& ar)
	{
		ar & LN_NVP2(id);
	}
};

TEST_F(Test_Serialization2, Uuid)
{
	Uuid id(_T("35BBEEBB-A428-4B7F-B4F6-08AA6D5239AA"));
	String json;

	//- [ ]  Save
	{
		JsonTextOutputArchive ar;

		UuidTest1 t;
		t.id = id;
		ar.process(t);

		json = ar.toString(tr::JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_archive_version\":1,\"lumino_archive_root\":{\"id\":\"35BBEEBB-A428-4B7F-B4F6-08AA6D5239AA\"}}"), json);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);

		UuidTest1 t;
		ar.process(t);

		ASSERT_EQ(id, t.id);
	}
}

//------------------------------------------------------------------------------
//## ベースクラスをシリアライズしたい

class BaseTest1
{
public:
	int x = 0;

	void serialize(Archive2& ar)
	{
		ar & LN_NVP2(x);
	}
};

class DerivedTest1
	: public BaseTest1
{
public:
	int y = 0;

	void serialize(Archive2& ar)
	{
		ar & Archive2::BaseClass<BaseTest1>(this);
		ar & LN_NVP2(y);
	}
};

TEST_F(Test_Serialization2, BaseClass)
{
	String json;

	//- [ ]  Save
	{
		JsonTextOutputArchive ar;

		DerivedTest1 t;
		t.x = 55;
		t.y = 77;
		ar.process(t);

		json = ar.toString(tr::JsonFormatting::None);
		ASSERT_EQ(_T("{\"lumino_archive_version\":1,\"lumino_archive_root\":{\"lumino_base_class\":{\"x\":55},\"y\":77}}"), json);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);

		DerivedTest1 t;
		ar.process(t);

		ASSERT_EQ(55, t.x);
		ASSERT_EQ(77, t.y);
	}
}


//------------------------------------------------------------------------------
//## デフォルト値を指定して Load する
// ユーザーオブジェクトのデフォルト値は対応しない。というか、RefObject のサブクラスでそれやろうとすると
// コピー禁止 (コピーコンストラクタ delete) が邪魔する。

TEST_F(Test_Serialization2, DefaultValue)
{
	class DefaultTest1
	{
	public:
		int x = 1;
		int y = 2;

		void serialize(Archive2& ar)
		{
			ar & LN_NVP2(x);
			ar & LN_NVP2(y);
		}
	};
	class DefaultTest2
	{
	public:
		int x = 10;
		int z = 20;

		void serialize(Archive2& ar)
		{
			ar & LN_NVP2(x);
			ar & LN_NVP2(z, 55);
		}
	};

	String json;

	//- [ ]  Save
	{
		JsonTextOutputArchive ar;
		DefaultTest1 t;
		ar.process(t);
		json = ar.toString(tr::JsonFormatting::None);
	}

	//- [ ] Load
	{
		JsonTextInputArchive ar(json);
		DefaultTest2 t;
		ar.process(t);
		ASSERT_EQ(1, t.x);
		ASSERT_EQ(55, t.z);
	}
}

#endif

