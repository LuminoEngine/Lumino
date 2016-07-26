#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>
#include "../../../src/Scene/MME/MMEShader.h"

class Test_Graphics_XFile : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_XFile, Basic)
{
	//auto shader = MMEShader::Create(LN_LOCALFILE("../../../src/Scene/Resource/BasicForwardRendering.fx"));
	auto shader = MMEShader::Create(LN_LOCALFILE("TestData/Grid.fx"));
	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/img2.png"));

	//auto mesh = StaticMesh::Create(LN_LOCALFILE("TestData/Box_1.x"));
	//auto mesh = StaticMesh::Create(_T("D:/tmp/box1.x"));
	//auto mesh = StaticMesh::CreateSphere(2, 32, 16);
	auto mesh = StaticMesh::CreateScreenPlane();
	mesh->SetShader(shader);
	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(Material::MaterialTextureParameter, tex);

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetDefault3DCamera()->SetCameraBehavior(cb);

	while (Engine::UpdateFrame());
}
