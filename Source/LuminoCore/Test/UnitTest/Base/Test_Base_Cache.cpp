#include <TestConfig.h>
#include <Lumino/Base/Cache.h>

class Test_Base_Cache : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static int g_count = 0;

class CacheTest
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	CacheTest(size_t objSize)
	{
		m_cacheObjectInfo.cacheMemorySize = objSize;
		g_count++;
	}
	virtual ~CacheTest()
	{
		g_count--;
	}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Cache, ObjectCount)
{
	g_count = 0;
	Ref<CacheManager> manager(LN_NEW CacheManager(2, 0), false);
	
	CacheTest* obj1 = LN_NEW CacheTest(10);
	CacheTest* obj2 = LN_NEW CacheTest(10);
	CacheTest* obj3 = LN_NEW CacheTest(10);
	CacheTest* obj4 = LN_NEW CacheTest(10);

	// キャッシュに登録
	manager->registerCacheObject(CacheKey(_T("obj1")), obj1);
	manager->registerCacheObject(CacheKey(_T("obj2")), obj2);
	manager->registerCacheObject(CacheKey(_T("obj3")), obj3);
	manager->registerCacheObject(CacheKey(_T("obj4")), obj4);

	// それぞれ find して参照カウントチェック
	CacheTest* obj;
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj1"))));
	obj->release();
	ASSERT_EQ(obj1, obj);
	ASSERT_EQ(1, obj->getReferenceCount());
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj2"))));
	obj->release();
	ASSERT_EQ(obj2, obj);
	ASSERT_EQ(1, obj->getReferenceCount());
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj3"))));
	obj->release();
	ASSERT_EQ(obj3, obj);
	ASSERT_EQ(1, obj->getReferenceCount());
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj4"))));
	obj->release();
	ASSERT_EQ(obj4, obj);
	ASSERT_EQ(1, obj->getReferenceCount());

	// 全部 Release。キャッシュに入る
	obj1->release();
	obj2->release();
	obj3->release();

	// サイズ2のキャッシュなので obj1 は完全に消え、他は残る
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj1"))));
	ASSERT_EQ(NULL, obj);
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj2"))));
	ASSERT_EQ(obj2, obj);
	ASSERT_EQ(1, obj->getReferenceCount());
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj3"))));
	ASSERT_EQ(obj3, obj);
	ASSERT_EQ(1, obj->getReferenceCount());

	// また Release。キャッシュに入る
	obj2->release();

	// キャッシュをクリアする。参照の残っている obj3 だけ残る。
	manager->clearCache();
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj1"))));
	ASSERT_EQ(NULL, obj);
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj2"))));
	ASSERT_EQ(NULL, obj);
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj3"))));
	ASSERT_EQ(obj3, obj);
	obj->release();
	ASSERT_EQ(1, obj->getReferenceCount());

	// また Release。キャッシュに入る
	obj3->release();

	// 後処理。キャッシュに残っているオブジェクトを全解放する
	manager->finalizeCache();

	// Release。もう Finalize() 済みなのでそのまま解放される。
	obj4->release();

	// Finalize() 後に登録はできない
	CacheTest* obj5 = LN_NEW CacheTest(10);
	manager->registerCacheObject(CacheKey(_T("obj5")), obj5);
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj5"))));
	ASSERT_EQ(NULL, obj);
	obj5->release();	// そのまま消える
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj5"))));
	ASSERT_EQ(NULL, obj);

	// カウントは戻っているはず
	ASSERT_EQ(0, g_count);
	ASSERT_EQ(1, manager->getReferenceCount());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Cache, ObjectSize)
{
	g_count = 0;
	Ref<CacheManager> manager(LN_NEW CacheManager(2, 1), false);	// 1byte しかキャッシュに置けない

	CacheTest* obj1 = LN_NEW CacheTest(1);
	CacheTest* obj2 = LN_NEW CacheTest(10);
	CacheTest* obj3 = LN_NEW CacheTest(1);
	ASSERT_EQ(3, g_count);

	manager->registerCacheObject(CacheKey(_T("obj1")), obj1);
	manager->registerCacheObject(CacheKey(_T("obj2")), obj2);
	manager->registerCacheObject(CacheKey(_T("obj3")), obj3);

	// キャッシュには置けないサイズでも、まだ生きていれば FindObjectAddRef() で取れる
	CacheTest* obj;
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj1"))));
	obj->release();
	ASSERT_EQ(obj1, obj);
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj2"))));
	obj->release();
	ASSERT_EQ(obj2, obj);
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj3"))));
	obj->release();
	ASSERT_EQ(obj3, obj);

	// 参照が消えるが、キャッシュには置けない大きさなのですぐに delete される
	obj2->release();
	ASSERT_EQ(2, g_count);

	// 1byte ぎりぎりキャッシュに置ける
	obj1->release();
	ASSERT_EQ(2, g_count);

	// サイズオーバーでキャッシュに置けない。すぐ delete
	obj3->release();
	ASSERT_EQ(1, g_count);

	manager->clearCache();

	// カウントは戻っているはず
	ASSERT_EQ(0, g_count);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Cache, NotUseCache)
{
	Ref<CacheManager> manager(LN_NEW CacheManager(0, 0), false);

	CacheTest* obj1 = LN_NEW CacheTest(1);

	manager->registerCacheObject(CacheKey(_T("obj1")), obj1);

	// 生きていれば FindObjectAddRef() で取れる
	CacheTest* obj;
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj1"))));
	obj->release();
	ASSERT_EQ(obj1, obj);

	// キャッシュは使用していないのですぐ delete
	obj1->release();
	ASSERT_EQ(0, g_count);

	// 念のため。もういない
	obj = dynamic_cast<CacheTest*>(manager->findObjectAddRef(CacheKey(_T("obj1"))));
	ASSERT_EQ(NULL, obj);
}
