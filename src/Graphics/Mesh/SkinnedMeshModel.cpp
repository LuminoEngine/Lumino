
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
	UpdateIK();

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
static Quaternion LimitIKRotation(const PmxIKResource::IKLink& ikLink, const Quaternion& localRot)
{
	//if (!ikLink.IsRotateLimit) return;

	RotationOrder type;
	bool locked;
	Vector3 euler;

	type = RotationOrder::ZXY;
	euler = localRot.ToEulerAngles(type, &locked);
	if (locked)
	{
		type = RotationOrder::YZX;
		euler = localRot.ToEulerAngles(type, &locked);
		if (locked)
		{
			type = RotationOrder::XYZ;
			euler = localRot.ToEulerAngles(type, &locked);
			if (locked)
			{
				LN_CHECK_STATE(0);	// あり得ないはずだが…
			}
		}
	}

	// 角度修正
	NormalizeEular(&euler);
	euler.Clamp(ikLink.MinLimit, ikLink.MaxLimit);

	// 戻す
	Matrix rotMat = Matrix::MakeRotationEulerAngles(euler, type);
	return Quaternion::MakeFromRotationMatrix(rotMat);
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateIK()
{
	for (PmxIKResource* ik : m_meshResource->iks)
	{
		UpdateIKInternal2(ik);
		//SkinnedMeshBone* ikTarget = m_allBoneList[ik->IKTargetBoneIndex];
		//ikTarget->m_ikLocalRotationMatrix = Matrix::MakeRotationQuaternion(ikTarget->m_userRotation);
		//ikTarget->m_ikLocalMatrix =
		//	Matrix::MakeTranslation(-ikTarget->m_localTransform.translation) *
		//	ikTarget->m_ikLocalRotationMatrix *
		//	Matrix::MakeTranslation(ikTarget->m_localTransform.translation);
		//for (int i = 0; i < ik->LoopCount; ++i)
		//{
		//	UpdateIKInternal(ik, 0, i, ik->LoopCount / 2, 0);
		//}
	}
}
static float GetUpperLowerRadian(float fR, float lower, float upper, bool loopFlag)
{
	if (fR < lower)
	{
		float num = 2.0f * lower - fR;
		if (num <= upper && loopFlag)
		{
			fR = num;
		}
		else
		{
			fR = lower;
		}
	}
	if (fR > upper)
	{
		float num2 = 2.0f * upper - fR;
		if (num2 >= lower && loopFlag)
		{
			fR = num2;
		}
		else
		{
			fR = upper;
		}
	}
	return fR;
}
static void UpdateIKChildMatrix(SkinnedMeshBone* ikLinkBone, Matrix parentMatrix, Matrix parent_mat_normal, SkinnedMeshBone* endframe, SkinnedMeshBone* target)
{

	ikLinkBone->m_combinedMatrix = ikLinkBone->m_ikLocalMatrix * parentMatrix;
	ikLinkBone->m_position = Vector3::TransformCoord(ikLinkBone->GetCore()->OrgPosition, ikLinkBone->m_combinedMatrix);
	ikLinkBone->m_globalRotationMatrix = ikLinkBone->m_ikLocalRotationMatrix * parent_mat_normal;

	if (ikLinkBone->GetCore()->Name == _T("左足首"))
	{
		printf("");
	}

	for (SkinnedMeshBone* bone : ikLinkBone->m_children)
	{
		if (bone != endframe)
		{
			UpdateIKChildMatrix(bone, ikLinkBone->m_combinedMatrix, ikLinkBone->m_globalRotationMatrix, endframe, target);
		}
	}

	//if (frame.Sibling != null && (frame.Sibling.Bone.BoneFlags & BoneType.IK_Child) > (BoneType)0 && (target.Bone.BoneFlags & BoneType.Unvisible) > (BoneType)0)
	//{
	//	this.UpdateIKChildMatrix(frame.Sibling, matrix, mat_normal, endframe, target);
	//}
	//if (ikLinkBone != endframe)
	//{
	//	//if (frame.Sibling != null)
	//	//{
	//	//	this.UpdateIKChildMatrix(frame.Sibling, matrix, mat_normal, endframe, target);
	//	//}
	//	if (frame.FirstChild != null)
	//	{
	//		
	//	}
	//}
}
//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateIKInternal(PmxIKResource* ik, int linkIndex, int loop, int ikt, int depth)
{
	/*
		[注意] MMDのIKには、IKの計算結果はIK系列の子ボーンには適用されないという特徴があります。
		http://ch.nicovideo.jp/penguin/blomaga/ar70894
	*/
	PmxIKResource::IKLink& ikLink = ik->IKLinks[linkIndex];
	SkinnedMeshBone* ikLinkBone = m_allBoneList[ikLink.LinkBoneIndex];	// 動かすボーン
	SkinnedMeshBone* ikBone = m_allBoneList[ik->IKBoneIndex];			// IK ボーン (IK 情報を持つボーン。目標地点)
	SkinnedMeshBone* effector = m_allBoneList[ik->IKTargetBoneIndex];	// IK ターゲットボーン (エフェクタ。IKに向くべきボーンたちの中の先頭ボーン)

	// ↓これらの Position はグローバル空間
	Vector3 vector = Vector3::SafeNormalize(ikLinkBone->GetPosition() - effector->GetPosition());
	Vector3 vector2 = Vector3::SafeNormalize(ikLinkBone->GetPosition() - ikBone->GetPosition());
	Vector3 rotationAxis = Vector3::SafeNormalize(Vector3::Cross(vector, vector2));

	const Matrix& matNormal = ikLinkBone->m_globalRotationMatrix;
	float restrictRadian = ik->IKRotateLimit;

	// IKLink についている角度制限を　rotationAxis にほどこす
	if (ikLink.IsRotateLimit && loop < ikt)
	{
		const Matrix& parentMatNormal = ikLinkBone->GetParent()->m_globalRotationMatrix;

		if (ikLink.MinLimit.y == 0.0f && ikLink.MaxLimit.y == 0.0f && ikLink.MinLimit.z == 0.0f && ikLink.MaxLimit.z == 0.0f)
		{
			// parentMatNormal の右方向と乗算
			float num = rotationAxis.x * parentMatNormal.m11 + rotationAxis.y * parentMatNormal.m12 + rotationAxis.z * parentMatNormal.m13;
			if (num >= 0.0f)
			{
				rotationAxis.x = 1.0f;
			}
			else
			{
				rotationAxis.x = -1.0f;
			}
			rotationAxis.y = 0.0f;
			rotationAxis.z = 0.0f;
		}
		else
		{
			// parentMatNormal の上方向と乗算
			if (ikLink.MinLimit.x == 0.0f && ikLink.MaxLimit.x == 0.0f && ikLink.MinLimit.z == 0.0f && ikLink.MaxLimit.z == 0.0f)
			{
				float num2 = rotationAxis.x * parentMatNormal.m21 + rotationAxis.y * parentMatNormal.m22 + rotationAxis.z * parentMatNormal.m23;
				if (num2 >= 0.0f)
				{
					rotationAxis.y = 1.0f;
				}
				else
				{
					rotationAxis.y = -1.0f;
				}
				rotationAxis.x = 0.0f;
				rotationAxis.z = 0.0f;
			}
			else
			{
				if (ikLink.MinLimit.x == 0.0f && ikLink.MaxLimit.x == 0.0f && ikLink.MinLimit.y == 0.0f && ikLink.MaxLimit.y == 0.0f)
				{
					// parentMatNormal の前方向と乗算
					float num3 = rotationAxis.x * parentMatNormal.m31 + rotationAxis.y * parentMatNormal.m32 + rotationAxis.z * parentMatNormal.m33;
					if (num3 >= 0.0f)
					{
						rotationAxis.z = 1.0f;
					}
					else
					{
						rotationAxis.z = -1.0f;
					}
					rotationAxis.x = 0.0f;
					rotationAxis.y = 0.0f;
				}
				else
				{
					// 回転のみの座標返還
					Vector3 v;
					v.x = rotationAxis.x * matNormal.m11 + rotationAxis.y * matNormal.m12 + rotationAxis.z * matNormal.m13;
					v.y = rotationAxis.x * matNormal.m21 + rotationAxis.y * matNormal.m22 + rotationAxis.z * matNormal.m23;
					v.z = rotationAxis.x * matNormal.m31 + rotationAxis.y * matNormal.m32 + rotationAxis.z * matNormal.m33;
					rotationAxis = Vector3::SafeNormalize(v);
				}
			}
		}
	}
	// 角度制限しない場合
	else
	{
		// 回転のみの座標返還
		Vector3 v;
		v.x = rotationAxis.x * matNormal.m11 + rotationAxis.y * matNormal.m12 + rotationAxis.z * matNormal.m13;
		v.y = rotationAxis.x * matNormal.m21 + rotationAxis.y * matNormal.m22 + rotationAxis.z * matNormal.m23;
		v.z = rotationAxis.x * matNormal.m31 + rotationAxis.y * matNormal.m32 + rotationAxis.z * matNormal.m33;
		rotationAxis = Vector3::SafeNormalize(v);
	}

	// 回転角
	float num4 = Vector3::Dot(vector, vector2);
	if (num4 > 1.0f)
	{
		num4 = 1.0f;
	}
	else
	{
		if (num4 < -1.0f)
		{
			num4 = -1.0f;
		}
	}
	num4 = acosf(num4);
	if (num4 > restrictRadian * (float)(depth + 1))
	{
		num4 = restrictRadian * (float)(depth + 1);
	}

	Quaternion tq = Quaternion::Identity;
	if (rotationAxis != Vector3::Zero) tq = Quaternion::MakeFromRotationAxis(rotationAxis, num4);
	ikLinkBone->m_ikQuaternion = tq * ikLinkBone->m_ikQuaternion;
	if (loop == 0)
	{
		ikLinkBone->m_ikQuaternion *= ikLinkBone->m_userRotation;// *ikLinkBone->m_inherehRotation * ikLinkBone->m_morphRotation;
	}
	Matrix matrix = Matrix::MakeRotationQuaternion(ikLinkBone->m_ikQuaternion);

	// ↓角度制限。matrix と ikLinkBone->m_ikQuaternion に結果を設定する
	// m_ikQuaternion は前回値として保持している。
	if (ikLink.IsRotateLimit)
	{
		if ((double)ikLink.MinLimit.x > -1.5707963267948966 && (double)ikLink.MaxLimit.x < 1.5707963267948966)
		{
			float num5 = -matrix.m32;
			float num6 = asinf(num5);
			if (abs(num6) > 1.535889f)
			{
				if (num6 < 0.0f)
				{
					num6 = -1.535889f;
				}
				else
				{
					num6 = 1.535889f;
				}
			}
			float num7 = cosf(num6);
			float num8 = matrix.m31 / num7;
			float num9 = matrix.m33 / num7;
			float num10 = atan2f(num8, num9);
			float num11 = matrix.m12 / num7;
			float num12 = matrix.m22 / num7;
			float num13 = atan2f(num11, num12);
			bool loopFlag = loop < ikt;
			num6 = GetUpperLowerRadian(num6, ikLink.MinLimit.x, ikLink.MaxLimit.x, loopFlag);
			num10 = GetUpperLowerRadian(num10, ikLink.MinLimit.y, ikLink.MaxLimit.y, loopFlag);
			num13 = GetUpperLowerRadian(num13, ikLink.MinLimit.z, ikLink.MaxLimit.z, loopFlag);
			matrix = Matrix::MakeRotationZ(num13) * Matrix::MakeRotationX(num6) * Matrix::MakeRotationY(num10);
		}
		else
		{
			if ((double)ikLink.MinLimit.y > -1.5707963267948966 && (double)ikLink.MaxLimit.y < 1.5707963267948966)
			{
				float num14 = -matrix.m13;
				float num15 = (float)asinf((double)num14);
				if (abs(num15) > 1.535889f)
				{
					if (num15 < 0.0f)
					{
						num15 = -1.535889f;
					}
					else
					{
						num15 = 1.535889f;
					}
				}
				float num16 = (float)cosf((double)num15);
				float num17 = matrix.m23 / num16;
				float num18 = matrix.m33 / num16;
				float num19 = (float)atan2f((double)num17, (double)num18);
				float num20 = matrix.m12 / num16;
				float num21 = matrix.m11 / num16;
				float num22 = (float)atan2f((double)num20, (double)num21);
				bool loopFlag2 = loop < ikt;
				num19 = GetUpperLowerRadian(num19, ikLink.MinLimit.x, ikLink.MaxLimit.x, loopFlag2);
				num15 = GetUpperLowerRadian(num15, ikLink.MinLimit.y, ikLink.MaxLimit.y, loopFlag2);
				num22 = GetUpperLowerRadian(num22, ikLink.MinLimit.z, ikLink.MaxLimit.z, loopFlag2);
				matrix = Matrix::MakeRotationX(num19) * Matrix::MakeRotationY(num15) * Matrix::MakeRotationZ(num22);
			}
			else
			{
				float num23 = -matrix.m21;
				float num24 = (float)asinf((double)num23);
				if (abs(num24) > 1.535889f)
				{
					if (num24 < 0.0f)
					{
						num24 = -1.535889f;
					}
					else
					{
						num24 = 1.535889f;
					}
				}
				float num25 = (float)cosf((double)num24);
				float num26 = matrix.m23 / num25;
				float num27 = matrix.m22 / num25;
				float num28 = (float)atan2f((double)num26, (double)num27);
				float num29 = matrix.m31 / num25;
				float num30 = matrix.m11 / num25;
				float num31 = (float)atan2f((double)num29, (double)num30);
				bool loopFlag3 = loop < ikt;
				num28 = GetUpperLowerRadian(num28, ikLink.MinLimit.x, ikLink.MaxLimit.x, loopFlag3);
				num31 = GetUpperLowerRadian(num31, ikLink.MinLimit.y, ikLink.MaxLimit.y, loopFlag3);
				num24 = GetUpperLowerRadian(num24, ikLink.MinLimit.z, ikLink.MaxLimit.z, loopFlag3);
				matrix = Matrix::MakeRotationY(num31) * Matrix::MakeRotationZ(num24) * Matrix::MakeRotationX(num28);
			}
		}
		ikLinkBone->m_ikQuaternion = Quaternion::MakeFromRotationMatrix(matrix);
	}

	

	ikLinkBone->m_ikLocalRotationMatrix = matrix;
	ikLinkBone->m_ikLocalMatrix = Matrix::MakeTranslation(-ikLinkBone->m_localTransform.translation) * matrix * Matrix::MakeTranslation(ikLinkBone->m_localTransform.translation);
	ikLinkBone->m_combinedMatrix = ikLinkBone->m_ikLocalMatrix * ikLinkBone->GetParent()->GetCombinedMatrix();  // これが最終出力
	ikLinkBone->m_position = Vector3::TransformCoord(ikLinkBone->GetCore()->OrgPosition, ikLinkBone->m_combinedMatrix);
	ikLinkBone->m_globalRotationMatrix = ikLinkBone->m_ikLocalRotationMatrix *  ikLinkBone->GetParent()->m_globalRotationMatrix;
	
	if (ikLinkBone->GetCore()->Name == _T("左足首"))
	{
		printf("");
	}


	for (int i = 0; i < ikLinkBone->m_children.GetCount(); ++i)
	{
		UpdateIKChildMatrix(ikLinkBone->m_children[i], ikLinkBone->m_combinedMatrix, ikLinkBone->m_globalRotationMatrix, effector, ikBone);
	}
	if (linkIndex < ik->IKLinks.GetCount() - 1)
	{
		linkIndex++;
		UpdateIKInternal(ik, linkIndex, loop, ikt, depth + 1);
	}

}

void SkinnedMeshModel::UpdateIKInternal2(PmxIKResource* ik)
{
	//ModelFrame2* effector = mOwnerModel->getFrame( ikTargetBone->getFrameCore()->IKTargetBoneIndex );

	

	SkinnedMeshBone* ikBone = m_allBoneList[ik->IKBoneIndex];			// IK ボーン (IK 情報を持つボーン。目標地点)
	SkinnedMeshBone* effector = m_allBoneList[ik->IKTargetBoneIndex];	// IK ターゲットボーン (エフェクタ。IKに向くべきボーンたちの中の先頭ボーン)
	int ikt = ik->LoopCount / 2;

	// IK構成ボーンのグローバル行列を再更新する
	for (short jk = ik->IKLinks.GetCount() - 1; jk >= 0; --jk)
	{
		m_allBoneList[ik->IKLinks[jk].LinkBoneIndex]->UpdateGlobalTransform(false);
	}
	effector->UpdateGlobalTransform(false);

	if (effector->m_combinedMatrix.GetPosition().y > 10)
	{
		effector->m_combinedMatrix.GetPosition().Print();
	}

	for (int iCalc = 0; iCalc < ik->LoopCount; iCalc++)
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
			//effPos.Print();
			//targetPos.Print();

			// ワールド座標系から注目ノードの局所座標系への変換
			// (IKリンク基準のローカル座標系へ変換する行列)
			Matrix invCoord = Matrix::MakeInverse(ikLinkBone->GetCombinedMatrix());

			// 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
			// (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
			Vector3 localEffPos = Vector3::TransformCoord(effPos, invCoord);

			// (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
			Vector3 localTargetPos = Vector3::TransformCoord(targetPos, invCoord);

			// 十分近ければ終了
			if ((localEffPos - localTargetPos).GetLengthSquared() < 0.0000001f) return;

			// (1) 基準関節→エフェクタ位置への方向ベクトル
			Vector3 localEffDir = Vector3::SafeNormalize(localEffPos);
			// (2) 基準関節→目標位置への方向ベクトル
			Vector3 localTargetDir = Vector3::SafeNormalize(localTargetPos);

			//if (ikLinkBone->GetCore()->Name.Contains(_T("ひざ")))
			//{
			//	// X 軸を切る (MikuMikuPenguin から参考にした)
			//	localEffDir.Set(0, localEffDir.y, localEffDir.z);
			//	localEffDir.Normalize();
			//	localTargetDir.Set(0, localTargetDir.y, localTargetDir.z);
			//	localTargetDir.Normalize();
			//}




			// 回転角
			float rotationDotProduct = Vector3::Dot(localEffDir, localTargetDir);
			rotationDotProduct = Math::Clamp(rotationDotProduct, -1.0f, 1.0f);
			float rotationAngle = acosf(rotationDotProduct);

			if (0.00000001f < fabsf(rotationAngle))
			{
				// 回転量制限をかける
				if (rotationAngle > ik->IKRotateLimit /16)
				{
					rotationAngle = ik->IKRotateLimit / 16;
				}
				//if (rotationAngle > ik->IKRotateLimit / 8)
				//	rotationAngle = ik->IKRotateLimit / 8;
				//if (rotationAngle < -ik->IKRotateLimit / 8)
				//	rotationAngle = -ik->IKRotateLimit / 8;

				// 回転軸
				Vector3 rotationAxis = Vector3::Cross(localEffDir, localTargetDir);
				if (rotationAxis.GetLengthSquared() < 0.0000001f) continue;
				rotationAxis.Normalize();

				// 関節回転量の補正
				Quaternion rotQuat(rotationAxis, rotationAngle);


				if (ikLink.IsRotateLimit)
				{
					//Vector3 angles = rotQuat.ToEulerAngles();

					//if (angles.x < -3.14159f)	angles.x = -3.14159f;
					//if (-0.002f < angles.x)		angles.x = -0.002f;
					//angles.y = 0.0f;
					//angles.z = 0.0f;

					//rotQuat = Quaternion::MakeFromEulerAngles(angles);
					rotQuat = LimitIKRotation(ikLink, rotQuat);
				}

				rotQuat.Normalize();


				ikLinkBone->GetLocalTransformPtr()->rotation = Quaternion::Multiply(ikLinkBone->GetLocalTransformPtr()->rotation, rotQuat);
				ikLinkBone->GetLocalTransformPtr()->rotation.Normalize();




				for (short jk = ik->IKLinks.GetCount() - 1; jk >= 0; --jk)
				{
					m_allBoneList[ik->IKLinks[jk].LinkBoneIndex]->UpdateGlobalTransform(false);
				}
				effector->UpdateGlobalTransform(true);

				if (effector->m_combinedMatrix.GetPosition().y > 10)
				{
					effector->m_combinedMatrix.GetPosition().Print();
				}

				if (ikLinkBone->GetCore()->Name == _T("左足首") && ikLinkBone->m_combinedMatrix.GetPosition().y > 10)
				{
					ikLinkBone->m_combinedMatrix.GetPosition().Print();
				}

			}
#if 0
#if 0
			//回転量制限をかける
			if (rotationAngle > Math::PI * ik->IKRotateLimit * (iLink + 1))
				rotationAngle = Math::PI * ik->IKRotateLimit * (iLink + 1);
			if (rotationAngle < -Math::PI * ik->IKRotateLimit * (iLink + 1))
				rotationAngle = -Math::PI * ik->IKRotateLimit * (iLink + 1);

#else
			//回転量制限をかける
			//if (rotationAngle > ik->IKRotateLimit)
			//	rotationAngle = ik->IKRotateLimit;
			//if (rotationAngle < -ik->IKRotateLimit)
			//	rotationAngle = -ik->IKRotateLimit;
#endif

			// 回転軸
			Vector3 rotationAxis = Vector3::SafeNormalize(Vector3::Cross(localEffDir, localTargetDir));

			//if (ikLink.IsRotateLimit && iCalc < ikt)
			//{
			//	const Matrix& parentMatNormal = ikLinkBone->GetParent()->m_globalRotationMatrix;

			//	if (ikLink.MinLimit.y == 0.0f && ikLink.MaxLimit.y == 0.0f && ikLink.MinLimit.z == 0.0f && ikLink.MaxLimit.z == 0.0f)
			//	{
			//		rotationAxis.Set((rotationAxis.x >= 0.0f) ? 1.0f : -1.0f, 0.0f, 0.0f);
			//	}
			//	else
			//	{
			//		if (ikLink.MinLimit.x == 0.0f && ikLink.MaxLimit.x == 0.0f && ikLink.MinLimit.z == 0.0f && ikLink.MaxLimit.z == 0.0f)
			//		{
			//			rotationAxis.Set(0.0f, (rotationAxis.y >= 0.0f) ? 1.0f : -1.0f, 0.0f);
			//		}
			//		else
			//		{
			//			if (ikLink.MinLimit.x == 0.0f && ikLink.MaxLimit.x == 0.0f && ikLink.MinLimit.y == 0.0f && ikLink.MaxLimit.y == 0.0f)
			//			{
			//				rotationAxis.Set(0.0f, 0.0f, (rotationAxis.z >= 0.0f) ? 1.0f : -1.0f);
			//			}
			//		}
			//	}
			//}
			//




			if (!Math::IsNaN(rotationAngle) && rotationAngle > 1.0e-3f && !rotationAxis.IsNaNOrInf())
			{
				// 関節回転量の補正
				Quaternion rotQuat(rotationAxis, rotationAngle);
				ikLinkBone->GetLocalTransformPtr()->rotation = Quaternion::Multiply(ikLinkBone->GetLocalTransformPtr()->rotation, rotQuat);

				// 回転制限
				//LimitIKRotation(ikLink, ikLinkBone);

				// 末端のほうから、IK構成ボーンのグローバル行列を再更新する
				// TODO: IKLinks の順番は付け根→末端へを前提にしている。念のため事前チェックしたほうがいいかも
				for (short jk = ik->IKLinks.GetCount() - 1; jk >= 0; --jk)
				{
					m_allBoneList[ik->IKLinks[jk].LinkBoneIndex]->UpdateGlobalTransform(false);
				}
				effector->UpdateGlobalTransform(true);

				if (effector->m_combinedMatrix.GetPosition().y > 10)
				{
					effector->m_combinedMatrix.GetPosition().Print();
				}

				if (ikLinkBone->GetCore()->Name == _T("左足首") && ikLinkBone->m_combinedMatrix.GetPosition().y > 10)
				{
					ikLinkBone->m_combinedMatrix.GetPosition().Print();
				}
			}
#endif
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
	Matrix parentRotation;
	if (m_parent != nullptr)
	{
		m_combinedMatrix *= m_parent->GetCombinedMatrix();
	}

	if (m_combinedMatrix.IsNaNOrInf())
	{
		printf("");
	}

	// 子ボーン更新
	if (hierarchical)
	{
		for (SkinnedMeshBone* bone : m_children)
		{
			bone->UpdateGlobalTransform(hierarchical);
		}
	}

	m_position = Vector3::TransformCoord(GetCore()->OrgPosition, m_combinedMatrix);
	m_userRotation = m_localTransform.rotation;
	Matrix matrix2 = Matrix::MakeRotationQuaternion(/*m_morphRotation * */m_userRotation/* * m_inherehRotation*/);
	m_globalRotationMatrix = matrix2 * parentRotation;
	m_ikLocalRotationMatrix = Matrix::Identity;
	m_ikQuaternion = Quaternion::Identity;
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
