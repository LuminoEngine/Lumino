#include "Common.hpp"
#include "../src/Base/RefObjectCache.hpp"

class Test_Base_ObjectCache : public ::testing::Test {};

TEST_F(Test_Base_ObjectCache, ObjectCounting)
{
	class TestObj : public RefObject
	{
	public:
		String name;
		TestObj(String n) : name(n) { }
	};

	detail::ObjectCache<String, TestObj> cache1;
	cache1.init(2);

	auto obj1 = makeRef<TestObj>(u"obj1");
	auto obj2 = makeRef<TestObj>(u"obj2");
	auto obj3 = makeRef<TestObj>(u"obj3");
	cache1.registerObject(obj1->name, obj1);
	cache1.registerObject(obj2->name, obj2);
	cache1.registerObject(obj3->name, obj3);

	// release していないので、すべてのオブジェクトは aliveList に入っている
	{
		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(3, aliveObjects.size());
		ASSERT_EQ(u"obj1", aliveObjects[0]->name);
		ASSERT_EQ(u"obj2", aliveObjects[1]->name);
		ASSERT_EQ(u"obj3", aliveObjects[2]->name);
		ASSERT_EQ(0, cache1.freeObjects().size());
	}

	// 2個 release。freeList に入る
	{
		cache1.releaseObject(obj1);
		cache1.releaseObject(obj2);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(1, aliveObjects.size());
		ASSERT_EQ(u"obj3", aliveObjects[0]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
		ASSERT_EQ(u"obj1", freeObjects[0]->name);
		ASSERT_EQ(u"obj2", freeObjects[1]->name);
	}

	// alive object を find。参照カウントや aliveList, freeList に変化なし。
	{
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(obj3));

		TestObj* obj = cache1.findObject(u"obj3");
		ASSERT_EQ(u"obj3", obj->name);
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(obj));

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(1, aliveObjects.size());

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
	}

	// free object を find。freeList から aliveList に移る。
	{
		TestObj* obj = cache1.findObject(u"obj1");
		ASSERT_EQ(u"obj1", obj->name);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(2, aliveObjects.size());
		ASSERT_EQ(u"obj3", aliveObjects[0]->name);
		ASSERT_EQ(u"obj1", aliveObjects[1]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(1, freeObjects.size());
		ASSERT_EQ(u"obj2", freeObjects[0]->name);
	}

	// すべて release。freeList に入る。最も古い obj2 は削除。
	{
		cache1.releaseObject(obj1);
		cache1.releaseObject(obj3);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(0, aliveObjects.size());

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
		ASSERT_EQ(u"obj1", freeObjects[0]->name);
		ASSERT_EQ(u"obj3", freeObjects[1]->name);

		ASSERT_EQ(nullptr, cache1.findObject(u"obj2"));	// オブジェクトなし

		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(obj1));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj2));	// ローカル変数からの参照のみ
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(obj3));
	}

	// dispose. すべて削除
	{
		cache1.dispose();

		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj1));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj2));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj3));
	}
}

