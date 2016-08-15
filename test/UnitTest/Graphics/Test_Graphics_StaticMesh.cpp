#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>

class Test_Graphics_StaticMesh : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
//TEST_F(Test_Graphics_StaticMesh, Box)
//{
//	auto box = StaticMesh::CreateBox(Vector3(1, 2, 3));
//
//
//	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
//	Camera::GetMain3DCamera()->SetCameraBehavior(cb);
//
//	while (Engine::UpdateFrame());
//}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_StaticMesh, DepthTest)
{
	auto tex1 = Texture2D::Create(32, 32);
	auto tex2 = Texture2D::Create(32, 32);
	tex1->Clear(Color32::Red);
	tex2->Clear(Color32::Blue);

	auto box1 = StaticMesh::CreateBox(Vector3(1, 2, 3));
	auto box2 = StaticMesh::CreateBox(Vector3(2, 1, 1));
	box1->GetMaterials()->GetAt(0)->SetMaterialTexture(tex1);
	box2->GetMaterials()->GetAt(0)->SetMaterialTexture(tex2);

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	while (Engine::UpdateFrame())
	{
		printf("----\n");
	}
}

