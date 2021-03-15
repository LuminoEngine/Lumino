
#pragma once

namespace ln {
class DiagnosticsManager;
class AnimationClip;

namespace detail {

class AsciiLineReader
{
public:
	struct Token
	{
		const char* begin;
		int length;
		
		bool is(const char* str, int len) const
		{
			return (len == length && strncmp(begin, str, len) == 0);
		}

		float toFloat() const
		{
			return StringHelper::toFloat(begin, length);
		}

		int toInt() const
		{
			return StringHelper::toInt32(begin, length);
		}
	};

	AsciiLineReader();
	void reset(Stream* stream);
	bool readLine();
	bool readToken();
	const char* lineHead() const { return m_lineHead; }
	const char* lineEnd() const { return m_lineHead + m_lineLength; }
	int lineLength() const { return m_lineLength; }
	const Token& token() const { return m_lineTokens[m_lineTokenIndex]; }
	const Token& token(int index) const { return m_lineTokens[index]; }
	int tokenCount() const { return static_cast<int>(m_lineTokens.size()); }


	bool startWith(const char* str, int len) const {
		return (len <= m_lineLength || strcmp(m_lineHead, str) == 0);
	}

private:
	bool readLineRaw();
	bool readBuffer();
	void splitLineTokens();

	Stream* m_stream;
	std::array<char, 1024> m_buffer;
	size_t m_bufferPos;
	size_t m_bufferLength;

	std::vector<char> m_lineCache;
	const char* m_lineHead;
	int m_lineLength;

	std::vector<Token> m_lineTokens;
	int m_lineTokenIndex;
};
	
/*
	[2020/10/10] Mixamo と 自作モデルでエクスポートした姿勢が一致しない？
	----------
	Mixamo の Unity 用モーション (FBX) は Blender でインポートしてみると
	Armature などオブジェクト姿勢は RotatonX=90 になっている。
	Blender は Z-Up R-Hand なので、この修正だけで一般的な Y-Up R-Hand に一致する。

	Blender の BVH エクスポータは Blender の座標系でそのままエクスポートする。
	そのためこの修正値が必要。

	これがない自作のモデルは、Y-Up R-Hand である bvh_viewer.exe で見ると前に倒れて見える。	
*/
class BvhImporter
{
public:
	BvhImporter(AssetManager* assetManager, DiagnosticsManager* diag);
	bool import(AnimationClip* clip, const AssetPath& assetPath, const AnimationClipImportSettings* settings);

private:
	enum ChannelEnum
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};

	// TODO: 結局 index しか要らなくなったので class 化不要
	struct Channel
	{
		int index;
		//int joint;
		//ChannelEnum type;
	};

	struct Joint
	{
		std::string name;
		int index;
		//int parent;
		//std::vector<int> children;
		//std::vector<int> channels;
		std::array<int16_t, 6> channels;	// 要素番号は ChannelEnum
		Vector3 offset;		// プレビュー用モデルの表示などで利用できる参考情報
		Vector3 site;
		bool hasSite;
		bool isRoot;
	};

	bool readHierarchy();
	bool readMotion();
	Joint* currentJoint() const { return m_joints.back().get(); }
	void setFrameData(int frame, int ch, float v) { m_frameData[frame * m_channels.size() + ch] = v; }
	float rameData(int frame, int ch) { return m_frameData[frame * m_channels.size() + ch]; }
	static HumanoidBones mapHumanoidBonesMixamoUnity(const String& name);

	AssetManager* m_assetManager;
	DiagnosticsManager* m_diag;
	AsciiLineReader m_reader;

	std::vector<std::shared_ptr<Channel>> m_channels;
	std::vector<std::shared_ptr<Joint>> m_joints;
	std::vector<int> m_jointStack;
	int m_rootJointIndex = -1;

	int m_frames;
	float m_frameTime;
	std::vector<float> m_frameData;

	float m_minOffsetY;
	float m_scale = 1.0f;

	bool m_flipZ;
	bool m_flipX;
};

} // namespace detail
} // namespace ln
