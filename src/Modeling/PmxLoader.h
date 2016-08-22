
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class BinaryReader;
class Texture2D;
class PmxSkinnedMeshResource;
namespace detail { class ModelManager; }

class PmxLoader
{
public:

#pragma pack(push, 1)

	enum PMX_Encode
	{
		PMX_Encode_UTF16	= 0,
		PMX_Encode_UTF8		= 1,
	};

	struct PMX_Header
	{
		char	Magic[4];		// "PMX " (PMX1.0は"Pmx ")
		float	Version;		// PMDバージョン番号
		byte_t	DataSize;		// 後続するデータ列のバイトサイズ  PMX2.0は 8 で固定
		byte_t	Data[8];		// [0] - エンコード方式  | 0:UTF16 1:UTF8
								// [1] - 追加UV数 	| 0～4 詳細は頂点参照
								// [2] - 頂点Indexサイズ | 1,2,4 のいずれか
								// [3] - テクスチャIndexサイズ | 1,2,4 のいずれか
								// [4] - 材質Indexサイズ | 1,2,4 のいずれか
								// [5] - ボーンIndexサイズ | 1,2,4 のいずれか
								// [6] - モーフIndexサイズ | 1,2,4 のいずれか
								// [7] - 剛体Indexサイズ | 1,2,4 のいずれか
	};

#pragma pack( pop )

public:
	PmxLoader();
	~PmxLoader();

	RefPtr<PmxSkinnedMeshResource> Load(detail::ModelManager* manager, Stream* stream, const PathName& baseDir, bool isDynamic, ModelCreationFlag flags);

private:
	PMX_Encode getEncode() { return (PMX_Encode)m_pmxHeader.Data[0]; }
	int getAdditionalUVCount() { return m_pmxHeader.Data[1]; }
	int getVertexIndexSize() { return m_pmxHeader.Data[2]; }
	int getTextureIndexSize() { return m_pmxHeader.Data[3]; }
	int getMaterialIndexSize() { return m_pmxHeader.Data[4]; }
	int getBoneIndexSize() { return m_pmxHeader.Data[5]; }
	int getMorphIndexSize() { return m_pmxHeader.Data[6]; }
	int getRigidBodyIndexSize() { return m_pmxHeader.Data[7]; }

private:
	void LoadModelInfo(BinaryReader* reader);
	void LoadVertices(BinaryReader* reader);
	void LoadIndices(BinaryReader* reader);
	void LoadTextureTable(BinaryReader* reader, const PathName& baseDir);
	void LoadMaterials(BinaryReader* reader);
	void LoadBones(BinaryReader* reader);
	void LoadMorphs(BinaryReader* reader);
	void LoadDisplayFrame(BinaryReader* reader);
	void LoadRigidBodys(BinaryReader* reader);
	void LoadJoints(BinaryReader* reader);

	String ReadString(BinaryReader* reader);
	void CalcSDEFCorrection();

private:
	detail::ModelManager*			m_manager;
	bool							m_isDynamic;
	ModelCreationFlag				m_flags;
	RefPtr<PmxSkinnedMeshResource>	m_modelCore;		///< 最終出力
	PMX_Header						m_pmxHeader;
	Array<RefPtr<Texture>>			m_textureTable;
	ByteBuffer						m_tmpBuffer;
	bool							m_hasSDEF;
};

LN_NAMESPACE_END
