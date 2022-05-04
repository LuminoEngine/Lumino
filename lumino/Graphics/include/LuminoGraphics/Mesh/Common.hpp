#pragma once
#include <LuminoGraphics/Common.hpp>

namespace ln {
class MeshImportSettings;
class MeshPrimitive;
class MeshModel;
class SkinnedMeshModel;
struct MeshSection2;

// 標準ボーン
enum class HumanoidBones
{
	None,

	// Body
	Hips,
	Spine,
	Chest,		// opt
	UpperChest,	// opt

	// Left Arm
	LeftShoulder,	// opt 鎖骨原点
	LeftUpperArm,	// 型原点
	LeftLowerArm,	// 肘原点
	LeftHand,		// 手首原点

	// Right Arm
	RightShoulder,	// opt 鎖骨原点
	RightUpperArm,	// 型原点
	RightLowerArm,	// 肘原点
	RightHand,		// 手首原点

	// Left Leg
	LeftUpperLeg,
	LeftLowerLeg,	// 膝原点
	LeftFoot,		// かかと原点
	LeftToes,		// opt つまさき

	// Right Leg
	RightUpperLeg,
	RightLowerLeg,	// 膝原点
	RightFoot,		// かかと原点
	RightToes,		// opt つまさき

	// Head
	Neck,			// opt 首下原点
	Head,			// 首上原点
	LeftEye,		// opt 
	RightEye,		// opt 
	Jaw,			// opt

	// Left Hand
	LeftThumbProximal,		// opt 親指 付け根
	LeftThumbIntermediate,	// opt 親指 中
	LeftThumbDistal,		// opt 親指 先
	LeftIndexProximal,		// opt 親指 付け根
	LeftIndexIntermediate,	// opt 親指 中
	LeftIndexDistal,		// opt 親指 先
	LeftMiddleProximal,		// opt 中指 付け根
	LeftMiddleIntermediate,	// opt 中指 中
	LeftMiddleDistal,		// opt 中指 先
	LeftRingProximal,		// opt 薬指 付け根
	LeftRingIntermediate,	// opt 薬指 中
	LeftRingDistal,			// opt 薬指 先
	LeftLittleProximal,		// opt 子指 付け根
	LeftLittleIntermediate,	// opt 子指 中
	LeftLittleDistal,		// opt 子指 先

	// Right Hand
	RightThumbProximal,		// opt 親指 付け根
	RightThumbIntermediate,	// opt 親指 中
	RightThumbDistal,		// opt 親指 先
	RightIndexProximal,		// opt 親指 付け根
	RightIndexIntermediate,	// opt 親指 中
	RightIndexDistal,		// opt 親指 先
	RightMiddleProximal,		// opt 中指 付け根
	RightMiddleIntermediate,	// opt 中指 中
	RightMiddleDistal,		// opt 中指 先
	RightRingProximal,		// opt 薬指 付け根
	RightRingIntermediate,	// opt 薬指 中
	RightRingDistal,			// opt 薬指 先
	RightLittleProximal,		// opt 子指 付け根
	RightLittleIntermediate,	// opt 子指 中
	RightLittleDistal,		// opt 子指 先
};

//enum class MeshCreationFlags
//{
//    None = 0x0000,
//    //StrictResourceLoading = 0x04,		// マテリアルに含まれるテクスチャファイルが見つからなくても無視する
//    //ModelCreateFlag_CopyMaterials = 0x08,		// マテリアルを共有せずにコピーする
//};
//LN_FLAGS_OPERATORS(BoneType);

//class RenderFeature
//	: public Object
//{
//public:
//	virtual void flush() = 0;
//
//private:
//};

namespace detail {
class MeshModelInstance;
} // namespace detail
} // namespace ln

