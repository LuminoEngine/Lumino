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


	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color32::Gray);
	auto* g = Engine::BeginRendering();

	g->Set2DRenderingMode();	// TODO: デフォルトは 2D にして、明示の必要ないようにしたいなぁ・・・
	g->Clear(ClearFlags::All, ColorF::Gray);
	g->SetBrush(brush1);
	g->DrawRectangle(Rect(0, 0, 83, 83), Color32::White);

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.FrameTextureBrush.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_GraphicsContext, DrawText1)
{
	auto* g = Engine::BeginRendering();

	g->Set2DRenderingMode();
	g->Clear(ClearFlags::All, ColorF::Gray);
	g->SetBrush(ColorBrush::Red);
	g->DrawText(_T("DrawText"), PointF(10, 10));

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.DrawText1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_GraphicsContext, DrawText_UserFont)
{
	// TODO: Register しなくても直接読みたい
	Font::RegisterFontFile(LN_LOCALFILE("../../../tools/VLGothic/VL-Gothic-Regular.ttf"));
	auto font = Font::Create();
	font->SetName(_T("VL Gothic"));

	auto* g = Engine::BeginRendering();

	g->Set2DRenderingMode();
	g->Clear(ClearFlags::All, ColorF::Gray);
	g->SetBrush(ColorBrush::Red);
	g->SetFont(font);
	g->DrawText(_T("DrawText"), PointF(10, 10));

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.DrawText_UserFont.png")));
}

