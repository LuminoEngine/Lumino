#include <TestConfig.h>
#include <Lumino/Mesh/Mesh.h>
#include "../../../Source/Graphics/GraphicsManager.h"


//==============================================================================
class Test_Graphics_Color : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Constructor)
{
	Color32 c1;
	ASSERT_EQ(Color32(0, 0, 0, 0), c1);
	Color32 c2(1, 2, 3);
	ASSERT_EQ(Color32(1, 2, 3, 255), c2);
	Color32 c3(1, 2, 3, 4);
	ASSERT_EQ(Color32(1, 2, 3, 4), c3);
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Operators)
{
	Color32 c1(1, 2, 3, 4);
	Color32 c2(1, 2, 3, 4);
	Color32 c3(1, 2, 3, 5);
	ASSERT_EQ(true, c1 == c2);
	ASSERT_EQ(false, c1 == c3);
	ASSERT_EQ(true, c1 != c3);
	ASSERT_EQ(false, c1 != c2);
}

//==============================================================================
class Test_Graphics_Viewport : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Viewport, Basic)
{
	Size size;
	size = Engine::getMainViewport()->GetViewSize();
	ASSERT_EQ(160, size.width);
	ASSERT_EQ(120, size.height);
}

//==============================================================================
class Test_Graphics_Rendering : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Basic)
{

	//if (Engine::BeginRendering())
	//{
	//	Engine::renderFrame();

	//	auto* r = Engine::getWorld2D()->getRenderer();
	//	r->Clear(ClearFlags::Color, Color::Red);
	//	auto tex = Texture2D::create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));
	//	r->DrawSprite2D(Size(32, 32), tex, Rect(0, 0, 32, 32), Color::White);

