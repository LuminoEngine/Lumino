
#include "Internal.hpp"
#include "MeshBoneMapper.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// MeshBoneMapper

struct HumanoidBoneNamePair
{
	HumanoidBones key;
	String name;
};

static const HumanoidBoneNamePair s_humanoidBoneNames[] = {
	{ HumanoidBones::None, _TT("None") },

	{ HumanoidBones::Hips, _TT("Hips") },
	{ HumanoidBones::Spine, _TT("Spine") },
	{ HumanoidBones::Chest, _TT("Chest") },
	{ HumanoidBones::UpperChest, _TT("UpperChest") },

	{ HumanoidBones::LeftShoulder, _TT("LeftShoulder") },
	{ HumanoidBones::LeftUpperArm, _TT("LeftUpperArm") },
	{ HumanoidBones::LeftLowerArm, _TT("LeftLowerArm") },
	{ HumanoidBones::LeftHand, _TT("LeftHand") },

	{ HumanoidBones::RightShoulder, _TT("RightShoulder") },
	{ HumanoidBones::RightUpperArm, _TT("RightUpperArm") },
	{ HumanoidBones::RightLowerArm, _TT("RightLowerArm") },
	{ HumanoidBones::RightHand, _TT("RightHand") },

	{ HumanoidBones::LeftUpperLeg, _TT("LeftUpperLeg") },
	{ HumanoidBones::LeftLowerLeg, _TT("LeftLowerLeg") },
	{ HumanoidBones::LeftFoot, _TT("LeftFoot") },
	{ HumanoidBones::LeftToes, _TT("LeftToes") },

	{ HumanoidBones::RightUpperLeg, _TT("RightUpperLeg") },
	{ HumanoidBones::RightLowerLeg, _TT("RightLowerLeg") },
	{ HumanoidBones::RightFoot, _TT("RightFoot") },
	{ HumanoidBones::RightToes, _TT("RightToes") },

	{ HumanoidBones::Neck, _TT("Neck") },
	{ HumanoidBones::Head, _TT("Head") },
	{ HumanoidBones::LeftEye, _TT("LeftEye") },
	{ HumanoidBones::RightEye, _TT("RightEye") },
	{ HumanoidBones::Jaw, _TT("Jaw") },

	{ HumanoidBones::LeftThumbProximal, _TT("LeftThumbProximal") },
	{ HumanoidBones::LeftThumbIntermediate, _TT("LeftThumbIntermediate") },
	{ HumanoidBones::LeftThumbDistal, _TT("LeftThumbDistal") },
	{ HumanoidBones::LeftIndexProximal, _TT("LeftIndexProximal") },
	{ HumanoidBones::LeftIndexIntermediate, _TT("LeftIndexIntermediate") },
	{ HumanoidBones::LeftIndexDistal, _TT("LeftIndexDistal") },
	{ HumanoidBones::LeftMiddleProximal, _TT("LeftMiddleProximal") },
	{ HumanoidBones::LeftMiddleIntermediate, _TT("LeftMiddleIntermediate") },
	{ HumanoidBones::LeftMiddleDistal, _TT("LeftMiddleDistal") },
	{ HumanoidBones::LeftRingProximal, _TT("LeftRingProximal") },
	{ HumanoidBones::LeftRingIntermediate, _TT("LeftRingIntermediate") },
	{ HumanoidBones::LeftRingDistal, _TT("LeftRingDistal") },
	{ HumanoidBones::LeftLittleProximal, _TT("LeftLittleProximal") },
	{ HumanoidBones::LeftLittleIntermediate, _TT("LeftLittleIntermediate") },
	{ HumanoidBones::LeftLittleDistal, _TT("LeftLittleDistal") },

	{ HumanoidBones::RightThumbProximal, _TT("RightThumbProximal") },
	{ HumanoidBones::RightThumbIntermediate, _TT("RightThumbIntermediate") },
	{ HumanoidBones::RightThumbDistal, _TT("RightThumbDistal") },
	{ HumanoidBones::RightIndexProximal, _TT("RightIndexProximal") },
	{ HumanoidBones::RightIndexIntermediate, _TT("RightIndexIntermediate") },
	{ HumanoidBones::RightIndexDistal, _TT("RightIndexDistal") },
	{ HumanoidBones::RightMiddleProximal, _TT("RightMiddleProximal") },
	{ HumanoidBones::RightMiddleIntermediate, _TT("RightMiddleIntermediate") },
	{ HumanoidBones::RightMiddleDistal, _TT("RightMiddleDistal") },
	{ HumanoidBones::RightRingProximal, _TT("RightRingProximal") },
	{ HumanoidBones::RightRingIntermediate, _TT("RightRingIntermediate") },
	{ HumanoidBones::RightRingDistal, _TT("RightRingDistal") },
	{ HumanoidBones::RightLittleProximal, _TT("RightLittleProximal") },
	{ HumanoidBones::RightLittleIntermediate, _TT("RightLittleIntermediate") },
	{ HumanoidBones::RightLittleDistal, _TT("RightLittleDistal") },
};

