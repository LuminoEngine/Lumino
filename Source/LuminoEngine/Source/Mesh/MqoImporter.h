
#pragma once
#include <Lumino/Mesh/Mesh.h>

LN_NAMESPACE_BEGIN
class Stream;
namespace detail {
class ModelManager;

class MqoImporter
{
public:
	MqoImporter();
	virtual ~MqoImporter() = default;

	RefPtr<StaticMeshModel> Import(ModelManager* manager, const PathName& parentDir, Stream* stream);

private:
	struct MqoFace
	{
		int				vertexCount;		// 面を構成する頂点数 (3 or 4)
		int				vertexIndices[4];	// 面を構成する頂点のインデックス（頂点数分の数が存在）
		int				materialIndex;		// 材質インデックス
		Vector2			uv[4];				// UV値 (Vector2[4])
		uint32_t		colors[4];			// 頂点カラー
		Vector3			normal;				// 面法線
		Vector3			vertexNormals[3];			// 頂点法線 (これを使う時点では必ず三角形になっている)
	};

	struct MqoFacePointRef
	{
		//MqoFace*		face;
		int				faceIndex;
		int				pointIndex;
		int				meshVertexNumber;	// 頂点バッファに落とすときの頂点番号
	};

	struct MqoVertex
	{
		Vector3					position;
		//List<int>				referencedFaceIndices;
		List<MqoFacePointRef>	referenced;	// 頂点を参照している MqoFace の1点 (完全に同一要素を持つ点のうち1つ)
	};

	//struct VertexSource
	//{
	//	//Vertex*		vertex;				// m_mqoVertexList の要素を指す
	//	MqoFace*	face;					// m_mqoFaceList の要素を指す
	//	int			vertexIndexInFace;		// face 内の何番目の頂点情報を指すか
	//};

	void LoadMaterials(StreamReader* reader);
	void LoadObject(StreamReader* reader);
	void ReadInts(const StringRef& str, int* values, int valuesCount);
	void ReadUInts(const StringRef& str, uint32_t* values, int valuesCount);
	void ReadFloats(const StringRef& str, float* values, int valuesCount);
	void ReadVertexChunk(StreamReader* reader);
	void ReadFaceChunk(StreamReader* reader);
	void InitMqoFace(MqoFace* face);
	int AddFaceIndices(MeshResource* mesh, int startIndexBufferIndex, int faceIndex);
	int PutVertexSource(int faceIndex, int pointIndex);
	bool EqualsFacePoint(const MqoFace* face1, int pointIndex1, const MqoFace* face2, int pointIndex2);

	ModelManager*			m_manager;
	PathName				m_filePath;
	PathName				m_parentDir;
	List<MqoVertex>			m_mqoVertexList;
	List<MqoFace>			m_mqoFaceList;
	RefPtr<StaticMeshModel>	m_model;
	int						m_meshIndexCount;
	int						m_meshVertexCount;
};

} // namespace detail
LN_NAMESPACE_END
