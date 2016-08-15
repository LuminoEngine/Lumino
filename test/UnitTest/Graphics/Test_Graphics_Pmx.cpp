#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/MeshModelObject.h>
#include "../../../src/Scene/MME/MMEShader.h"

class Test_Graphics_Pmx : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Pmx, Basic)
{
	auto shader = MMEShader::Create(LN_LOCALFILE("../../../src/Scene/Resource/BasicForwardRenderingTmp.fx"));
	auto mesh = SkinnedMesh::Create(_T("D:/MMD/ƒ‚ƒfƒ‹/Appearance Miku/Appearance Miku_BDEF.pmx"));
	mesh->SetShader(shader);
	
	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	while (Engine::UpdateFrame())
	{
	}
}
