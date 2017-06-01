#include <TestConfig.h>

class Test_Animation : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Animation, Basic)
{
	int count = 0;
	float value = 0;
	auto curve = ln::FloatEasingAnimationTimeline::Create(5.0f, 1.0f, EasingMode::Linear);	// リソース。スタイルとかの。
	auto clock = curve->Start(nullptr, &value, 0.0f, [&count]() { count++; });

	for (int i = 0; i < 90; i++) Engine::Update(); // advance 1.5s

	ASSERT_EQ(1, count);
	ASSERT_FLOAT_EQ(5.0f, value);
}