MeshBoneMapper::MeshBoneMapper()
{
}

void MeshBoneMapper::map(SkinnedMeshModel* model)
{
	m_model = model;

	std::fill(m_humanoidBoneNodeIndices.begin(), m_humanoidBoneNodeIndices.end(), HumanoidBoneInfo{ -1, 0 });

	int nodeCount = m_model->m_nodes.size();
	m_nodes.resize(nodeCount);
	for (int i = 0; i < nodeCount; i++) {
		m_nodes[i].node = m_model->m_nodes[i];
		m_nodes[i].nodeIndex = i;
	}

	//int boneCount = skeleton->boneCount();
	//m_bones.resize(boneCount);
	//for (int i = 0; i < boneCount; i++) {
	//	m_bones[i].bone = skeleton->bone(i);
	//	m_bones[i].boneIndex = i;

	//	int nodeIndex = skeleton->bone(i)->nodeIndex();
	//	if (LN_REQUIRE(m_nodes[nodeIndex].boneIndex == -1)) return;
	//	m_nodes[nodeIndex].boneIndex = i;
	//}

	// 深さを測る
	for (int i : m_model->m_rootNodes) {
		calculateNodeDepthHieratical(&m_nodes[i], 0);
	}

	// BoneNode を大分類
	for (int i = 0; i < nodeCount; i++) {
		if (m_nodes[i].node->m_boneNode) {
			makeMajorKindByName(&m_nodes[i], m_nodes[i].node->name());
		}
	}

	resolveBodyBones();
	resolveArmBones(false);
	resolveArmBones(true);
	resolveLegBones(false);
	resolveLegBones(true);
	resolveHeadBones();
	resolveEyeBones();
	resolveJawBones();
	resolveFingerBones();

	for (int i = 0; i < m_humanoidBoneNodeIndices.size(); i++) {
		m_model->m_humanoidBoneNodeIndices[i] = m_humanoidBoneNodeIndices[i].nodeIndex;
	}

#if 1	// Debug
	for (int i = 0; i < m_humanoidBoneNodeIndices.size(); i++) {
		int nodeIndex = m_humanoidBoneNodeIndices[i].nodeIndex;
		if (nodeIndex >= 0) {
			const MeshNode* meshNode = m_model->m_nodes[nodeIndex];
			const auto& node = m_nodes[nodeIndex];
			std::cout << s_humanoidBoneNames[i].name << ": " << nodeIndex << " " << meshNode->name()
				<< " (depth:" << node.depth << ", x:" << meshNode->initialLocalTransform().position().x << ")" << std::endl;
		}
		else
			std::cout << s_humanoidBoneNames[i].name << ": -" << std::endl;
	}
#endif
}

void MeshBoneMapper::calculateNodeDepthHieratical(NodeInfo* node, int depth)
{
	node->depth = depth;

	for (int childIndex : node->node->children()) {
		calculateNodeDepthHieratical(&m_nodes[childIndex], depth + 1);
	}
}

