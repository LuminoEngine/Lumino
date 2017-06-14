#include <TestConfig.h>
#if 0
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
	//auto shader = MMEShader::create(LN_LOCALFILE("../../../src/Scene/Resource/BasicForwardRendering.fx"));
	auto shader = MMEShader::create(LN_LOCALFILE("TestData/Grid.fx"));
	auto tex = Texture2D::create(LN_LOCALFILE("TestData/Grid1.png"), TextureFormat::R8G8B8A8/*, 4*/);

	//auto mesh = StaticMeshComponent::create(LN_LOCALFILE("TestData/Box_1.x"));
	//auto mesh = StaticMeshComponent::create(_T("D:/tmp/box1.x"));
	//auto mesh = StaticMeshComponent::createSphere(2, 32, 16);
	//auto mesh = StaticMeshComponent::createScreenPlane();
	auto mesh = StaticMeshComponent::CreateSquarePlane(Vector2(1, 2), Vector3::UnitY, MeshCreationFlags::DynamicBuffers);
	mesh->SetShader(shader);
	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(Material::MaterialTextureParameter, tex);


	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::makeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);


	//auto sprite = Sprite3DComponent::create(1, 1);
	//sprite->setTexture(tex);

	//while (Engine::updateFrame());

	struct Line
	{
		Vector3	from;
		Vector3	to;
	};

	while (Engine::UpdateFrame())
	{
	}
}
#endif
