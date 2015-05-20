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
	RefPtr<GeometryRenderer> gr(GeometryRenderer::Create(TestEnvironment::Manager));

	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	r->SetRenderTarget(0, swap->GetBackBuffer());
	r->SetDepthBuffer(swap->GetBackBufferDepth());
	r->Clear(true, true, ColorF(0, 1, 1, 1), 1.0f);

	gr->SetTransform(Matrix::Identity);
	gr->SetViewProjTransform(Matrix::Identity);
	gr->BeginPass();
	gr->DrawRect(RectF(0, 0, 1, 1), RectF(0, 0, 1, 1), ColorF::Red);
	gr->EndPass();

	swap->Present();

	//TestEnvironment::SaveScreenShot(LOCALFILE("TestData/Test_Graphics_GeometryRenderer.DrawRect.png"));
	ASSERT_TRUE(TestEnvironment::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_GeometryRenderer.DrawRect.png")));
}
