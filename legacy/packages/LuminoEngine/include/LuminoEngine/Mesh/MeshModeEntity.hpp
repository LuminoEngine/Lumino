#pragma once
#include "SkinnedMeshModel.hpp"

namespace ln {
class MeshModel;
namespace detail {
class MeshModelInstance;
class SkeletonInstance;

class BoneInstance
    : public RefObject {
public:
    BoneInstance(SkeletonInstance* owner, int boneIndex);

    void setPosition(const Vector3& value);
    void setPosition(float x, float y, float z) { setPosition(Vector3(x, y, z)); }

    void setRotation(const Quaternion& value);
    void setRotation(float x, float y, float z) { setRotation(Quaternion::makeFromEulerAngles(Vector3(x, y, z))); }

    void setScale(const Vector3& value);
    void setScale(float x, float y, float z) { setScale(Vector3(x, y, z)); }
    void setScale(float xyz) { setScale(Vector3(xyz, xyz, xyz)); }

    void setTransform(const AttitudeTransform& value);
    const AttitudeTransform& localTransform() const;


    SkeletonInstance* owner() const { return m_owner; }
    MeshNode* node() const;
    const Matrix& combinedTransform() const;

    //void resetLocalTransform();

private:
    AttitudeTransform& writableLocalTransform();

    SkeletonInstance* m_owner;
    int m_boneIndex;
};

class SkeletonInstance
    : public RefObject {
public:
    SkeletonInstance(MeshModelInstance* owner, int skeletonIndex);

    MeshModelInstance* owner() const { return m_owner; }
    const Ref<MeshSkeleton>& skeletonModel() const;
    const List<Ref<BoneInstance>>& bones() const { return m_bones; }
    const Ref<Texture2D>& skinningMatricesTexture() const { return m_skinningMatricesTexture; }

    //void resetBoneLocalTransforms();
    void updateSkinningMatrices(GraphicsCommandList* dataTransferCommandList);

private:
    MeshModelInstance* m_owner;
    int m_skeletonIndex;
    List<Ref<BoneInstance>> m_bones;
    Ref<Texture2D> m_skinningMatricesTexture;
    bool m_updateSkinningMatricesTextureNeeded;

    friend class MeshModelInstance;
};

class MorphInstance
    : public RefObject {
public:
    MorphInstance(MeshModelInstance* owner, int meshContainerIndex);
    int meshContainerIndex() const { return m_meshContainerIndex; }
    void setWeight(int index, float value);
    float weight(int index) const { return m_weights[index]; }
    const std::array<int, MaxRenderMorphTargets> priorityTargets() const { return m_priorityTargets; }

    void updatePriorityTargets();
    bool getMorphWeights(std::array<float, MaxRenderMorphTargets>* outMorphWeights) const;

    Ref<VertexBuffer> m_blendResult;

private:
    MeshModelInstance* m_owner;
    int m_meshContainerIndex;
    std::vector<float> m_weights;
    std::array<int, MaxRenderMorphTargets> m_priorityTargets;
};

// MeshModel の描画にかかわる動的なデータ
class MeshModelInstance
    : public RefObject {
public:
    MeshModelInstance(MeshModel* model);

    const Ref<MeshModel>& model() const { return m_model; }

    bool hasSkeleton() const { return !m_skeletons.isEmpty(); }
    bool hasMorph() const { return !m_morphs.isEmpty(); }

    const List<Ref<SkeletonInstance>>& skeletons() const { return m_skeletons; }
    const List<Ref<MorphInstance>>& morphs() const { return m_morphs; }
    //const Array<Ref<Material>>& materials() const { return m_materials; }

    void setMaterial(int index, Material* material);

    // Skeleton のリセット及び、アニメーションの適用を行う。
    // 通常、フレームの最初に呼び出す。
    // このあと、各ボーンを手動で変更できる。
    void updateFrame(float elapsedTime);

    // 各ボーンの結合済み行列を再帰的に更新する。
    void updateBoneTransformHierarchy();

    //void updateSkinningMatrices();

    Material* getActualMaterial(int index) const;

private:
    void updateNodeTransformsHierarchical(int nodeIndex, const Matrix& parentTransform, bool hierarchical);
    void updateBoneTransformHierarchyIfNeeded();

    Ref<MeshModel> m_model;
    List<Ref<SkeletonInstance>> m_skeletons;
    List<Ref<MorphInstance>> m_morphs;
    Array<Ref<Material>> m_materials;
    Array<AttitudeTransform> m_nodeLocalTransforms;
    Array<Matrix> m_nodeCombinedMatrices;
    //int32_t m_frameRevision;
    bool m_updateCombinedMatrixNeeded;

    friend class BoneInstance;
    friend class SkeletonInstance;
};

} // namespace detail
} // namespace ln
