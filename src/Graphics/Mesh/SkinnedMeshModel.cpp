
#include "../Internal.h"
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Graphics/Mesh/SkinnedMeshModel.h>
#include <Lumino/Physics/Collider.h>	// TODO: MMD でのみ必要
#include <Lumino/Physics/RigidBody.h>	// TODO: MMD でのみ必要
#include <Lumino/Physics/Joint.h>	// TODO: MMD でのみ必要
#include "../GraphicsManager.h"
#include "../../Physics/PhysicsManager.h"
#include "../../Modeling/PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN


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

class CCDIK
{
public:

	SkinnedMeshModel* owner;

	void UpdateTransform()
	{
		for (SkinnedMeshBone* ikBone : owner->m_ikBoneList)
		{
			UpdateEachIKBoneTransform(ikBone->m_ikInfo);
		}
	}

	void UpdateEachIKBoneTransform(PmxIKResource* ik)
	{
		for (int iCalc = 0; iCalc < ik->LoopCount; iCalc++)
		{
			SkinnedMeshBone* ikBone = owner->m_allBoneList[ik->IKBoneIndex];			// IK ボーン (IK 情報を持つボーン。目標地点)
			SkinnedMeshBone* effector = owner->m_allBoneList[ik->IKTargetBoneIndex];	// IK ターゲットボーン (エフェクタ。IKに向くべきボーンたちの中の先頭ボーン)

			IKloop(ik, ikBone, effector);
		}
	}
	void IKloop(PmxIKResource* ik, SkinnedMeshBone* ikBone, SkinnedMeshBone* effector)
	{
		//Vector3 TargetGlobalPos = Vector3::TransformCoord(ikBone->GetCore()->OrgPosition, ikBone->GetCombinedMatrix());

		// IKボーンのグローバル位置
		const Vector3& targetPos = ikBone->GetCombinedMatrix().GetPosition();

		for (int iLink = 0; iLink < ik->IKLinks.GetCount(); ++iLink)
		{
			PmxIKResource::IKLink& ikLink = ik->IKLinks[iLink];
			SkinnedMeshBone* ikLinkBone = owner->m_allBoneList[ikLink.LinkBoneIndex];

			// ワールド座標系から注目ノードの局所座標系への変換
			// (IKリンク基準のローカル座標系へ変換する行列)
			Matrix toLinkLocal = Matrix::MakeInverse(ikLinkBone->GetCombinedMatrix());

			//Vector3 effectorPos = Vector3::TransformCoord(effector->GetCore()->OrgPosition, effector->GetCombinedMatrix() * toLinkLocal);
			//Vector3 link2Effector = Vector3::SafeNormalize(effectorPos - ikLinkBone->GetCore()->OrgPosition);
		
			//Vector3 targetPos = Vector3::TransformCoord(TargetGlobalPos, toLinkLocal);
			//Vector3 link2Target = Vector3::SafeNormalize(targetPos - ikLinkBone->GetCore()->OrgPosition);

			// エフェクタのグローバル位置
			const Vector3& effPos = effector->GetCombinedMatrix().GetPosition();

			// 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
			// (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
			Vector3 localEffPos = Vector3::TransformCoord(effPos, toLinkLocal);

			// (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
			Vector3 localTargetPos = Vector3::TransformCoord(targetPos, toLinkLocal);

			// (1) 基準関節→エフェクタ位置への方向ベクトル
			Vector3 link2Effector = Vector3::SafeNormalize(localEffPos);
			// (2) 基準関節→目標位置への方向ベクトル
			Vector3 link2Target = Vector3::SafeNormalize(localTargetPos);

			IKLinkCalc(ikLink, ikLinkBone, link2Effector, link2Target, ik->IKRotateLimit);
		}
	}

