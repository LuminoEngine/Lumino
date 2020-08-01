
#include "Internal.hpp"
#include "VmdLoader.hpp"

namespace ln {

#if 0
//==============================================================================
// VmdLoader
//==============================================================================

//------------------------------------------------------------------------------
VmdLoader::~VmdLoader()
{
}

//------------------------------------------------------------------------------
bool VmdLoader::load(Stream* stream)
{
	m_lastFramePosition = 0.0;
	BinaryReader reader(stream);

	//-----------------------------------------------------
	// ヘッダのチェック

	VMD_Header vmdHeader;
	reader.read(&vmdHeader, sizeof(VMD_Header));
	if (strncmp(vmdHeader.szHeader, "Vocaloid Motion Data 0002", 30) != 0) {
		return false;
	}

	//-----------------------------------------------------
	// ボーンアニメーション

	uint32_t allBoneKeyCount = reader.readUInt32();
	for (uint32_t i = 0; i < allBoneKeyCount; ++i)
	{
		VMD_Motion vmdMotion;
		reader.read(&vmdMotion, sizeof(VMD_Motion));

		// 最大フレーム更新
		if (m_lastFramePosition < (float)vmdMotion.ulFrameNo) {
			m_lastFramePosition = (float)vmdMotion.ulFrameNo;
		}

		//if (strcmp(vmdMotion.szBoneName, "センター先") == 0) {
		//	printf("[%s]", vmdMotion.szBoneName);
		//}


		VMDBezierSQTTransformAnimation2::BoneFrameData frame;
		frame.Time = static_cast< double >(vmdMotion.ulFrameNo) / 30.0f;
		frame.Position = vmdMotion.vec3Position;
		frame.Rotation = vmdMotion.vec4Rotation;
		for (int iC = 0; iC < 4; ++iC)
		{
			frame.Curves[iC].v1.set(vmdMotion.Interpolation[0][0][iC] / 128.0f, vmdMotion.Interpolation[0][1][iC] / 128.0f);
			frame.Curves[iC].v2.set(vmdMotion.Interpolation[0][2][iC] / 128.0f, vmdMotion.Interpolation[0][3][iC] / 128.0f);
		}

		// 既存アニメーション検索
		BoneAnimationIndexMap::iterator itr =
			m_boneAnimationIndexMap.find(vmdMotion.szBoneName);


		// 新しいボーン
		if (itr == m_boneAnimationIndexMap.end())
		{
			BoneAnimation anim;
			anim.TargetBoneName = Encoding::fromBytes(vmdMotion.szBoneName, 15, Encoding::getEncoding(EncodingType::SJIS));

			// 名前・インデックスの対応
			m_boneAnimationIndexMap.insert(BoneAnimationIndexPair(vmdMotion.szBoneName, m_boneAnimationList.getCount()));

			// アニメーション作成、キー追加
			anim.AnimationCurve = Ref<VMDBezierSQTTransformAnimation2>::makeRef();
			anim.AnimationCurve->addFrame(frame);
			m_boneAnimationList.add(anim);
		}
		// すでにあるボーン
		else
		{
			m_boneAnimationList[(itr->second)].AnimationCurve->addFrame(frame);
		}
	}

	// キーフレーム順にソート
	for (BoneAnimation& anim : m_boneAnimationList)
	{
		anim.AnimationCurve->sortKeyFrame();
	}

	//-----------------------------------------------------
	// 表情アニメーション

	uint32_t allFaceKeyCount = reader.readUInt32();
	for (uint32_t i = 0; i < allFaceKeyCount; ++i)
	{
		VMD_Face vmdFace;
		reader.read(&vmdFace, sizeof(VMD_Face));

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
			anim.TargetFaceName = Encoding::fromBytes(vmdFace.szFaceName, 15, Encoding::getEncoding(EncodingType::SJIS));
			//anim.TargetFaceName = String(vmdFace.szFaceName);

			// 名前・インデックスの対応
			m_faceAnimationIndexMap.insert(FaceAnimationIndexPair(vmdFace.szFaceName, m_faceAnimationList.getCount()));

			// アニメーション作成、キー追加
			anim.AnimationCurve = Ref<FloatAnimationCurve>::makeRef();
			anim.AnimationCurve->addKeyFrame(vmdFace.ulFrameNo, vmdFace.fFactor);
			m_faceAnimationList.add(anim);
		}
		// すでにある表情
		else
		{
			m_faceAnimationList[(itr->second)].AnimationCurve->addKeyFrame(
				vmdFace.ulFrameNo, vmdFace.fFactor);
		}
	}

