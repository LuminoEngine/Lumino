#include "Common.hpp"
#include <LuminoCore/Base/RefObject.hpp>

//==============================================================================
//# RefObject
class Test_Base_RefObject : public ::testing::Test { };

class TestObjA : public RefObject
{
public:
	int a = 100;
};

class TestObjB : public TestObjA
{
public:
};

class TestObjC;

static void TestFunc1(const Ref<TestObjA>& ptr)
{
	Ref<TestObjC> p1;
}

//## usage
TEST_F(Test_Base_RefObject, Construct)
{
	TestObjA obj1;
	TestObjA* ptr1 = nullptr;

	Ref<TestObjA> ref1;
	Ref<TestObjA> ref2 = nullptr;

	ASSERT_EQ(nullptr, ref1.get());
	ASSERT_EQ(nullptr, ref2.get());

	//* [ ] set raw pointer
	{
		{
			Ref<TestObjA> ref3(&obj1);
			ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));

			Ref<TestObjA> ref4(&obj1, false);
			ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
			RefObjectHelper::retain(&obj1);

			Ref<TestObjA> ref5(&obj1, true);
			ASSERT_EQ(4, RefObjectHelper::getReferenceCount(&obj1));

			ASSERT_EQ(&obj1, ref3.get());
			ASSERT_EQ(&obj1, ref4.get());
			ASSERT_EQ(&obj1, ref5.get());
		}
		ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
	}

	//* [ ] copy
	{
		TestObjA obj2;
		TestObjB obj3;

		Ref<TestObjA> ref3(&obj2);
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(&obj2, ref3.get());

		Ref<TestObjA> ref4(ref3);
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(&obj2, ref4.get());

		Ref<TestObjB> ref5(&obj3);

		// other ptr
		Ref<TestObjA> ref6(&obj3);
		Ref<TestObjA> ref7(ref5);

		ASSERT_EQ(4, RefObjectHelper::getReferenceCount(&obj3));
		ASSERT_EQ(&obj3, ref6.get());
		ASSERT_EQ(&obj3, ref7.get());
	}

	//* [ ] move
	{
		TestObjA obj2;
		TestObjB obj3;

		Ref<TestObjA> ref3(&obj2);
		Ref<TestObjB> ref4(&obj3);

        // ダウンキャストなのでNG
		//Ref<TestObjB> ref5(std::move(ref3));
		//Ref<TestObjB> ref6(std::move(ref4));

		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj3));
	}

	TestFunc1(ptr1);	// can implicit cast
}

//## usage
TEST_F(Test_Base_RefObject, Assign)
{
	TestObjA obj1;
	TestObjB obj2;
	Ref<TestObjA> ref1(&obj1);
	Ref<TestObjB> ref2(&obj2);
	Ref<TestObjB> nullref;

	//* [ ] copy
	{
		Ref<TestObjA> ref3;
		Ref<TestObjA> ref4;

		ref3 = ref1;
		ref4 = ref2;
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(&obj1, ref3.get());
		ASSERT_EQ(&obj2, ref4.get());

		ref3 = nullptr;
		ref4 = nullptr;
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(nullptr, ref3.get());
		ASSERT_EQ(nullptr, ref4.get());

		ref3 = ref1;
		ref4 = ref2;
		ref3 = nullref;
		ref4 = nullref;
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(nullptr, ref3.get());
		ASSERT_EQ(nullptr, ref4.get());

		// self assign
		ref3 = ref1;
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj1));
		ref3 = ref3;
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(&obj1, ref3.get());
	}

	//* [ ] move
	{
		Ref<TestObjA> ref3;
		Ref<TestObjA> ref4;

		ref3 = std::move(ref1);
		ref4 = std::move(ref2);
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
		ASSERT_EQ(nullptr, ref1.get());
		ASSERT_EQ(nullptr, ref2.get());
		ASSERT_EQ(&obj1, ref3.get());
		ASSERT_EQ(&obj2, ref4.get());

		// self assign
		ref1 = &obj1;
		ref3 = ref1;
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj1));
		ref3 = std::move(ref3);
		ASSERT_EQ(3, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(&obj1, ref3.get());
	}

	ref1 = nullptr;
	ref2 = nullptr;
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));

	//* [ ] raw ptr
	{
		Ref<TestObjA> ref3;
		Ref<TestObjA> ref4;

		ref3 = &obj1;
		ref4 = &obj2;
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
		ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
	}
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
}

TEST_F(Test_Base_RefObject, reset)
{
	TestObjA obj1;

	Ref<TestObjA> ref1;
	ref1.reset();

	ref1.reset(&obj1);
	ASSERT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
	ASSERT_EQ(&obj1, ref1.get());

	ref1.reset();
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
	ASSERT_EQ(nullptr, ref1.get());

	ref1.reset(&obj1, false);
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
	ASSERT_EQ(&obj1, ref1.get());

	TestObjA* obj1p = ref1.detach();
	ASSERT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
	ASSERT_EQ(nullptr, ref1.get());

	ASSERT_EQ(&obj1, obj1p);
}

TEST_F(Test_Base_RefObject, Operators)
{
	TestObjA obj1;
	Ref<TestObjA> ref1(&obj1);
	Ref<TestObjA> ref2;


	ASSERT_EQ(100, (*ref1).a);
	ASSERT_EQ(100, ref1->a);

	bool r1 = ref1;
	ASSERT_EQ(true, r1);

	ref1 = nullptr;
	bool r2 = ref1;
	ASSERT_EQ(false, r2);



	TestObjA obj2;
	ref1 = &obj1;
	ref2 = &obj1;

	ASSERT_EQ(true, ref1 == ref2);
	ASSERT_EQ(false, ref1 != ref2);

	ref1 < nullptr;
}