TEST_F(Test_Base_ObjectCache, MemoryCounting)
{
	class TestObj : public RefObject
	{
	public:
		String name;
		TestObj(String n) : name(n) { }
	};

	detail::ObjectCache<String, TestObj> cache1;
	cache1.init(0, 1500);

	auto obj1 = makeRef<TestObj>(u"obj1");
	auto obj2 = makeRef<TestObj>(u"obj2");
	auto obj3 = makeRef<TestObj>(u"obj3");
	auto obj4 = makeRef<TestObj>(u"obj4");
	cache1.registerObject(obj1->name, obj1, 500);
	cache1.registerObject(obj2->name, obj2, 1000);
	cache1.registerObject(obj3->name, obj3, 1500);
	cache1.registerObject(obj4->name, obj4, 2000);

	// release していないので、すべてのオブジェクトは aliveList に入っている
	{
		ASSERT_EQ(obj1, cache1.findObject(u"obj1"));
		ASSERT_EQ(obj2, cache1.findObject(u"obj2"));
		ASSERT_EQ(obj3, cache1.findObject(u"obj3"));
		ASSERT_EQ(obj4, cache1.findObject(u"obj4"));
	}

	// 2個 release。キャッシュいっぱい。ちょうど freeList に入る
	{
		cache1.releaseObject(obj1);
		cache1.releaseObject(obj2);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(2, aliveObjects.size());
		ASSERT_EQ(u"obj3", aliveObjects[0]->name);
		ASSERT_EQ(u"obj4", aliveObjects[1]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
		ASSERT_EQ(u"obj1", freeObjects[0]->name);
		ASSERT_EQ(u"obj2", freeObjects[1]->name);
	}

	// free object を find。freeList から aliveList に移る。
	{
		TestObj* obj = cache1.findObject(u"obj1");
		ASSERT_EQ(u"obj1", obj->name);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(3, aliveObjects.size());
		ASSERT_EQ(u"obj3", aliveObjects[0]->name);
		ASSERT_EQ(u"obj4", aliveObjects[1]->name);
		ASSERT_EQ(u"obj1", aliveObjects[2]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(1, freeObjects.size());
		ASSERT_EQ(u"obj2", freeObjects[0]->name);
	}

	// obj1 を release。キャッシュいっぱい。ちょうど freeList に入る
	{
		cache1.releaseObject(obj1);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(2, aliveObjects.size());
		ASSERT_EQ(u"obj3", aliveObjects[0]->name);
		ASSERT_EQ(u"obj4", aliveObjects[1]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
		ASSERT_EQ(u"obj2", freeObjects[0]->name);
		ASSERT_EQ(u"obj1", freeObjects[1]->name);
	}

	// obj4 を release。そもそもキャッシュに入らないので即削除。
	{
		cache1.releaseObject(obj4);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(1, aliveObjects.size());
		ASSERT_EQ(u"obj3", aliveObjects[0]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
		ASSERT_EQ(u"obj2", freeObjects[0]->name);
		ASSERT_EQ(u"obj1", freeObjects[1]->name);
	}

	// obj3 を release。キャッシュいっぱいなので、obj1 と obj2 は削除。
	{
		cache1.releaseObject(obj3);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(0, aliveObjects.size());

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(1, freeObjects.size());
		ASSERT_EQ(u"obj3", freeObjects[0]->name);

		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj1));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj2));
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(obj3));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj4));
	}

	// dispose. すべて削除
	{
		cache1.dispose();

		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj1));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj2));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj3));
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(obj4));
	}
}

TEST_F(Test_Base_ObjectCache, GCObjects)
{
	class TestObj : public RefObject
	{
	public:
		String name;
		TestObj(String n) : name(n) { }
	};

	detail::ObjectCache<String, TestObj> cache1;
	cache1.init(2);

	auto obj1 = makeRef<TestObj>(u"obj1");
	auto obj2 = makeRef<TestObj>(u"obj2");
	cache1.registerObject(obj1->name, obj1);
	cache1.registerObject(obj2->name, obj2);

	// release していないので、すべてのオブジェクトは aliveList に入っている
	{
		ASSERT_EQ(obj1, cache1.findObject(u"obj1"));
		ASSERT_EQ(obj2, cache1.findObject(u"obj2"));
	}

	// obj1 の参照を切って GC. obj1 は freeList に入っている
	{
		obj1 = nullptr;
		cache1.collectUnreferenceObjects(true);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(1, aliveObjects.size());
		ASSERT_EQ(u"obj2", aliveObjects[0]->name);

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(1, freeObjects.size());
		ASSERT_EQ(u"obj1", freeObjects[0]->name);
	}
}

TEST_F(Test_Base_ObjectCache, GCObjects_Ones)
{
	class TestObj : public RefObject
	{
	public:
		String name;
		TestObj(String n) : name(n) { }
	};

	detail::ObjectCache<String, TestObj> cache1;
	cache1.init(2);

	auto obj1 = makeRef<TestObj>(u"obj1");
	auto obj2 = makeRef<TestObj>(u"obj2");
	cache1.registerObject(obj1->name, obj1);
	cache1.registerObject(obj2->name, obj2);

	// release していないので、すべてのオブジェクトは aliveList に入っている
	{
		ASSERT_EQ(obj1, cache1.findObject(u"obj1"));
		ASSERT_EQ(obj2, cache1.findObject(u"obj2"));
	}

	// 両方の参照を切って GC. ひとつずつ回収される。
	{
		obj1 = nullptr;
		obj2 = nullptr;
		cache1.collectUnreferenceObjects(false);

		auto aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(1, aliveObjects.size());
		ASSERT_EQ(u"obj2", aliveObjects[0]->name);	// obj2 はまだ生きてる

		auto freeObjects = cache1.freeObjects();
		ASSERT_EQ(1, freeObjects.size());
		ASSERT_EQ(u"obj1", freeObjects[0]->name);

		cache1.collectUnreferenceObjects(false);

		aliveObjects = cache1.aliveObjects();
		ASSERT_EQ(0, aliveObjects.size());

		freeObjects = cache1.freeObjects();
		ASSERT_EQ(2, freeObjects.size());
		ASSERT_EQ(u"obj1", freeObjects[0]->name);
		ASSERT_EQ(u"obj2", freeObjects[1]->name);
	}
}

