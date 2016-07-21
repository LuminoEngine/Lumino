#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>
#include "../../../src/Scene/MME/MMEShader.h"

class Test_Scene_StaticMesh : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_StaticMesh, Box)
{
	auto shader = MMEShader::Create(LN_LOCALFILE("../../../src/Scene/Resource/BasicForwardRendering.fx"));

	auto mesh = StaticMesh::CreateBox(Vector3(3, 2, 1));

	mesh->getmat

	while (Engine::UpdateFrame());
}

