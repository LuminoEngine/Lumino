
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include "MeshManager.hpp"
#include "CCDIKSolver.hpp"
#include "MeshModelInstance.hpp"

namespace ln {
	

//==============================================================================
// MeshBone

MeshNode* MeshBone::node() const
{
	return m_skeleton->m_model->m_nodes[m_node];
}

//==============================================================================
// MeshSkeleton

MeshSkeleton::MeshSkeleton()
{
}

bool MeshSkeleton::init(MeshModel* model)
{
	if (!Object::init()) return false;

	m_model = model;
	return true;
}

MeshBone* MeshSkeleton::bone(int index) const
{
	return m_bones[index];
}

//const Matrix& MeshSkeleton::boneGlobalMatrix(int index) const
//{
//}

void MeshSkeleton::addBone(int linkNode, const Matrix& inverseInitialMatrix)
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
// MeshModel

Ref<MeshModel> MeshModel::load(const StringRef& filePath, MeshImportSettings* settings)
{
	return detail::EngineDomain::meshManager()->createSkinnedMeshModel(
		filePath, settings ? settings : MeshImportSettings::defaultSettings());
	//return detail::EngineDomain::meshManager()->acquireStaticMeshModel(filePath, 1.0f);
	//auto model = makeObject<MeshModel>();
 //   detail::EngineDomain::meshManager()->loadStaticMeshModel(model, filePath, scale);
	//return model;
}

//Ref<MeshModel> MeshModel::loadSkinned(const StringRef& filePath)
//{
//	return detail::EngineDomain::meshManager()->createSkinnedMeshModel(filePath, 1.0f);
//}

MeshModel::MeshModel()
    : m_type(detail::InternalMeshModelType::StaticMesh)
{
}

MeshModel::MeshModel(detail::InternalMeshModelType type)
    : m_type(type)
{
}

void MeshModel::clear()
{
	m_meshContainers = {};
	m_nodes = {};
	m_materials = {};
	m_rootNodes = {};
	m_nodeGlobalTransforms = {};
}

void MeshModel::serialize(Serializer2& ar)
{
	Object::serialize(ar);
	ar & makeNVP(u"filePath", m_filePath);

	if (ar.isLoading()) {
		clear();
		detail::EngineDomain::meshManager()->loadStaticMeshModel(this, m_filePath, m_scale);
	}
}

MeshNode* MeshModel::findNode(StringRef name) const
{
	int index = findNodeIndex(name);
	if (index < 0)
		return nullptr;
	else
		return m_nodes[index];
}

Material* MeshModel::findMaterial(StringRef name) const
{
	return m_materials.findIf([&](const auto& x) { return x->m_name == name; }).valueOr(nullptr);
}

int MeshModel::findNodeIndex(StringRef name) const
{
	return m_nodes.indexOfIf([&](const auto& x) { return x->name() == name; });
}

MeshContainer* MeshModel::addMeshContainer(MeshPrimitive* mesh)
{
	auto meshContainer = makeObject<MeshContainer>();
	meshContainer->addMeshPrimitive(mesh);
	meshContainer->m_index = m_meshContainers.size();
	m_meshContainers.add(meshContainer);
	return meshContainer;
}

MeshNode* MeshModel::addNode()
{
	auto node = makeObject<MeshNode>();
	node->m_model = this;
	node->m_index = m_nodes.size();
	m_nodes.add(node);
	return node;
}

MeshNode* MeshModel::addMeshContainerNode(MeshPrimitive* mesh)
{
	auto container = addMeshContainer(mesh);
	auto node = addNode();
	node->setMeshContainerIndex(container->m_index);
	return node;
}

void MeshModel::addMeshContainer(MeshContainer* meshContainer)
{
	if (LN_REQUIRE(meshContainer)) return;
	m_meshContainers.add(meshContainer);
}

void MeshModel::addNode(MeshNode* node)
{
    if (LN_REQUIRE(node)) return;
	node->m_model = this;
    node->m_index = m_nodes.size();
    m_nodes.add(node);
}

void MeshModel::addMaterial(Material* material)
{
	m_materials.add(material);
}

Material* MeshModel::material(int index)
{
	return m_materials[index];
}

AnimationController* MeshModel::animationController() const
{
	return m_animationController;
}

MeshNode* MeshModel::findHumanoidBone(HumanoidBones boneKind) const
{
	int index = humanoidBoneIndex(boneKind);
	if (index >= 0)
		return m_nodes[index];
	else
		return nullptr;
}

//AABB MeshModel::boundingBox() const
//{
//	AABB result;
//	for (const auto& meshContainer : m_meshContainers) {
//		const auto& meshPrimitive = meshContainer->meshPrimitives()[0];
//		result.attemptInfrate(meshPrimitive->boundingBox());
//	}
//	return result;
//}

void MeshModel::addRootNode(int index)
{
    m_rootNodes.add(index);
}

void MeshModel::resetNodeLocalTransforms()
{
	for (auto& node : meshNodes()) {
		node->resetLocalTransform();
	}
}

void MeshModel::updateNodeTransforms()
{
    m_nodeGlobalTransforms.resize(m_nodes.size());
    for (int index : m_rootNodes) {
        updateNodeTransformsHierarchical(index, Matrix::Identity, true);
    }
}

void MeshModel::addSkeleton(MeshSkeleton* skeleton)
{
	m_skeletons.add(skeleton);
}

void MeshModel::updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform, bool hierarchical)
{
    auto node = m_nodes[nodeIndex];

	//if (node->name() == u"右腕") {
	//	std::cout << node->name() << std::endl;
	//}
	//std::cout << nodeIndex << std::endl;


	if (1) {
		Matrix local = Matrix::makeScaling(node->m_localTransform.scale);
		local.rotateQuaternion(node->m_localTransform.rotation);
		local.translate(node->m_localTransform.translation);

		//local.transpose();

		//if (node->m_localTransform.rotation != Quaternion::Identity) {
		//	printf("");
		//}

		m_nodeGlobalTransforms[nodeIndex] = local * node->initialLocalTransform() * parentTransform;   // NOTE: glTF はこの順である必要がある。
	}
	else {
		/*
		Matrix local;
		local.translate(node->initialLocalTransform().position());
		local.scale(node->m_localTransform.scale);
		local.rotateQuaternion(node->m_localTransform.rotation);
		local.translate(node->m_localTransform.translation);
		*/
		// TODO: * ではなく一気に作ったほうがはやいかも
		Matrix local =
			//Matrix::makeTranslation(node->initialLocalTransform().position()) *
			Matrix::makeScaling(node->m_localTransform.scale) *
			Matrix::makeRotationQuaternion(node->m_localTransform.rotation) *
			Matrix::makeTranslation(node->m_localTransform.translation);// *
			//Matrix::makeTranslation(node->initialLocalTransform().position());
		local.translate(node->initialLocalTransform().position());
		m_nodeGlobalTransforms[nodeIndex] = local;
		m_nodeGlobalTransforms[nodeIndex] *= parentTransform;
	}

	// glview.cc と比べて Node の Transform の差分は無し。
	//float* m = m_nodeGlobalTransforms[nodeIndex].data();
	//for (int i = 0; i < 16; i++) std::cout << m[i] << ", ";

	if (hierarchical) {
		for (int child : node->m_children) {
			updateNodeTransformsHierarchical(child, m_nodeGlobalTransforms[nodeIndex], hierarchical);
		}
	}
}