void MeshBoneMapper::makeMajorKindByName(NodeInfo* info, const String& name)
{
	struct NamePair
	{
		int priority;
		String name;
		MajorKind kind;
		HumanoidBones boneKind;
	};

	static const NamePair nameMap[] = {
		{ 0, _TT("Hips"), MajorKind::Body, HumanoidBones::Hips },
		{ 0, _TT("Spine"), MajorKind::Body, HumanoidBones::Spine },	// Spine,Spine1,Spine3
		{ 0, _TT("Chest"), MajorKind::Body, HumanoidBones::Chest },	// Chest,UpperChest
		{ 0, _TT("Center"), MajorKind::Body, HumanoidBones::None },	// (PronamaChan)

		{ 0, _TT("Neck"), MajorKind::Head, HumanoidBones::Neck },
		{ 0, _TT("Head"), MajorKind::Head, HumanoidBones::Head },
		{ 0, _TT("Eye"), MajorKind::LeftEye, HumanoidBones::LeftEye },	// LeftEye,RightEye
		{ 0, _TT("Jaw"), MajorKind::Jaw, HumanoidBones::Jaw },
		{ 0, _TT("Mouth"), MajorKind::Jaw, HumanoidBones::Jaw },	// (AliciaSolid)

		{ 0, _TT("Shoulder"), MajorKind::LeftArm, HumanoidBones::LeftShoulder },	// 左右を伴うものは一度 Left に集める
		{ 0, _TT("Arm"), MajorKind::LeftArm, HumanoidBones::None },	// UpperArm,LowerArm,ForeArm
		{ 0, _TT("Hand"), MajorKind::LeftArm, HumanoidBones::LeftHand },
		{ 0, _TT("Elbow"), MajorKind::LeftArm, HumanoidBones::LeftLowerArm },	// (PronamaChan)
		{ 0, _TT("Wrist"), MajorKind::LeftArm, HumanoidBones::LeftHand },	// (PronamaChan)

		// "LeftHandThumb" というように、大体 "Hand" を含むため、Hand よりは優先度高くする
		{ 1, _TT("Thumb"), MajorKind::LeftThumb, HumanoidBones::None },	// Thumb1,ThumbProximal,ThumbIntermediate,ThumbDistal
		{ 1, _TT("Index"), MajorKind::LeftIndex, HumanoidBones::None },	// Index1,IndexProximal,IndexIntermediate,IndexDistal
		{ 1, _TT("Middle"), MajorKind::LeftMiddle, HumanoidBones::None },	// Middle1,MiddleProximal,MiddleIntermediate,MiddleDistal
		{ 1, _TT("Ring"), MajorKind::LeftRing, HumanoidBones::None },	// Ring1,RingProximal,RingIntermediate,RingDistal
		{ 1, _TT("Little"), MajorKind::LeftLittle, HumanoidBones::None },	// Little1,LittleProximal,LittleIntermediate,LittleDistal
		{ 1, _TT("Pinky"), MajorKind::LeftLittle, HumanoidBones::None },	// (AliciaSolid) 

		{ 0, _TT("Leg"), MajorKind::LeftLeg, HumanoidBones::None },	// UpperLeg,LowerLeg,Leg
		{ 0, _TT("Foot"), MajorKind::LeftLeg, HumanoidBones::None },
		{ 0, _TT("Knee"), MajorKind::LeftLeg, HumanoidBones::None },	// (PronamaChan)
		{ 0, _TT("Ankle"), MajorKind::LeftLeg, HumanoidBones::None },	// (PronamaChan)
		{ 0, _TT("Toe"), MajorKind::LeftLeg, HumanoidBones::None },



		/*
		
		PronamaChan
			center -> Hips
			foot_L -> LeftUpperLeg		// bone の「深さ」も観ないとダメかも
			knee_L -> LeftLowerLeg
			ankle_L -> LeftFoot
			upper_body-> Spine
			upper_body2 -> Chest
			shoulder_L -> LeftShoulder
			arm_L -> LeftUpperArm
			elbow_L -> LeftLowerArm
			wrist_L -> LeftHand
			toe2_L -> LeftToes	// toe2_L_end が実際の先端。最初に "toe" っぽいものを集めた後、一番深さの小さいのを選択する必要がある
			eye_L -> LeftEye
			thumb_L0 -> LeftThumbProximal
			thumb_L1 -> LeftThumbIntermediate
			thumb_L2 -> LeftThumbDistal
			indexF_L1 -> LeftIndexProximal
			*/

		/*
		AliciaSolid
		- Neck -> Neck
		- Neck1 -> None
		- Head -> Neck
		- eye_L -> LeftEye
		- eye_light_L -> None
		- eye_light_R -> None
		- eye_R -> RightEye
		- HeadEnd -> None
		- mouth -> Jaw
*/
	};

	info->majorKind = MajorKind::None;
	bool isRight = false;
	const auto& words = splitWords(name);
	for (const auto& word : words) {
		if (StringHelper::indexOf(word.data(), word.length(), _TT("right"), 5, 0, CaseSensitivity::CaseInsensitive) == 0) {
			isRight = true;
		}
		else if (word.length() == 1 && (word[0] == 'R' || word[0] == 'r')) {
			// "thumb_R1" のようなケース。"Ring" と衝突しないようにする
			isRight = true;
		}
		else {
			for (int i = 0; i < LN_ARRAY_SIZE_OF(nameMap); i++) {
				const auto e = nameMap[i];
				if (e.priority > info->majorKindPriority) {
					// 単語先頭の一致を評価する
					if (StringHelper::indexOf(word.data(), e.name.length(), e.name.c_str(), e.name.length(), 0, CaseSensitivity::CaseInsensitive) >= 0) {
						info->majorKind = e.kind;
						info->majorKindPriority = e.priority;
						info->nameMached = e.boneKind;
						break;
					}
				}
			}
		}
	}

	info->wordCount = words.size();
	info->isRight = isRight;

	if (isRight) {
		if (info->majorKind == MajorKind::LeftArm) info->majorKind = MajorKind::RightArm;
		if (info->majorKind == MajorKind::LeftHand) info->majorKind = MajorKind::RightHand;
		if (info->majorKind == MajorKind::LeftLeg) info->majorKind = MajorKind::RightLeg;
		if (info->majorKind == MajorKind::LeftEye) info->majorKind = MajorKind::RightEye;

		if (info->majorKind == MajorKind::LeftThumb) info->majorKind = MajorKind::RightThumb;
		if (info->majorKind == MajorKind::LeftIndex) info->majorKind = MajorKind::RightIndex;
		if (info->majorKind == MajorKind::LeftMiddle) info->majorKind = MajorKind::RightMiddle;
		if (info->majorKind == MajorKind::LeftRing) info->majorKind = MajorKind::RightRing;
		if (info->majorKind == MajorKind::LeftLittle) info->majorKind = MajorKind::RightLittle;
	}

	if (info->majorKind != MajorKind::None) {
		m_majorGroups[static_cast<int>(info->majorKind)].add(info);
	}
}

