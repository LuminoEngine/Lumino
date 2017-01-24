#include <TestConfig.h>
#include <Lumino/Graphics/Mesh/Mesh.h>
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
	size = Engine::GetMainViewport()->GetSize();
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
	//	Engine::Render();

	//	auto* r = Engine::GetDefault2DLayer()->GetRenderer();
	//	r->Clear(ClearFlags::Color, Color::Red);
	//	auto tex = Texture2D::Create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));
	//	r->DrawSprite2D(Size(32, 32), tex, RectF(0, 0, 32, 32), Color::White);

	//	Engine::EndRendering();
	//}

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Basic1.png"), 99, true));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Clear)
{
	// <Test> 2D ビューのクリア
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto* r1 = Engine::GetDefault2DLayer()->GetRenderer();
			r1->Clear(ClearFlags::Color, Color::Blue);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear1.png")));
	}
	// <Test> 3D ビューのクリア
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto* r1 = Engine::GetDefault3DLayer()->GetRenderer();
			r1->Clear(ClearFlags::Color, Color::Red);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear2.png")));
	}
	// <Test> 2D、3D ビュークリアの組み合わせ。2D が手前になる
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Clear(ClearFlags::Color, Color::Blue);
			Engine::GetDefault3DLayer()->GetRenderer()->Clear(ClearFlags::Color, Color::Red);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear3.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawLinePrimitive)
{
	// <Test> 1本
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->DrawLinePrimitive(
				Vector3(0, 0, 0), Color::Red,
				Vector3(50, 50, 0), Color::White);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawLinePrimitive1.png")));
	}
	//// <Test> 2本連続
	//{
	//	if (Engine::BeginRendering())
	//	{
	//		Engine::Render();
	//		Engine::GetDefault2DLayer()->GetRenderer()->DrawLinePrimitive(
	//			Vector3(0, 0, 0), Color::Red,
	//			Vector3(20, 50, 0), Color::White);
	//		Engine::GetDefault2DLayer()->GetRenderer()->DrawLinePrimitive(
	//			Vector3(20, 50, 0), Color::Red,
	//			Vector3(0, 50, 0), Color::Blue);
	//		Engine::EndRendering();
	//	}
	//	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawLinePrimitive2.png")));
	//}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSquarePrimitive)
{
	{
		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawSquarePrimitive(
				Vector3(-1, 2, 0), Vector2(0, 0), Color::Red,
				Vector3(-1, -2, 0), Vector2(0, 1), Color::Green,
				Vector3(1, 2, 0), Vector2(1, 0), Color::Blue,
				Vector3(1, -2, 0), Vector2(1, 1), Color::White);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawSquarePrimitive(
				Vector3(0, 0, 0), Vector2(0, 0), Color::Red,
				Vector3(0, 50, 0), Vector2(0, 1), Color::Green,
				Vector3(50, 0, 0), Vector2(1, 0), Color::Blue,
				Vector3(50, 50, 0), Vector2(1, 1), Color::White);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSquarePrimitive1_.png"), 95, true));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawBox)
{
	ScopedCameraPosition cp(5, 5, -5);
	{
		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto r = Engine::GetDefault3DLayer()->GetRenderer();
			r->DrawBox(Box(1));
			r->SetTransform(Matrix::MakeTranslation(3, 0, 0));
			r->DrawBox(Box(2));
			r->SetTransform(Matrix::MakeRotationY(Math::PI / 4) * Matrix::MakeTranslation(-3, 0, 0));
			r->DrawBox(Box(2));
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawBox1.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSphere)
{
	ScopedCameraPosition cp(3, 3, -3);
	{
		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto context = Engine::GetDefault3DLayer()->GetRenderer();

			context->DrawSphere(2, 10, 10, Color::Red);
			context->DrawSphere(1, 4, 4, Color::Green, Matrix::MakeTranslation(2, 0, 0));

			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSphere1.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawCylinder)
{
	ScopedCameraPosition cp(2, 2, 2);
	{
		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto context = Engine::GetDefault3DLayer()->GetRenderer();

			float r = 0.1f;
			float d = 1.0f;
			context->DrawCylinder(r, d, 8, 1, Color::Red, Matrix::MakeRotationZ(Math::PIDiv2) * Matrix::MakeTranslation(d / 2, 0, 0));
			context->DrawCylinder(r, d, 8, 1, Color::Green, Matrix::MakeTranslation(0, d / 2, 0));
			context->DrawCylinder(r, d, 8, 1, Color::Blue, Matrix::MakeRotationX(Math::PIDiv2) * Matrix::MakeTranslation(0, 0, d / 2));

			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawCylinder1.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawMesh)
{
	//{
	//	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	//	mesh->InitializeSquarePlane(detail::GraphicsManager::GetInstance(), Vector2(2, 1), Vector3::UnitY, MeshCreationFlags::None);
	//	mesh->GetMeshResource()->SetPosition(0, Vector3(-2, 1, 0));
	//	mesh->GetMeshResource()->SetNormal(0, Vector3(1, 0, 0));
	//	mesh->GetMeshResource()->SetPosition(1, Vector3(-2, -1, 0));
	//	mesh->GetMeshResource()->SetNormal(1, Vector3(1, 1, 0));
	//	mesh->GetMeshResource()->SetPosition(2, Vector3(2, 1, 0));
	//	mesh->GetMeshResource()->SetNormal(2, Vector3(0, 1, 0));
	//	mesh->GetMeshResource()->SetPosition(3, Vector3(2, -1, 0));
	//	mesh->GetMeshResource()->SetNormal(3, Vector3(0, 0, 1));
	//	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	//	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	//	while (1)
	//	{
	//		Engine::BeginFrameUpdate();	// update camera transform
	//		if (Engine::BeginRendering())
	//		{
	//			Engine::Render();
	//			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, mesh->GetMeshResource()->GetMaterial(0));
	//			Engine::EndRendering();
	//		}
	//		Engine::EndFrameUpdate();
	//	}
	//	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	//}
	{
		auto mesh = RefPtr<StaticMeshModel>::MakeRef();
		mesh->InitializeSphere(detail::GraphicsManager::GetInstance(), 2, 8, 4, MeshCreationFlags::None);

		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, mesh->GetMeshResource()->GetMaterial(0));
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1_1.png"), 95, true));	// 一致率 95%。もし真っ黒になったりしたらわかる
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Blit)
{
	// <Test> デフォルトのレンダーターターゲットへの転送
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Blit(tex);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
	// <Test> 別のレンダーターゲットへの転送
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
		auto rt1 = RenderTargetTexture::Create(tex->GetSize());
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Blit(tex, rt1, nullptr);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Blit1.png")));	// まだこの時点では灰色

		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Blit(rt1);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawText_)
{
	auto font = Font::GetDefault();
	float w = 160;

	// <Issue> フレームをまたいだ連続描画で、後のテキストが表示されない問題
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetFont(font);
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::White);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Lumino"), RectF(0, 0, w, 100), StringFormatFlags::LeftAlignment);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText1.png")));
	}
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetFont(font);
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::White);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text1"), RectF(0, 0, w, 100), StringFormatFlags::LeftAlignment);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text2"), RectF(0, 0, w, 100), StringFormatFlags::CenterAlignment);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text3"), RectF(0, 0, w, 100), StringFormatFlags::RightAlignment);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText2.png")));
	}
	// <Test> Transform
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetFont(font);
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::White);
			Engine::GetDefault2DLayer()->GetRenderer()->SetTransform(Matrix::MakeTranslation(10, 20, 0));
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text1"), PointF::Zero);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText3.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSprite)
{
	auto tex = Texture2D::Create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));

	// <Test>
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* r = Engine::GetDefault2DLayer()->GetRenderer();
			r->SetBlendMode(BlendMode::Alpha);

			r->DrawSprite(Vector3(48, 0, 10), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);	// 手前
			r->DrawSprite(Vector3(0, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(32, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(0, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 1), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(32, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(0, 1, 0, 1), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(0, 0, 1, 1), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(96, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 0.5), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(128, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 0), SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(0, 64, 0), Size(16, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(16, 64, 0), Size(32, 16), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(16, 80, 0), Size(16, 16), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(64, 64, 0), Size(16, 16), Vector2::Zero, tex, RectF(16, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(80, 64, 0), Size(16, 16), Vector2::Zero, tex, RectF(0, 16, 32, 16), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 80, 0), Size(16, 16), Vector2::Zero, tex, RectF(16, 16, 16, 16), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(80, 80, 0), Size(16, 16), Vector2::Zero, tex, RectF(0, 0, 16, 16), Color::White, SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(96, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(128, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 16), Color::White, SpriteBaseDirection::Basic2D);

			Engine::EndRendering();
		}

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite1.png"), 95));
	}

	// <Test> Sprite の BlendMode
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* r = Engine::GetDefault2DLayer()->GetRenderer();

			r->SetBlendMode(BlendMode::Normal); 
			r->DrawSprite(Vector3(0, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(0, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(0, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			
			r->SetBlendMode(BlendMode::Normal);
			r->DrawSprite(Vector3(16, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->SetBlendMode(BlendMode::Alpha);
			r->DrawSprite(Vector3(80, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->SetBlendMode(BlendMode::Add);
			r->DrawSprite(Vector3(16, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			
			r->SetBlendMode(BlendMode::Subtract);
			r->DrawSprite(Vector3(80, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->SetBlendMode(BlendMode::Multiply);
			r->DrawSprite(Vector3(16, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			Engine::EndRendering();
		}

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite2.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawRectangle)
{
	// <Test> 単色塗りつぶし
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Red);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(RectF(10, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(RectF(110, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Blue);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(RectF(10, 70, 30, 40));
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawRectangle1.png")));
	}
	// <Issue> もう一度同じことができる
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Red);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(RectF(10, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(RectF(110, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Blue);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(RectF(10, 70, 30, 40));
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawRectangle1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawFrameRectangle)
{
	// <Test> BorderFrame + Tile
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(RectI(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode::Tile);
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush2->SetSourceRect(RectI(0, 160, 32, 32));
		brush2->SetWrapMode(BrushWrapMode::Tile);
		brush2->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush2->SetBorderThickness(ThicknessF(0, 2, 4, 8));

		auto brush3 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush3->SetSourceRect(RectI(0, 160, 32, 32));
		brush3->SetWrapMode(BrushWrapMode::Tile);
		brush3->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush3->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* g = Engine::GetDefault2DLayer()->GetRenderer();
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

			ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawFrameRectangle1.png")));
		}
	}
	
	// <Test> BorderFrame + Stretch
	// <Test> BoxFrame + Stretch
	// <Test> BoxFrame + Tile
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(RectI(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode::Stretch);
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush2->SetSourceRect(RectI(0, 192, 32, 32));
		brush2->SetWrapMode(BrushWrapMode::Stretch);
		brush2->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush2->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush3 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush3->SetSourceRect(RectI(0, 192, 32, 32));
		brush3->SetWrapMode(BrushWrapMode::Tile);
		brush3->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush3->SetBorderThickness(ThicknessF(8, 8, 8, 8));


		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* g = Engine::GetDefault2DLayer()->GetRenderer();
			g->Clear(ClearFlags::All, Color::Gray);
			g->SetBrush(brush1);
			g->DrawRectangle(RectF(0, 0, 43, 63));
			g->SetBrush(brush2);
			g->DrawRectangle(RectF(50, 0, 43, 63));
			g->SetBrush(brush3);
			g->DrawRectangle(RectF(100, 0, 43, 63));
			Engine::EndRendering();
		}

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawFrameRectangle2.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, ZSort)
{
	auto t1 = Texture2D::Create(LN_LOCALFILE("TestData/Sprite2.png"));
	auto s1 = Sprite3D::Create(5, 5, t1);
	auto s2 = Sprite3D::Create(8, 2, t1);
	s1->SetPosition(0, 0, 0);	// 手前
	s2->SetPosition(0, 0, 1);	// 奥
	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.ZSort1.png"), 90));
}

//==============================================================================
class Test_Graphics_Texture : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Blit)
{
	auto tex1 = Texture2D::Create(LN_LOCALFILE("TestData/Numbers1.png"));
	auto tex2 = Texture2D::Create(100, 100);
	tex2->Blit(20 * 0, 20 * 0, tex1, RectI(8 * 0, 14 * 0, 8, 14));
	tex2->Blit(20 * 1, 20 * 1, tex1, RectI(8 * 1, 14 * 1, 8, 14));
	tex2->Blit(20 * 2, 20 * 2, tex1, RectI(8 * 2, 14 * 2, 8, 14));
	tex2->Blit(20 * 3, 20 * 3, tex1, RectI(8 * 3, 14 * 3, 8, 14));

	auto sprite = Sprite2D::Create(tex2);
	sprite->SetBlendMode(BlendMode::Alpha);
	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.Blit1.png")));
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, DrawText)
{
	auto font = Font::Create();
	auto texture = Texture2D::Create(160, 120);
	texture->DrawText(_T("Left"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
	texture->DrawText(_T("Center"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Center);
	texture->DrawText(_T("Rigth"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Right);
	//texture->DrawText("Justify", Rect(0, 32, 120, 160), font, Color32::White, Color32::White, 0, TextAlignment::Justify);
	auto sprite = Sprite2D::Create(texture);
	sprite->SetBlendMode(BlendMode::Alpha);
	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.DrawText1.png")));
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Issues)
{
	// <Issue> Clear の後に Drawtext すると、Clear されていない問題の修正。
	// <Issue> Drawtext 連続で描画すると、1フレーム前の内容がクリアされる問題の修正。
	{
		auto font = Font::Create();
		auto texture = Texture2D::Create(160, 120);
		texture->DrawText(_T("__________"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		auto sprite = Sprite2D::Create(texture);
		Engine::Update();

		texture->Clear(Color32(0, 0, 0, 0));
		texture->DrawText(_T("Clear1"), RectI(0, 0, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		Engine::Update();

		texture->DrawText(_T("Clear2"), RectI(0, 32, 160, 120), font, Color32::White, Color32::White, 0, TextAlignment::Left);
		Engine::Update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Texture.Clear1.png")));
	}
}

