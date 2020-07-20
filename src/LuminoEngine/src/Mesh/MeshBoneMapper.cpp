
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
	{ HumanoidBones::None, u"None" },

	{ HumanoidBones::Hips, u"Hips" },
	{ HumanoidBones::Spine, u"Spine" },
	{ HumanoidBones::Chest, u"Chest" },
	{ HumanoidBones::UpperChest, u"UpperChest" },

	{ HumanoidBones::LeftShoulder, u"LeftShoulder" },
	{ HumanoidBones::LeftUpperArm, u"LeftUpperArm" },
	{ HumanoidBones::LeftLowerArm, u"LeftLowerArm" },
	{ HumanoidBones::LeftHand, u"LeftHand" },

	{ HumanoidBones::RightShoulder, u"RightShoulder" },
	{ HumanoidBones::RightUpperArm, u"RightUpperArm" },
	{ HumanoidBones::RightLowerArm, u"RightLowerArm" },
	{ HumanoidBones::RightHand, u"RightHand" },

	{ HumanoidBones::LeftUpperLeg, u"LeftUpperLeg" },
	{ HumanoidBones::LeftLowerLeg, u"LeftLowerLeg" },
	{ HumanoidBones::LeftFoot, u"LeftFoot" },
	{ HumanoidBones::LeftToes, u"LeftToes" },

	{ HumanoidBones::RightUpperLeg, u"RightUpperLeg" },
	{ HumanoidBones::RightLowerLeg, u"RightLowerLeg" },
	{ HumanoidBones::RightFoot, u"RightFoot" },
	{ HumanoidBones::RightToes, u"RightToes" },

	{ HumanoidBones::Neck, u"Neck" },
	{ HumanoidBones::Head, u"Head" },
	{ HumanoidBones::LeftEye, u"LeftEye" },
	{ HumanoidBones::RightEye, u"RightEye" },
	{ HumanoidBones::Jaw, u"Jaw" },

	{ HumanoidBones::LeftThumbProximal, u"LeftThumbProximal" },
	{ HumanoidBones::LeftThumbIntermediate, u"LeftThumbIntermediate" },
	{ HumanoidBones::LeftThumbDistal, u"LeftThumbDistal" },
	{ HumanoidBones::LeftIndexProximal, u"LeftIndexProximal" },
	{ HumanoidBones::LeftIndexIntermediate, u"LeftIndexIntermediate" },
	{ HumanoidBones::LeftIndexDistal, u"LeftIndexDistal" },
	{ HumanoidBones::LeftMiddleProximal, u"LeftMiddleProximal" },
	{ HumanoidBones::LeftMiddleIntermediate, u"LeftMiddleIntermediate" },
	{ HumanoidBones::LeftMiddleDistal, u"LeftMiddleDistal" },
	{ HumanoidBones::LeftRingProximal, u"LeftRingProximal" },
	{ HumanoidBones::LeftRingIntermediate, u"LeftRingIntermediate" },
	{ HumanoidBones::LeftRingDistal, u"LeftRingDistal" },
	{ HumanoidBones::LeftLittleProximal, u"LeftLittleProximal" },
	{ HumanoidBones::LeftLittleIntermediate, u"LeftLittleIntermediate" },
	{ HumanoidBones::LeftLittleDistal, u"LeftLittleDistal" },

	{ HumanoidBones::RightThumbProximal, u"RightThumbProximal" },
	{ HumanoidBones::RightThumbIntermediate, u"RightThumbIntermediate" },
	{ HumanoidBones::RightThumbDistal, u"RightThumbDistal" },
	{ HumanoidBones::RightIndexProximal, u"RightIndexProximal" },
	{ HumanoidBones::RightIndexIntermediate, u"RightIndexIntermediate" },
	{ HumanoidBones::RightIndexDistal, u"RightIndexDistal" },
	{ HumanoidBones::RightMiddleProximal, u"RightMiddleProximal" },
	{ HumanoidBones::RightMiddleIntermediate, u"RightMiddleIntermediate" },
	{ HumanoidBones::RightMiddleDistal, u"RightMiddleDistal" },
	{ HumanoidBones::RightRingProximal, u"RightRingProximal" },
	{ HumanoidBones::RightRingIntermediate, u"RightRingIntermediate" },
	{ HumanoidBones::RightRingDistal, u"RightRingDistal" },
	{ HumanoidBones::RightLittleProximal, u"RightLittleProximal" },
	{ HumanoidBones::RightLittleIntermediate, u"RightLittleIntermediate" },
	{ HumanoidBones::RightLittleDistal, u"RightLittleDistal" },
};

