
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include "MeshModelInstance.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// BoneInstance

BoneInstance::BoneInstance(SkeletonInstance* owner, int boneIndex)
	: m_owner(owner)
	, m_boneIndex(boneIndex)
{
}

MeshNode* BoneInstance::node() const
{
	const auto& skeleton = m_owner->skeletonModel();
	const auto& bone = skeleton->bone(m_boneIndex);
	return m_owner->owner()->model()->m_nodes[bone->nodeIndex()];
}

const Matrix& BoneInstance::combinedTransform() const
{
	const auto& skeleton = m_owner->skeletonModel();
	const auto& bone = skeleton->bone(m_boneIndex);
	const auto& meshModel = m_owner->owner()->model();
	return meshModel->nodeGlobalTransform(bone->m_node);
}

//==============================================================================
// SkeletonInstance

SkeletonInstance::SkeletonInstance(MeshModelInstance* owner, int skeletonIndex)
	: m_owner(owner)
	, m_skeletonIndex(skeletonIndex)
{
	const auto& skeleton = skeletonModel();
	for (int i = 0; i < skeleton->boneCount(); i++) {
		m_bones.add(makeRef<BoneInstance>(this, i));
	}
}

const Ref<MeshSkeleton>& SkeletonInstance::skeletonModel() const
{
	return m_owner->model()->skeletons()[m_skeletonIndex];
}

void SkeletonInstance::updateSkinningMatrices()
{
	const auto& meshModel = m_owner->model();
	const auto& boneModels = skeletonModel()->m_bones;

	if (!m_skinningMatricesTexture || m_skinningMatricesTexture->height() != boneModels.size()) {
		m_skinningMatricesTexture = makeObject<Texture2D>(4, boneModels.size(), TextureFormat::RGBA32F);
		m_skinningMatricesTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
	}

	Bitmap2D* bitmap = m_skinningMatricesTexture->map(MapMode::Write);
	Matrix* data = reinterpret_cast<Matrix*>(bitmap->data());

	//const auto& nodes = model->meshNodes();
	for (int i = 0; i < boneModels.size(); i++) {
		const auto& bone = boneModels[i];

		// GLTF
		//data[i] = model->nodeGlobalTransform(bone->m_node) * bone->m_inverseInitialMatrix;
		// PMX
		data[i] = bone->m_inverseInitialMatrix * meshModel->nodeGlobalTransform(bone->m_node);
		// TODO: ↑どっちが正しい？
		// Three.js のコードを見ると、GLTFLoader では読み取った inverseBindMatrices を Skeleton クラスのコンストラクタに
		// そのまま渡している。転置とかはしていない。
		// Skeleton コンストラクタは boneInverses が省略されると、bone が持っている World行列を単純に inverse して、
		// boneInverses を作っている。calculateInverses: function () {　あたり。
		// というか「逆行列をかけて元に戻す」の計算順序的には GLTF の方が正しい。
	}

	m_skinningMatricesTexture->unmap();
}

//==============================================================================
// MorphInstance

MorphInstance::MorphInstance(MeshModelInstance* owner, int meshContainerIndex)
	: m_owner(owner)
	, m_meshContainerIndex(meshContainerIndex)
	, m_weights()
{
	const auto& meshContainer = owner->model()->meshContainers()[m_meshContainerIndex];
	const auto& meshPrimitive = meshContainer->meshPrimitives()[0];
	m_weights.resize(meshPrimitive->morphTargetCount(), 0.0f);

	VertexBuffer* baseBuffer = meshPrimitive->vertexBuffer(InterleavedVertexGroup::Main);
	m_blendResult = makeObject<VertexBuffer>(
		baseBuffer->size(),
		baseBuffer->data(),
		GraphicsResourceUsage::Static);

	// TODO: tmp
	updatePriorityTargets();
}

void MorphInstance::setWeight(int index, float value)
{
	m_weights[index] = value;
}

void MorphInstance::updatePriorityTargets()
{
	int i = 0;
	for (; i < std::min(m_weights.size(), m_priorityTargets.size()); i++) {
		m_priorityTargets[i] = i;
	}
	for (; i < m_priorityTargets.size(); i++) {
		m_priorityTargets[i] = -1;
	}
}

bool MorphInstance::getMorphWeights(std::array<float, MaxRenderMorphTargets>* outMorphWeights) const
{
	bool hasAnyWeight = false;

	for (size_t i = 0; i < outMorphWeights->size(); i++) {
		float weight = 0.0f;
		if (m_priorityTargets[i] >= 0) {
			weight = m_weights[m_priorityTargets[i]];
		}

		if (weight > 0.0f) {
			hasAnyWeight = true;
		}

		(*outMorphWeights)[i] = weight;

		
		if (m_priorityTargets[i] >= 0) {
			
		}
		else {
			(*outMorphWeights)[i] = 0.0f;
		}
	}

	return hasAnyWeight;
}

//==============================================================================
// MeshModelInstance

MeshModelInstance::MeshModelInstance(MeshModel* model)
	: m_model(model)
	, m_skeletons()
{
	const auto& skeletons = m_model->skeletons();
	for (int i = 0; i < skeletons.size(); i++) {
		m_skeletons.add(makeRef<SkeletonInstance>(this, i));
	}

	const auto& meshContainers = m_model->meshContainers();
	for (int i = 0; i < meshContainers.size(); i++) {
		m_morphs.add(makeRef<MorphInstance>(this, i));
	}
}

void MeshModelInstance::updateSkinningMatrices()
{
	for (auto& skeleton : m_skeletons) {
		skeleton->updateSkinningMatrices();
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

} // namespace detail
} // namespace ln

