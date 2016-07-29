#include <TestConfig.h>
#include <Lumino/Graphics/DrawingContext.h>
#include "../../src/Graphics/RendererImpl.h"

class Test_Graphics_GraphicsContext : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_GraphicsContext, DrawRectangle)
{
	// <Test>
	{
		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::Gray);
		g->SetBrush(ColorBrush::Red);
		g->DrawRectangle(RectF(10, 20, 30, 40));
		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.DrawRectangle.png")));
	}
	// <Test> テクスチャ全体を拡大して描画する
	{
		auto brush = TextureBrush::Create(LN_LOCALFILE("TestData/img1_BYTE_R8G8B8A8_20x20.png"));

		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::White);
		g->SetBrush(brush);
		g->DrawRectangle(RectF(10, 20, 30, 40));
		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.DrawRectangle2.png")));
	}
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(Rect(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode_Stretch);	// TODO:
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::Gray);
		g->SetBrush(brush1);
		g->DrawRectangle(RectF(0, 0, 83, 83));
		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.DrawRectangle3.png"), 100, true));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_GraphicsContext, FrameTextureBrush)
{
	auto brush1 = FrameTextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
	brush1->SetSourceRect(Rect(0, 192, 32, 32));
	brush1->SetInnerAreaSourceRect(Rect(0, 64, 64, 64));
	brush1->SetWrapMode(BrushWrapMode_Stretch);	// TODO:
	brush1->SetThickness(8);


	Engine::BeginRendering();
	auto* g = Engine::GetMainDrawingContext();
	g->Clear(ClearFlags::All, Color::Gray);
	g->SetBrush(brush1);
	g->DrawRectangle(Rect(0, 0, 83, 83));

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.FrameTextureBrush_test.png"), 100, true));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_GraphicsContext, DrawText1)
{
	Engine::BeginRendering();
	auto* g = Engine::GetMainDrawingContext();
	g->Set2DRenderingMode();
	g->Clear(ClearFlags::All, Color::Gray);
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

	Engine::BeginRendering();
	auto* g = Engine::GetMainDrawingContext();
	g->Set2DRenderingMode();
	g->Clear(ClearFlags::All, Color::Gray);
	g->SetBrush(ColorBrush::Red);
	g->SetFont(font);
	g->DrawText(_T("DrawText"), PointF(10, 10));

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_GraphicsContext.DrawText_UserFont.png")));
}

