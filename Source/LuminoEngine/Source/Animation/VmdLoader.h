
#pragma once
#include <map>
#include "../Animation/AnimationClip.h"

LN_NAMESPACE_BEGIN

// VMD モーションファイルの読み取り
class VmdLoader
{
public:
#pragma pack(push, 1)
	// ファイルヘッダ
	struct VMD_Header
	{
		char	szHeader[30];			// "Vocaloid Motion Data 0002"
		char	szModelName[20];		// 対象モデル名
	};

	// モーションデータ
	struct VMD_Motion
	{
		char	szBoneName[15];			// ボーン名

		unsigned long	ulFrameNo;		// フレーム番号

		Vector3	vec3Position;			// 位置
		Quaternion	vec4Rotation;		// 回転(クォータニオン)

		//char	cInterpolationX[16];	// 補間情報 X軸移動
		//char	cInterpolationY[16];	// 補間情報 Y軸移動
		//char	cInterpolationZ[16];	// 補間情報 Z軸移動
		//char	cInterpolationRot[16];	// 補間情報 回転

		byte_t	Interpolation[4][4][4];
	};

	// 表情データ
	struct VMD_Face
	{
		char			szFaceName[15];	// 表情名
		unsigned long	ulFrameNo;		// フレーム番号
		float			fFactor;		// ブレンド率
	};
#pragma pack( pop )

	struct BoneAnimation
	{
		String TargetBoneName;
		RefPtr<VMDBezierSQTTransformAnimation2>	AnimationCurve;
	};
	
	struct FaceAnimation
	{
		String TargetFaceName;
		RefPtr<FloatAnimationCurve>	AnimationCurve;
	};

	typedef List<BoneAnimation>	BoneAnimationList;
	typedef List<FaceAnimation>	FaceAnimationList;

public:
	VmdLoader() {}
	~VmdLoader();

	// 読み込み
	bool load(Stream* stream);

	double getLastFramePosition() const { return m_lastFramePosition; }
	BoneAnimationList& getBoneAnimationList() { return m_boneAnimationList; }
	FaceAnimationList& getFaceAnimationList() { return m_faceAnimationList; }

private:
	// こちらの検索キーは StringA。VMD ファイルから読み取ったキー名をそのまま比較できるようにするため。
    typedef std::map<StringA, int>	BoneAnimationIndexMap;
	typedef std::pair<StringA, int>	BoneAnimationIndexPair;
	typedef std::map<StringA, int>	FaceAnimationIndexMap;
	typedef std::pair<StringA, int>	FaceAnimationIndexPair;
	
	double					m_lastFramePosition;
	BoneAnimationList		m_boneAnimationList;
	BoneAnimationIndexMap	m_boneAnimationIndexMap;
	FaceAnimationList		m_faceAnimationList;
	FaceAnimationIndexMap   m_faceAnimationIndexMap;
};

LN_NAMESPACE_END
