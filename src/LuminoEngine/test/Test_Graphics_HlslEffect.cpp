#include "Common.hpp"

class Test_Graphics_HlslEffect : public ::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_HlslEffect, Basic)
{
	auto shader1 = newObject<Shader>(LN_ASSETFILE("test.fx"));
}

