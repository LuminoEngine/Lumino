
#include <LuminoEngine.h>
#include <Lumino/Scene/MeshModelObject.h>
using namespace ln;

void Main()
{
	Engine::Initialize();

	Engine::GetDefaultSceneGraph3D()->VisibleGridPlane = true;

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);


	auto mesh = SkinnedMesh::Create(_T("D:/MMD/Materials/モデル/Appearance Miku/Appearance Miku_BDEF.pmx"));

	while (Engine::Update())
	{
	}
}

