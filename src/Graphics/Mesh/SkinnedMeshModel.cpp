
#include "../Internal.h"
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Graphics/Mesh/SkinnedMeshModel.h>
#include "../GraphicsManager.h"
#include "../../Modeling/PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SkinnedMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SkinnedMeshModel, Object);

//------------------------------------------------------------------------------
SkinnedMeshModel::SkinnedMeshModel()
	: m_meshResource(nullptr)
	, m_materials(nullptr)
	, m_allBoneList()
	, m_rootBoneList()
	, m_skinningMatrices()
	, m_skinningMatricesTexture(nullptr)
	, m_animator(nullptr)
{
}

//------------------------------------------------------------------------------
SkinnedMeshModel::~SkinnedMeshModel()
{
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::Initialize(GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh)
{
	if (LN_CHECKEQ_ARG(manager == nullptr)) return;
	if (LN_CHECKEQ_ARG(sharingMesh == nullptr)) return;

	// メッシュ(バッファ類)は共有する
	m_meshResource = sharingMesh;

	// マテリアルはコピーする
	// TODO: コピー有無のフラグがあったほうがいいかも？
	int count = m_meshResource->materials.GetCount();
	m_materials = RefPtr<MaterialList>::MakeRef();
	m_materials->Resize(count);
	for (int i = 0; i < count; ++i)
	{
		m_materials->SetAt(i, m_meshResource->materials.GetAt(i)->MakeCommonMaterial());
	}

	//---------------------------------------------------------
	// Bone のインスタンス化
	int boneCount = m_meshResource->bones.GetCount();
	if (boneCount > 0)
	{
		m_allBoneList.Resize(boneCount);
		// まずは Bone を作る
		for (int i = 0; i < boneCount; i++)
		{
			m_allBoneList[i] = SkinnedMeshBonePtr::MakeRef();
			m_allBoneList[i]->Initialize(m_meshResource->bones[i]);
		}
		// 次に子と親を繋げる
		for (int i = 0; i < boneCount; i++)
		{
			int parentIndex = m_meshResource->bones[i]->ParentBoneIndex;
			if (0 <= parentIndex && parentIndex < boneCount)
			{
				m_allBoneList[parentIndex]->AddChildBone(m_allBoneList[i]);
			}
			else
			{
				m_rootBoneList.Add(m_allBoneList[i]);	// 親がいない。ルートボーンとして覚えておく
			}
		}

		// ボーン行列を書き込むところを作る
		m_skinningMatrices.Resize(boneCount);
		m_skinningMatricesTexture = RefPtr<Texture2D>::MakeRef();
		m_skinningMatricesTexture->Initialize(manager, SizeI(4, boneCount), TextureFormat::R32G32B32A32_Float, false);	// TODO: Dynamic、NoManaged

		// アニメーション管理
		m_animator = RefPtr<Animator>::MakeRef();
		m_animator->Create(this);
	}
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateBoneTransformHierarchy()
{
	for (SkinnedMeshBone* bone : m_rootBoneList)
	{
		bone->UpdateTransformHierarchy(Matrix::Identity);
	}
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateSkinningMatrices()
{
	// スキニング行列の作成
	for (int i = 0; i < m_allBoneList.GetCount(); i++)
	{
		/*
			初期姿勢は、スキニングしなくても同じ姿勢。
			つまり、頂点スキニングで初期姿勢にしたいときは Matrix::Identity のボーン行列を渡す。

			ボーンは最初からオフセットが入ってるけど、
			それをスキニングに適用すると姿勢が崩れてしまう。
			そのため、初期オフセットを打ち消す処理が必要。それが GetInitialTranstormInv()。

			ID3DXSkinInfo::GetBoneOffsetMatrix() で
			取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) がこれにあたるものっぽい。
			サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
		*/
		m_skinningMatrices[i] = m_allBoneList[i]->GetCore()->GetInitialTranstormInv();
		m_skinningMatrices[i] *= m_allBoneList[i]->GetCombinedMatrix();
	}

	// スキニングテクスチャ更新
	if (!m_skinningMatricesTexture.IsNull())
	{
		m_skinningMatricesTexture->SetSubData(Point(0, 0), &m_skinningMatrices[0]);
	}

	// 全てのローカルトランスフォームをリセットする
	//		リセットしておかないと、IKで問題が出る。
	//		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
	//		なお、一連の更新の最後で行っているのは、アニメーションからの更新を外部で行っているため。
	// TODO: できれば一連の処理の中で必ず通るところに移動したい
	for (SkinnedMeshBone* bone : m_allBoneList)
	{
		bone->ResetLocalTransform();
	}
}

//------------------------------------------------------------------------------
int SkinnedMeshModel::GetAnimationTargetAttributeCount() const
{
	return m_allBoneList.GetCount();
}

//------------------------------------------------------------------------------
IAnimationTargetAttribute* SkinnedMeshModel::GetAnimationTargetAttribute(int index)
{
	return m_allBoneList[index].Get();
}

//==============================================================================
// SkinnedMeshBone
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SkinnedMeshBone, Object);

//------------------------------------------------------------------------------
SkinnedMeshBone::SkinnedMeshBone()
	: m_core(nullptr)
	, m_children()
	, m_localTransform()
	, m_combinedMatrix()
{
}

//------------------------------------------------------------------------------
SkinnedMeshBone::~SkinnedMeshBone()
{
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::Initialize(PmxBoneResource* boneResource)
{
	m_core = boneResource;
}

//------------------------------------------------------------------------------
PmxBoneResource* SkinnedMeshBone::GetCore() const
{
	return m_core;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::UpdateTransformHierarchy(const Matrix& parentMatrix)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	m_combinedMatrix = m_localTransform;

	// 親からの平行移動量
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	// 親行列と結合
	m_combinedMatrix *= parentMatrix;

	// 子ボーン更新
	for (SkinnedMeshBone* bone : m_children)
	{
		bone->UpdateTransformHierarchy(m_combinedMatrix);
	}
}

//------------------------------------------------------------------------------
const String& SkinnedMeshBone::GetAnimationTargetName() const
{
	return m_core->Name;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::SetAnimationTargetValue(ValueType type, const void* value)
{
	LN_CHECK_ARG(type == ValueType_SQTTransform);
	m_localTransform = *((SQTTransform*)value);
}

LN_NAMESPACE_END
