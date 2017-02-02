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
	auto mesh1 = BoxMesh::Create();

	auto mesh2 = BoxMesh::Create(2, 1, 1);
	mesh2->SetPosition(0, 0, 2);

	auto mesh3 = BoxMesh::Create(Vector3(1, 2, 1));
	mesh3->SetPosition(-2, 0, 0);

	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_StaticMesh.BoxMesh1.png")));
}

