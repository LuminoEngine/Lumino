
#pragma once
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>

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
		LeftEye,
		RightEye,
		LeftArm,
		RightArm,
		LeftHand,
		RightHand,
		LeftLeg,
		RightLeg,
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

	//NodeInfo* getNodeInfo(int boneIndex) { return &m_nodes[m_skeleton->bone(boneIndex)->nodeIndex()]; }
	//MeshNode* getMeshNodeByBone(int boneIndex) const { return m_skeleton->bone(boneIndex)->node(); }
	//MeshNode* getMeshNodeByBone(const BoneInfo* bone) const { return getMeshNodeByBone(bone->boneIndex); }
	//int getDepth(const BoneInfo* info) const { return m_nodes[m_skeleton->bone(info->boneIndex)->nodeIndex()].depth; }

	void calculateNodeDepthHieratical(NodeInfo* node, int depth);
	void makeMajorKindByName(NodeInfo* info, const String& name);
	const List<StringRef>& splitWords(const String& name);
	void resolveBodyBones();
	void resolveArmBones(bool isRight);
	void resolveLegBones(bool isRight);
	void resolveHeadBones();
	void resolveEyeBones();

	SkinnedMeshModel* m_model;
	//MeshArmature* m_skeleton;
	List<NodeInfo> m_nodes;
	//List<BoneInfo> m_bones;
	List<StringRef> m_splitCache;
	std::array<List<NodeInfo*>, 11> m_majorGroups;
	int m_lowerBranchNodeDepth = -1;
	int m_upperBranchNodeDepth = -1;
};

} // namespace detail
} // namespace ln