const List<StringView>& MeshBoneMapper::splitWords(const String& name)
{
	m_splitCache.clear();

	const Char* begin = name.c_str();
	const Char* pos = name.c_str();
	for (; *pos; pos++) {
		if (begin < pos) {
			if (isupper(*pos)) {
				m_splitCache.add(StringView(begin, pos - begin));
				begin = pos;
			}
			else if (*pos == '_') {
				m_splitCache.add(StringView(begin, pos - begin));
				begin = pos + 1;
				//pos++;
			}
		}
	}
	if (begin < pos) {
		m_splitCache.add(StringView(begin, pos - begin));
	}

	// thumb_L0 や Body2 など、単語末尾に続く数字を除く
	for (int i = 0; i < m_splitCache.size(); i++) {
		const auto str = m_splitCache[i];
		if (isdigit(str[str.length() - 1])) {
			int nums = 0;
			for (; nums < str.length(); nums++) {
				if (!isdigit(str[str.length() - nums - 1])) {
					break;
				}
			}
			m_splitCache[i] = StringView(str.data(), str.length() - nums);
		}
	}

	return m_splitCache;
}

void MeshBoneMapper::resolveBodyBones()
{
	auto& bones = m_majorGroups[static_cast<int>(MajorKind::Body)];
	if (bones.isEmpty()) return;

	// 深さの順に並び変える
	std::sort(bones.begin(), bones.end(), [this](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });

	if (bones.size() == 1) {
		// Require の数以下は強制設定
		setHumanoidBoneIndex(HumanoidBones::Hips, 0, bones[0]->nodeIndex);
	}
	else if (bones.size() == 2) {
		// Require の数以下は強制設定
		setHumanoidBoneIndex(HumanoidBones::Hips, 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::Spine, 0, bones[1]->nodeIndex);
	}
	else if (bones.size() == 3) {
		setHumanoidBoneIndex(HumanoidBones::Hips, 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::Spine, 0, bones[1]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::Chest, 0, bones[2]->nodeIndex);
	}
	else if (bones.size() == 4) {
		setHumanoidBoneIndex(HumanoidBones::Hips, 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::Spine, 0, bones[1]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::Chest, 0, bones[2]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::UpperChest, 0, bones[3]->nodeIndex);
	}
	else {
#if 0	// Debug
		for (auto& bone : bones) {
			std::cout << bone->bone->node()->name() << std::endl;
		}
#endif
		// まず Hips と UpperChest を、Depth から決める
		//setHumanoidBoneIndex(HumanoidBones::Hips, bones.front()->boneIndex);
		//setHumanoidBoneIndex(HumanoidBones::UpperChest, bones.back()->boneIndex);

		// Branch となっている Bone を上下から検索する
		int firstBranch = bones.indexOfIf([this](const NodeInfo* x) { return x->node->children().size() >= 2; });
		int lastBranch = bones.lastIndexOfIf([this](const NodeInfo* x) { return x->node->children().size() >= 2; });
		if (LN_REQUIRE(firstBranch >= 0 && lastBranch >= 0 && firstBranch < lastBranch)) return;

		setHumanoidBoneIndex(HumanoidBones::Hips, 0, bones[firstBranch]->nodeIndex);
		setHumanoidBoneIndex(HumanoidBones::UpperChest, 0, bones[lastBranch]->nodeIndex);

		const HumanoidBones spineOrChest[] = {
			HumanoidBones::Spine,
			HumanoidBones::Chest,
		};
		for (int i = 0; i < std::min(2, (lastBranch - firstBranch)); i++) {
			setHumanoidBoneIndex(spineOrChest[i], 0, bones[firstBranch + 1 + i]->nodeIndex);
		}

		/*
			AliciaSolid
			- Hips->Hips
			- Spine->Spine
			- Spine1->Chest
			- Spine2->なし
			- Spine3->UpperChest	// 多分Shoulderの付け根
		*/
	}

	const HumanoidBones bodyBones[] = {
		HumanoidBones::Hips,
		HumanoidBones::Spine,
		HumanoidBones::Chest,
		HumanoidBones::UpperChest,
	};
	// 下から見ていって、最初に見つかった分岐を持つ Bone を、m_lowerBranchNodeIndex にする (通常、腰など)
	for (int i = 0; i < LN_ARRAY_SIZE_OF(bodyBones); i++) {
		int nodeIndex = humanoidBoneIndex(bodyBones[i]);
		if (nodeIndex >= 0 && !m_nodes[nodeIndex].node->children().isEmpty()) {
			m_lowerBranchNodeDepth = m_nodes[nodeIndex].depth;
		}
	}
	// 上から見ていって、最初に見つかった分岐を持つ Bone を、m_lowerBranchNodeIndex にする (通常、胸など)
	for (int i = LN_ARRAY_SIZE_OF(bodyBones) - 1; i >= 0; i--) {
		int nodeIndex = humanoidBoneIndex(bodyBones[i]);
		if (nodeIndex >= 0 && !m_nodes[nodeIndex].node->children().isEmpty()) {
			m_upperBranchNodeDepth = m_nodes[nodeIndex].depth;
		}
	}
}

