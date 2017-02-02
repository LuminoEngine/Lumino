#include <TestConfig.h>

class Test_Scene_StaticMesh : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_StaticMesh, BoxMesh)
{
	ScopedCameraPosition cp(2, 2, -2);

	// <Rest> Create
	{
		ScopedCameraPosition cp(2, 2, -2);
		auto mesh1 = BoxMesh::Create();

		auto mesh2 = BoxMesh::Create(2, 1, 1);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = BoxMesh::Create(Vector3(1, 2, 1));
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.BoxMesh1.png"), 95));
	}

	// <Rest> ReverseFaces
	{
		auto mesh1 = BoxMesh::Create(1, 1, 1);
		mesh1->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();

		auto mesh2 = BoxMesh::Create(2, 1, 1);
		mesh2->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = BoxMesh::Create(Vector3(1, 2, 1));
		mesh3->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.BoxMesh2.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_StaticMesh, SphereMesh)
{
	ScopedCameraPosition cp(2, 2, -2);

	// <Rest> Create
	{
		ScopedCameraPosition cp(2, 2, -2);
		auto mesh1 = SphereMesh::Create();

		auto mesh2 = SphereMesh::Create(1);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = SphereMesh::Create(1, 4);
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.SphereMesh1.png"), 95));
	}

	// <Rest> ReverseFaces
	{
		auto mesh1 = SphereMesh::Create(0.5);
		mesh1->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();

		auto mesh2 = SphereMesh::Create(1);
		mesh2->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = SphereMesh::Create(1, 4);
		mesh3->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.SphereMesh2.png"), 95));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_StaticMesh, TeapotMesh)
{
	ScopedCameraPosition cp(2, 2, -2);

	// <Rest> Create
	{
		ScopedCameraPosition cp(2, 2, -2);
		auto mesh1 = TeapotMesh::Create();

		auto mesh2 = TeapotMesh::Create(2);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = TeapotMesh::Create(1, 1);
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.TeapotMesh1.png"), 95, true));
	}
}
