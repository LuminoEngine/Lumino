
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "MeshModelInstance.hpp"
#include "PmxImporter.hpp"	// TODO: 依存したくない
#include "MeshManager.hpp"
#include "CCDIKSolver.hpp"

namespace ln {

#if 0
//==============================================================================
// SkinnedMeshBone

const String& SkinnedMeshBone::name() const
{
	return m_data->Name;
}

void SkinnedMeshBone::postInitialize(SkinnedMeshModel* owner, int depth)
{
    m_depth = depth;

    for (SkinnedMeshBone* bone : m_children)
    {
        bone->postInitialize(owner, m_depth + 1);
    }
}

void SkinnedMeshBone::addChildBone(SkinnedMeshBone* bone)
{
    if (LN_REQUIRE(bone != nullptr)) return;
    if (LN_REQUIRE(bone->m_parent == nullptr)) return;
    m_children.add(bone);
    bone->m_parent = this;
}

void SkinnedMeshBone::updateGlobalTransform(bool hierarchical)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	//m_combinedMatrix = m_localTransform;
	// TODO: * ではなく一気に作ったほうがはやいかも
    m_combinedMatrix =
        //Matrix::makeTranslation(-m_data->OrgPosition) *
        Matrix::makeScaling(m_localTransform.scale) *
        Matrix::makeRotationQuaternion(m_localTransform.rotation) *
        Matrix::makeTranslation(m_localTransform.translation);// *
        //Matrix::makeTranslation(m_data->OrgPosition);


															 // 親からの平行移動量
	m_combinedMatrix.translate(m_data->getOffsetFromParent());

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
#endif

//==============================================================================
// MeshBone

MeshNode* MeshBone::node() const
{
	return m_skeleton->m_model->m_nodes[m_node];
}

//==============================================================================
// MeshArmature

MeshArmature::MeshArmature()
{
}

bool MeshArmature::init(SkinnedMeshModel* model)
{
	if (!Object::init()) return false;

	m_model = model;
	return true;
}

MeshBone* MeshArmature::bone(int index) const
{
	return m_bones[index];
}

//const Matrix& MeshArmature::boneGlobalMatrix(int index) const
//{
//}

void MeshArmature::addBone(int linkNode, const Matrix& inverseInitialMatrix)
{
	auto bone = makeObject<MeshBone>();
	bone->m_skeleton = this;
	bone->m_node = linkNode;
	bone->m_inverseInitialMatrix = inverseInitialMatrix;
	m_bones.add(bone);

	if (bone->node()->parentNodeIndex() >= 0) {
		m_rootBones.add(bone);
	}
	bone->node()->m_boneNode = true;
}

//==============================================================================
// SkinnedMeshModel

Ref<SkinnedMeshModel> SkinnedMeshModel::load(const StringRef& filePath)
{
	return detail::EngineDomain::meshManager()->createSkinnedMeshModel(filePath, 1.0f);
}

SkinnedMeshModel::SkinnedMeshModel()
    : StaticMeshModel(detail::InternalMeshModelType::SkinnedMesh)
{

	std::fill(m_humanoidBoneNodeIndices.begin(), m_humanoidBoneNodeIndices.end(), -1);
}

MeshNode* SkinnedMeshModel::findHumanoidBone(HumanoidBones boneKind) const
{
	int index = humanoidBoneIndex(boneKind);
	if (index >= 0)
		return m_nodes[index];
	else
		return nullptr;
}

void SkinnedMeshModel::addSkeleton(MeshArmature* skeleton)
{
	m_skeletons.add(skeleton);
}

AnimationController* SkinnedMeshModel::animationController() const
{
	return m_animationController;
}

void SkinnedMeshModel::beginUpdate()
{
    // 全てのローカルトランスフォームをリセットする
    //		リセットしておかないと、IKで問題が出る。
    //		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
    //		なお、一連の更新の最後で行っているのは、アニメーションからの更新を外部で行っているため。
    // TODO: できれば一連の処理の中で必ず通るところに移動したい
	resetNodeLocalTransforms();

}

// 姿勢更新① (ユーザー更新の前に確定する必要があるもの)
//		・ボーンのグローバル行列更新
//		・剛体更新 (フレーム位置を剛体位置へ)
void SkinnedMeshModel::preUpdate()
{


	updateBoneTransformHierarchy();

#ifdef SMESH_MIG
	if (m_needResetMorph)
	{
		m_meshResources[0]->morphBase->apply(m_meshResources[0]);
	}
#endif
}

// 姿勢更新② (ユーザー更新の後、描画の前に行う必要があるもの)
//		・モーフ更新
//		・IK更新
//		・剛体更新 (剛体位置をフレーム位置へ)
//		・スキニング行列の作成
void SkinnedMeshModel::postUpdate()
{
#ifdef SMESH_MIG
	updateMorph();

#endif
	// IK 更新
	updateIK();

	updateBoneTransformHierarchy();

#ifdef SMESH_MIG
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
#endif
}

void SkinnedMeshModel::updateBoneTransformHierarchy()
{
	updateNodeTransforms();

	//for (SkinnedMeshBone* bone : m_rootBoneList)
	//{
	//	bone->updateGlobalTransform(true);
	//}
}

void SkinnedMeshModel::updateIK()
{
    detail::CCDIKSolver ik;
    ik.owner = this;
	ik.m_skeleton = m_skeletons[0];
    ik.UpdateTransform();
}

void SkinnedMeshModel::writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer)
{
  //  for (int i = 0; i < m_allBoneList.size(); i++)
  //  {
		//matrixesBuffer[i] = m_allBoneList[i]->m_data->getInitialTranstormInv() * m_allBoneList[i]->getCombinedMatrix();
  //      localQuaternionsBuffer[i] = Quaternion::makeFromRotationMatrix(matrixesBuffer[i]);
  //  }
}