void MeshBoneMapper::resolveArmBones(bool isRight)
{
	auto& bones = m_majorGroups[static_cast<int>(isRight ? MajorKind::RightArm : MajorKind::LeftArm)];
	if (bones.size() == 0) {
		return;
	}

	// 深さの順に並び変える
	std::sort(bones.begin(), bones.end(), [this](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });

	const HumanoidBones kindMap[2][4] =
	{
		{ HumanoidBones::LeftShoulder, HumanoidBones::LeftUpperArm, HumanoidBones::LeftLowerArm, HumanoidBones::LeftHand, },
		{ HumanoidBones::RightShoulder, HumanoidBones::RightUpperArm, HumanoidBones::RightLowerArm, HumanoidBones::RightHand, },
	};
	const HumanoidBones* armBoneKinds = kindMap[isRight ? 1 : 0];

	if (bones.size() == 1) {
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[0]->nodeIndex);
	}
	else if (bones.size() == 2) {
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[2], 0, bones[1]->nodeIndex);
	}
	else if (bones.size() == 3) {
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[2], 0, bones[1]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[3], 0, bones[2]->nodeIndex);
	}
	else if (bones.size() == 4) {
		setHumanoidBoneIndex(armBoneKinds[0], 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[1]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[2], 0, bones[2]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[3], 0, bones[3]->nodeIndex);
	}
	else {
#if 1	// Debug
		for (auto& bone : bones) {
			std::cout << bone->node->name() << std::endl;
		}
#endif
		LN_NOTIMPLEMENTED();
	}
}

void MeshBoneMapper::resolveLegBones(bool isRight)
{
	auto& bones = m_majorGroups[static_cast<int>(isRight ? MajorKind::RightLeg : MajorKind::LeftLeg)];
	if (bones.size() == 0) {
		return;
	}

	// 深さの順に並び変える
	std::sort(bones.begin(), bones.end(), [this](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });

	const HumanoidBones kindMap[2][4] =
	{
		{ HumanoidBones::LeftUpperLeg, HumanoidBones::LeftLowerLeg, HumanoidBones::LeftFoot, HumanoidBones::LeftToes, },
		{ HumanoidBones::RightUpperLeg, HumanoidBones::RightLowerLeg, HumanoidBones::RightFoot, HumanoidBones::RightToes, },
	};
	const HumanoidBones* armBoneKinds = kindMap[isRight ? 1 : 0];

	if (bones.size() == 1) {
		setHumanoidBoneIndex(armBoneKinds[0], 0, bones[0]->nodeIndex);
	}
	else if (bones.size() == 2) {
		setHumanoidBoneIndex(armBoneKinds[0], 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[1]->nodeIndex);
	}
	else if (bones.size() == 3) {
		setHumanoidBoneIndex(armBoneKinds[0], 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[1]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[2], 0, bones[2]->nodeIndex);
	}
	else if (bones.size() == 4) {
		setHumanoidBoneIndex(armBoneKinds[0], 0, bones[0]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[1], 0, bones[1]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[2], 0, bones[2]->nodeIndex);
		setHumanoidBoneIndex(armBoneKinds[3], 0, bones[3]->nodeIndex);
	}
	else {
#if 0	// Debug
		for (auto& bone : bones) {
			std::cout << bone->node->name() << std::endl;
		}
#endif
		for (int i = 0; i < 4; i++) {
			setHumanoidBoneIndex(armBoneKinds[i], 0, bones[i]->nodeIndex);
		}

		/*
			AliciaSolid
			- LeftUpLeg->LeftUpperLeg
			- LeftLeg->LeftLowerLeg
			- LeftFoot->LeftFoot
			- LeftToeBase->LeftToes
			- LeftToeEnd->なし
		*/
	}

}

