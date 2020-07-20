
#include "Internal.hpp"
#include "MeshBoneMapper.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// MeshBoneMapper

MeshBoneMapper::MeshBoneMapper()
{
}

void MeshBoneMapper::map(MeshArmature* skeleton)
{
	m_model = skeleton->m_model;

	int nodeCount = skeleton->boneCount();
	m_nodes.resize(nodeCount);
	for (int i = 0; i < nodeCount; i++) {
		m_nodes[i].node = m_model->m_nodes[i];
	}

	int boneCount = skeleton->boneCount();
	m_bones.resize(boneCount);
	for (int i = 0; i < boneCount; i++) {
		m_bones[i].bone = skeleton->bone(i);
		m_nodes[skeleton->bone(i)->nodeIndex()].boneIndex = i;
	}

	// 深さを測る
	for (int i = 0; i < m_model->m_rootNodes.size(); i++) {
		calculateNodeDepthHieratical(&m_nodes[m_model->m_rootNodes[i]], 0);
	}

	// 大分類
	for (int i = 0; i < m_model->m_nodes.size(); i++) {
		makeMajorKindByName(&m_nodes[i], m_nodes[i].node->name());
	}

	resolveBodyBones();

}

void MeshBoneMapper::calculateNodeDepthHieratical(NodeInfo* node, int depth)
{
	node->depth = depth;

	for (int child : node->node->children()) {
		calculateNodeDepthHieratical(&m_nodes[child], depth + 1);
	}
}

void MeshBoneMapper::makeMajorKindByName(NodeInfo* info, const String& name)
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
				// 単語先頭の一致を評価する
				if (StringHelper::indexOf(word.data(), word.length(), nameMap[i].name.c_str(), nameMap[i].name.length(), 0, CaseSensitivity::CaseInsensitive) == 0) {
					info->majorKind = nameMap[i].kind;
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
	auto& nodes = m_majorGroups[static_cast<int>(MajorKind::Body)];

	// 深さの順に並び変える
	std::sort(nodes.begin(), nodes.end(), [](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });

	if (nodes.size() == 1) {
		// Require の数以下は強制設定
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, nodes[0]->boneIndex);
	}
	else if (nodes.size() == 2) {
		// Require の数以下は強制設定
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, nodes[0]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Spine, nodes[1]->boneIndex);
	}
	else if (nodes.size() == 3) {
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, nodes[0]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Spine, nodes[1]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Chest, nodes[2]->boneIndex);
	}
	else if (nodes.size() == 4) {
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, nodes[0]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Spine, nodes[1]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Chest, nodes[2]->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::UpperChest, nodes[3]->boneIndex);
	}
	else {
		// まず Hips と UpperChest を、Depth から決める
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::Hips, nodes.front()->boneIndex);
		m_skeleton->setHumanoidBoneIndex(HumanoidBones::UpperChest, nodes.back()->boneIndex);

		// TODO: 名前で優先度を決める
		LN_NOTIMPLEMENTED();
	}

	HumanoidBones bodyBones[] = {
		HumanoidBones::Hips,
		HumanoidBones::Spine,
		HumanoidBones::Chest,
		HumanoidBones::UpperChest,
	};
	// 下から見ていって、最初に見つかった分岐を持つ Bone を、m_lowerBranchNodeIndex にする (通常、腰など)
	for (int i = 0; i < LN_ARRAY_SIZE_OF(bodyBones); i++) {
		int boneIndex = m_skeleton->humanoidBoneIndex(bodyBones[i]);
		if (boneIndex >= 0 && !m_skeleton->bone(boneIndex)->node()->children().isEmpty()) {
			m_lowerBranchNodeDepth = m_nodes[m_skeleton->bone(boneIndex)->nodeIndex()].depth;
		}
	}
	// 上から見ていって、最初に見つかった分岐を持つ Bone を、m_lowerBranchNodeIndex にする (通常、胸など)
	for (int i = LN_ARRAY_SIZE_OF(bodyBones) - 1; i >= 0; i--) {
		int boneIndex = m_skeleton->humanoidBoneIndex(bodyBones[i]);
		if (boneIndex >= 0 && !m_skeleton->bone(boneIndex)->node()->children().isEmpty()) {
			m_upperBranchNodeDepth = m_nodes[m_skeleton->bone(boneIndex)->nodeIndex()].depth;
		}
	}
}

void MeshBoneMapper::resolveLegs()
{
	const auto& nodes = m_majorGroups[static_cast<int>(MajorKind::LeftLeg)];
	auto min = std::min_element(nodes.begin(), nodes.end(), [](const NodeInfo* a, const NodeInfo* b) { return a->depth < b->depth; });


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
