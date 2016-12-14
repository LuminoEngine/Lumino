#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>

class Test_Graphics_MeshResource : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_MeshResource, AddSquare)
{
	// 自己照明
	auto material = Material::Create();
	material->SetBuiltinColorParameter(Material::EmissiveParameter, Color::White);

	// <Test> 四角形1つ
	{
		auto mesh = MeshResource::Create();
		mesh->AddSquare(
			Vertex{ Vector3(-2, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Red },
			Vertex{ Vector3(-2, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Green },
			Vertex{ Vector3(2, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Blue },
			Vertex{ Vector3(2, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::White });

		Engine::BeginFrameUpdate();
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, material);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_MeshResource.AddSquare1.png"), 95));
	}

	// <Test> 描画しつつ追加
	{
		auto mesh = MeshResource::Create();

		// 1回目
		mesh->AddSquare(
			Vertex{ Vector3(-2, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Red },
			Vertex{ Vector3(-2, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Green },
			Vertex{ Vector3(0, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Blue },
			Vertex{ Vector3(0, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
		Engine::BeginFrameUpdate();
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, material);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_MeshResource.AddSquare2.png"), 95));

		// 2回目
		mesh->AddSquare(
			Vertex{ Vector3(0, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Red },
			Vertex{ Vector3(0, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Green },
			Vertex{ Vector3(2, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Blue },
			Vertex{ Vector3(2, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
		Engine::BeginFrameUpdate();
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, material);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_MeshResource.AddSquare3.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_MeshResource, Clear)
{
	// 自己照明
	auto material = Material::Create();
	material->SetBuiltinColorParameter(Material::AmbientParameter, Color::White);

	// <Test> 描画しつつ追加
	{
		auto mesh = MeshResource::Create();

		// 1回目
		mesh->AddSquare(
			Vertex{ Vector3(-2, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Red },
			Vertex{ Vector3(-2, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Green },
			Vertex{ Vector3(0, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Blue },
			Vertex{ Vector3(0, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
		Engine::BeginFrameUpdate();
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, material);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_MeshResource.Clear1.png"), 95));

		// 2回目
		mesh->Clear();		// clear
		mesh->AddSquare(
			Vertex{ Vector3(0, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Red },
			Vertex{ Vector3(0, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Green },
			Vertex{ Vector3(2, 0, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::Blue },
			Vertex{ Vector3(2, 2, 0), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
		Engine::BeginFrameUpdate();
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, material);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_MeshResource.Clear2.png"), 95));
	}
}
