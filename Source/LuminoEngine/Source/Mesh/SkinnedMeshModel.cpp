
#include "../Internal.h"
#include <Lumino/Mesh/Mesh.h>
#include <Lumino/Mesh/SkinnedMeshModel.h>
#include <Lumino/Physics/CollisionShape.h>	// TODO: MMD でのみ必要
#include <Lumino/Physics/RigidBody.h>	// TODO: MMD でのみ必要
#include <Lumino/Physics/Joint.h>	// TODO: MMD でのみ必要
#include <Lumino/Physics/PhysicsWorld.h>	// TODO: MMD でのみ必要
#include "../Graphics/GraphicsManager.h"
#include "../Physics/PhysicsManager.h"
#include "PmxSkinnedMesh.h"

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
			if (ikBone->m_ikInfo)
			{
				UpdateEachIKBoneTransform(ikBone->m_ikInfo);
			}
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
		//Vector3 TargetGlobalPos = Vector3::transformCoord(ikBone->GetCore()->OrgPosition, ikBone->getCombinedMatrix());

		// IKボーンのグローバル位置
		const Vector3& targetPos = ikBone->getCombinedMatrix().getPosition();

		for (int iLink = 0; iLink < ik->IKLinks.getCount(); ++iLink)
		{
			PmxIKResource::IKLink& ikLink = ik->IKLinks[iLink];
			SkinnedMeshBone* ikLinkBone = owner->m_allBoneList[ikLink.LinkBoneIndex];

			// ワールド座標系から注目ノードの局所座標系への変換
			// (IKリンク基準のローカル座標系へ変換する行列)
			Matrix toLinkLocal = Matrix::makeInverse(ikLinkBone->getCombinedMatrix());

			//Vector3 effectorPos = Vector3::transformCoord(effector->GetCore()->OrgPosition, effector->getCombinedMatrix() * toLinkLocal);
			//Vector3 link2Effector = Vector3::safeNormalize(effectorPos - ikLinkBone->GetCore()->OrgPosition);
		
			//Vector3 targetPos = Vector3::transformCoord(TargetGlobalPos, toLinkLocal);
			//Vector3 link2Target = Vector3::safeNormalize(targetPos - ikLinkBone->GetCore()->OrgPosition);

			// エフェクタのグローバル位置
			const Vector3& effPos = effector->getCombinedMatrix().getPosition();

			// 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
			// (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
			Vector3 localEffPos = Vector3::transformCoord(effPos, toLinkLocal);

			// (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
			Vector3 localTargetPos = Vector3::transformCoord(targetPos, toLinkLocal);

			// (1) 基準関節→エフェクタ位置への方向ベクトル
			Vector3 link2Effector = Vector3::safeNormalize(localEffPos);
			// (2) 基準関節→目標位置への方向ベクトル
			Vector3 link2Target = Vector3::safeNormalize(localTargetPos);

			IKLinkCalc(ikLink, ikLinkBone, link2Effector, link2Target, ik->IKRotateLimit);
		}
	}

	void IKLinkCalc(PmxIKResource::IKLink& ikLink, SkinnedMeshBone* ikLinkBone, const Vector3& link2Effector, const Vector3& link2Target, float RotationLimited)
	{
		// 回転角度を求める
		float dot = Vector3::dot(link2Effector, link2Target);
		if (dot > 1.0f) dot = 1.0f;
		float rotationAngle = Math::clamp(acosf(dot), -RotationLimited, RotationLimited);
		if (Math::isNaN(rotationAngle)) return;
		if (rotationAngle <= 1.0e-3f) return;

		// 回転軸を求める
		Vector3 rotationAxis = Vector3::cross(link2Effector, link2Target);

		// 軸を中心として回転する行列を作成する
		Quaternion rotation = Quaternion(rotationAxis, rotationAngle);
		rotation.normalize();
		ikLinkBone->getLocalTransformPtr()->rotation = rotation * ikLinkBone->getLocalTransformPtr()->rotation;

		// 回転量制限
		ikLinkBone->getLocalTransformPtr()->rotation = RestrictRotation(ikLink, ikLinkBone->getLocalTransformPtr()->rotation);

		ikLinkBone->updateGlobalTransform(true);
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
		euler = localRot.toEulerAngles(type, &locked);
		if (locked)
		{
			type = RotationOrder::YZX;
			euler = localRot.toEulerAngles(type, &locked);
			if (locked)
			{
				type = RotationOrder::ZXY;
				euler = localRot.toEulerAngles(type, &locked);
				if (locked)
				{
					// localRotの要素がすべて 0 だった
					euler = Vector3::Zero;
				}
			}
		}

		// 角度修正
		NormalizeEular(&euler);
		euler.clamp(ikLink.MinLimit, ikLink.MaxLimit);

		// 戻す
		return Quaternion::makeFromRotationMatrix(Matrix::makeRotationEulerAngles(euler, type));
	}
};



