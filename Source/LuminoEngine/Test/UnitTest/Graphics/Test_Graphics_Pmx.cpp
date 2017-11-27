#include <TestConfig.h>
#if 0
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
	auto mesh = SkinnedMeshComponent::create(_LT("D:/MMD/モデル/Appearance Miku/Appearance Miku_BDEF.pmx"));
	//auto clip = AnimationClip::create(_LT("D:/MMD/モーション/Love&Joy/love&joyお面無しver.vmd"));
	//auto clip = AnimationClip::create(_LT("D:/MMD/モーション/Zigg-Zagg/ZZ-MikuV2.vmd"));
	auto clip1 = AnimationClip::create(_LT("D:/MMD/モーション/走歩スv2.2full/歩く/A01_SO_女の子歩き_s591_p40.vmd"));
	auto clip2 = AnimationClip::create(_LT("D:/MMD/モーション/ln/Idle.vmd"));
	clip1->SetName(_LT("walk"));
	clip2->SetName(_LT("idle"));
	auto model = mesh->GetSkinnedMeshModel();
	auto animator = model->GetAnimator();
	animator->AddAnimationClip(clip1);
	animator->AddAnimationClip(clip2);
	animator->Play(clip2->GetName().c_str());
	
	auto cb = Ref<CylinderMouseMoveCameraBehavior>::makeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	Viewport::GetMainViewport()->SetBackgroundColor(Color::White);

	while (Engine::UpdateFrame())
	{
		if (Input::IsPressed(InputButtons::Left))
		{
			Vector3 pos = mesh->GetPosition();
			pos.x -= 0.05;
			mesh->SetPosition(pos);
			mesh->SetAngles(0, Math::PI / 2, 0);
		}
		if (Input::IsOffTriggered(InputButtons::Left))
		{
			animator->Play(clip2->GetName().c_str());
		}
		if (Input::IsTriggered(InputButtons::Left))
		{
			animator->Play(clip1->GetName().c_str());
		}
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Pmx, Skinning)
{
	auto shader = MMEShader::create(LN_LOCALFILE("../../../src/Scene/Resource/ForwardRenderingSkinnedMesh3D.fx"));
	//auto shader = MMEShader::create(LN_LOCALFILE("TestData/BasicForwardRendering.fx"));
	auto mesh = StaticMeshComponent::CreatePlane(Vector2(8, 8), 8, 8);
	auto res = mesh->GetStaticMeshModel()->GetMeshResource();

	for (int i = 0; i < res->GetVertexCount(); ++i)
	{
		auto& pos = res->GetPosition(i);
		float w = (pos.x + 4.0f) / 8.0f;
		res->SetBlendIndices(i, 0, 1, 0, 0);
		res->SetBlendWeights(i, 1.0f - w, w, 0, 0);
		//res->setBlendWeights(i, 1.0f, 0, 0, 0);
	}

	std::array<Matrix, 2> bones;
	bones[0] = Matrix::Identity;//Matrix(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);//Matrix(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);//Matrix::Identity;//
	bones[1] = Matrix::MakeRotationY(Math::PI / 2) * Matrix::MakeTranslation(10, 0, 0); //Matrix(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);//
	auto skinnedTex = Texture2D::create(SizeI(4, 2), TextureFormat::R32G32B32A32_Float, false);
	skinnedTex->SetSubData(PointI::Zero, &bones[0]);
	
	mesh->SetShader(shader);
	Vector4 invSize;
	invSize.x = 1.0f / skinnedTex->GetWidth();
	invSize.y = 1.0f / skinnedTex->GetHeight();
	mesh->GetMaterials()->GetAt(0)->SetVectorParameter(_LT("lnBoneTextureReciprocalSize"), invSize);
	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(_LT("lnBoneTexture"), skinnedTex);


	mesh->GetMaterials()->GetAt(0)->SetTextureParameter(CommonMaterial::MaterialTextureParameter, skinnedTex);

	ShaderVariable* v;
	v = shader->FindVariable(_LT("lnBoneTextureReciprocalSize"));
	if (v) {
		v->SetVector(invSize);
	}

	v = shader->FindVariable(_LT("lnBoneTexture"));
	if (v) {
		v->SetTexture(skinnedTex);
	}


	auto cb = Ref<CylinderMouseMoveCameraBehavior>::makeRef();
	Camera::GetMain3DCamera()->SetCameraBehavior(cb);

	while (Engine::UpdateFrame())
	{
	}
}
#endif
