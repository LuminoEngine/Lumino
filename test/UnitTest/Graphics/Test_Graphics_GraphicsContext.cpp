#include <TestConfig.h>
#include "../../src/Graphics/RendererImpl.h"

class Test_Graphics_GraphicsContext : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_GraphicsContext, FrameTextureBrush)
{
	auto brush1 = FrameTextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
	brush1->SetSourceRect(Rect(0, 192, 32, 32));
	brush1->SetInnerAreaSourceRect(Rect(0, 64, 64, 64));
	brush1->SetWrapMode(BrushWrapMode_Stretch);
	brush1->SetThickness(8);


	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::Gray);
	auto* g = Engine::BeginRendering();

	g->PushState();

	Engine::Render();

	g->PopState();

	g->Set2DRenderingMode();
	g->Clear(ClearFlags::All, ColorF::Gray);
	g->SetBrush(brush1);
	//g->DrawRectangle(Rect(0, 0, 43 * 3, 43*2), Color::White);
	g->DrawRectangle(Rect(0, 0, 83, 83), Color::White);
	g->Flush();

	Engine::EndRendering();

	TestEnv::SaveScreenShot(LN_LOCALFILE("TestData/test.png"));
}