MeshBoneMapper::MeshBoneMapper()
{
}

void MeshBoneMapper::map(MeshArmature* skeleton)
{
	m_skeleton = skeleton;
	m_model = skeleton->m_model;

	int nodeCount = m_model->m_nodes.size();
	m_nodes.resize(nodeCount);
	for (int i = 0; i < nodeCount; i++) {
		m_nodes[i].node = m_model->m_nodes[i];
	}

	int boneCount = skeleton->boneCount();
	m_bones.resize(boneCount);
	for (int i = 0; i < boneCount; i++) {
		m_bones[i].bone = skeleton->bone(i);
		m_bones[i].boneIndex = i;

		int nodeIndex = skeleton->bone(i)->nodeIndex();
		if (LN_REQUIRE(m_nodes[nodeIndex].boneIndex == -1)) return;
		m_nodes[nodeIndex].boneIndex = i;
	}

	// 深さを測る
	for (int i : m_model->m_rootNodes) {
		calculateNodeDepthHieratical(&m_nodes[i], 0);
	}

	// 大分類
	for (int i = 0; i < boneCount; i++) {
		makeMajorKindByName(&m_bones[i], getMeshNodeByBone(i)->name());
	}

	resolveBodyBones();


#if 1	// Debug
	for (int i = 0; i < m_skeleton->m_humanoidBoneIndices.size(); i++) {
		int boneIndex = m_skeleton->m_humanoidBoneIndices[i];
		if (boneIndex >= 0)
			std::cout << s_humanoidBoneNames[i].name << ": " << boneIndex << " " << getMeshNodeByBone(boneIndex)->name() << std::endl;
		else
			std::cout << i << ": -" << std::endl;
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

void MeshBoneMapper::makeMajorKindByName(BoneInfo* info, const String& name)
{
	struct NamePair
	{
		String name;
		MajorKind kind;
		HumanoidBones boneKind;
	};

	static const NamePair nameMap[] = {
		{ u"Hips", MajorKind::Body, HumanoidBones::Hips },
		{ u"Spine", MajorKind::Body, HumanoidBones::Spine },	// Spine,Spine1,Spine3
		{ u"Chest", MajorKind::Body, HumanoidBones::Chest },	// Chest,UpperChest
		{ u"Center", MajorKind::Body, HumanoidBones::None },	// (PronamaChan)

		{ u"Neck", MajorKind::Head, HumanoidBones::Neck },
		{ u"Head", MajorKind::Head, HumanoidBones::Head },
		{ u"Eye", MajorKind::Head, HumanoidBones::LeftEye },	// LeftEye,RightEye
		{ u"Jaw", MajorKind::Head, HumanoidBones::Jaw },
		{ u"Mouth", MajorKind::Head, HumanoidBones::Jaw },	// (AliciaSolid)

		{ u"Shoulder", MajorKind::LeftArm, HumanoidBones::LeftShoulder },	// 左右を伴うものは一度 Left に集める
		{ u"Arm", MajorKind::LeftArm, HumanoidBones::None },	// UpperArm,LowerArm,ForeArm
		{ u"Hand", MajorKind::LeftArm, HumanoidBones::LeftHand },
		{ u"Elbow", MajorKind::LeftArm, HumanoidBones::LeftLowerArm },	// (PronamaChan)
		{ u"Wrist", MajorKind::LeftArm, HumanoidBones::LeftHand },	// (PronamaChan)
		
		{ u"Thumb", MajorKind::LeftHand },	// Thumb1,ThumbProximal,ThumbIntermediate,ThumbDistal
		{ u"Index", MajorKind::LeftHand },	// Index1,IndexProximal,IndexIntermediate,IndexDistal
		{ u"Middle", MajorKind::LeftHand },	// Middle1,MiddleProximal,MiddleIntermediate,MiddleDistal
		{ u"Ring", MajorKind::LeftHand },	// Ring1,RingProximal,RingIntermediate,RingDistal
		{ u"Little", MajorKind::LeftHand },	// Little1,LittleProximal,LittleIntermediate,LittleDistal
		{ u"Pinky", MajorKind::LeftHand },	// (AliciaSolid) 

		{ u"Leg", MajorKind::LeftLeg },	// UpperLeg,LowerLeg,Leg
		{ u"Foot", MajorKind::LeftLeg },
		{ u"Knee", MajorKind::LeftLeg },	// (PronamaChan)
		{ u"Ankle", MajorKind::LeftLeg },	// (PronamaChan)
		{ u"Toe", MajorKind::LeftLeg },



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
	};

	info->majorKind = MajorKind::None;
	bool isRight = false;
	const auto& words = splitWords(name);
	for (const auto& word : words) {
		if (StringHelper::indexOf(word.data(), word.length(), u"right", 5, 0, CaseSensitivity::CaseInsensitive) == 0) {
			isRight = true;
		}
		else if (word.length() >= 1 && (word[0] == 'R' || word[0] == 'r')) {
			isRight = true;
		}
		else {
			for (int i = 0; i < LN_ARRAY_SIZE_OF(nameMap); i++) {
				const auto e = nameMap[i];
				// 単語先頭の一致を評価する
				if (StringHelper::indexOf(word.data(), e.name.length(), e.name.c_str(), e.name.length(), 0, CaseSensitivity::CaseInsensitive) == 0) {
					info->majorKind = e.kind;
					break;
				}
			}
		}
	}

	if (isRight) {
		if (info->majorKind == MajorKind::LeftArm) info->majorKind = MajorKind::RightArm;
		if (info->majorKind == MajorKind::LeftHand) info->majorKind = MajorKind::RightHand;
		if (info->majorKind == MajorKind::LeftLeg) info->majorKind = MajorKind::RightLeg;
	}

	if (info->majorKind != MajorKind::None) {
		m_majorGroups[static_cast<int>(info->majorKind)].add(info);
	}
}

const List<StringRef>& MeshBoneMapper::splitWords(const String& name)
{
	m_splitCache.clear();

	const Char* begin = name.c_str();
	const Char* pos = name.c_str();
	for (; *pos; pos++) {
		if (isupper(*pos)) {
			m_splitCache.add(StringRef(begin, pos - begin));
			begin = pos;
		}
		else if (*pos == '_') {
			m_splitCache.add(StringRef(begin, pos - begin));
			begin = pos;
			pos++;
		}
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
			m_splitCache[i] = StringRef(str.data(), str.length() - nums);
		}
	}

	return m_splitCache;
}

void MeshBoneMapper::resolveBodyBones()
{
	auto& bones = m_majorGroups[static_cast<int>(MajorKind::Body)];

	// 深さの順に並び変える
	std::sort(bones.begin(), bones.end(), [this](const BoneInfo* a, const BoneInfo* b) { return getDepth(a) < getDepth(b); });

	if (bones.size() == 1) {
		// Require の数以下は強制設定
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, bones[0]->boneIndex);
	}
	else if (bones.size() == 2) {
		// Require の数以下は強制設定
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, bones[0]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Spine, bones[1]->boneIndex);
	}
	else if (bones.size() == 3) {
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, bones[0]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Spine, bones[1]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Chest, bones[2]->boneIndex);
	}
	else if (bones.size() == 4) {
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, bones[0]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Spine, bones[1]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Chest, bones[2]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::UpperChest, bones[3]->boneIndex);
	}
	else {
#if 1	// Debug
		for (auto& bone : bones) {
			std::cout << bone->bone->node()->name() << std::endl;
		}
#endif
		// まず Hips と UpperChest を、Depth から決める
		//m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, bones.front()->boneIndex);
		//m_skeleton->setHumanoidBoneIndex(HumanoidBones::UpperChest, bones.back()->boneIndex);

		// Branch となっている Bone を上下から検索する
		int firstBranch = bones.indexOfIf([this](const BoneInfo* x) { return getMeshNodeByBone(x)->children().size() >= 2; });
		int lastBranch = bones.lastIndexOfIf([this](const BoneInfo* x) { return getMeshNodeByBone(x)->children().size() >= 2; });
		if (LN_REQUIRE(firstBranch >= 0 && lastBranch >= 0 && firstBranch < lastBranch)) return;

		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, bones[firstBranch]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::UpperChest, bones[lastBranch]->boneIndex);

		const HumanoidBones spineOrChest[] = {
			HumanoidBones::Spine,
			HumanoidBones::Chest,
		};
		for (int i = 0; i < std::min(2, (lastBranch - firstBranch)); i++) {
			m_skeleton->setHumanoidBoneIndex(spineOrChest[i], bones[firstBranch + 1 + i]->boneIndex);
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
		int boneIndex = m_skeleton->humanoidBoneIndex(bodyBones[i]);
		if (boneIndex >= 0 && !m_skeleton->bone(boneIndex)->node()->children().isEmpty()) {
			m_lowerBranchNodeDepth = getDepth(&m_bones[boneIndex]);
		}
	}
	// 上から見ていって、最初に見つかった分岐を持つ Bone を、m_lowerBranchNodeIndex にする (通常、胸など)
	for (int i = LN_ARRAY_SIZE_OF(bodyBones) - 1; i >= 0; i--) {
		int boneIndex = m_skeleton->humanoidBoneIndex(bodyBones[i]);
		if (boneIndex >= 0 && !m_skeleton->bone(boneIndex)->node()->children().isEmpty()) {
			m_upperBranchNodeDepth = getDepth(&m_bones[boneIndex]);
		}
	}
}

