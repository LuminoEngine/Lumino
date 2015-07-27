#include <TestConfig.h>

class Test_Graphics_GeometryRenderer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_GeometryRenderer, DrawRect)
{
	RefPtr<GeometryRenderer> gr(GeometryRenderer::Create(TestEnv::Manager));

	Renderer* r = TestEnv::BeginRendering();

	gr->SetTransform(Matrix::Identity);
	gr->SetViewProjTransform(Matrix::Identity);
	gr->BeginPass();
	gr->DrawRect(RectF(0, 0, 1, 1), RectF(0, 0, 1, 1), ColorF::Red);
	gr->EndPass();

	TestEnv::EndRendering();

	//TestEnv::SaveScreenShot(LOCALFILE("TestData/Test_Graphics_GeometryRenderer.DrawRect.png"));
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_GeometryRenderer.DrawRect.png")));
}