void MeshModel::beginUpdate()
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
void MeshModel::preUpdate()
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
void MeshModel::postUpdate()
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

void MeshModel::updateBoneTransformHierarchy()
{
	updateNodeTransforms();

	//for (SkinnedMeshBone* bone : m_rootBoneList)
	//{
	//	bone->updateGlobalTransform(true);
	//}
}

void MeshModel::updateIK()
{
	detail::CCDIKSolver ik;
	ik.owner = this;
	ik.m_skeleton = skeleton(0);	// TODO:
	ik.UpdateTransform();
}

void MeshModel::writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer)
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

Ref<detail::MeshModelInstance> MeshModel::createMeshModelInstance()
{
	return makeRef<detail::MeshModelInstance>(this);
}

void MeshModel::calculateBoundingBox()
{
	m_boundingBox.invalidate();

	for (const auto& node : m_nodes) {
		if (node->hasMeshContainer()) {
			MeshPrimitive* meshPrimitive = m_meshContainers[node->meshContainerIndex()]->meshPrimitive();
			meshPrimitive->calculateBoundingBox();

			AABB aabb = meshPrimitive->boundingBox();
			aabb.transform(node->initialLocalTransform());
			m_boundingBox.attemptInfrate(aabb);
		}
	}
}

// 左手座標系状、Z+を正面としたときの、各ボーンの位置関係を検証する。
void MeshModel::verifyHumanoidBones()
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
// MeshHelper

