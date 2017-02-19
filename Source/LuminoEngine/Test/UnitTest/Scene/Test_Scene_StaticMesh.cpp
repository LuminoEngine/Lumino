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
		auto mesh1 = StaticMesh::CreateBox();

		auto mesh2 = StaticMesh::CreateBox(2, 1, 1);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMesh::CreateBox(Vector3(1, 2, 1));
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.BoxMesh1.png"), 95));
	}

	// <Rest> ReverseFaces
	{
		auto mesh1 = StaticMesh::CreateBox(1, 1, 1);
		mesh1->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();

		auto mesh2 = StaticMesh::CreateBox(2, 1, 1);
		mesh2->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMesh::CreateBox(Vector3(1, 2, 1));
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
		auto mesh1 = StaticMesh::CreateSphere();

		auto mesh2 = StaticMesh::CreateSphere(1);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMesh::CreateSphere(1, 4);
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.SphereMesh1.png"), 95));
	}

	// <Rest> ReverseFaces
	{
		auto mesh1 = StaticMesh::CreateSphere(0.5);
		mesh1->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();

		auto mesh2 = StaticMesh::CreateSphere(1);
		mesh2->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMesh::CreateSphere(1, 4);
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
		auto mesh1 = StaticMesh::CreateTeapot();

		auto mesh2 = StaticMesh::CreateTeapot(2);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMesh::CreateTeapot(1, 1);
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.TeapotMesh1.png"), 95, true));
	}
}