void MeshBoneMapper::resolveHeadBones()
{
	auto& bones = m_majorGroups[static_cast<int>(MajorKind::Head)];
	if (bones.size() == 0) {
		return;
	}

	// 深さの順に並び変える
	std::sort(bones.begin(), bones.end(), [this](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });

#if 1	// Debug
	for (auto& bone : bones) {
		std::cout << bone->node->name() << std::endl;
	}
#endif

	// Neck 側からたどっていって、最初に見つかった Branch を Head にしてみる
	int headIndex = bones.indexOfIf([](const NodeInfo* x) { return x->node->children().size() >= 2; });
	if (headIndex >= 0) {
		setHumanoidBoneIndex(HumanoidBones::Head, 0, bones[headIndex]->nodeIndex);
	}

	// 少なくとも Head よりは Depth が浅いことを確認して、front を Neck にする
	if (headIndex >= 1) {
		setHumanoidBoneIndex(HumanoidBones::Neck, 0, bones.front()->nodeIndex);
	}


	//for (const auto& node : bones) {
	//	//if (node->nameMached == HumanoidBones::Jaw) {
	//	//	setHumanoidBoneIndex(HumanoidBones::Jaw, 0, node->nodeIndex);
	//	//}
	//	//else if (node->nameMached == HumanoidBones::LeftEye) {
	//	//	if (!node->isRight)
	//	//		setHumanoidBoneIndex(HumanoidBones::LeftEye, node->nodeIndex);
	//	//	else
	//	//		setHumanoidBoneIndex(HumanoidBones::RightEye, node->nodeIndex);
	//	//}
	//	//else {
	//		if (humanoidBoneIndex(HumanoidBones::Head) < 0) {
	//			// Head (Require) が無ければ Head 扱いする
	//			setHumanoidBoneIndex(HumanoidBones::Head, 0, node->nodeIndex);
	//		}
	//		else if (humanoidBoneIndex(HumanoidBones::Neck) < 0) {
	//			// 既に Head が見つかっている場合、それを Neck に落とした後、node を Head 扱いする。
	//			// node は depth でソートされているので、Neck の方が必ず浅くなる。
	//			setHumanoidBoneIndex(HumanoidBones::Neck, 0, humanoidBoneIndex(HumanoidBones::Head));
	//			setHumanoidBoneIndex(HumanoidBones::Head, 0, node->nodeIndex);
	//		}
	//		else {
	//			// Neck->Head の上にさらに Bone があるようだが、これはとりださない
	//		}
	//	//}
	//}
}

void MeshBoneMapper::resolveEyeBones()
{
	// eye_L と eye_light_L の場合は eye_L を優先したい。
	// Depth で分けられるものでもないので、単語数が少ない方をより明示的な指定とみなして優先する。
	
	// Left
	{
		const auto& nodes = m_majorGroups[static_cast<int>(MajorKind::LeftEye)];
		if (!nodes.isEmpty()) {
			const auto& itr = std::min_element(nodes.begin(), nodes.end(), [](const NodeInfo* a, const NodeInfo* b) { return a->wordCount < b->wordCount; });
			setHumanoidBoneIndex(HumanoidBones::LeftEye, 0, (*itr)->nodeIndex);
		}
	}
	// Right
	{
		const auto& nodes = m_majorGroups[static_cast<int>(MajorKind::RightEye)];
		if (!nodes.isEmpty()) {
			const auto& itr = std::min_element(nodes.begin(), nodes.end(), [](const NodeInfo* a, const NodeInfo* b) { return a->wordCount < b->wordCount; });
			setHumanoidBoneIndex(HumanoidBones::RightEye, 0, (*itr)->nodeIndex);
		}
	}
}

void MeshBoneMapper::resolveJawBones()
{
	const auto& nodes = m_majorGroups[static_cast<int>(MajorKind::Jaw)];
	if (!nodes.isEmpty()) {
		const auto& itr = std::min_element(nodes.begin(), nodes.end(), [](const NodeInfo* a, const NodeInfo* b) { return a->wordCount < b->wordCount; });
		setHumanoidBoneIndex(HumanoidBones::Jaw, 0, (*itr)->nodeIndex);
	}
}