	// キーフレーム順にソート
	//for (FaceAnimation& anim : m_faceAnimationList)
	//{
	//	anim.AnimationCurve->sortKeyFrame();
	//}
	return true;
}
#endif


namespace detail
{

//==============================================================================
// VmdData
//==============================================================================

Quaternion VmdData::interpolateQuaternionStrict(const MotionFrameData& left, const MotionFrameData& right, float x, float motionpos, float framediff, float* outT)
{
	return Quaternion::slerp(left.quaternion, right.quaternion, interpolateStrict(right.interpolRA, right.interpolRB, x, motionpos, framediff, outT));
}

float VmdData::interpolateStrict(const Point& pointF, const Point& pointF2, float x, float start, double framediff, float* outT)
{
	if (x == 0.f)
	{
		*outT = 0.f;
		return 0.f;
	}
	if (x == 1.f)
	{
		*outT = 1.f;
		return 1.f;
	}
	if (Math::nearEqual(pointF.x, pointF.y) && Math::nearEqual(pointF2.x, pointF2.y))
	{
		*outT = x;
		return x;
	}
	float t = recursiveInterop(pointF.x, pointF2.x, start, 1.f, x, framediff);
	*outT = t;
	return (float)(3.0 * (std::pow(1.0 - (double)t, 2.0) * (double)t * (double)pointF.y + (1.0 - (double)t) * std::pow((double)t, 2.0) * (double)pointF2.y) + std::pow((double)t, 3.0));
}

float VmdData::recursiveInterop(float p2x, float p3x, float min, float max, float x, double framediff)
{
	const float Range = 0.001f;

	float median = (max + min) / 2.0f;
	float num2 = (float)(3.0 * (std::pow(1.0 - (double)median, 2.0) * (double)median * (double)p2x + (1.0 - (double)median) * std::pow((double)median, 2.0) * (double)p3x) + std::pow((double)median, 3.0));
	if (x < num2)
	{
		if (!(framediff < (double)Range) && !(num2 - x < Range) && min != max)
		{
			return recursiveInterop(p2x, p3x, min, median, x, framediff / 2.0);
		}
		return median;
	}
	if (!(framediff < (double)Range) && !(x - num2 < Range) && min != max)
	{
		return recursiveInterop(p2x, p3x, median, max, x, framediff / 2.0);
	}
	return median;
}

//==============================================================================
// VmdFile
//==============================================================================

Ref<VmdData> VmdFile::load(Stream* stream)
{
	LN_CHECK(stream);

	static const float FrameRate = 30.0f;
	static const Point DefaultInterpolA = Point(20.0f / 127.0f, 20.0f / 127.0f);
	static const Point DefaultInterpolB = Point(107.0f / 127.0f, 107.0f / 127.0f);
	static const bool rotateY180 = false;	// MMD モデルは Z- が正面になっていて、モーションもこれを前提としている。それを Z+ に直すかどうか
	static const Matrix adjustMatrix = Matrix::makeRotationY(Math::PI);
	static const uint8_t name_AllParents[] = { 0x91, 0x53, 0x82, 0xc4, 0x82, 0xcc, 0x90, 0x65, 0x00 };	// "全ての親"

	auto vmdData = makeRef<VmdData>();

	BinaryReader reader(stream);
	
	//-----------------------------------------------------
	// check header
	VMD_Header vmdHeader;
	reader.read(&vmdHeader, sizeof(VMD_Header));
	if (strncmp(vmdHeader.szHeader, "Vocaloid Motion Data 0002", 30) != 0)
	{
		return nullptr;
	}

	//-----------------------------------------------------
	// read bone data
	uint32_t allBoneKeyCount = reader.readUInt32();
	for (uint32_t i = 0; i < allBoneKeyCount; ++i)
	{
		VMD_Motion vmdMotion;
		reader.read(&vmdMotion, sizeof(VMD_Motion));

		if (strcmp(vmdMotion.szBoneName, (const char*)name_AllParents) == 0)
			continue;

		// make frame data
		MotionFrameData frame;
		frame.frameTime = static_cast<float>(vmdMotion.ulFrameNo) / FrameRate;
		frame.position = vmdMotion.vec3Position;
		frame.quaternion = vmdMotion.vec4Rotation;
		frame.interpolXA.x = vmdMotion.Interpolation[0] / 127.0f;
		frame.interpolYA.x = vmdMotion.Interpolation[1] / 127.0f;
		frame.interpolZA.x = vmdMotion.Interpolation[2] / 127.0f;
		frame.interpolRA.x = vmdMotion.Interpolation[3] / 127.0f;
		frame.interpolXA.y = vmdMotion.Interpolation[4] / 127.0f;
		frame.interpolYA.y = vmdMotion.Interpolation[5] / 127.0f;
		frame.interpolZA.y = vmdMotion.Interpolation[6] / 127.0f;
		frame.interpolRA.y = vmdMotion.Interpolation[7] / 127.0f;
		frame.interpolXB.x = vmdMotion.Interpolation[8] / 127.0f;
		frame.interpolYB.x = vmdMotion.Interpolation[9] / 127.0f;
		frame.interpolZB.x = vmdMotion.Interpolation[10] / 127.0f;
		frame.interpolRB.x = vmdMotion.Interpolation[11] / 127.0f;
		frame.interpolXB.y = vmdMotion.Interpolation[12] / 127.0f;
		frame.interpolYB.y = vmdMotion.Interpolation[13] / 127.0f;
		frame.interpolZB.y = vmdMotion.Interpolation[14] / 127.0f;
		frame.interpolRB.y = vmdMotion.Interpolation[15] / 127.0f;

		if (rotateY180)
		{
			frame.position.transformCoord(adjustMatrix);
			Vector3 angles = frame.quaternion.toEulerAngles();
			angles.x *= -1;
			angles.z *= -1;
			frame.quaternion = Quaternion::makeFromEulerAngles(angles);
		}
		//if (String(vmdMotion.szBoneName) == u"左腕") {
		//	Vector3 angles = frame.quaternion.toEulerAngles();
		//	angles.z -= Math::PI/6;
		//	frame.quaternion = Quaternion::makeFromEulerAngles(angles);

		//}

		// add frame to track
		auto itr = m_boneTrackMap.find(vmdMotion.szBoneName);
		if (itr == m_boneTrackMap.end())
		{
			MotionFrameTrack track;
			track.boneName = String::fromCString(vmdMotion.szBoneName, -1, TextEncoding::getEncoding(EncodingType::SJIS));
			track.frames.add(frame);
			
			m_boneTrackMap[vmdMotion.szBoneName] = vmdData->MotionData.size();
			vmdData->MotionData.add(track);
		}
		else
		{
			vmdData->MotionData[(itr->second)].frames.add(frame);
		}
		
		// update last time
		vmdData->lastFrameTime = std::max(vmdData->lastFrameTime, frame.frameTime);
	}

	// sort tracks
	for (auto& track : vmdData->MotionData)
	{
		std::stable_sort(track.frames.begin(), track.frames.end(), [](const MotionFrameData& a, const MotionFrameData& b){ return a.frameTime < b.frameTime; });
	}

	//-----------------------------------------------------
	// read morph data
	if (!reader.isEof())
	{
		uint32_t allFaceKeyCount = reader.readUInt32();
		for (uint32_t i = 0; i < allFaceKeyCount; ++i)
		{
			VMD_Face vmdFace;
			reader.read(&vmdFace, sizeof(VMD_Face));

			// make frame data
			MorphFrameData frame;
			frame.frameTime = static_cast<float>(vmdFace.ulFrameNo) / FrameRate;
			frame.weight = vmdFace.fFactor;
			frame.interpolA = DefaultInterpolA;
			frame.interpolB = DefaultInterpolB;

			// add frame to track
			auto itr = m_morphTrackMap.find(vmdFace.szFaceName);
			if (itr == m_morphTrackMap.end())
			{
				MorphFrameTrack track;
				track.morphName = String::fromCString(vmdFace.szFaceName, -1, TextEncoding::getEncoding(EncodingType::SJIS));
				track.frames.add(frame);
				
				m_morphTrackMap[vmdFace.szFaceName] = vmdData->MorphData.size();
				vmdData->MorphData.add(track);
			}
			else
			{
				vmdData->MorphData[(itr->second)].frames.add(frame);
			}
		
			// update last time
			vmdData->lastFrameTime = std::max(vmdData->lastFrameTime, frame.frameTime);
		}
		
		// sort tracks
		for (auto& track : vmdData->MorphData)
		{
			std::stable_sort(track.frames.begin(), track.frames.end(), [](const MorphFrameData& a, const MorphFrameData& b){ return a.frameTime < b.frameTime; });
		}
	}
	
	
	//-----------------------------------------------------
	// read camera data
	
	// skip.
	
	
	//-----------------------------------------------------
	// read light data
	
	// skip.
	
	return vmdData;
}

} // namespace detail
} // namespace ln

