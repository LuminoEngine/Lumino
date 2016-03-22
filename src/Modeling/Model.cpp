
#include "../Internal.h"
#include <Lumino/Graphics/Utils.h>
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RendererImpl.h"
#include "ModelManager.h"
#include "ModelBone.h"
#include "Model.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Model
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Model::Model()
	: m_manager(nullptr)
	, m_modelCore()
	, m_animator()
	, m_boneList()
	, m_rootBoneList()
	, m_skinningMatrices(nullptr)
	, m_skinningMatricesTexture()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Model::~Model()
{
	LN_SAFE_DELETE_ARRAY(m_skinningMatrices);

	LN_FOREACH(ModelBone* obj, m_boneList) {
		obj->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::Create(detail::ModelManager* manager, const PathName& filePath)
{
	m_manager = manager;
	m_modelCore.Attach(manager->CreateModelCore(filePath), false);

	//---------------------------------------------------------
	// Bone のインスタンス化
	int boneCount = m_modelCore->Bones.GetCount();
	if (boneCount > 0)
	{
		m_boneList.Resize(boneCount);
		// まずは Create する。
		for (int i = 0; i < boneCount; i++)
		{
			m_boneList[i] = LN_NEW ModelBone();
			m_boneList[i]->Create(m_modelCore->Bones[i]);
		}
		// 次に子と親を繋げる
		for (int i = 0; i < boneCount; i++)
		{
			int parentIndex = m_modelCore->Bones[i]->ParentBoneIndex;
			if (0 <= parentIndex && parentIndex < boneCount) {
				m_boneList[parentIndex]->AddChildBone(m_boneList[i]);
			}
			else {
				m_rootBoneList.Add(m_boneList[i]);	// 親がいない。ルートボーンとして覚えておく
			}
		}

		// ボーン行列を書き込むところ
		m_skinningMatrices = LN_NEW Matrix[boneCount];
		m_skinningMatricesTexture.Attach(LN_NEW Texture2D(), false);
		Size size = Size(4, boneCount);
		RefPtr<Bitmap> bitmap(LN_NEW Bitmap(size, Utils::TranslatePixelFormat(TextureFormat_R32G32B32A32_Float)), false);
		m_skinningMatricesTexture->CreateCore(m_manager->GetGraphicsManager(), size, TextureFormat_R32G32B32A32_Float, 1, bitmap);


		// アニメーション管理
		m_animator.Attach(LN_NEW Animator());
		m_animator->Create(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::UpdateBoneTransformHierarchy()
{
	LN_FOREACH(ModelBone* bone, m_rootBoneList)
	{
		bone->UpdateTransformHierarchy(Matrix::Identity);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::UpdateSkinningMatrices()
{
	// スキニング行列の作成
	for (int i = 0; i < m_boneList.GetCount(); i++)
	{
		/*
		初期姿勢は、スキニングしなくても同じ姿勢。
		つまり、頂点スキニングで初期姿勢にしたいときは Matrix::Identity のボーン行列を渡す。

		ボーンは最初からオフセットが入ってるけど、
		それをスキニングに適用すると姿勢が崩れてしまう。
		そのため、初期オフセットを打ち消す処理が必要。それが GetInitialTranstormInv()。

		ID3DXSkinInfo::GetBoneOffsetMatrix() で
		取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) が
		これにあたるものっぽい。
		サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
		*/
		m_skinningMatrices[i] = m_boneList[i]->GetCore()->GetInitialTranstormInv();
		m_skinningMatrices[i] *= m_boneList[i]->GetCombinedMatrix();
	}

	// スキニングテクスチャ更新
	if (!m_skinningMatricesTexture.IsNull())
	{
		m_skinningMatricesTexture->SetSubData(Point(0, 0), m_skinningMatrices);
	}


	// 全てのローカルトランスフォームをリセットする
	//		リセットしておかないと、IKで問題が出る。
	//		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
	//		なお、一連の更新の最後で行っているのは、アニメーションからの更新を
	//		外部で行っているため。
	// TODO: できれば一連の処理の中で必ず通るところに移動したい
	LN_FOREACH(ModelBone* obj, m_boneList) {
		obj->ResetLocalTransform();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Model::GetSubsetCount() const
{
	return m_modelCore->Material.Attributes.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Material& Model::GetMaterial(int subsetIndex) const
{
	return m_modelCore->Material.Materials[subsetIndex];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::DrawSubset(int subsetIndex)
{
	//g_ttttt = m_modelCore->VertexBuffer->m_deviceObj;

	Details::Renderer* r = m_manager->GetGraphicsManager()->GetRenderer();
	r->SetVertexBuffer(m_modelCore->VertexBuffer);
	r->SetIndexBuffer(m_modelCore->IndexBuffer);
	r->DrawPrimitiveIndexed(
		PrimitiveType_TriangleList,
		m_modelCore->Material.Attributes[subsetIndex].StartIndex,
		m_modelCore->Material.Attributes[subsetIndex].PrimitiveNum);
}

LN_NAMESPACE_END