void MeshBoneMapper::resolveFingerBones()
{
	// 各グループを Depth 順にソートして詰めるだけ

	const auto proc = [this](MajorKind majorKind, const std::array<HumanoidBones, 3>& boneKinds) {
		auto& nodes = m_majorGroups[static_cast<int>(majorKind)];
		std::sort(nodes.begin(), nodes.end(), [this](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });

		const int count = std::min(nodes.size(), (int)boneKinds.size());
		for (int i = 0; i < count; i++) {
			setHumanoidBoneIndex(boneKinds[i], 0, nodes[i]->nodeIndex);
		}
	};

	proc(MajorKind::LeftThumb, {
		HumanoidBones::LeftThumbProximal,
		HumanoidBones::LeftThumbIntermediate,
		HumanoidBones::LeftThumbDistal });
	proc(MajorKind::LeftIndex, {
		HumanoidBones::LeftIndexProximal,
		HumanoidBones::LeftIndexIntermediate,
		HumanoidBones::LeftIndexDistal });
	proc(MajorKind::LeftMiddle, {
		HumanoidBones::LeftMiddleProximal,
		HumanoidBones::LeftMiddleIntermediate,
		HumanoidBones::LeftMiddleDistal });
	proc(MajorKind::LeftRing, {
		HumanoidBones::LeftRingProximal,
		HumanoidBones::LeftRingIntermediate,
		HumanoidBones::LeftRingDistal });
	proc(MajorKind::LeftLittle, {
		HumanoidBones::LeftLittleProximal,
		HumanoidBones::LeftLittleIntermediate,
		HumanoidBones::LeftLittleDistal });
	proc(MajorKind::RightThumb, {
		HumanoidBones::RightThumbProximal,
		HumanoidBones::RightThumbIntermediate,
		HumanoidBones::RightThumbDistal });
	proc(MajorKind::RightIndex, {
		HumanoidBones::RightIndexProximal,
		HumanoidBones::RightIndexIntermediate,
		HumanoidBones::RightIndexDistal });
	proc(MajorKind::RightMiddle, {
		HumanoidBones::RightMiddleProximal,
		HumanoidBones::RightMiddleIntermediate,
		HumanoidBones::RightMiddleDistal });
	proc(MajorKind::RightRing, {
		HumanoidBones::RightRingProximal,
		HumanoidBones::RightRingIntermediate,
		HumanoidBones::RightRingDistal });
	proc(MajorKind::RightLittle, {
		HumanoidBones::RightLittleProximal,
		HumanoidBones::RightLittleIntermediate,
		HumanoidBones::RightLittleDistal });
}

#if 0

