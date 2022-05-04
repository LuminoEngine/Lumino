
#include "Internal.hpp"
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/MeshModeEntity.hpp>
#include <LuminoGraphics/Mesh/AnimationController.hpp>

namespace ln {
namespace detail {

//==============================================================================
// BoneInstance

BoneInstance::BoneInstance(SkeletonInstance* owner, int boneIndex)
    : m_owner(owner)
    , m_boneIndex(boneIndex) {
}

void BoneInstance::setPosition(const Vector3& value) {
    writableLocalTransform().translation = value;
}

void BoneInstance::setRotation(const Quaternion& value) {
    writableLocalTransform().rotation = value;
}

void BoneInstance::setScale(const Vector3& value) {
    writableLocalTransform().scale = value;
}

void BoneInstance::setTransform(const AttitudeTransform& value) {
    writableLocalTransform() = value;
}

AttitudeTransform& BoneInstance::writableLocalTransform() {
    const auto& skeleton = m_owner->skeletonModel();
    const auto& bone = skeleton->bone(m_boneIndex);
    return m_owner->owner()->m_nodeLocalTransforms[bone->nodeIndex()];
}

MeshNode* BoneInstance::node() const {
    const auto& skeleton = m_owner->skeletonModel();
    const auto& bone = skeleton->bone(m_boneIndex);
    return m_owner->owner()->model()->m_nodes[bone->nodeIndex()];
}

const Matrix& BoneInstance::combinedTransform() const {
    const auto& skeleton = m_owner->skeletonModel();
    const auto& bone = skeleton->bone(m_boneIndex);
    const auto& meshModel = m_owner->owner()->model();
    return meshModel->nodeGlobalTransform(bone->m_node);
}

//void BoneInstance::resetLocalTransform() {
//    m_localTransform = AttitudeTransform::Identity;
//}

//==============================================================================
// SkeletonInstance

SkeletonInstance::SkeletonInstance(MeshModelInstance* owner, int skeletonIndex)
    : m_owner(owner)
    , m_skeletonIndex(skeletonIndex)
    , m_updateSkinningMatricesTextureNeeded(true) {
    const auto& skeleton = skeletonModel();
    for (int i = 0; i < skeleton->boneCount(); i++) {
        m_bones.add(makeRef<BoneInstance>(this, i));
    }
}

const Ref<MeshSkeleton>& SkeletonInstance::skeletonModel() const {
    return m_owner->model()->skeletons()[m_skeletonIndex];
}

//void SkeletonInstance::resetBoneLocalTransforms() {
//    for (auto& bone : m_bones) {
//        bone->resetLocalTransform();
//    }
//}

void SkeletonInstance::updateSkinningMatrices(GraphicsCommandList* dataTransferCommandList) {
    // CombinedMatrix を最新にしておく。
    m_owner->updateBoneTransformHierarchyIfNeeded();

    const auto& meshModel = m_owner->model();
    const auto& boneModels = skeletonModel()->m_bones;

    if (!m_skinningMatricesTexture || m_skinningMatricesTexture->height() != boneModels.size()) {
        m_skinningMatricesTexture = makeObject<Texture2D>(4, boneModels.size(), TextureFormat::RGBA32F);
        m_skinningMatricesTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
    }

    Bitmap2D* bitmap = m_skinningMatricesTexture->map(MapMode::Write);
    Matrix* data = reinterpret_cast<Matrix*>(bitmap->data());

    // const auto& nodes = model->meshNodes();
    for (int i = 0; i < boneModels.size(); i++) {
        const auto& bone = boneModels[i];

        // GLTF
        // data[i] = model->nodeGlobalTransform(bone->m_node) * bone->m_inverseInitialMatrix;
        // PMX
        data[i] = bone->m_inverseInitialMatrix * m_owner->m_nodeCombinedMatrices[bone->m_node];
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
    , m_weights() {
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

void MorphInstance::setWeight(int index, float value) {
    m_weights[index] = value;
}

void MorphInstance::updatePriorityTargets() {
    int i = 0;
    for (; i < std::min(m_weights.size(), m_priorityTargets.size()); i++) {
        m_priorityTargets[i] = i;
    }
    for (; i < m_priorityTargets.size(); i++) {
        m_priorityTargets[i] = -1;
    }
}

bool MorphInstance::getMorphWeights(std::array<float, MaxRenderMorphTargets>* outMorphWeights) const {
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
    //, m_frameRevision(0)
    , m_updateCombinedMatrixNeeded(0) {
    const auto& skeletons = m_model->skeletons();
    for (int i = 0; i < skeletons.size(); i++) {
        m_skeletons.add(makeRef<SkeletonInstance>(this, i));
    }

#if 0 // TODO: meshContainers を直接 Morph にすると、完全に Static な Mesh でも Morph 有効になってしまうので考え直す
	const auto& meshContainers = m_model->meshContainers();
	for (int i = 0; i < meshContainers.size(); i++) {
		m_morphs.add(makeRef<MorphInstance>(this, i));
	}
#endif
    m_materials.resize(m_model->materialCount());
    m_nodeLocalTransforms.resize(m_model->m_nodes.size());
}

void MeshModelInstance::setMaterial(int index, Material* material) {
    m_materials[index] = material;
}

void MeshModelInstance::updateFrame(float elapsedTime) {
    // 全てのローカルトランスフォームをリセットする
    //		リセットしておかないと、IKで問題が出る。
    //		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
    for (int i = 0; i < m_nodeLocalTransforms.length(); i++) {
        m_nodeLocalTransforms[i] = AttitudeTransform::Identity;
    }
    //for (auto& skeleton : m_skeletons) {
    //    skeleton->resetBoneLocalTransforms();
    //}

    if (auto* controller = m_model->animationController()) {
        controller->advanceTime(0.016);
    }

    // Mark dirty
    m_updateCombinedMatrixNeeded = true;
    for (auto& skeleton : m_skeletons) {
        skeleton->m_updateSkinningMatricesTextureNeeded = true;
    }
}

void MeshModelInstance::updateBoneTransformHierarchy() {
    m_nodeCombinedMatrices.resize(m_model->m_nodes.size());
    for (int index : m_model->m_rootNodes) {
        updateNodeTransformsHierarchical(index, Matrix::Identity, true);
    }

    m_updateCombinedMatrixNeeded = false;
}

Material* MeshModelInstance::getActualMaterial(int index) const {
    if (m_materials[index]) {
        return m_materials[index];
    }
    return m_model->m_materials[index];
}

void MeshModelInstance::updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform, bool hierarchical) {
    auto node = m_model->m_nodes[nodeIndex];

    // if (node->name() == _TT("右腕") {
    //	std::cout << node->name() << std::endl;
    // }
    // std::cout << nodeIndex << std::endl;

    const auto& localTransform = m_nodeLocalTransforms[nodeIndex]; // node->localTransform();

    if (1) {
        Matrix local = Matrix::makeScaling(localTransform.scale);
        local.rotateQuaternion(localTransform.rotation);
        local.translate(localTransform.translation);

        // local.transpose();

        // if (localTransform.rotation != Quaternion::Identity) {
        //	printf("");
        // }

        m_nodeCombinedMatrices[nodeIndex] = local * node->initialLocalTransform() * parentTransform; // NOTE: glTF はこの順である必要がある。
    }
    else {
        /*
        Matrix local;
        local.translate(node->initialLocalTransform().position());
        local.scale(localTransform.scale);
        local.rotateQuaternion(localTransform.rotation);
        local.translate(localTransform.translation);
        */
        // TODO: * ではなく一気に作ったほうがはやいかも
        Matrix local =
            // Matrix::makeTranslation(node->initialLocalTransform().position()) *
            Matrix::makeScaling(localTransform.scale) *
            Matrix::makeRotationQuaternion(localTransform.rotation) *
            Matrix::makeTranslation(localTransform.translation); // *
                                                                         // Matrix::makeTranslation(node->initialLocalTransform().position());
        local.translate(node->initialLocalTransform().position());
        m_nodeCombinedMatrices[nodeIndex] = local;
        m_nodeCombinedMatrices[nodeIndex] *= parentTransform;
    }

    // glview.cc と比べて Node の Transform の差分は無し。
    // float* m = m_nodeGlobalTransforms[nodeIndex].data();
    // for (int i = 0; i < 16; i++) std::cout << m[i] << ", ";

    if (hierarchical) {
        for (int child : node->childrenIndices()) {
            updateNodeTransformsHierarchical(child, m_nodeCombinedMatrices[nodeIndex], hierarchical);
        }
    }
}

void MeshModelInstance::updateBoneTransformHierarchyIfNeeded() {
    if (m_updateCombinedMatrixNeeded) {
        updateBoneTransformHierarchy();
    }
}

#if 0
void MeshModelInstance::updateSkinningMatrices(GraphicsCommandList* dataTransferCommandList) {

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
#endif

} // namespace detail
} // namespace ln