//int SkinnedMeshModel::getAnimationTargetElementCount() const
//{
//	return 0;//m_allBoneList.size();
//}
//
//const String& SkinnedMeshModel::getAnimationTargetElementName(int index) const
//{
//	return String::Empty;//m_allBoneList[index]->name();
//}
//
//AnimationValueType SkinnedMeshModel::getAnimationTargetElementValueType(int index) const
//{
//	return AnimationValueType::Transform;
//}
//
//void SkinnedMeshModel::setAnimationTargetElementValue(int index, const AnimationValue& value)
//{
//	//if (value.type() == AnimationValueType::Transform)
//	//{
//	//	(*m_allBoneList[index]->localTransformPtr()) = value.getTransform();
//	//}
//}

Ref<detail::MeshModelInstance> SkinnedMeshModel::createMeshModelInstance()
{
	return makeRef<detail::MeshModelInstance>(this);
}

// 左手座標系状、Z+を正面としたときの、各ボーンの位置関係を検証する。
void SkinnedMeshModel::verifyHumanoidBones()
{
	updateNodeTransforms();

	struct Pair { HumanoidBones left; HumanoidBones right; };
	Pair sidePairs[] = {
		{ HumanoidBones::LeftShoulder, HumanoidBones::RightShoulder },
		{ HumanoidBones::LeftUpperArm, HumanoidBones::RightUpperArm },
		{ HumanoidBones::LeftLowerArm, HumanoidBones::RightLowerArm },
		{ HumanoidBones::LeftHand, HumanoidBones::RightHand },

		{ HumanoidBones::LeftUpperLeg, HumanoidBones::RightUpperLeg },
		{ HumanoidBones::LeftLowerLeg, HumanoidBones::RightLowerLeg },
		{ HumanoidBones::LeftFoot, HumanoidBones::RightFoot },
		{ HumanoidBones::LeftToes, HumanoidBones::RightToes },

		{ HumanoidBones::LeftThumbProximal, HumanoidBones::RightThumbProximal },
		{ HumanoidBones::LeftThumbIntermediate, HumanoidBones::RightThumbIntermediate },
		{ HumanoidBones::LeftThumbDistal, HumanoidBones::RightThumbDistal },
		{ HumanoidBones::LeftIndexProximal, HumanoidBones::RightIndexProximal },
		{ HumanoidBones::LeftIndexIntermediate, HumanoidBones::RightIndexIntermediate },
		{ HumanoidBones::LeftIndexDistal, HumanoidBones::RightIndexDistal },
		{ HumanoidBones::LeftMiddleProximal, HumanoidBones::RightMiddleProximal },
		{ HumanoidBones::LeftMiddleIntermediate, HumanoidBones::RightMiddleIntermediate },
		{ HumanoidBones::LeftMiddleDistal, HumanoidBones::RightMiddleDistal },
		{ HumanoidBones::LeftRingProximal, HumanoidBones::RightRingProximal },
		{ HumanoidBones::LeftRingIntermediate, HumanoidBones::RightRingIntermediate },
		{ HumanoidBones::LeftRingDistal, HumanoidBones::RightRingDistal },
		{ HumanoidBones::LeftLittleProximal, HumanoidBones::RightLittleProximal },
		{ HumanoidBones::LeftLittleIntermediate, HumanoidBones::RightLittleIntermediate },
		{ HumanoidBones::LeftLittleDistal, HumanoidBones::RightLittleDistal },
	};

	for (const auto& pair : sidePairs) {
		auto* left = findHumanoidBone(pair.left);
		auto* right = findHumanoidBone(pair.right);
		if (left && right) {
			const auto& leftPos = left->globalMatrix().position();
			const auto& rightPos = right->globalMatrix().position();
			if (leftPos.x <= rightPos.x) {
				// OK
			}
			else {
				LN_WARNING("VerifyHumanoidBones error.");
			}
		}
	}
}

//==============================================================================
// MeshDiag