// name だけじゃ多分求めきれない。子ボーンの数なども必要になりそうなので MeshBone を受け取る。
HumanoidBones SkinnedMeshModel::mapToHumanoidBones(const MeshBone* bone)
{
	struct NamePair
	{
		HumanoidBones kind;
		String name;
	};

	static const NamePair standardNameMap[] = {
		{ HumanoidBones::Hips, _TT("Hips" },
		{ HumanoidBones::Spine, _TT("Spine" },
		{ HumanoidBones::Chest, _TT("Chest" },
		{ HumanoidBones::UpperChest, _TT("UpperChest" },

		{ HumanoidBones::LeftShoulder, _TT("LeftShoulder" },
		{ HumanoidBones::LeftUpperArm, _TT("LeftUpperArm" },
		{ HumanoidBones::LeftLowerArm, _TT("LeftLowerArm" },
		{ HumanoidBones::LeftHand, _TT("LeftHand" },

		{ HumanoidBones::RightShoulder, _TT("RightShoulder" },
		{ HumanoidBones::RightUpperArm, _TT("RightUpperArm" },
		{ HumanoidBones::RightLowerArm, _TT("RightLowerArm" },
		{ HumanoidBones::RightHand, _TT("RightHand" },

		{ HumanoidBones::LeftUpperLeg, _TT("LeftUpperLeg" },
		{ HumanoidBones::LeftLowerLeg, _TT("LeftLowerLeg" },
		{ HumanoidBones::LeftFoot, _TT("LeftFoot" },
		{ HumanoidBones::LeftToes, _TT("LeftToes" },

		{ HumanoidBones::RightUpperLeg, _TT("RightUpperLeg" },
		{ HumanoidBones::RightLowerLeg, _TT("RightLowerLeg" },
		{ HumanoidBones::RightFoot, _TT("RightFoot" },
		{ HumanoidBones::RightToes, _TT("RightToes" },

		{ HumanoidBones::Neck, _TT("Neck" },
		{ HumanoidBones::Head, _TT("Head" },
		{ HumanoidBones::LeftEye, _TT("LeftEye" },
		{ HumanoidBones::RightEye, _TT("RightEye" },
		{ HumanoidBones::Jaw, _TT("Jaw" },

		{ HumanoidBones::LeftThumbProximal, _TT("LeftThumbProximal" },
		{ HumanoidBones::LeftThumbIntermediate, _TT("LeftThumbIntermediate" },
		{ HumanoidBones::LeftThumbDistal, _TT("" },
		{ HumanoidBones::LeftIndexProximal, _TT("" },
		{ HumanoidBones::LeftIndexIntermediate, _TT("" },
		{ HumanoidBones::LeftIndexDistal, _TT("" },
		{ HumanoidBones::LeftMiddleProximal, _TT("" },
		{ HumanoidBones::LeftMiddleIntermediate, _TT("" },
		{ HumanoidBones::LeftMiddleDistal, _TT("" },
		{ HumanoidBones::LeftRingProximal, _TT("" },
		{ HumanoidBones::LeftRingIntermediate, _TT("" },
		{ HumanoidBones::LeftRingDistal, _TT("" },
		{ HumanoidBones::LeftLittleProximal, _TT("" },
		{ HumanoidBones::LeftLittleIntermediate, _TT("" },
		{ HumanoidBones::LeftLittleDistal, _TT("" },

		{ HumanoidBones::RightThumbProximal, _TT("" },
		{ HumanoidBones::RightThumbIntermediate, _TT("" },
		{ HumanoidBones::RightThumbDistal, _TT("" },
		{ HumanoidBones::RightIndexProximal, _TT("" },
		{ HumanoidBones::RightIndexIntermediate, _TT("" },
		{ HumanoidBones::RightIndexDistal, _TT("" },
		{ HumanoidBones::RightMiddleProximal, _TT("" },
		{ HumanoidBones::RightMiddleIntermediate, _TT("" },
		{ HumanoidBones::RightMiddleDistal, _TT("" },
		{ HumanoidBones::RightRingProximal, _TT("" },
		{ HumanoidBones::RightRingIntermediate, _TT("" },
		{ HumanoidBones::RightRingDistal, _TT("" },
		{ HumanoidBones::RightLittleProximal, _TT("" },
		{ HumanoidBones::RightLittleIntermediate, _TT("" },
		{ HumanoidBones::RightLittleDistal,	_TT("" },
	};

	/*
		AliciaSolid
			Up -> Upper

			LeftLeg-> LeftLowerLeg

			Spine->Spine
			Spine1->Chest
			Spine3->UpperChest	// 多分Shoulderの付け根

			LeftArm -> LeftUpperArm
			LeftForeArm -> LeftLowerArm

			LeftToeBase -> LeftToe

			eye_L -> LeftEye

			mouth ->Jaw

			LeftHandThumb1 -> LeftThumbProximal
			LeftHandThumb2 -> LeftThumbIntermediate
			LeftHandThumb3 -> LeftThumbDistal

			LeftHandIndex1-> LeftIndexProximal

			LeftHandPinky1 -> LeftLittleProximal

		PronamaChan
			center -> Hips
			foot_L -> LeftUpperLeg		// bone の「深さ」も観ないとダメかも
			knee_L -> LeftLowerLeg
			ankle_L -> LeftFoot
			upper_body-> Spine
			upper_body2 -> Chest
			shoulder_L -> LeftShoulder
			arm_L -> LeftUpperArm
			elbow_L -> LeftLowerArm
			wrist_L -> LeftHand
			toe2_L -> LeftToes	// toe2_L_end が実際の先端。最初に "toe" っぽいものを集めた後、一番深さの小さいのを選択する必要がある
			eye_L -> LeftEye
			thumb_L0 -> LeftThumbProximal
			thumb_L1 -> LeftThumbIntermediate
			thumb_L2 -> LeftThumbDistal
			indexF_L1 -> LeftIndexProximal

		実装方針：
			- まず大分類して、ネストを測る。分類は、まずはブランチ間。ここは名前検索する。
				- 胴体 (Hips~UpperChest)
				- 腕 (Shoulder~Hand) 左右
				- 手 (各種指) 左右
				- 足 (UpperLeg~Toes) 左右
				- 頭 (Neck~Jaw)
			- 胴体、ネストの小さい方から埋めていく (Chest, UpperChest が opt なので) 数が多い場合は UpperChest が必ず Branch を持つようにする
			- 腕は、ネストの大きい方から埋めていく （Shoulder が opt なので）
			- 足は、ネストの小さい方から埋めていく (Toes が opt なので)
			- 手は全部 opt なので、ネストの小さい方から埋めていく
	*/
}
#endif
} // namespace detail
} // namespace ln
