
#pragma once
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>

namespace ln {
namespace detail {

class MeshBoneMapper
{
public:
	MeshBoneMapper();
	void map(MeshArmature* skeleton);

private:
	enum class MajorKind
	{
		None,
		Body,
		Head,
		LeftArm,
		RightArm,
		LeftHand,
		RightHand,
		LeftLeg,
		RightLeg,
	};

	enum class PriorityKind
	{
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
		int boneIndex = -1;
	};

	struct BoneInfo
	{
		MeshBone* bone;
		int boneIndex;
		int depth = 0;
		MajorKind majorKind;
		HumanoidBones nameMached;
	};

	MeshNode* getMeshNodeByBone(int boneIndex) const { return m_skeleton->bone(boneIndex)->node(); }

	void calculateNodeDepthHieratical(BoneInfo* bone, int depth);
	void makeMajorKindByName(BoneInfo* info, const String& name);
	const List<StringRef>& splitWords(const String& name);
	void resolveBodyBones();
	void resolveLegs();

	SkinnedMeshModel* m_model;
	MeshArmature* m_skeleton;
	List<NodeInfo> m_nodes;
	List<BoneInfo> m_bones;
	List<StringRef> m_splitCache;
	std::array<List<BoneInfo*>, 9> m_majorGroups;
	int m_lowerBranchNodeDepth = -1;
	int m_upperBranchNodeDepth = -1;
};

} // namespace detail
} // namespace ln
