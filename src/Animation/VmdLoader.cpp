
#include "Internal.h"
#include "VmdLoader.h"

LN_NAMESPACE_BEGIN
  
//==============================================================================
// VmdLoader
//==============================================================================

//------------------------------------------------------------------------------
VmdLoader::~VmdLoader()
{
}

//------------------------------------------------------------------------------
bool VmdLoader::Load(Stream* stream)
{
	m_lastFramePosition = 0.0;
	BinaryReader reader(stream);

	//-----------------------------------------------------
	// ヘッダのチェック

	VMD_Header vmdHeader;
	reader.Read(&vmdHeader, sizeof(VMD_Header));
	if (strncmp(vmdHeader.szHeader, "Vocaloid Motion Data 0002", 30) != 0) {
		return false;
	}

	//-----------------------------------------------------
	// ボーンアニメーション

	uint32_t allBoneKeyCount = reader.ReadUInt32();
	for (uint32_t i = 0; i < allBoneKeyCount; ++i)
	{
		VMD_Motion vmdMotion;
		reader.Read(&vmdMotion, sizeof(VMD_Motion));

		// 最大フレーム更新
		if (m_lastFramePosition < (float)vmdMotion.ulFrameNo) {
			m_lastFramePosition = (float)vmdMotion.ulFrameNo;
		}

		//if (strcmp(vmdMotion.szBoneName, "センター先") == 0) {
		//	printf("[%s]", vmdMotion.szBoneName);
		//}

		// 既存アニメーション検索
		BoneAnimationIndexMap::iterator itr =
			m_boneAnimationIndexMap.find(vmdMotion.szBoneName);


		// 新しいボーン
		if (itr == m_boneAnimationIndexMap.end())
		{
			BoneAnimation anim;
			anim.TargetBoneName.ConvertFrom(vmdMotion.szBoneName, 15, Encoding::GetEncoding(EncodingType::SJIS));

			// 名前・インデックスの対応
			m_boneAnimationIndexMap.insert(BoneAnimationIndexPair(vmdMotion.szBoneName, m_boneAnimationList.GetCount()));

			// アニメーション作成、キー追加
			anim.AnimationCurve = RefPtr<VMDBezierSQTTransformAnimation>::MakeRef();
			anim.AnimationCurve->AddKeyFrame(
				static_cast< double >(vmdMotion.ulFrameNo),
				vmdMotion.vec3Position,
				vmdMotion.vec4Rotation,
				vmdMotion.cInterpolationX,
				vmdMotion.cInterpolationY,
				vmdMotion.cInterpolationZ,
				vmdMotion.cInterpolationRot);
			m_boneAnimationList.Add(anim);
		}
		// すでにあるボーン
		else
		{
			m_boneAnimationList[(itr->second)].AnimationCurve->AddKeyFrame(
				static_cast< double >(vmdMotion.ulFrameNo),
				vmdMotion.vec3Position,
				vmdMotion.vec4Rotation,
				vmdMotion.cInterpolationX,
				vmdMotion.cInterpolationY,
				vmdMotion.cInterpolationZ,
				vmdMotion.cInterpolationRot);
		}
	}

	// キーフレーム順にソート
	for (BoneAnimation& anim : m_boneAnimationList)
	{
		anim.AnimationCurve->SortKeyFrame();
	}

	//-----------------------------------------------------
	// 表情アニメーション

	uint32_t allFaceKeyCount = reader.ReadUInt32();
	for (uint32_t i = 0; i < allFaceKeyCount; ++i)
	{
		VMD_Face vmdFace;
		reader.Read(&vmdFace, sizeof(VMD_Face));

		// 最大フレーム更新
		if (m_lastFramePosition < (float)vmdFace.ulFrameNo)
			m_lastFramePosition = (float)vmdFace.ulFrameNo;

		// 既存アニメーション検索
		FaceAnimationIndexMap::iterator itr =
			m_faceAnimationIndexMap.find(vmdFace.szFaceName);

		// 新しい表情
		if (itr == m_faceAnimationIndexMap.end())
		{
			FaceAnimation anim;
			anim.TargetFaceName.ConvertFrom(vmdFace.szFaceName, 15, Encoding::GetEncoding(EncodingType::SJIS));
			//anim.TargetFaceName = String(vmdFace.szFaceName);

			// 名前・インデックスの対応
			m_faceAnimationIndexMap.insert(FaceAnimationIndexPair(anim.TargetFaceName, m_faceAnimationList.GetCount()));

			// アニメーション作成、キー追加
			anim.AnimationCurve = RefPtr<FloatAnimationCurve>::MakeRef();
			anim.AnimationCurve->AddKeyFrame(vmdFace.ulFrameNo, vmdFace.fFactor);
			m_faceAnimationList.Add(anim);
		}
		// すでにある表情
		else
		{
			m_faceAnimationList[(itr->second)].AnimationCurve->AddKeyFrame(
				vmdFace.ulFrameNo, vmdFace.fFactor);
		}
	}

	// キーフレーム順にソート
	//for (FaceAnimation& anim : m_faceAnimationList)
	//{
	//	anim.AnimationCurve->SortKeyFrame();
	//}
	return true;
}

LN_NAMESPACE_END
