#include <TestConfig.h>
#include <Lumino/Reflection/ReflectionObject.h>

class Test_Base_WeakRefPtr : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

class WeakRefTest1 : public Object
{
public:
	WeakRefTest1() { count++; }
	virtual ~WeakRefTest1() { count--; }
	int m = 100;
	static int count;
};
int WeakRefTest1::count = 0;

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic)
{
	tr::WeakRefPtr<WeakRefTest1> weak;
	{
		auto ptr = Ref<WeakRefTest1>::makeRef();
		weak = tr::WeakRefPtr<WeakRefTest1>(ptr);
		ASSERT_EQ(true, weak.isAlive());
		ASSERT_EQ(100, weak.resolve()->m);
		ASSERT_EQ(1, WeakRefTest1::count);	// まだ生きている
	}
	ASSERT_EQ(0, WeakRefTest1::count);	// デストラクタが動いた
	ASSERT_EQ(false, weak.isAlive());
	ASSERT_EQ(true, weak.resolve().isNull());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic2)
{
	tr::WeakRefPtr<WeakRefTest1> weak;
	{
		WeakRefTest1 t1;	// ローカルオブジェクト
		WeakRefTest1* ptr = &t1;
		weak = tr::WeakRefPtr<WeakRefTest1>(ptr);
		ASSERT_EQ(true, weak.isAlive());
		ASSERT_EQ(100, weak.resolve()->m);
		ASSERT_EQ(1, WeakRefTest1::count);	// まだ生きている
	}
	ASSERT_EQ(0, WeakRefTest1::count);	// デストラクタが動いた
	ASSERT_EQ(false, weak.isAlive());
	ASSERT_EQ(true, weak.resolve().isNull());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic3)
{
	tr::WeakRefPtr<WeakRefTest1> weak;// (&t1);
	{
		WeakRefTest1 t1;	// ローカルオブジェクト
		weak = &t1;
	}
}
//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic4)
{
	WeakRefTest1 t1;	// ローカルオブジェクト
	{
		//tr::WeakRefPtr<WeakRefTest1> weak;// (&t1);
		//weak = &t1;
		//t1.requestWeakRefInfo();
	}
}
