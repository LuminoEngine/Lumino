#include "Common.hpp"
#include <LuminoEngine/Base/Promise.hpp>

class Test_Base_Promise : public ::testing::Test {};

LN_PROMISE()
using TestPromise1 = Promise<Ref<Object>>;

static Ref<TestPromise1> createObjectAsync()
{
	return TestPromise1::run([](TestPromise1* p) {
		p->resolve(makeObject<Object>());
	});
}

TEST_F(Test_Base_Promise, Basic)
{


	Ref<Object> result;
	createObjectAsync()
		->thenWith([&](Ref<Object> value) {
			result = value;
		});

	while (!result) TestEnv::updateFrame();
}

