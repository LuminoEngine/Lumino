
#pragma once
#include <LuminoGraphics/Mesh/SkinnedMeshModel.hpp>

namespace ln {
namespace detail {

class MeshBoneMapper
{
public:
	MeshBoneMapper();
	void map(SkinnedMeshModel* model);

private:
	enum class MajorKind
	{
		None,
		Body,
		Head,
		Jaw,
		LeftEye,
		RightEye,
		LeftArm,
		RightArm,
		LeftHand,
		RightHand,
		LeftLeg,
		RightLeg,

		LeftThumb,
		LeftIndex,
		LeftMiddle,
		LeftRing,
		LeftLittle,

		RightThumb,
		RightIndex,
		RightMiddle,
		RightRing,
		RightLittle,
	};

	enum class PriorityKind
	{
		None,

		Hips,
		Spine,
		Chest,
		UpperChest,

		Neck,
		Head,
		Eye,
		Jaw,

		Shoulder,
		UpperArm,
		LowerArm,
		Hand,
	};

	struct NodeInfo
	{
		MeshNode* node;
		int nodeIndex = -1;
		int depth = 0;
		MajorKind majorKind;
		int majorKindPriority = -1;
		HumanoidBones nameMached;
		bool isRight = false;
		int wordCount = 0;
	};

	struct HumanoidBoneInfo
	{
		int nodeIndex = -1;
		int accuracy = 0;
	};

	//NodeInfo* getNodeInfo(int boneIndex) { return &m_nodes[m_skeleton->bone(boneIndex)->nodeIndex()]; }
	//MeshNode* getMeshNodeByBone(int boneIndex) const { return m_skeleton->bone(boneIndex)->node(); }
	//MeshNode* getMeshNodeByBone(const BoneInfo* bone) const { return getMeshNodeByBone(bone->boneIndex); }
	//int getDepth(const BoneInfo* info) const { return m_nodes[m_skeleton->bone(info->boneIndex)->nodeIndex()].depth; }

	void setHumanoidBoneIndex(HumanoidBones kind, int accuracy, int nodeIndex) { m_humanoidBoneNodeIndices[static_cast<int>(kind)].nodeIndex = nodeIndex; m_humanoidBoneNodeIndices[static_cast<int>(kind)].accuracy = accuracy; }
	int humanoidBoneAccuracy(HumanoidBones kind) const { return m_humanoidBoneNodeIndices[static_cast<int>(kind)].accuracy; }
	int humanoidBoneIndex(HumanoidBones kind) const { return m_humanoidBoneNodeIndices[static_cast<int>(kind)].nodeIndex; }
	void calculateNodeDepthHieratical(NodeInfo* node, int depth);
	void makeMajorKindByName(NodeInfo* info, const String& name);
	const List<StringView>& splitWords(const String& name);
	void resolveBodyBones();
	void resolveArmBones(bool isRight);
	void resolveLegBones(bool isRight);
	void resolveHeadBones();
	void resolveEyeBones();
	void resolveJawBones();
	void resolveFingerBones();

	SkinnedMeshModel* m_model;
	//MeshSkeleton* m_skeleton;
	List<NodeInfo> m_nodes;
	//List<BoneInfo> m_bones;
	List<StringView> m_splitCache;
	std::array<List<NodeInfo*>, 22> m_majorGroups;
	int m_lowerBranchNodeDepth = -1;
	int m_upperBranchNodeDepth = -1;
	std::array<HumanoidBoneInfo, 56> m_humanoidBoneNodeIndices;
};

} // namespace detail
} // namespace ln
