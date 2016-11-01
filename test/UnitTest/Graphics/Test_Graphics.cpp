#include <TestConfig.h>
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <Lumino/Graphics/Mesh.h>
#include "../../../src/Graphics/GraphicsManager.h"


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
	const SizeI& size = Viewport::GetMainViewport()->GetSize();
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
	//	r->DrawSprite2D(SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color::White);

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
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSquarePrimitive)
{
	{
		Engine::UpdateFrame2();	// update camera transform
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
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSquarePrimitive1.png")));
	}
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawMesh)
{
	{

		auto mesh = RefPtr<StaticMeshModel>::MakeRef();
		mesh->InitializeSphere(detail::GraphicsManager::GetInstance(), 2, 4, 4, MeshCreationFlags::None);

		Engine::UpdateFrame2();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, mesh->GetMeshResource()->GetMaterial(0));
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"), 99, true));
	}
}