	void IKLinkCalc(PmxIKResource::IKLink& ikLink, SkinnedMeshBone* ikLinkBone, const Vector3& link2Effector, const Vector3& link2Target, float RotationLimited)
	{
		// 回転角度を求める
		float dot = Vector3::Dot(link2Effector, link2Target);
		if (dot > 1.0f) dot = 1.0f;
		float rotationAngle = Math::Clamp(acosf(dot), -RotationLimited, RotationLimited);
		if (Math::IsNaN(rotationAngle)) return;
		if (rotationAngle <= 1.0e-3f) return;

		// 回転軸を求める
		Vector3 rotationAxis = Vector3::Cross(link2Effector, link2Target);

		// 軸を中心として回転する行列を作成する
		Quaternion rotation = Quaternion(rotationAxis, rotationAngle);
		rotation.Normalize();
		ikLinkBone->GetLocalTransformPtr()->rotation = rotation * ikLinkBone->GetLocalTransformPtr()->rotation;

		// 回転量制限
		ikLinkBone->GetLocalTransformPtr()->rotation = RestrictRotation(ikLink, ikLinkBone->GetLocalTransformPtr()->rotation);

		ikLinkBone->UpdateGlobalTransform(true);
	}

	Quaternion RestrictRotation(const PmxIKResource::IKLink& ikLink, const Quaternion& localRot)
	{
		if (!ikLink.IsRotateLimit) return localRot;

		RotationOrder type;
		bool locked;
		Vector3 euler;

		// まずオイラー角に分解する。
		// 分解の試行順序は XYZ が一番最初でなければならない (Love&Joy モーションで破綻する)
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
					LN_CHECK_STATE(0);	// あり得ないはずだが…。localRotの要素がすべて0とか。
				}
			}
		}

		// 角度修正
		NormalizeEular(&euler);
		euler.Clamp(ikLink.MinLimit, ikLink.MaxLimit);

		// 戻す
		return Quaternion::MakeFromRotationMatrix(Matrix::MakeRotationEulerAngles(euler, type));
	}
};



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
			
			// IK ボーンを集める
			if (m_meshResource->bones[i]->IsIK)
			{
				m_ikBoneList.Add(m_allBoneList[i]);
			}
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
		for (SkinnedMeshBone* rootBone : m_rootBoneList)
		{
			rootBone->PostInitialize(this, 0);
		}
		for (PmxIKResource* ik : m_meshResource->iks)
		{
			m_allBoneList[ik->IKBoneIndex]->m_ikInfo = ik;
		}

		// ボーン行列を書き込むところを作る
		m_skinningMatrices.Resize(boneCount);
		m_skinningMatricesTexture = RefPtr<Texture2D>::MakeRef();
		m_skinningMatricesTexture->Initialize(manager, SizeI(4, boneCount), TextureFormat::R32G32B32A32_Float, false);	// TODO: Dynamic、NoManaged

		m_skinningLocalQuaternions.Resize(boneCount);
		m_skinningLocalQuaternionsTexture = RefPtr<Texture2D>::MakeRef();
		m_skinningLocalQuaternionsTexture->Initialize(manager, SizeI(1, boneCount), TextureFormat::R32G32B32A32_Float, false);	// TODO: Dynamic、NoManaged

		// アニメーション管理
		m_animator = RefPtr<Animator>::MakeRef();
		m_animator->Create(this);
	}


	struct BoneComparer
	{
		int boneCount;

		bool operator () (SkinnedMeshBone* x, SkinnedMeshBone* y) const
		{
			//後であればあるほどスコアが大きくなるように計算する
			int xScore = 0;
			int yScore = 0;
			if (x->GetCore()->TransformAfterPhysics)
			{
				xScore += boneCount * boneCount;
			}
			if (y->GetCore()->TransformAfterPhysics)
			{
				yScore += boneCount * boneCount;
			}
			xScore += boneCount * x->m_depth;
			yScore += boneCount * y->m_depth;
			xScore += x->GetCore()->GetBoneIndex();
			yScore += y->GetCore()->GetBoneIndex();
			return xScore < yScore;
		}

	} cmp;
	cmp.boneCount = m_allBoneList.GetCount();

	std::sort(m_ikBoneList.begin(), m_ikBoneList.end(), cmp);

	//---------------------------------------------------------
	// 物理演算
	m_physicsWorld = RefPtr<detail::PhysicsWorld>::MakeRef();
	m_physicsWorld->Initialize(manager->GetPhysicsManager());
	m_physicsWorld->SetGravity(Vector3(0, -9.80f * 10.0f, 0));

	m_rigidBodyList.Resize(m_meshResource->rigidBodys.GetCount());
	for (int i = 0; i < m_meshResource->rigidBodys.GetCount(); ++i)
	{
		m_rigidBodyList[i] = RefPtr<detail::MmdSkinnedMeshRigidBody>::MakeRef();
		m_rigidBodyList[i]->Initialize(this, m_meshResource->rigidBodys[i], 1.0f);
	}

	m_jointList.Resize(m_meshResource->joints.GetCount());
	for (int i = 0; i < m_meshResource->joints.GetCount(); ++i)
	{
		m_jointList[i] = RefPtr<detail::MmdSkinnedMeshJoint>::MakeRef();
		m_jointList[i]->Initialize(this, m_meshResource->joints[i]);
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
	UpdateBoneTransformHierarchy();

	// 付与適用
	UpdateBestow();

#if 1
	for (detail::MmdSkinnedMeshRigidBody* body : m_rigidBodyList)
	{
		body->UpdateBeforePhysics();
	}

	m_physicsWorld->StepSimulation(0.016);


	for (detail::MmdSkinnedMeshRigidBody* body : m_rigidBodyList)
	{
		body->UpdateAfterPhysics();
	}
#endif


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
		m_skinningMatrices[i] = m_allBoneList[i]->GetCore()->GetInitialTranstormInv() * m_allBoneList[i]->GetCombinedMatrix();
		//m_skinningMatrices[i] = m_allBoneList[i]->GetCombinedMatrix();
	
		m_skinningLocalQuaternions[i] = Quaternion::MakeFromRotationMatrix(m_skinningMatrices[i]);//m_allBoneList[i]->m_localTransform.rotation;//

	}

	// スキニングテクスチャ更新
	if (!m_skinningMatricesTexture.IsNull())
	{
		m_skinningMatricesTexture->SetSubData(Point(0, 0), &m_skinningMatrices[0]);
		m_skinningLocalQuaternionsTexture->SetSubData(Point(0, 0), &m_skinningLocalQuaternions[0]);
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
void SkinnedMeshModel::UpdateIK()
{
	CCDIK ik;
	ik.owner = this;
	ik.UpdateTransform();
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateBestow()
{
	for (SkinnedMeshBone* bone : m_allBoneList)
	{
		if (bone->GetCore()->IsMoveProvided)
		{
			SkinnedMeshBone* parent = m_allBoneList[bone->GetCore()->ProvidedParentBoneIndex];
			bone->m_localTransform.translation += Vector3::Lerp(Vector3::Zero, parent->m_localTransform.translation, bone->GetCore()->ProvidedRatio);
		}
		if (bone->GetCore()->IsRotateProvided)
		{
			SkinnedMeshBone* parent = m_allBoneList[bone->GetCore()->ProvidedParentBoneIndex];
			bone->m_localTransform.rotation *= Quaternion::Slerp(Quaternion::Identity, parent->m_localTransform.rotation, bone->GetCore()->ProvidedRatio);
		}
	}
}

//------------------------------------------------------------------------------
int SkinnedMeshModel::GetAnimationTargetAttributeCount() const
{
	return m_allBoneList.GetCount();
}

//------------------------------------------------------------------------------
detail::IAnimationTargetAttribute* SkinnedMeshModel::GetAnimationTargetAttribute(int index)
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
	, m_depth(0)
	, m_ikInfo(nullptr)
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
void SkinnedMeshBone::PostInitialize(SkinnedMeshModel* owner, int depth)
{
	m_depth = depth;

	for (SkinnedMeshBone* bone : m_children)
	{
		bone->PostInitialize(owner, m_depth + 1);
	}
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
	//m_combinedMatrix = m_localTransform;
	m_combinedMatrix =
		//Matrix::MakeTranslation(-m_core->OrgPosition) *
		Matrix::MakeRotationQuaternion(m_localTransform.rotation) *
		Matrix::MakeTranslation(m_localTransform.translation)/* *
															 Matrix::MakeTranslation(m_core->OrgPosition)*/;


	// 親からの平行移動量
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	//m_combinedMatrix =
	//	Matrix::MakeTranslation(-m_core->OrgPosition) *
	//	Matrix::MakeRotationQuaternion(m_localTransform.rotation) *
	//	Matrix::MakeTranslation(m_localTransform.translation) *
	//	Matrix::MakeTranslation(m_core->OrgPosition);

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


namespace detail
{

//==============================================================================
// MmdSkinnedMeshRigidBody
//------------------------------------------------------------------------------
/*
	剛体はシーンのワールド空間に配置され、物理演算される。
	ボーンの位置へ移動する、またはボーンを剛体の位置へ移動する場合は
	ワールド空間⇔モデルグローバル空間の変換を伴う。
*/
//==============================================================================

//------------------------------------------------------------------------------
MmdSkinnedMeshRigidBody::MmdSkinnedMeshRigidBody()
	: m_ownerModel(nullptr)
	, m_resource(nullptr)
	, m_bone(nullptr)
	, m_rigidBody(nullptr)
	, m_boneLocalPosition()
{

}

//------------------------------------------------------------------------------
MmdSkinnedMeshRigidBody::~MmdSkinnedMeshRigidBody()
{

}

//------------------------------------------------------------------------------
void MmdSkinnedMeshRigidBody::Initialize(SkinnedMeshModel* ownerModel, PmxRigidBodyResource* rigidBodyResource, float scale)
{
	m_ownerModel = ownerModel;
	m_resource = rigidBodyResource;
	if (m_resource->RelatedBoneIndex > 0)
	{
		m_bone = ownerModel->m_allBoneList[m_resource->RelatedBoneIndex];
	}
	else
	{
		m_bone = nullptr;
	}

	// 衝突判定形状
	ColliderPtr collider;
	switch (rigidBodyResource->ColShapeData.Type)
	{
		case CollisionShapeType_Sphere:
		{
			auto c = SphereColliderPtr::MakeRef();
			c->Initialize(m_resource->ColShapeData.Sphere.Radius * scale);
			collider = c;
			break;
		}
		case CollisionShapeType_Box:
		{
			auto c= BoxColliderPtr::MakeRef();
			Vector3 size(m_resource->ColShapeData.Box.Width * scale, m_resource->ColShapeData.Box.Height * scale, m_resource->ColShapeData.Box.Depth * scale);
			size *= 2.0f;
			c->Initialize(size);
			collider = c;
			break;
		}
		case CollisionShapeType_Capsule:
		{
			auto c = CapsuleColliderPtr::MakeRef();
			c->Initialize(m_resource->ColShapeData.Capsule.Radius * scale, m_resource->ColShapeData.Capsule.Height * scale);
			collider = c;
			break;
		}
	}

	//Matrix bias;
	//bias.RotateZ(m_resource->.z);
	//bias.RotateX(pmd_rigidbody_->vec3Rotation.x);
	//bias.RotateY(pmd_rigidbody_->vec3Rotation.y);


	Matrix initialTransform = m_resource->InitialTransform;
	initialTransform.m41 *= scale;
	initialTransform.m42 *= scale;
	initialTransform.m43 *= scale;
	m_boneLocalPosition = Matrix::MakeInverse(initialTransform);

	m_boneOffset = initialTransform;
	m_boneOffset.m41 -= m_bone->GetCore()->OrgPosition.x;
	m_boneOffset.m42 -= m_bone->GetCore()->OrgPosition.y;
	m_boneOffset.m43 -= m_bone->GetCore()->OrgPosition.z;

	m_offsetBodyToBone = Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition) * m_boneLocalPosition;

	RigidBody::ConfigData data;
	data.InitialTransform = &initialTransform;
	data.Scale = scale;
	data.Group = m_resource->Group;
	data.GroupMask = m_resource->GroupMask;
	data.Friction = m_resource->Friction;
	data.Restitution = m_resource->Restitution;		// HitFraction	
	data.LinearDamping = m_resource->LinearDamping;
	data.AngularDamping = m_resource->AngularDamping;
	data.AdditionalDamping = true;
	if (m_resource->RigidBodyType == RigidBodyType_Physics ||
		m_resource->RigidBodyType == RigidBodyType_PhysicsAlignment)
	{
		data.Mass = m_resource->Mass;
		data.KinematicObject = false;
	}
	else
	{
		data.KinematicObject = true;
	}

	m_rigidBody = RefPtr<RigidBody>::MakeRef();
	m_rigidBody->Initialize(collider, data);
	ownerModel->m_physicsWorld->AddRigidBody(m_rigidBody);
}

//------------------------------------------------------------------------------
RigidBody* MmdSkinnedMeshRigidBody::GetRigidBody() const
{
	return m_rigidBody;
}

//------------------------------------------------------------------------------
void MmdSkinnedMeshRigidBody::UpdateBeforePhysics()
{
	if (m_resource->RigidBodyType == RigidBodyType_ControlledByBone)
	{
		Matrix t =
			//Matrix::MakeScalingScaling(1.0f / scale, 1.0f / scale, 1.0f / scale) *
			//m_resource->InitialTransform * /*Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition) **/
			
			m_boneOffset
			* m_bone->GetCombinedMatrix() *
			m_ownerModel->GetWorldTransform()
			;
		m_rigidBody->SetWorldTransform(t);
		m_rigidBody->SetLinearVelocity(Vector3::Zero);
		m_rigidBody->SetAngularVelocity(Vector3::Zero);
		m_rigidBody->Activate();
	}
	else
	{
		m_rigidBody->MarkMMDDynamic();
	}
}

//------------------------------------------------------------------------------
void MmdSkinnedMeshRigidBody::UpdateAfterPhysics()
{
	Matrix matrix;

	if (m_resource->RigidBodyType == RigidBodyType_Physics ||
		m_resource->RigidBodyType == RigidBodyType_PhysicsAlignment)
	{

		if (0/*stillMode*/)  // 「静止」モード・物理演算のぷるぷるを抑える
		{
		}
		else
		{
			matrix = /*Matrix::MakeScalingScaling(scale, scale, scale) * */m_rigidBody->GetWorldTransform();
		}

		if (matrix.IsNaNOrInf())
		{
			matrix = Matrix::Identity;
		}

		matrix = matrix * m_ownerModel->GetWorldTransformInverse();

		if (1/*NoRelatedBone*/)
		{
#if 1
#if 0
			// 剛体初期姿勢の逆行列を乗算することで、剛体がどれだけ動いたかがわかる
			Matrix globalPose = /*m_boneLocalPosition * */matrix;
			//Matrix globalPose = Matrix::MakeTranslation(matrix.GetPosition());
			//Matrix localPose = Matrix::MakeInverse(m_resource->InitialTransform/*m_bone->GetParent()->GetCombinedMatrix()*/) * globalPose;
			//Matrix localPose = m_boneLocalPosition * globalPose;
			Matrix localPose = globalPose * m_boneLocalPosition;
			//Matrix localPose = globalPose * Matrix::MakeInverse(m_boneOffset);

			//Matrix localPose = globalPose * Matrix::MakeInverse(m_resource->InitialTransform);
			//Matrix mat = Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition) * localPose * Matrix::MakeTranslation(-m_bone->GetCore()->OrgPosition);
			//m_bone->m_combinedMatrix = globalPose;

			//localPose.Translate(-m_bone->GetCore()->GetOffsetFromParent());
			
			//m_bone->m_localTransform.translation = localPose.GetPosition();
			//m_bone->m_localTransform.rotation = Quaternion::MakeFromRotationMatrix(localPose);
			//m_bone->UpdateGlobalTransform(false);


			//if (m_resource->RigidBodyType == RigidBodyType_PhysicsAlignment)
			//{
			//	localPose.m41 = 0.0f;
			//	localPose.m42 = 0.0f;
			//	localPose.m43 = 0.0f;
			//}

			m_bone->m_combinedMatrix =
				localPose *
				Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition)
				;

			//m_bone->m_combinedMatrix = matrix;
#endif

			//----
			//matrix.m41 = m_resource->InitialTransform.GetPosition().x;
			//matrix.m42 = m_resource->InitialTransform.GetPosition().y;
			//matrix.m43 = m_resource->InitialTransform.GetPosition().z;

			m_bone->m_combinedMatrix =
				(m_offsetBodyToBone * matrix);

#else
			Matrix mat = Matrix::MakeTranslation(-m_bone->GetCore()->OrgPosition) * m_boneLocalPosition * matrix;

			m_bone->m_combinedMatrix = mat;
#endif
		}
	}

	// ボーン位置合わせ
	if (m_resource->RigidBodyType == RigidBodyType_PhysicsAlignment)
	{
#if 0
		//Matrix parentMat = //m_bone->m_combinedMatrix;
		Matrix matrix = m_bone->m_combinedMatrix * m_boneLocalPosition;//Matrix::MakeInverse(parentMat);    // ボーンのローカル行列(親から見てどれだけ移動しているか)
		//if ((frame.Bone.BoneFlags & BoneType.TransformAfterPhysics) > (BoneType)0)
		//{
		//	Vector3 vector = frame.Bone.Position;
		//	Matrix matrix2 = matrix;
		//	matrix2.M41 = (matrix2.M42 = (matrix2.M43 = 0f));
		//	matrix = Matrix.Translation(-vector) * frame.RotationMatrix * matrix2 * Matrix.Translation(frame.UserXYZ) * Matrix.Translation(vector);
		//	frame.CombinedMatrix = matrix * parentMat;
		//}
		Matrix bodyMat = m_rigidBody->GetWorldTransform();
		bodyMat.Translate(-matrix.m41, -matrix.m42, -matrix.m43);
		m_rigidBody->SetWorldTransform(bodyMat);

		//matrix.m41 = 0.0f;
		//matrix.m42 = 0.0f;
		//matrix.m43 = 0.0f;
		//m_bone->m_combinedMatrix =
		//	matrix *
		//	Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition) *
		//	parentMat;
#endif
		//Vector3 offset = /*m_bone->m_combinedMatrix.GetPosition() + */m_resource->InitialTransform.GetPosition();

		// ボーン位置合わせの時の剛体のあるべき姿勢
		Matrix t =
			m_boneOffset *
			m_bone->GetCombinedMatrix() *
			m_ownerModel->GetWorldTransform();
		Vector3 offset = t.GetPosition();

		Matrix bodyMat = m_rigidBody->GetWorldTransform();
		bodyMat.m41 = offset.x;
		bodyMat.m42 = offset.y;
		bodyMat.m43 = offset.z;
		m_rigidBody->SetWorldTransform(bodyMat);

		// TODO: 位置合わせの時はボーンの姿勢も調整する。(回転成分のみを適用する)

		//uint32_t aa = 0xFFFFFFFF & -3;

		//Matrix matrix = m_bone->m_combinedMatrix * m_boneLocalPosition;
		//matrix.m41 = 0.0f;
		//matrix.m42 = 0.0f;
		//matrix.m43 = 0.0f;
		//m_bone->m_combinedMatrix =
		//	matrix *
		//	Matrix::MakeTranslation(m_bone->m_core->GetOffsetFromParent()) *
		//	m_bone->GetParent()->GetCombinedMatrix();

		//matrix.m41 = 0.0f;
		//matrix.m42 = 0.0f;
		//matrix.m43 = 0.0f;
		//m_bone->m_combinedMatrix =
		//	m_offsetBodyToBone * matrix *
		//	Matrix::MakeTranslation(m_bone->m_core->GetOffsetFromParent()) * m_bone->GetParent()->GetCombinedMatrix();
	}
}

//==============================================================================
// MmdSkinnedMeshJoint
//==============================================================================

//------------------------------------------------------------------------------
MmdSkinnedMeshJoint::MmdSkinnedMeshJoint()
	: m_joint(nullptr)
{

}

//------------------------------------------------------------------------------
MmdSkinnedMeshJoint::~MmdSkinnedMeshJoint()
{

}

//------------------------------------------------------------------------------
void MmdSkinnedMeshJoint::Initialize(SkinnedMeshModel* ownerModel, PmxJointResource* jointResource)
{
	LN_CHECK_ARG(ownerModel != nullptr);
	LN_CHECK_ARG(jointResource != nullptr);

	MmdSkinnedMeshRigidBody* bodyA = ownerModel->m_rigidBodyList[jointResource->RigidBodyAIndex];
	MmdSkinnedMeshRigidBody* bodyB = ownerModel->m_rigidBodyList[jointResource->RigidBodyBIndex];

	// モデル空間上でのジョイント姿勢
	Matrix jointOffset =
		Matrix::MakeRotationYawPitchRoll(jointResource->Rotation.y, jointResource->Rotation.x, jointResource->Rotation.z) *
		Matrix::MakeTranslation(jointResource->Position);

	Matrix transInvA = Matrix::MakeInverse(bodyA->GetRigidBody()->GetWorldTransform());
	Matrix transInvB = Matrix::MakeInverse(bodyB->GetRigidBody()->GetWorldTransform());

	// 各剛体のローカルから見たジョイント位置
	Matrix frameInA = jointOffset * transInvA;
	Matrix frameInB = jointOffset * transInvB;

	m_joint = RefPtr<DofSpringJoint>::MakeRef();
	m_joint->Initialize(bodyA->GetRigidBody(), bodyB->GetRigidBody(), frameInA, frameInB);

	// SpringPositionStiffness.x
	if (jointResource->SpringPositionStiffness.x != 0.0f)
	{
		m_joint->SetStiffness(0, jointResource->SpringPositionStiffness.x);
		m_joint->EnableSpring(0, true);
	}
	else
	{
		m_joint->EnableSpring(0, false);
	}

	// SpringPositionStiffness.y
	if (jointResource->SpringPositionStiffness.y != 0.0f)
	{
		m_joint->SetStiffness(1, jointResource->SpringPositionStiffness.y);
		m_joint->EnableSpring(1, true);
	}
	else
	{
		m_joint->EnableSpring(1, false);
	}

	// SpringPositionStiffness.z
	if (jointResource->SpringPositionStiffness.z != 0.0f)
	{
		m_joint->SetStiffness(2, jointResource->SpringPositionStiffness.z);
		m_joint->EnableSpring(2, true);
	}
	else
	{
		m_joint->EnableSpring(2, false);
	}

	m_joint->SetStiffness(3, jointResource->SpringRotationStiffness.x);
	m_joint->EnableSpring(3, true);
	m_joint->SetStiffness(4, jointResource->SpringRotationStiffness.y);
	m_joint->EnableSpring(4, true);
	m_joint->SetStiffness(5, jointResource->SpringRotationStiffness.z);
	m_joint->EnableSpring(5, true);

	m_joint->SetLinearLowerLimit(jointResource->PositionLimitLower);
	m_joint->SetLinearUpperLimit(jointResource->PositionLimitUpper);
	m_joint->SetAngularLowerLimit(jointResource->RotationLimitLower);
	m_joint->SetAngularUpperLimit(jointResource->RotationLimitUpper);

	ownerModel->m_physicsWorld->AddJoint(m_joint);
} 

}

LN_NAMESPACE_END
