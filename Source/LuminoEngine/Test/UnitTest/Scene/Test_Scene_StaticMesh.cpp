#include <TestConfig.h>

#if 0

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
		auto mesh1 = StaticMeshComponent::CreateBox();

		auto mesh2 = StaticMeshComponent::CreateBox(2, 1, 1);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMeshComponent::CreateBox(Vector3(1, 2, 1));
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.BoxMesh1.png"), 95));
	}

	// <Rest> reverseFaces
	{
		auto mesh1 = StaticMeshComponent::CreateBox(1, 1, 1);
		mesh1->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();

		auto mesh2 = StaticMeshComponent::CreateBox(2, 1, 1);
		mesh2->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMeshComponent::CreateBox(Vector3(1, 2, 1));
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
		auto mesh1 = StaticMeshComponent::CreateSphere();

		auto mesh2 = StaticMeshComponent::CreateSphere(1);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMeshComponent::CreateSphere(1, 4);
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.SphereMesh1.png"), 95));
	}

	// <Rest> reverseFaces
	{
		auto mesh1 = StaticMeshComponent::CreateSphere(0.5);
		mesh1->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();

		auto mesh2 = StaticMeshComponent::CreateSphere(1);
		mesh2->GetStaticMeshModel()->GetMeshResource()->ReverseFaces();
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMeshComponent::CreateSphere(1, 4);
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
		auto mesh1 = StaticMeshComponent::CreateTeapot();

		auto mesh2 = StaticMeshComponent::CreateTeapot(2);
		mesh2->SetPosition(0, 0, 2);

		auto mesh3 = StaticMeshComponent::CreateTeapot(1, 1);
		mesh3->SetPosition(-2, 0, 0);

		Engine::Update();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.TeapotMesh1.png"), 95, true));
	}
}

#endif
