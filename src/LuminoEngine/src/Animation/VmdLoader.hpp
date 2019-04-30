
#pragma once
#include <map>
#include <unordered_map>

namespace ln {

#if 0
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
		Ref<VMDBezierSQTTransformAnimation2>	AnimationCurve;
	};
	
	struct FaceAnimation
	{
		String TargetFaceName;
		Ref<FloatAnimationCurve>	AnimationCurve;
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
	// こちらの検索キーは std::string。VMD ファイルから読み取ったキー名をそのまま比較できるようにするため。
    typedef std::map<std::string, int>	BoneAnimationIndexMap;
	typedef std::pair<std::string, int>	BoneAnimationIndexPair;
	typedef std::map<std::string, int>	FaceAnimationIndexMap;
	typedef std::pair<std::string, int>	FaceAnimationIndexPair;
	
	double					m_lastFramePosition;
	BoneAnimationList		m_boneAnimationList;
	BoneAnimationIndexMap	m_boneAnimationIndexMap;
	FaceAnimationList		m_faceAnimationList;
	FaceAnimationIndexMap   m_faceAnimationIndexMap;
};

#endif













namespace detail
{

struct MotionFrameData
{
	float frameTime;		// sec
	Vector3 position;
	Quaternion quaternion;
	Point interpolXA;		// 0.0-1.0
	Point interpolXB;
	Point interpolYA;
	Point interpolYB;
	Point interpolZA;
	Point interpolZB;
	Point interpolRA;
	Point interpolRB;
};

struct MorphFrameData
{
	float frameTime;		// sec
	float weight;
	Point interpolA;
	Point interpolB;
};

struct MotionFrameTrack
{
	String boneName;
	List<MotionFrameData> frames;
};
	
struct MorphFrameTrack
{
	String morphName;
	List<MorphFrameData> frames;
};

class VmdData
	: public RefObject
{
public:
	float lastFrameTime;
	List<MotionFrameTrack>	MotionData;
	List<MorphFrameTrack>	MorphData;
	//List<CameraFrameData> CameraData;
	//List<CameraPropertyData> CameraPropertyData;
	//List<LightFrameData> LightData;

	static Quaternion interpolateQuaternionStrict(const MotionFrameData& left, const MotionFrameData& right, float x, float motionpos, float framediff, float* outT);
	static float interpolateStrict(const Point& _p2, const Point& _p3, float x, float start, double framediff, float* t);

private:
	static float recursiveInterop(float p2x, float p3x, float min, float max, float x, double framediff);
};

class VmdFile
{
public:
#pragma pack(push, 1)
	
	struct VMD_Header
	{
		char szHeader[30];		// "Vocaloid Motion Data 0002"
		char szModelName[20];	// 対象モデル名
	};
	
	struct VMD_Motion
	{
		char	szBoneName[15];			// ボーン名

		unsigned long	ulFrameNo;		// フレーム番号

		Vector3	vec3Position;			// 位置
		Quaternion	vec4Rotation;		// 回転(クォータニオン)

		//byte_t	cInterpolationX[16];	// 補間情報 X軸移動 (0-127)
		//byte_t	cInterpolationY[16];	// 補間情報 Y軸移動 (0-127)
		//byte_t	cInterpolationZ[16];	// 補間情報 Z軸移動 (0-127)
		//byte_t	cInterpolationRot[16];	// 補間情報 回転 (0-127)
		byte_t	Interpolation[4*4*4];		// 補間情報 (先頭から 16byte だけ使用)
	};

	struct VMD_Face
	{
		char			szFaceName[15];	// 表情名
		unsigned long	ulFrameNo;		// フレーム番号
		float			fFactor;		// ブレンド率
	};
	
#pragma pack( pop )
	
	Ref<VmdData> load(Stream* stream);
	
private:
	std::unordered_map<std::string, int> m_boneTrackMap;	// <boneName, trackIndex>
	std::unordered_map<std::string, int> m_morphTrackMap;	// <morphName, trackIndex>
};



} // namespace detail
} // namespace ln
