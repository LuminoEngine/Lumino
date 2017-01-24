#include <TestConfig.h>
#include <Lumino/Reflection/ReflectionObject.h>

class Test_Base_WeakRefPtr : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

class WeakRefTest1 : public tr::ReflectionObject
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
		auto ptr = RefPtr<WeakRefTest1>::MakeRef();
		weak = tr::WeakRefPtr<WeakRefTest1>(ptr);
		ASSERT_EQ(true, weak.IsAlive());
		ASSERT_EQ(100, weak.Resolve()->m);
		ASSERT_EQ(1, WeakRefTest1::count);	// まだ生きている
	}
	ASSERT_EQ(0, WeakRefTest1::count);	// デストラクタが動いた
	ASSERT_EQ(false, weak.IsAlive());
	ASSERT_EQ(true, weak.Resolve().IsNull());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic2)
{
	tr::WeakRefPtr<WeakRefTest1> weak;
	{
		WeakRefTest1 t1;	// ローカルオブジェクト
		WeakRefTest1* ptr = &t1;
		weak = tr::WeakRefPtr<WeakRefTest1>(ptr);
		ASSERT_EQ(true, weak.IsAlive());
		ASSERT_EQ(100, weak.Resolve()->m);
		ASSERT_EQ(1, WeakRefTest1::count);	// まだ生きている
	}
	ASSERT_EQ(0, WeakRefTest1::count);	// デストラクタが動いた
	ASSERT_EQ(false, weak.IsAlive());
	ASSERT_EQ(true, weak.Resolve().IsNull());
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
		//t1.RequestWeakRefInfo();
	}
}
