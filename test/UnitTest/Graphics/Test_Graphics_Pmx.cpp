#include <TestConfig.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/MeshModelObject.h>
#include <Lumino/Graphics/Mesh/SkinnedMeshModel.h>
#include "../../../src/Scene/MME/MMEShader.h"
#include "../../../src/Animation/AnimationClip.h"

class Test_Graphics_Pmx : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Pmx, Basic)
{
	auto mesh = SkinnedMesh::Create(_T("D:/MMD/ƒ‚ƒfƒ‹/Appearance Miku/Appearance Miku_BDEF.pmx"));
	auto clip = AnimationClip::Create(_T("D:/MMD/ƒ‚[ƒVƒ‡ƒ“/Love&Joy/love&joy‚¨–Ê–³‚µver.vmd"));
	auto model = mesh->GetSkinnedMeshModel();
	auto animator = model->GetAnimator();
	animator->AddAnimationClip(clip);
	animator->Play(clip->GetName().c_str());
	
	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::White);

	while (Engine::UpdateFrame())
	{
	}
}
