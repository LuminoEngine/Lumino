
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
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
// MeshArmature

MeshArmature::MeshArmature()
{
}

bool MeshArmature::init()
{
	if (!Object::init()) return false;
	return true;
}

MeshBone* MeshArmature::bone(int index) const
{
	return m_bones[index];
}

void MeshArmature::addBone(int linkNode, const Matrix& inverseInitialMatrix)
{
	auto bone = makeObject<MeshBone>();
	bone->m_node = linkNode;
	bone->m_inverseInitialMatrix = inverseInitialMatrix;
	m_bones.add(bone);
}

void MeshArmature::updateSkinningMatrices(SkinnedMeshModel* model)
{
	if (!m_skinningMatricesTexture || m_skinningMatricesTexture->height() != m_bones.size()) {
		m_skinningMatricesTexture = makeObject<Texture2D>(4, m_bones.size(), TextureFormat::RGBA32F);
		m_skinningMatricesTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
	}

	Bitmap2D* bitmap = m_skinningMatricesTexture->map(MapMode::Write);
	Matrix* data = reinterpret_cast<Matrix*>(bitmap->data());

	const auto& nodes = model->meshNodes();
	for (int i = 0; i < m_bones.size(); i++) {
		const auto& bone = m_bones[i];
		

		data[i] = model->nodeGlobalTransform(bone->m_node) * bone->m_inverseInitialMatrix;

		//// TODO: test
		//if (i == 0) {
		//	data[i] = Matrix::makeRotationZ(0.3);
		//}
	}

	m_skinningMatricesTexture->unmap();
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
    //for (SkinnedMeshBone* bone : m_allBoneList)
    //{
    //    bone->resetLocalTransform();
    //}

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

	// スキニング行列の作成
	updateSkinningMatrices();
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
    //detail::CCDIKSolver ik;
    //ik.owner = this;
    //ik.UpdateTransform();
}

void SkinnedMeshModel::updateSkinningMatrices()
{
	for (auto& skeleton : m_skeletons) {
		skeleton->updateSkinningMatrices(this);
	}
#if 0
	// スキニング行列の作成
	for (int i = 0; i < m_allBoneList.size(); i++)
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
#endif
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


//==============================================================================
// AnimationController

AnimationController::AnimationController()
{
}

bool AnimationController::init(SkinnedMeshModel* model)
{
	if (!Object::init()) return false;
	m_model = model;

	m_core = makeObject<AnimationMixerCore>(this);
	m_core->addLayer(makeObject<AnimationLayer>(m_core));
	return true;
}

void AnimationController::advanceTime(float elapsedTime)
{
	m_core->advanceTime(elapsedTime);
}

detail::AnimationTargetElementBlendLink* AnimationController::onRequireBinidng(const String& name)
{
	auto tb = m_bindings.findIf([&](const auto& x) { return x->name == name; });
	if (tb) {
		return *tb;
	}


	int index = m_model->findNodeIndex(name);
	if (index >= 0) {
		auto binding = makeRef<detail::AnimationTargetElementBlendLink>(AnimationValueType::Transform);
		binding->name = name;
		binding->targetIndex = index;
		m_bindings.add(binding);
		return binding;
	}
	else {
		return nullptr;
	}
}

void AnimationController::onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding)
{
	m_model->meshNodes()[binding->targetIndex]->setTransform(binding->rootValue.getTransform());
}

} // namespace ln

