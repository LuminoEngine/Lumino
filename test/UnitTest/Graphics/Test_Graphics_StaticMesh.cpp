#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>

class Test_Graphics_StaticMesh : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_StaticMesh, Box)
{
	auto box = StaticMesh::CreateBox(Vector3(1, 2, 3));


	//auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	//Camera::GetDefault3DCamera()->SetCameraBehavior(cb);

	while (Engine::UpdateFrame());
}