	//	Engine::endRendering();
	//}

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Basic1.png"), 99, true));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, clear)
{
	// <Test> 2D ビューのクリア
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto* r1 = Engine::getWorld2D()->getRenderer();
		r1->clear(ClearFlags::Color, Color::Blue);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear1.png")));
	}
	// <Test> 3D ビューのクリア
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto* r1 = Engine::getWorld3D()->getRenderer();
		r1->clear(ClearFlags::Color, Color::Red);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear2.png")));
	}
	// <Test> 2D、3D ビュークリアの組み合わせ。2D が手前になる
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->clear(ClearFlags::Color, Color::Blue);
		Engine::getWorld3D()->getRenderer()->clear(ClearFlags::Color, Color::Red);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear3.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawLinePrimitive)
{
	// <Test> 1本
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->DrawLinePrimitive(
			Vector3(0, 0, 0), Color::Red,
			Vector3(50, 50, 0), Color::White);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawLinePrimitive1.png")));
	}
	//// <Test> 2本連続
	//{
	//	if (Engine::BeginRendering())
	//	{
	//		Engine::renderFrame();
	//		Engine::getWorld2D()->getRenderer()->DrawLinePrimitive(
	//			Vector3(0, 0, 0), Color::Red,
	//			Vector3(20, 50, 0), Color::White);
	//		Engine::getWorld2D()->getRenderer()->DrawLinePrimitive(
	//			Vector3(20, 50, 0), Color::Red,
	//			Vector3(0, 50, 0), Color::Blue);
	//		Engine::endRendering();
	//	}
	//	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawLinePrimitive2.png")));
	//}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSquarePrimitive)
{
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld3D()->getRenderer()->DrawSquarePrimitive(
			Vector3(-1, 2, 0), Vector2(0, 0), Color::Red,
			Vector3(-1, -2, 0), Vector2(0, 1), Color::Green,
			Vector3(1, 2, 0), Vector2(1, 0), Color::Blue,
			Vector3(1, -2, 0), Vector2(1, 1), Color::White);
		Engine::getWorld2D()->getRenderer()->DrawSquarePrimitive(
			Vector3(0, 0, 0), Vector2(0, 0), Color::Red,
			Vector3(0, 50, 0), Vector2(0, 1), Color::Green,
			Vector3(50, 0, 0), Vector2(1, 0), Color::Blue,
			Vector3(50, 50, 0), Vector2(1, 1), Color::White);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSquarePrimitive1_.png"), 95, true));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawBox)
{
	ScopedCameraPosition cp(5, 5, -5);
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto r = Engine::getWorld3D()->getRenderer();
		r->DrawBox(Box(1));
		r->SetTransform(Matrix::makeTranslation(3, 0, 0));
		r->DrawBox(Box(2));
		r->SetTransform(Matrix::makeRotationY(Math::PI / 4) * Matrix::makeTranslation(-3, 0, 0));
		r->DrawBox(Box(2));
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawBox1.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSphere)
{
	ScopedCameraPosition cp(3, 3, -3);
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto context = Engine::getWorld3D()->getRenderer();

		context->DrawSphere(2, 10, 10, Color::Red);
		context->DrawSphere(1, 4, 4, Color::Green, Matrix::makeTranslation(2, 0, 0));

		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSphere1.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawCylinder)
{
	ScopedCameraPosition cp(2, 2, 2);
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto context = Engine::getWorld3D()->getRenderer();

		float r = 0.1f;
		float d = 1.0f;
		context->DrawCylinder(r, d, 8, 1, Color::Red, Matrix::makeRotationZ(Math::PIDiv2) * Matrix::makeTranslation(d / 2, 0, 0));
		context->DrawCylinder(r, d, 8, 1, Color::Green, Matrix::makeTranslation(0, d / 2, 0));
		context->DrawCylinder(r, d, 8, 1, Color::Blue, Matrix::makeRotationX(Math::PIDiv2) * Matrix::makeTranslation(0, 0, d / 2));

		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawCylinder1.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawMesh)
{
	//{
	//	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	//	mesh->InitializeSquarePlane(detail::GraphicsManager::getInstance(), Vector2(2, 1), Vector3::UnitY, MeshCreationFlags::None);
	//	mesh->GetMeshResource()->setPosition(0, Vector3(-2, 1, 0));
	//	mesh->GetMeshResource()->SetNormal(0, Vector3(1, 0, 0));
	//	mesh->GetMeshResource()->setPosition(1, Vector3(-2, -1, 0));
	//	mesh->GetMeshResource()->SetNormal(1, Vector3(1, 1, 0));
	//	mesh->GetMeshResource()->setPosition(2, Vector3(2, 1, 0));
	//	mesh->GetMeshResource()->SetNormal(2, Vector3(0, 1, 0));
	//	mesh->GetMeshResource()->setPosition(3, Vector3(2, -1, 0));
	//	mesh->GetMeshResource()->SetNormal(3, Vector3(0, 0, 1));
	//	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	//	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	//	while (1)
	//	{
	//		Engine::BeginFrameUpdate();	// update camera transform
	//		if (Engine::BeginRendering())
	//		{
	//			Engine::renderFrame();
	//			Engine::getWorld3D()->getRenderer()->DrawMesh(mesh, 0, mesh->GetMeshResource()->GetMaterial(0));
	//			Engine::endRendering();
	//		}
	//		Engine::EndFrameUpdate();
	//	}
	//	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	//}
	{
		auto mesh = RefPtr<StaticMeshModel>::makeRef();
		mesh->InitializeSphere(detail::GraphicsManager::getInstance(), 2, 8, 4, MeshCreationFlags::None);

		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld3D()->getRenderer()->DrawMesh(mesh->GetMeshResource(0), 0, mesh->GetMaterial(0));
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1_1.png"), 95, true));	// 一致率 95%。もし真っ黒になったりしたらわかる
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, blit)
{
	// <Test> デフォルトのレンダーターターゲットへの転送
	{
		auto tex = Texture2D::create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->blit(tex);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
	// <Test> 別のレンダーターゲットへの転送
	{
		auto tex = Texture2D::create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
		auto rt1 = RenderTargetTexture::create(tex->getSize());
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->blit(tex, rt1, nullptr);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Blit1.png")));	// まだこの時点では灰色

		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->blit(rt1);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawText_)
{
	auto font = Font::getDefault();
	float w = 160;

	// <Issue> フレームをまたいだ連続描画で、後のテキストが表示されない問題
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->SetFont(font);
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::White);
		Engine::getWorld2D()->getRenderer()->DrawText_(_T("Lumino"), Rect(0, 0, w, 100), StringFormatFlags::LeftAlignment);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText1.png")));
	}
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->SetFont(font);
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::White);
		Engine::getWorld2D()->getRenderer()->DrawText_(_T("Text1"), Rect(0, 0, w, 100), StringFormatFlags::LeftAlignment);
		Engine::getWorld2D()->getRenderer()->DrawText_(_T("Text2"), Rect(0, 0, w, 100), StringFormatFlags::CenterAlignment);
		Engine::getWorld2D()->getRenderer()->DrawText_(_T("Text3"), Rect(0, 0, w, 100), StringFormatFlags::RightAlignment);
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText2.png")));
	}
	// <Test> transform
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->SetFont(font);
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::White);
		Engine::getWorld2D()->getRenderer()->SetTransform(Matrix::makeTranslation(10, 20, 0));
		Engine::getWorld2D()->getRenderer()->DrawText_(_T("Text1"), PointF());
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText3.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSprite)
{
	auto tex = Texture2D::create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));

	// <Test>
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();

		auto* r = Engine::getWorld2D()->getRenderer();
		r->setBlendMode(BlendMode::Alpha);

		r->DrawSprite(Vector3(48, 0, 10), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);	// 手前
		r->DrawSprite(Vector3(0, 0, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(32, 0, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->DrawSprite(Vector3(0, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color(1, 0, 0, 1), SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(32, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color(0, 1, 0, 1), SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(64, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color(0, 0, 1, 1), SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(96, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color(1, 0, 0, 0.5), SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(128, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color(1, 0, 0, 0), SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->DrawSprite(Vector3(0, 64, 0), Size(16, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(16, 64, 0), Size(32, 16), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(16, 80, 0), Size(16, 16), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->DrawSprite(Vector3(64, 64, 0), Size(16, 16), Vector2::Zero, tex, Rect(16, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(80, 64, 0), Size(16, 16), Vector2::Zero, tex, Rect(0, 16, 32, 16), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(64, 80, 0), Size(16, 16), Vector2::Zero, tex, Rect(16, 16, 16, 16), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(80, 80, 0), Size(16, 16), Vector2::Zero, tex, Rect(0, 0, 16, 16), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->DrawSprite(Vector3(96, 64, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(128, 64, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 16), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		LN_TEST_END_FRAME;

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite1.png"), 95));
	}

	// <Test> Sprite の BlendMode
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();

		auto* r = Engine::getWorld2D()->getRenderer();

		r->setBlendMode(BlendMode::Normal); 
		r->DrawSprite(Vector3(0, 0, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(64, 0, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(0, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(64, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
		r->DrawSprite(Vector3(0, 64, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
			
		r->setBlendMode(BlendMode::Normal);
		r->DrawSprite(Vector3(16, 0, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->setBlendMode(BlendMode::Alpha);
		r->DrawSprite(Vector3(80, 0, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->setBlendMode(BlendMode::Add);
		r->DrawSprite(Vector3(16, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);
			
		r->setBlendMode(BlendMode::Subtract);
		r->DrawSprite(Vector3(80, 32, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		r->setBlendMode(BlendMode::Multiply);
		r->DrawSprite(Vector3(16, 64, 0), Size(32, 32), Vector2::Zero, tex, Rect(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D, ln::BillboardType::None);

		LN_TEST_END_FRAME;

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite2.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawRectangle)
{
	// <Test> 単色塗りつぶし
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::Red);
		Engine::getWorld2D()->getRenderer()->DrawRectangle(Rect(10, 20, 30, 40));
		Engine::getWorld2D()->getRenderer()->DrawRectangle(Rect(110, 20, 30, 40));
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::Blue);
		Engine::getWorld2D()->getRenderer()->DrawRectangle(Rect(10, 70, 30, 40));
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawRectangle1.png")));
	}
	// <Issue> もう一度同じことができる
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::Red);
		Engine::getWorld2D()->getRenderer()->DrawRectangle(Rect(10, 20, 30, 40));
		Engine::getWorld2D()->getRenderer()->DrawRectangle(Rect(110, 20, 30, 40));
		Engine::getWorld2D()->getRenderer()->setBrush(Brush::Blue);
		Engine::getWorld2D()->getRenderer()->DrawRectangle(Rect(10, 70, 30, 40));
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawRectangle1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawFrameRectangle)
{
	// <Test> BorderFrame + Tile
	{
		auto brush1 = TextureBrush::create(LN_LOCALFILE("TestData/Window.png"));
		brush1->setSourceRect(0, 192, 32, 32);
		brush1->setWrapMode(BrushWrapMode::Tile);
		brush1->getImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->getBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::create(LN_LOCALFILE("TestData/Window.png"));
		brush2->setSourceRect(0, 160, 32, 32);
		brush2->setWrapMode(BrushWrapMode::Tile);
		brush2->getImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush2->getBorderThickness(ThicknessF(0, 2, 4, 8));

		auto brush3 = TextureBrush::create(LN_LOCALFILE("TestData/Window.png"));
		brush3->setSourceRect(0, 160, 32, 32);
		brush3->setWrapMode(BrushWrapMode::Tile);
		brush3->getImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush3->getBorderThickness(ThicknessF(8, 8, 8, 8));

		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();

		auto* g = Engine::getWorld2D()->getRenderer();
		g->clear(ClearFlags::All, Color::Gray);

		// 厚さ均一
		g->setBrush(brush1);
		g->DrawRectangle(Rect(0, 0, 43, 63));

		// 厚さ差あり
		g->setBrush(brush2);
		g->DrawRectangle(Rect(60, 0, 43, 63));

		// 縮退 + 厚さ均一
		g->setBrush(brush3);
		g->DrawRectangle(Rect(0, 80, 6, 4));

		// 縮退 + 厚さ差あり
		g->setBrush(brush2);
		g->DrawRectangle(Rect(60, 80, 6, 4));

		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawFrameRectangle1.png")));
	}
	
	// <Test> BorderFrame + Stretch
	// <Test> BoxFrame + Stretch
	// <Test> BoxFrame + Tile
	{
		auto brush1 = TextureBrush::create(LN_LOCALFILE("TestData/Window.png"));
		brush1->setSourceRect(0, 192, 32, 32);
		brush1->setWrapMode(BrushWrapMode::Stretch);
		brush1->getImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->getBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::create(LN_LOCALFILE("TestData/Window.png"));
		brush2->setSourceRect(0, 192, 32, 32);
		brush2->setWrapMode(BrushWrapMode::Stretch);
		brush2->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush2->getBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush3 = TextureBrush::create(LN_LOCALFILE("TestData/Window.png"));
		brush3->setSourceRect(0, 192, 32, 32);
		brush3->setWrapMode(BrushWrapMode::Tile);
		brush3->getImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush3->getBorderThickness(ThicknessF(8, 8, 8, 8));

		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();

		auto* g = Engine::getWorld2D()->getRenderer();
		g->clear(ClearFlags::All, Color::Gray);
		g->setBrush(brush1);
		g->DrawRectangle(Rect(0, 0, 43, 63));
		g->setBrush(brush2);
		g->DrawRectangle(Rect(50, 0, 43, 63));
		g->setBrush(brush3);
		g->DrawRectangle(Rect(100, 0, 43, 63));
		Engine::presentFrame();
		LN_TEST_END_FRAME;

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawFrameRectangle2.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, ZSort)
{
	auto t1 = Texture2D::create(LN_LOCALFILE("TestData/Sprite2.png"));
	auto s1 = Sprite3DComponent::create(5, 5, t1);
	auto s2 = Sprite3DComponent::create(8, 2, t1);
	s1->setPosition(0, 0, 0);	// 手前
	s2->setPosition(0, 0, 1);	// 奥
	Engine::update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.ZSort1.png"), 90));
}


//==============================================================================
class Test_Graphics_DrawingContext : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DrawingContext, DrawChar)
{
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto* dc = Engine::getWorld2D()->getRenderer();
		//auto* dc = Engine::getMainWindow()->GetDrawingContext();
		dc->clear(ClearFlags::Color, Color::Blue);
		dc->setBrush(Brush::Black);
		dc->DrawChar('g', PointF(100, 100));
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_DrawingContext.DrawChar1.png"), 90, true));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_DrawingContext, DrawText_)
{
	//auto f = Font::create(_T("Meiryo UI"), 30);
	{
		LN_TEST_BEGIN_FRAME;
		Engine::renderFrame();
		auto* dc = Engine::getMainWindow()->GetDrawingContext();
		dc->clear(ClearFlags::Color, Color::White);
		dc->setBrush(Brush::Gray);
		//dc->SetFont(f);
		dc->setBlendMode(BlendMode::Alpha);
		dc->DrawText2(_T("テッセレーションとアウトラ\nインフォントの描画ができ\nるようになったんだね！\nすごーい！"), Rect(100, 100, 100, 100));
		LN_TEST_END_FRAME;
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_DrawingContext.DrawText1.png"), 90, true));
	}
}

//==============================================================================
class Test_Graphics_Texture : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, blit)
{
	auto tex1 = Texture2D::create(LN_LOCALFILE("TestData/Numbers1.png"));
	auto tex2 = Texture2D::create(100, 100);
	tex2->blit(20 * 0, 20 * 0, tex1, RectI(8 * 0, 14 * 0, 8, 14));
	tex2->blit(20 * 1, 20 * 1, tex1, RectI(8 * 1, 14 * 1, 8, 14));
	tex2->blit(20 * 2, 20 * 2, tex1, RectI(8 * 2, 14 * 2, 8, 14));
	tex2->blit(20 * 3, 20 * 3, tex1, RectI(8 * 3, 14 * 3, 8, 14));

	auto sprite = Sprite2DComponent::create(tex2);
	sprite->setBlendMode(BlendMode::Alpha);
	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.Blit1.png")));
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, drawText)
{
	auto font = Font::create();
	auto texture = Texture2D::create(160, 120);
	texture->drawText(_T("Left"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
	texture->drawText(_T("Center"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Center);
	texture->drawText(_T("Rigth"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Right);
	//texture->drawText("Justify", Rect(0, 32, 120, 160), font, Color32::White, Color32::White, 0, TextAlignment::Justify);
	auto sprite = Sprite2DComponent::create(texture);
	sprite->setBlendMode(BlendMode::Alpha);
	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.DrawText1.png")));
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Issues)
{
	// <Issue> Clear の後に Drawtext すると、Clear されていない問題の修正。
	// <Issue> Drawtext 連続で描画すると、1フレーム前の内容がクリアされる問題の修正。
	{
		auto font = Font::create();
		auto texture = Texture2D::create(160, 120);
		texture->drawText(_T("__________"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		auto sprite = Sprite2DComponent::create(texture);
		Engine::update();

		texture->clear(Color32(0, 0, 0, 0));
		texture->drawText(_T("Clear1"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		Engine::update();

		texture->drawText(_T("Clear2"), RectI(0, 32, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		Engine::update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.Clear1.png")));
	}
}