void MeshDiag::printNodes(const SkinnedMeshModel* model)
{
	bool hasRotationOrScale = false;

	for (const auto& node : model->m_nodes) {
		std::cout << node->name() << std::endl;
		const auto& t = node->initialLocalTransform();
		printf("  %f, %f, %f, %f\n", t.m[0][0], t.m[0][1], t.m[0][2], t.m[0][3]);
		printf("  %f, %f, %f, %f\n", t.m[1][0], t.m[1][1], t.m[1][2], t.m[1][3]);
		printf("  %f, %f, %f, %f\n", t.m[2][0], t.m[2][1], t.m[2][2], t.m[2][3]);
		printf("  %f, %f, %f, %f\n", t.m[3][0], t.m[3][1], t.m[3][2], t.m[3][3]);

		

		if (!Math::nearEqual(t.m[0][0], 1.0f) || !Math::nearEqual(t.m[0][1], 0.0f) || !Math::nearEqual(t.m[0][2], 0.0f) ||
			!Math::nearEqual(t.m[1][0], 0.0f) || !Math::nearEqual(t.m[1][1], 1.0f) || !Math::nearEqual(t.m[1][2], 0.0f) ||
			!Math::nearEqual(t.m[2][0], 0.0f) || !Math::nearEqual(t.m[2][1], 0.0f) || !Math::nearEqual(t.m[2][2], 1.0f)) {
			hasRotationOrScale = true;

			//auto a = Vector3::transformCoord(Vector3(0, 0, 0), t);
			//printf("");
		}
	}
	std::cout << "hasRotationOrScale: " << hasRotationOrScale << std::endl;
}

void MeshDiag::clearBoneInitialRotations(SkinnedMeshModel* model)
{
	model->updateNodeTransforms();

	if (1) {

		for (const auto& node : model->m_nodes) {
			//if (node->m_boneNode) {
				// 回転や拡縮も適用してグローバルな pos を求め、それを GlobalTransform に再設定することで位置情報のみにする
				auto pos = Vector3::transformCoord(Vector3(0, 0, 0), node->globalMatrix());

				if (0) {
					pos.x *= -1.0f;
				}

				node->setGlobalTransform(Matrix::makeTranslation(pos));
			//}
		}

		for (const auto& node : model->m_nodes) {
			//if (node->m_boneNode) {
				if (node->parentNodeIndex() >= 0) {
					// 親からの相対位置を InitialLocalTransform とする。
					// 普通は逆行列を求めるが、回転は行わなくなるので、移動ベクトルを反転するだけでよい。
					const auto& self = node->globalMatrix();
					const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();
					const auto relPos = self.position() - parent.position();
					node->setInitialLocalTransform(Matrix::makeTranslation(relPos));
				}
				else {
					// 親が無ければ GlobalTransform をそのまま InitialLocalTransform とする。
					node->setInitialLocalTransform(node->globalMatrix());
				}
			//}
		}

		for (const auto& skeleton : model->skeletons()) {
			for (const auto& bone : skeleton->m_bones) {
				MeshNode* node = bone->node();
				auto t = node->globalMatrix();

				// 逆行列で inverseInitialMatrix とする。
				// 回転は行わなくなるので、移動ベクトルを反転するだけでよい。
				bone->m_inverseInitialMatrix = Matrix::makeTranslation(-t.position());
			}
		}
	}
	else {




		for (const auto& skeleton : model->skeletons()) {
			for (const auto& bone : skeleton->m_bones) {
				MeshNode* node = bone->node();
				auto t = node->globalMatrix();

				// 逆行列で InitialLocalTransform とする。
				// ただ回転は行わないので、移動ベクトルを反転するだけでよい。
				bone->m_inverseInitialMatrix = Matrix::makeTranslation(-t.position());

				//if (node->parentNodeIndex() >= 0) {
				//	// 親からの相対位置を InitialLocalTransform とする。
				//	const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();
				//	const auto relPos = t.position() - parent.position();
				//	node->setInitialLocalTransform(Matrix::makeTranslation(relPos));
				//}
				//else {
				//	node->setInitialLocalTransform(Matrix::makeTranslation(t.position()));
				//}
			}
		}


		for (const auto& node : model->m_nodes) {
			if (node->m_boneNode) {
				auto t = node->globalMatrix();

				// 作業用変数として、回転成分を消して再設定
				//t.setRow(0, Vector4(1, 0, 0, 1));
				//t.setRow(1, Vector4(0, 1, 0, 1));
				//t.setRow(2, Vector4(0, 0, 1, 1));
				//node->setGlobalTransform(t);

				// 逆行列で InitialLocalTransform とする。
				// ただ回転は行わないので、移動ベクトルを反転するだけでよい。
				node->setInitialLocalTransform(Matrix::makeTranslation(-t.position()));

				if (node->parentNodeIndex() >= 0) {
					// 親からの相対位置を InitialLocalTransform とする。
					const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();
					const auto relPos = t.position() - parent.position();
					node->setInitialLocalTransform(Matrix::makeTranslation(relPos));

				}
			}
		}
	}
}


} // namespace ln

