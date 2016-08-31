#include <TestConfig.h>
#include <Lumino/Graphics/DrawingContext.h>
#include "../../src/Graphics/RendererImpl.h"

class Test_Graphics_DrawingContext : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DrawingContext, DrawRectangle)
{
	// <Test> 単色塗りつぶし
	{
		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::Gray);
		g->SetBrush(ColorBrush::Red);
		g->DrawRectangle(RectF(10, 20, 30, 40));
		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawRectangle.png")));
	}
	// <Test> テクスチャを描画する
	{
		auto brush = TextureBrush::Create(LN_LOCALFILE("TestData/img1_BYTE_R8G8B8A8_20x20.png"));

		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::White);
		g->SetBrush(brush);
		g->DrawRectangle(RectF(0, 0, 20, 20));
		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawRectangle2.png")));
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

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawRectangle3.png")));
	}
	// <Test> BorderFrame + Tile
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(Rect(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode::Tile);
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush2->SetSourceRect(Rect(0, 160, 32, 32));
		brush2->SetWrapMode(BrushWrapMode::Tile);
		brush2->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush2->SetBorderThickness(ThicknessF(0, 2, 4, 8));

		auto brush3 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush3->SetSourceRect(Rect(0, 160, 32, 32));
		brush3->SetWrapMode(BrushWrapMode::Tile);
		brush3->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush3->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::Gray);

		// 厚さ均一
		g->SetBrush(brush1);
		g->DrawRectangle(RectF(0, 0, 43, 63));

		// 厚さ差あり
		g->SetBrush(brush2);
		g->DrawRectangle(RectF(60, 0, 43, 63));

		// 縮退 + 厚さ均一
		g->SetBrush(brush3);
		g->DrawRectangle(RectF(0, 80, 6, 4));

		// 縮退 + 厚さ差あり
		g->SetBrush(brush2);
		g->DrawRectangle(RectF(60, 80, 6, 4));

		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawRectangle4.png")));
	}
	// <Test> BorderFrame + Stretch
	// <Test> BoxFrame + Stretch
	// <Test> BoxFrame + Tile
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(Rect(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode::Stretch);
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush2->SetSourceRect(Rect(0, 192, 32, 32));
		brush2->SetWrapMode(BrushWrapMode::Stretch);
		brush2->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush2->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush3 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush3->SetSourceRect(Rect(0, 192, 32, 32));
		brush3->SetWrapMode(BrushWrapMode::Tile);
		brush3->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush3->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		Engine::BeginRendering();
		auto* g = Engine::GetMainDrawingContext();
		g->Clear(ClearFlags::All, Color::Gray);
		g->SetBrush(brush1);
		g->DrawRectangle(RectF(0, 0, 43, 63));
		g->SetBrush(brush2);
		g->DrawRectangle(RectF(50, 0, 43, 63));
		g->SetBrush(brush3);
		g->DrawRectangle(RectF(100, 0, 43, 63));
		Engine::EndRendering();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawRectangle5.png")));
	}
}
#if 0
//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DrawingContext, FrameTextureBrush)
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

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.FrameTextureBrush.png")));
}
#endif
//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DrawingContext, DrawText1)
{
	Engine::BeginRendering();
	auto* g = Engine::GetMainDrawingContext();
	g->Set2DRenderingMode();
	g->Clear(ClearFlags::All, Color::Gray);
	g->SetBrush(ColorBrush::Red);
	g->DrawText(_T("DrawText"), PointF(10, 10));

	Engine::EndRendering();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawText1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DrawingContext, DrawText_UserFont)
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

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Graphics_DrawingContext.DrawText_UserFont.png")));
}

