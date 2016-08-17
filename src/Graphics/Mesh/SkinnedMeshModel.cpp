
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
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(sharingMesh != nullptr);

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
// 姿勢更新① (ユーザー更新の前に確定する必要があるもの)
//		・ボーンのグローバル行列更新
//		・剛体更新 (フレーム位置を剛体位置へ)
void SkinnedMeshModel::PreUpdate()
{
	UpdateBoneTransformHierarchy();
}

//------------------------------------------------------------------------------
// 姿勢更新② (ユーザー更新の後、描画の前に行う必要があるもの)
//		・モーフ更新
//		・IK更新
//		・剛体更新 (剛体位置をフレーム位置へ)
//		・スキニング行列の作成
void SkinnedMeshModel::PostUpdate()
{
	// IK 更新
	for (PmxIKResource* ik : m_meshResource->iks)
	{
		SolveIK(ik);
	}

	// スキニング行列の作成
	UpdateSkinningMatrices();
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateBoneTransformHierarchy()
{
	for (SkinnedMeshBone* bone : m_rootBoneList)
	{
		bone->UpdateGlobalTransform(true);
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
static void NormalizeEular(Vector3* eulers)
{
	float& x = eulers->x;
	float& y = eulers->y;
	float& z = eulers->z;
	if (x < -Math::PI || Math::PI < x)
	{
		if (x > 0)
		{
			x -= Math::PI * 2;
		}
		else
		{
			x += Math::PI * 2;
		}
	}
	if (y < -Math::PI*0.5f || Math::PI*0.5f < y)
	{
		if (y > 0)
		{
			y -= Math::PI * 2;
		}
		else
		{
			y += Math::PI * 2;
		}
	}
	if (z < -Math::PI || Math::PI < z)
	{
		if (z > 0)
		{
			z -= Math::PI * 2;
		}
		else
		{
			z += Math::PI * 2;
		}
	}
}

//------------------------------------------------------------------------------
static void LimitIKRotation(const PmxIKResource::IKLink& ikLink, SkinnedMeshBone* ikLinkBone)
{
	if (!ikLink.IsRotateLimit) return;

	Quaternion& localRot = ikLinkBone->GetLocalTransformPtr()->rotation;

	RotationOrder type;
	bool locked;
	Vector3 euler;

	type = RotationOrder::XYZ;
	euler = localRot.ToEulerAngles(type, &locked);
	if (locked)
	{
		type = RotationOrder::YZX;
		euler = localRot.ToEulerAngles(type, &locked);
		if (locked)
		{
			type = RotationOrder::ZXY;
			euler = localRot.ToEulerAngles(type, &locked);
			if (locked)
			{
				LN_ASSERT(0);	// あり得ないはずだが…
			}
		}
	}

	// 角度修正
	NormalizeEular(&euler);
	euler.Clamp(ikLink.MinLimit, ikLink.MaxLimit);

	// 戻す
	Matrix rotMat = Matrix::MakeRotationEulerAngles(euler, type);
	localRot = Quaternion::MakeFromRotationMatrix(rotMat);
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::SolveIK(PmxIKResource* ik)
{
	/*
		[注意] MMDのIKには、IKの計算結果はIK系列の子ボーンには適用されないという特徴があります。
		http://ch.nicovideo.jp/penguin/blomaga/ar70894
	*/

	//ModelFrame2* effector = mOwnerModel->getFrame( ikTargetBone->getFrameCore()->IKTargetBoneIndex );

	// IK構成ボーンのグローバル行列を再更新する
	//for ( short jk = ik->IKLinks.size() - 1 ; jk >= 0 ; --jk)
	//{
	//	mOwnerModel->getFrame(ik->IKLinks[jk].LinkBoneIndex)->updateGlobalMatrix(false);
	//	//frames[mIKCore->IKBoneIndexArray[j]].updateGlobalMatrix( false );
	//}
	//effector->updateGlobalMatrix( true );


	//ModelFrameCore* frameCore = ikTargetBone->getFrameCore();

	SkinnedMeshBone* ikBone = m_allBoneList[ik->IKBoneIndex];			// IK ボーン (IK 情報を持つボーン。目標地点)
	SkinnedMeshBone* effector = m_allBoneList[ik->IKTargetBoneIndex];	// IK ターゲットボーン (エフェクタ。IKに向くべきボーンたちの中の先頭ボーン)

	for (int i = 0; i < ik->LoopCount; ++i)
	{
		// IKボーンのグローバル位置
		const Vector3& targetPos = ikBone->GetCombinedMatrix().GetPosition();

		for (int iLink = 0; iLink < ik->IKLinks.GetCount(); ++iLink)
		{
			// 動かすボーン
			PmxIKResource::IKLink& ikLink = ik->IKLinks[iLink];
			SkinnedMeshBone* ikLinkBone = m_allBoneList[ikLink.LinkBoneIndex];

			// エフェクタの位置
			const Vector3& effPos = effector->GetCombinedMatrix().GetPosition();

			// ワールド座標系から注目ノードの局所座標系への変換
			// (IKリンク基準のローカル座標系へ変換する行列)
			Matrix invCoord = Matrix::MakeInverse(ikLinkBone->GetCombinedMatrix());
			
			// 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
			// (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
			Vector3 localEffPos = Vector3::TransformCoord(effPos, invCoord);

			// (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
			Vector3 localTargetPos = Vector3::TransformCoord(targetPos, invCoord);

			// (1) 基準関節→エフェクタ位置への方向ベクトル
			localEffPos.Normalize();
			// (2) 基準関節→目標位置への方向ベクトル
			localTargetPos.Normalize();

			// 回転角
			float rotationDotProduct = Vector3::Dot(localEffPos, localTargetPos);
			if (rotationDotProduct > 1.f) rotationDotProduct = 1.f;
			float rotationAngle = acosf(rotationDotProduct);

#if 0
			//回転量制限をかける
			if (rotationAngle > Math::PI * ik->IKRotateLimit * (iLink + 1))
				rotationAngle = Math::PI * ik->IKRotateLimit * (iLink + 1);
			if (rotationAngle < -Math::PI * ik->IKRotateLimit * (iLink + 1))
				rotationAngle = -Math::PI * ik->IKRotateLimit * (iLink + 1);

#else
			//回転量制限をかける
			if (rotationAngle > ik->IKRotateLimit)
				rotationAngle = ik->IKRotateLimit;
			if (rotationAngle < -ik->IKRotateLimit)
				rotationAngle = -ik->IKRotateLimit;
#endif

			// 回転軸
			Vector3 rotationAxis = Vector3::Cross(localEffPos, localTargetPos);
			
			//if (frame->getFrameCore()->IKLimitter)
			//	frame->getFrameCore()->IKLimitter->adjustAxisLimits( &rotationAxis );
			rotationAxis.Normalize();


			if (!Math::IsNaN(rotationAngle) && rotationAngle > 1.0e-3f && !rotationAxis.IsNaNOrInf())
			{
				// 関節回転量の補正
				Quaternion rotQuat(rotationAxis, rotationAngle);
				ikLinkBone->GetLocalTransformPtr()->rotation = Quaternion::Multiply(ikLinkBone->GetLocalTransformPtr()->rotation, rotQuat);

				// 回転制限
				LimitIKRotation(ikLink, ikLinkBone);

				// 末端のほうから、IK構成ボーンのグローバル行列を再更新する
				// TODO: IKLinks の順番は付け根→末端へを前提にしている。念のため事前チェックしたほうがいいかも
				for (short jk = ik->IKLinks.GetCount() - 1; jk >= 0; --jk)
				{
					m_allBoneList[ik->IKLinks[jk].LinkBoneIndex]->UpdateGlobalTransform(false);
				}
				effector->UpdateGlobalTransform(true);
			}
		}
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
	, m_parent(nullptr)
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
void SkinnedMeshBone::AddChildBone(SkinnedMeshBone* bone)
{
	LN_CHECK_ARG(bone != nullptr);
	LN_CHECK_ARG(bone->m_parent == nullptr);
	m_children.Add(bone);
	bone->m_parent = this;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::UpdateGlobalTransform(bool hierarchical)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	m_combinedMatrix = m_localTransform;

	// 親からの平行移動量
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	// 親行列と結合
	if (m_parent != nullptr)
	{
		m_combinedMatrix *= m_parent->GetCombinedMatrix();
	}

	// 子ボーン更新
	if (hierarchical)
	{
		for (SkinnedMeshBone* bone : m_children)
		{
			bone->UpdateGlobalTransform(hierarchical);
		}
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
