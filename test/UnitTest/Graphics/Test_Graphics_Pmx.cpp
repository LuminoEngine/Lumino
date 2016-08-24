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
	auto mesh = SkinnedMesh::Create(_T("D:/MMD/モデル/Appearance Miku/Appearance Miku_BDEF.pmx"));
	auto clip = AnimationClip::Create(_T("D:/MMD/モーション/Love&Joy/love&joyお面無しver.vmd"));
	//auto clip = AnimationClip::Create(_T("D:/MMD/モーション/Zigg-Zagg/ZZ-MikuV2.vmd"));
	clip->SetName(_T("dance"));
	auto model = mesh->GetSkinnedMeshModel();
	auto animator = model->GetAnimator();
	animator->AddAnimationClip(clip);
	animator->Play(clip->GetName().c_str());
	
	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::White);

	while (Engine::UpdateFrame())
	{
		if (Input::IsPressed(InputButtons::Left))
		{
			Vector3 pos = mesh->GetPosition();
			pos.x -= 0.05;
			mesh->SetPosition(pos);
		}
		if (Input::IsOffTriggered(InputButtons::Left))
		{
			animator->Play(_T(""), 5.0f);
		}
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Pmx, Skinning)
{
	auto shader = MMEShader::Create(LN_LOCALFILE("../../../src/Scene/Resource/ForwardRenderingSkinnedMesh3D.fx"));
	//auto shader = MMEShader::Create(LN_LOCALFILE("TestData/BasicForwardRendering.fx"));
	auto mesh = StaticMesh::CreatePlane(Vector2(8, 8), 8, 8);
	auto res = mesh->GetStaticMeshModel()->GetMeshResource();

	for (int i = 0; i < res->GetVertexCount(); ++i)
	{
		auto& pos = res->GetPosition(i);
		float w = (pos.x + 4.0f) / 8.0f;
		res->SetBlendIndices(i, 0, 1, 0, 0);
		res->SetBlendWeights(i, 1.0f - w, w, 0, 0);
		//res->SetBlendWeights(i, 1.0f, 0, 0, 0);
	}

	std::array<Matrix, 2> bones;
	bones[0] = Matrix::Identity;//Matrix(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);//Matrix(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);//Matrix::Identity;//
	bones[1] = Matrix::MakeRotationY(Math::PI / 2) * Matrix::MakeTranslation(10, 0, 0); //Matrix(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);//
	auto skinnedTex = Texture2D::Create(SizeI(4, 2), TextureFormat::R32G32B32A32_Float, false);
	skinnedTex->SetSubData(Point::Zero, &bones[0]);
	
	mesh->SetShader(shader);
	Vector4 invSize;
	invSize.x = 1.0f / skinnedTex->GetWidth();
	invSize.y = 1.0f / skinnedTex->GetHeight();
	mesh->GetMaterials()->GetAt(0)->SetVectorParameter(_T("lnBoneTextureReciprocalSize"), invSize);
	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(_T("lnBoneTexture"), skinnedTex);


	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(Material::MaterialTextureParameter, skinnedTex);

	ShaderVariable* v;
	v = shader->FindVariable(_T("lnBoneTextureReciprocalSize"));
	if (v) {
		v->SetVector(invSize);
	}

	v = shader->FindVariable(_T("lnBoneTexture"));
	if (v) {
		v->SetTexture(skinnedTex);
	}


	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	while (Engine::UpdateFrame())
	{
	}
}