void MeshBoneMapper::resolveLegs()
{


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
		{ HumanoidBones::Hips, u"Hips" },
		{ HumanoidBones::Spine, u"Spine" },
		{ HumanoidBones::Chest, u"Chest" },
		{ HumanoidBones::UpperChest, u"UpperChest" },

		{ HumanoidBones::LeftShoulder, u"LeftShoulder" },
		{ HumanoidBones::LeftUpperArm, u"LeftUpperArm" },
		{ HumanoidBones::LeftLowerArm, u"LeftLowerArm" },
		{ HumanoidBones::LeftHand, u"LeftHand" },

		{ HumanoidBones::RightShoulder, u"RightShoulder" },
		{ HumanoidBones::RightUpperArm, u"RightUpperArm" },
		{ HumanoidBones::RightLowerArm, u"RightLowerArm" },
		{ HumanoidBones::RightHand, u"RightHand" },

		{ HumanoidBones::LeftUpperLeg, u"LeftUpperLeg" },
		{ HumanoidBones::LeftLowerLeg, u"LeftLowerLeg" },
		{ HumanoidBones::LeftFoot, u"LeftFoot" },
		{ HumanoidBones::LeftToes, u"LeftToes" },

		{ HumanoidBones::RightUpperLeg, u"RightUpperLeg" },
		{ HumanoidBones::RightLowerLeg, u"RightLowerLeg" },
		{ HumanoidBones::RightFoot, u"RightFoot" },
		{ HumanoidBones::RightToes, u"RightToes" },

		{ HumanoidBones::Neck, u"Neck" },
		{ HumanoidBones::Head, u"Head" },
		{ HumanoidBones::LeftEye, u"LeftEye" },
		{ HumanoidBones::RightEye, u"RightEye" },
		{ HumanoidBones::Jaw, u"Jaw" },

		{ HumanoidBones::LeftThumbProximal, u"LeftThumbProximal" },
		{ HumanoidBones::LeftThumbIntermediate, u"LeftThumbIntermediate" },
		{ HumanoidBones::LeftThumbDistal, u"" },
		{ HumanoidBones::LeftIndexProximal, u"" },
		{ HumanoidBones::LeftIndexIntermediate, u"" },
		{ HumanoidBones::LeftIndexDistal, u"" },
		{ HumanoidBones::LeftMiddleProximal, u"" },
		{ HumanoidBones::LeftMiddleIntermediate, u"" },
		{ HumanoidBones::LeftMiddleDistal, u"" },
		{ HumanoidBones::LeftRingProximal, u"" },
		{ HumanoidBones::LeftRingIntermediate, u"" },
		{ HumanoidBones::LeftRingDistal, u"" },
		{ HumanoidBones::LeftLittleProximal, u"" },
		{ HumanoidBones::LeftLittleIntermediate, u"" },
		{ HumanoidBones::LeftLittleDistal, u"" },

		{ HumanoidBones::RightThumbProximal, u"" },
		{ HumanoidBones::RightThumbIntermediate, u"" },
		{ HumanoidBones::RightThumbDistal, u"" },
		{ HumanoidBones::RightIndexProximal, u"" },
		{ HumanoidBones::RightIndexIntermediate, u"" },
		{ HumanoidBones::RightIndexDistal, u"" },
		{ HumanoidBones::RightMiddleProximal, u"" },
		{ HumanoidBones::RightMiddleIntermediate, u"" },
		{ HumanoidBones::RightMiddleDistal, u"" },
		{ HumanoidBones::RightRingProximal, u"" },
		{ HumanoidBones::RightRingIntermediate, u"" },
		{ HumanoidBones::RightRingDistal, u"" },
		{ HumanoidBones::RightLittleProximal, u"" },
		{ HumanoidBones::RightLittleIntermediate, u"" },
		{ HumanoidBones::RightLittleDistal,	u"" },
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
