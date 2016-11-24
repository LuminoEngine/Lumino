
#include <LuminoEngine.h>
#include <Lumino/Scene/MeshModelObject.h>
#include <Lumino/Scene/Light.h>
using namespace ln;

void Main()
{
	Engine::Initialize();

	Engine::GetDefaultSceneGraph3D()->VisibleGridPlane = true;

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);


	//auto mesh = SkinnedMesh::Create(_T("D:/MMD/Materials/モデル/Appearance Miku/Appearance Miku_BDEF.pmx"));

	//auto tex = Texture2D::Create("D:/Proj/Volkoff/External/Lumino/test/UnitTest/Scene/TestData/Dice1.png");
	//auto sprite1 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite2 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite3 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite4 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite5 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite6 = Sprite3D::Create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);


	//auto tonePE = ToneImageEffect::Create();
	//tonePE->SetTone(ToneF(-0.1, -0.25, -0.5, 1.0));
	//CameraViewportLayer::GetDefault3D()->GetImageEffects()->Add(tonePE);

	//Engine::GetMainLight3D()->SetPosition();

	while (Engine::Update())
	{
	}
}