//==============================================================================
// SkinnedMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SkinnedMeshModel, Object);

//------------------------------------------------------------------------------
SkinnedMeshModel::SkinnedMeshModel()
	: m_meshResources()
	, /*m_materials(nullptr)
	, */m_allBoneList()
	, m_rootBoneList()
	, m_skinningMatrices()
	, m_skinningMatricesTexture(nullptr)
	//, m_animator(nullptr)
	, m_needResetMorph(false)
{
}

//------------------------------------------------------------------------------
SkinnedMeshModel::~SkinnedMeshModel()
{
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::initialize(detail::GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	if (LN_REQUIRE(sharingMesh != nullptr)) return;

	m_mesh = Object::makeRef<StaticMeshModel>(sharingMesh);

	// メッシュ(バッファ類)は共有する
	m_meshResources.add(sharingMesh);

	//---------------------------------------------------------
	// マテリアルのインスタンス化
	{
		int count = sharingMesh->materials.getCount();
		m_mesh->m_materials = Ref<MaterialList>::makeRef();
		m_mesh->m_materials->resize(count);
		for (int i = 0; i < count; ++i)
		{
			m_mesh->m_materials->getAt(i, sharingMesh->materials.getAt(i)->MakeCommonMaterial());
		}
	}

	//---------------------------------------------------------
	// Bone のインスタンス化
	int boneCount = m_meshResources[0]->bones.getCount();
	if (boneCount > 0)
	{
		m_allBoneList.resize(boneCount);
		// まずは Bone を作る
		for (int i = 0; i < boneCount; i++)
		{
			m_allBoneList[i] = SkinnedMeshBonePtr::makeRef();
			m_allBoneList[i]->initialize(m_meshResources[0]->bones[i]);
			m_allBoneList[i]->setBoneIndex(i);
			
			// IK ボーンを集める
			if (m_meshResources[0]->bones[i]->IsIK)
			{
				m_ikBoneList.add(m_allBoneList[i]);
			}
		}
		// 次に子と親を繋げる
		for (int i = 0; i < boneCount; i++)
		{
			int parentIndex = m_meshResources[0]->bones[i]->ParentBoneIndex;
			if (0 <= parentIndex && parentIndex < boneCount)
			{
				m_allBoneList[parentIndex]->addChildBone(m_allBoneList[i]);
			}
			else
			{
				m_rootBoneList.add(m_allBoneList[i]);	// 親がいない。ルートボーンとして覚えておく
			}
		}
		for (SkinnedMeshBone* rootBone : m_rootBoneList)
		{
			rootBone->postInitialize(this, 0);
		}
		for (PmxIKResource* ik : m_meshResources[0]->iks)
		{
			m_allBoneList[ik->IKBoneIndex]->m_ikInfo = ik;
		}

		// ボーン行列を書き込むところを作る
		m_skinningMatrices.resize(boneCount);
		m_skinningMatricesTexture = newObject<Texture2D>(SizeI(4, boneCount), TextureFormat::R32G32B32A32_Float, false, ResourceUsage::Static);	// TODO: Dynamic、NoManaged

		m_skinningLocalQuaternions.resize(boneCount);
		m_skinningLocalQuaternionsTexture = newObject<Texture2D>(SizeI(1, boneCount), TextureFormat::R32G32B32A32_Float, false, ResourceUsage::Static);	// TODO: Dynamic、NoManaged

		// animation
		m_animationController = newObject<AnimationController>(this);
	}


	struct BoneComparer
	{
		int boneCount;

		bool operator () (SkinnedMeshBone* x, SkinnedMeshBone* y) const
		{
			//後であればあるほどスコアが大きくなるように計算する
			int xScore = 0;
			int yScore = 0;
			if (x->getCore()->TransformAfterPhysics)
			{
				xScore += boneCount * boneCount;
			}
			if (y->getCore()->TransformAfterPhysics)
			{
				yScore += boneCount * boneCount;
			}
			xScore += boneCount * x->m_depth;
			yScore += boneCount * y->m_depth;
			xScore += x->getCore()->getBoneIndex();
			yScore += y->getCore()->getBoneIndex();
			return xScore < yScore;
		}

	} cmp;
	cmp.boneCount = m_allBoneList.getCount();

	std::sort(m_ikBoneList.begin(), m_ikBoneList.end(), cmp);


	//---------------------------------------------------------
	// Morph のインスタンス化
	int morphCount = m_meshResources[0]->morphs.getCount();
	if (morphCount > 0)
	{
		for (int i = 0; i < morphCount; i++)
		{
			auto morph = newObject<SkinnedMeshMorph>(m_meshResources[0]->morphs[i]);
			m_morphs.add(morph);
		}
	}


	//---------------------------------------------------------
	// 物理演算
	//m_physicsWorld = newObject<PhysicsWorld>();
	//m_physicsWorld->setGravity(Vector3(0, -9.80f * 10.0f, 0));

	//m_rigidBodyList.resize(m_meshResource->rigidBodys.getCount());
	//for (int i = 0; i < m_meshResource->rigidBodys.getCount(); ++i)
	//{
	//	m_rigidBodyList[i] = Ref<detail::MmdSkinnedMeshRigidBody>::makeRef();
	//	m_rigidBodyList[i]->initialize(this, m_meshResource->rigidBodys[i], 1.0f);
	//}

	//m_jointList.resize(m_meshResource->joints.getCount());
	//for (int i = 0; i < m_meshResource->joints.getCount(); ++i)
	//{
	//	m_jointList[i] = Ref<detail::MmdSkinnedMeshJoint>::makeRef();
	//	m_jointList[i]->initialize(this, m_meshResource->joints[i]);
	//}
}

//------------------------------------------------------------------------------
// 姿勢更新① (ユーザー更新の前に確定する必要があるもの)
//		・ボーンのグローバル行列更新
//		・剛体更新 (フレーム位置を剛体位置へ)
void SkinnedMeshModel::preUpdate()
{
	updateBoneTransformHierarchy();

	if (m_needResetMorph)
	{
		m_meshResources[0]->morphBase->apply(m_meshResources[0]);
	}
}

//------------------------------------------------------------------------------
// 姿勢更新② (ユーザー更新の後、描画の前に行う必要があるもの)
//		・モーフ更新
//		・IK更新
//		・剛体更新 (剛体位置をフレーム位置へ)
//		・スキニング行列の作成
void SkinnedMeshModel::postUpdate()
{
	updateMorph();

	// IK 更新
	updateIK();
	updateBoneTransformHierarchy();

	// 付与適用
	updateBestow();

#if 1
	for (detail::MmdSkinnedMeshRigidBody* body : m_rigidBodyList)
	{
		body->updateBeforePhysics();
	}

	if (m_physicsWorld)
	{
		m_physicsWorld->stepSimulation(0.016f);
	}

	for (detail::MmdSkinnedMeshRigidBody* body : m_rigidBodyList)
	{
		body->updateAfterPhysics();
	}
#endif


	// スキニング行列の作成
	updateSkinningMatrices();
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::updateBoneTransformHierarchy()
{
	for (SkinnedMeshBone* bone : m_rootBoneList)
	{
		bone->updateGlobalTransform(true);
	}
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::updateSkinningMatrices()
{
	// スキニング行列の作成
	for (int i = 0; i < m_allBoneList.getCount(); i++)
	{
		/*
			初期姿勢は、スキニングしなくても同じ姿勢。
			つまり、頂点スキニングで初期姿勢にしたいときは Matrix::Identity のボーン行列を渡す。

			ボーンは最初からオフセットが入ってるけど、
			それをスキニングに適用すると姿勢が崩れてしまう。
			そのため、初期オフセットを打ち消す処理が必要。それが getInitialTranstormInv()。

			ID3DXSkinInfo::GetBoneOffsetMatrix() で
			取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) がこれにあたるものっぽい。
			サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
		*/
		m_skinningMatrices[i] = m_allBoneList[i]->getCore()->getInitialTranstormInv() * m_allBoneList[i]->getCombinedMatrix();
		//m_skinningMatrices[i] = m_allBoneList[i]->getCombinedMatrix();
	
		m_skinningLocalQuaternions[i] = Quaternion::makeFromRotationMatrix(m_skinningMatrices[i]);//m_allBoneList[i]->m_localTransform.rotation;//

	}

	// スキニングテクスチャ更新
	if (!m_skinningMatricesTexture.isNull())
	{
		m_skinningMatricesTexture->setMappedData(&m_skinningMatrices[0]);
		m_skinningLocalQuaternionsTexture->setMappedData(&m_skinningLocalQuaternions[0]);
	}

	// 全てのローカルトランスフォームをリセットする
	//		リセットしておかないと、IKで問題が出る。
	//		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
	//		なお、一連の更新の最後で行っているのは、アニメーションからの更新を外部で行っているため。
	// TODO: できれば一連の処理の中で必ず通るところに移動したい
	for (SkinnedMeshBone* bone : m_allBoneList)
	{
		bone->resetLocalTransform();
	}
}

void SkinnedMeshModel::updateMorph()
{
	for (auto& morph : m_morphs)
	{
		if (morph->apply(this))
		{
			// モーフにより頂点バッファが変更された。
			// 次回の更新の初めにリセットしたい。
			// (もしリセットする必要がなければ頂点バッファに触らないようにし、デバイスへの送信がされないようにする)
			m_needResetMorph = true;
		}
	}
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::updateIK()
{
	CCDIK ik;
	ik.owner = this;
	ik.UpdateTransform();
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::updateBestow()
{
	for (SkinnedMeshBone* bone : m_allBoneList)
	{
		if (bone->getCore()->IsMoveProvided)
		{
			SkinnedMeshBone* parent = m_allBoneList[bone->getCore()->ProvidedParentBoneIndex];
			bone->m_localTransform.translation += Vector3::lerp(Vector3::Zero, parent->m_localTransform.translation, bone->getCore()->ProvidedRatio);
		}
		if (bone->getCore()->IsRotateProvided)
		{
			SkinnedMeshBone* parent = m_allBoneList[bone->getCore()->ProvidedParentBoneIndex];
			bone->m_localTransform.rotation *= Quaternion::slerp(Quaternion::Identity, parent->m_localTransform.rotation, bone->getCore()->ProvidedRatio);
		}
	}
}

MeshResource* SkinnedMeshModel::getMeshResource(int index) const
{
	return m_meshResources[index];
}

SkinnedMeshBone* SkinnedMeshModel::findBone(const StringRef& boneName) const
{
	for (auto& bone : m_allBoneList)
	{
		if (bone->name() == boneName)
			return bone;
	}
	return nullptr;
}

void SkinnedMeshModel::updateBoneTransform(const Matrix& worldTransform)
{
	setWorldTransform(worldTransform);
	preUpdate();
	postUpdate();
}

int SkinnedMeshModel::getAnimationTargetElementCount() const
{
	return m_allBoneList.getCount();
}

const String& SkinnedMeshModel::getAnimationTargetElementName(int index) const
{
	return m_allBoneList[index]->name();
}

AnimationValueType SkinnedMeshModel::getAnimationTargetElementValueType(int index) const
{
	return AnimationValueType::Transform;
}

void SkinnedMeshModel::setAnimationTargetElementValue(int index, const AnimationValue& value)
{
	if (value.type() == AnimationValueType::Transform)
	{
		(*m_allBoneList[index]->getLocalTransformPtr()) = value.getTransform();
	}
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
	, m_boneIndex(-1)
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
void SkinnedMeshBone::initialize(PmxBoneResource* boneResource)
{
	m_core = boneResource;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::postInitialize(SkinnedMeshModel* owner, int depth)
{
	m_depth = depth;

	for (SkinnedMeshBone* bone : m_children)
	{
		bone->postInitialize(owner, m_depth + 1);
	}
}

const String& SkinnedMeshBone::name() const
{
	return m_core->Name;
}

//------------------------------------------------------------------------------
PmxBoneResource* SkinnedMeshBone::getCore() const
{
	return m_core;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::addChildBone(SkinnedMeshBone* bone)
{
	if (LN_REQUIRE(bone != nullptr)) return;
	if (LN_REQUIRE(bone->m_parent == nullptr)) return;
	m_children.add(bone);
	bone->m_parent = this;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::updateGlobalTransform(bool hierarchical)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	//m_combinedMatrix = m_localTransform;
	// TODO: * ではなく一気に作ったほうがはやいかも
	m_combinedMatrix =
		//Matrix::MakeTranslation(-m_core->OrgPosition) *
		Matrix::makeScaling(m_localTransform.scale) *
		Matrix::makeRotationQuaternion(m_localTransform.rotation) *
		Matrix::makeTranslation(m_localTransform.translation)/* *
															 Matrix::MakeTranslation(m_core->OrgPosition)*/;


	// 親からの平行移動量
	m_combinedMatrix.translate(m_core->getOffsetFromParent());

	//m_combinedMatrix =
	//	Matrix::MakeTranslation(-m_core->OrgPosition) *
	//	Matrix::makeRotationQuaternion(m_localTransform.rotation) *
	//	Matrix::MakeTranslation(m_localTransform.translation) *
	//	Matrix::MakeTranslation(m_core->OrgPosition);

	// 親行列と結合
	if (m_parent != nullptr)
	{
		m_combinedMatrix *= m_parent->getCombinedMatrix();
	}

	// 子ボーン更新
	if (hierarchical)
	{
		for (SkinnedMeshBone* bone : m_children)
		{
			bone->updateGlobalTransform(hierarchical);
		}
	}
}

//==============================================================================
// SkinnedMeshMorph
//==============================================================================
SkinnedMeshMorph::SkinnedMeshMorph()
	: m_core(nullptr)
	, m_weight(0.0f)
{
}

SkinnedMeshMorph::~SkinnedMeshMorph()
{
}

void SkinnedMeshMorph::initialize(PmxMorphResource* core)
{
	Object::initialize();
	m_core = core;
}

const String& SkinnedMeshMorph::name() const
{
	return m_core->Name;
}

void SkinnedMeshMorph::setWeight(float value)
{
	m_weight = value;
}

bool SkinnedMeshMorph::active() const
{
	return !Math::nearEqual(m_weight, 0.0f);
}

bool SkinnedMeshMorph::apply(SkinnedMeshModel* model)
{
	if (active())
	{
		switch (m_core->MorphType)
		{
		case ModelMorphType_Vertex:
		{
			MeshResource* targetMesh = model->m_meshResources[0];
			for (auto& mo : m_core->MorphOffsets)
			{
				const Vector3& v1 = targetMesh->getPosition(mo.VertexMorphOffset.VertexIndex);
				const Vector3* v2 = reinterpret_cast<Vector3*>(mo.VertexMorphOffset.PositionOffset);
				targetMesh->setPosition(mo.VertexMorphOffset.VertexIndex, Vector3::lerp(v1, v1 + (*v2), m_weight));
			}
			break;
		}
		case ModelMorphType_UV:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_AdditionalUV1:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_AdditionalUV2:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_AdditionalUV3:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_AdditionalUV4:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_Bone:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_Matrial:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_Group:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_Flip:
			LN_NOTIMPLEMENTED();
			break;
		case ModelMorphType_Impulse:
			LN_NOTIMPLEMENTED();
			break;
		default:
			break;
		}

		return true;
	}

	return false;
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
void MmdSkinnedMeshRigidBody::initialize(SkinnedMeshModel* ownerModel, PmxRigidBodyResource* rigidBodyResource, float scale)
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
	CollisionShapePtr collider;
	switch (rigidBodyResource->ColShapeData.Type)
	{
		case CollisionShapeType_Sphere:
		{
			collider = newObject<SphereCollisionShape>(m_resource->ColShapeData.Sphere.Radius * scale);
			break;
		}
		case CollisionShapeType_Box:
		{
			Vector3 size(m_resource->ColShapeData.Box.Width * scale, m_resource->ColShapeData.Box.Height * scale, m_resource->ColShapeData.Box.Depth * scale);
			size *= 2.0f;
			collider = newObject<BoxCollisionShape>(size);
			break;
		}
		case CollisionShapeType_Capsule:
		{
			collider = newObject<CapsuleCollisionShape>(m_resource->ColShapeData.Capsule.Radius * scale, m_resource->ColShapeData.Capsule.Height * scale);
			break;
		}
	}

	//Matrix bias;
	//bias.rotateZ(m_resource->.z);
	//bias.rotateX(pmd_rigidbody_->vec3Rotation.x);
	//bias.rotateY(pmd_rigidbody_->vec3Rotation.y);


	Matrix initialTransform = m_resource->InitialTransform;
	initialTransform.m41 *= scale;
	initialTransform.m42 *= scale;
	initialTransform.m43 *= scale;
	m_boneLocalPosition = Matrix::makeInverse(initialTransform);

	m_boneOffset = initialTransform;
	m_boneOffset.m41 -= m_bone->getCore()->OrgPosition.x;
	m_boneOffset.m42 -= m_bone->getCore()->OrgPosition.y;
	m_boneOffset.m43 -= m_bone->getCore()->OrgPosition.z;

	m_offsetBodyToBone = Matrix::makeTranslation(m_bone->getCore()->OrgPosition) * m_boneLocalPosition;

	RigidBodyComponent::ConfigData data;
	data.InitialTransform = initialTransform;
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

	m_rigidBody = Ref<RigidBodyComponent>::makeRef();
	m_rigidBody->initializeCore(collider, data, scale);
	ownerModel->m_physicsWorld->addPhysicsObject(m_rigidBody);
}

//------------------------------------------------------------------------------
RigidBodyComponent* MmdSkinnedMeshRigidBody::getRigidBody() const
{
	return m_rigidBody;
}

//------------------------------------------------------------------------------
void MmdSkinnedMeshRigidBody::updateBeforePhysics()
{
	if (m_resource->RigidBodyType == RigidBodyType_ControlledByBone)
	{
		Matrix t =
			//Matrix::MakeScalingScaling(1.0f / scale, 1.0f / scale, 1.0f / scale) *
			//m_resource->InitialTransform * /*Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition) **/
			
			m_boneOffset
			* m_bone->getCombinedMatrix() *
			m_ownerModel->getWorldTransform()
			;
		m_rigidBody->setWorldTransform(t);
		m_rigidBody->setLinearVelocity(Vector3::Zero);
		m_rigidBody->setAngularVelocity(Vector3::Zero);
		m_rigidBody->activate();
	}
	else
	{
		m_rigidBody->markMMDDynamic();
	}
}

//------------------------------------------------------------------------------
void MmdSkinnedMeshRigidBody::updateAfterPhysics()
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
			matrix = /*Matrix::MakeScalingScaling(scale, scale, scale) * */m_rigidBody->getWorldTransform();
		}

		if (matrix.isNaNOrInf())
		{
			matrix = Matrix::Identity;
		}

		matrix = matrix * m_ownerModel->getWorldTransformInverse();

		if (1/*NoRelatedBone*/)
		{
#if 1
#if 0
			// 剛体初期姿勢の逆行列を乗算することで、剛体がどれだけ動いたかがわかる
			Matrix globalPose = /*m_boneLocalPosition * */matrix;
			//Matrix globalPose = Matrix::MakeTranslation(matrix.GetPosition());
			//Matrix localPose = Matrix::makeInverse(m_resource->InitialTransform/*m_bone->GetParent()->getCombinedMatrix()*/) * globalPose;
			//Matrix localPose = m_boneLocalPosition * globalPose;
			Matrix localPose = globalPose * m_boneLocalPosition;
			//Matrix localPose = globalPose * Matrix::makeInverse(m_boneOffset);

			//Matrix localPose = globalPose * Matrix::makeInverse(m_resource->InitialTransform);
			//Matrix mat = Matrix::MakeTranslation(m_bone->GetCore()->OrgPosition) * localPose * Matrix::MakeTranslation(-m_bone->GetCore()->OrgPosition);
			//m_bone->m_combinedMatrix = globalPose;

			//localPose.translate(-m_bone->GetCore()->getOffsetFromParent());
			
			//m_bone->m_localTransform.translation = localPose.GetPosition();
			//m_bone->m_localTransform.rotation = Quaternion::makeFromRotationMatrix(localPose);
			//m_bone->updateGlobalTransform(false);


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
			Matrix mat = Matrix::makeTranslation(-m_bone->getCore()->OrgPosition) * m_boneLocalPosition * matrix;

			m_bone->m_combinedMatrix = mat;
#endif
		}
	}

	// ボーン位置合わせ
	if (m_resource->RigidBodyType == RigidBodyType_PhysicsAlignment)
	{
#if 0
		//Matrix parentMat = //m_bone->m_combinedMatrix;
		Matrix matrix = m_bone->m_combinedMatrix * m_boneLocalPosition;//Matrix::makeInverse(parentMat);    // ボーンのローカル行列(親から見てどれだけ移動しているか)
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
			m_bone->getCombinedMatrix() *
			m_ownerModel->getWorldTransform();
		Vector3 offset = t.getPosition();

		Matrix bodyMat = m_rigidBody->getWorldTransform();
		bodyMat.m41 = offset.x;
		bodyMat.m42 = offset.y;
		bodyMat.m43 = offset.z;
		m_rigidBody->setWorldTransform(bodyMat);

		// TODO: 位置合わせの時はボーンの姿勢も調整する。(回転成分のみを適用する)

		//uint32_t aa = 0xFFFFFFFF & -3;

		//Matrix matrix = m_bone->m_combinedMatrix * m_boneLocalPosition;
		//matrix.m41 = 0.0f;
		//matrix.m42 = 0.0f;
		//matrix.m43 = 0.0f;
		//m_bone->m_combinedMatrix =
		//	matrix *
		//	Matrix::MakeTranslation(m_bone->m_core->getOffsetFromParent()) *
		//	m_bone->GetParent()->getCombinedMatrix();

		//matrix.m41 = 0.0f;
		//matrix.m42 = 0.0f;
		//matrix.m43 = 0.0f;
		//m_bone->m_combinedMatrix =
		//	m_offsetBodyToBone * matrix *
		//	Matrix::MakeTranslation(m_bone->m_core->getOffsetFromParent()) * m_bone->GetParent()->getCombinedMatrix();
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
void MmdSkinnedMeshJoint::initialize(SkinnedMeshModel* ownerModel, PmxJointResource* jointResource)
{
	if (LN_REQUIRE(ownerModel != nullptr)) return;
	if (LN_REQUIRE(jointResource != nullptr)) return;

	MmdSkinnedMeshRigidBody* bodyA = ownerModel->m_rigidBodyList[jointResource->RigidBodyAIndex];
	MmdSkinnedMeshRigidBody* bodyB = ownerModel->m_rigidBodyList[jointResource->RigidBodyBIndex];

	// モデル空間上でのジョイント姿勢
	Matrix jointOffset =
		Matrix::makeRotationYawPitchRoll(jointResource->Rotation.y, jointResource->Rotation.x, jointResource->Rotation.z) *
		Matrix::makeTranslation(jointResource->Position);

	Matrix transInvA = Matrix::makeInverse(bodyA->getRigidBody()->getWorldTransform());
	Matrix transInvB = Matrix::makeInverse(bodyB->getRigidBody()->getWorldTransform());

	// 各剛体のローカルから見たジョイント位置
	Matrix frameInA = jointOffset * transInvA;
	Matrix frameInB = jointOffset * transInvB;

	m_joint = Ref<DofSpringJoint>::makeRef();
	m_joint->initialize(bodyA->getRigidBody(), bodyB->getRigidBody(), frameInA, frameInB);

	// SpringPositionStiffness.x
	if (jointResource->SpringPositionStiffness.x != 0.0f)
	{
		m_joint->setStiffness(0, jointResource->SpringPositionStiffness.x);
		m_joint->enableSpring(0, true);
	}
	else
	{
		m_joint->enableSpring(0, false);
	}

	// SpringPositionStiffness.y
	if (jointResource->SpringPositionStiffness.y != 0.0f)
	{
		m_joint->setStiffness(1, jointResource->SpringPositionStiffness.y);
		m_joint->enableSpring(1, true);
	}
	else
	{
		m_joint->enableSpring(1, false);
	}

	// SpringPositionStiffness.z
	if (jointResource->SpringPositionStiffness.z != 0.0f)
	{
		m_joint->setStiffness(2, jointResource->SpringPositionStiffness.z);
		m_joint->enableSpring(2, true);
	}
	else
	{
		m_joint->enableSpring(2, false);
	}

	m_joint->setStiffness(3, jointResource->SpringRotationStiffness.x);
	m_joint->enableSpring(3, true);
	m_joint->setStiffness(4, jointResource->SpringRotationStiffness.y);
	m_joint->enableSpring(4, true);
	m_joint->setStiffness(5, jointResource->SpringRotationStiffness.z);
	m_joint->enableSpring(5, true);

	m_joint->setLinearLowerLimit(jointResource->PositionLimitLower);
	m_joint->setLinearUpperLimit(jointResource->PositionLimitUpper);
	m_joint->setAngularLowerLimit(jointResource->RotationLimitLower);
	m_joint->setAngularUpperLimit(jointResource->RotationLimitUpper);

	ownerModel->m_physicsWorld->addJoint(m_joint);
} 

}

LN_NAMESPACE_END