Box MeshHelper::makeAABB(const Vertex* vertices, uint32_t vertexCount)
{
	if (vertexCount == 0) return Box();

	Vector3 aabbMin = Vector3::Maximum;
	Vector3 aabbMax = Vector3::Minimum;
	for (uint32_t i = 0; i < vertexCount; i++) {
		const auto& v = vertices[i];
		aabbMin.x = std::min(aabbMin.x, v.position.x);
		aabbMin.y = std::min(aabbMin.y, v.position.y);
		aabbMin.z = std::min(aabbMin.z, v.position.z);
		aabbMax.x = std::max(aabbMax.x, v.position.x);
		aabbMax.y = std::max(aabbMax.y, v.position.y);
		aabbMax.z = std::max(aabbMax.z, v.position.z);
	}

	return Box(aabbMin, aabbMax);
}

std::array<const Char*, 1> MeshHelper::CandidateExtensions_MeshModel = { u".gltf" };

//==============================================================================
// MeshImportSettings

MeshImportSettings* MeshImportSettings::defaultSettings()
{
	static MeshImportSettings settings;
	return &settings;
}

MeshImportSettings::MeshImportSettings()
	: m_skeletonImport(true)
	, m_applyBoneTransformationsEnabled(true)
{
}

bool MeshImportSettings::init()
{
	return Object::init();
}

void MeshImportSettings::setSkeletonImport(bool value)
{
	m_skeletonImport = value;
}

void MeshImportSettings::setApplyBoneTransformationsEnabled(Optional<bool> value)
{
	m_applyBoneTransformationsEnabled = value;
}


//==============================================================================
// MeshDiag

void MeshDiag::printNodes(const MeshModel* model)
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


// すべての node transform を適用して、Position のみの Node Transform にする。
// これはキャラクターの手に武器を持たせたりするときの位置合わせを補助するための機能。
// 本来はモデリングソフト側でデフォルト姿勢(Tポーズ) は Bone や Node の回転が無い状態にしてほしいのだが、
// ソースデータの無いモデルやエクスポータの都合で回転が入ってしまう場合のケアとして利用する。
// 基本的には使わないことが推奨される。(glTFSample の Buggy 等は Node の回転が消されると正しく描画できない)
void MeshDiag::clearBoneInitialRotations(MeshModel* model)
{
	const bool inverse = false;

	model->updateNodeTransforms();

	if (1) {

		for (const auto& node : model->m_nodes) {

			if (inverse) {
			}
			else {
				// 位置のみ

				//if (node->m_boneNode) {
					// 回転や拡縮も適用してグローバルな pos を求め、それを GlobalTransform に再設定することで位置情報のみにする
				auto pos = Vector3::transformCoord(Vector3(0, 0, 0), node->globalMatrix());

				if (0) {
					pos.x *= -1.0f;
				}

				// 一時的に GlobalTransform の領域を使う
				node->setGlobalTransform(Matrix::makeTranslation(pos));
				//}
			}
		}

		for (const auto& node : model->m_nodes) {
			//if (node->m_boneNode) {
				if (node->parentNodeIndex() >= 0) {
					// 親からの相対位置を InitialLocalTransform とする。
					// 普通は逆行列を求めるが、回転は行わなくなるので、移動ベクトルを反転するだけでよい。
					const auto& self = node->globalMatrix();
					const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();

					if (inverse) {
						// 逆行列使用
						const auto m = Matrix::makeInverse(parent);
						node->setInitialLocalTransform(Matrix::multiply(self, m));
					}
					else {
						// 位置だけ
						const auto relPos = self.position() - parent.position();
						node->setInitialLocalTransform(Matrix::makeTranslation(relPos));
					}
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

				if (inverse) {
					// 逆行列使用
					bone->m_inverseInitialMatrix = Matrix::makeInverse(t);
				}
				else {
					// 位置だけ
					// 回転は行わなくなるので、移動ベクトルを反転するだけでよい。
					bone->m_inverseInitialMatrix = Matrix::makeTranslation(-t.position());
				}
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

