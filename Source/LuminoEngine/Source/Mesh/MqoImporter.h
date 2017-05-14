
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
	struct MqoFacePointRef;
	struct MqoFaceRef;
	struct MqoEdge;

	struct MqoFace
	{
		// Mqo data
		int				vertexCount;		// 面を構成する頂点数 (3 or 4) 読み込まれた後、四角形は3角形に分割される
		int				vertexIndices[4];	// 面を構成する頂点のインデックス（頂点数分の数が存在）
		int				materialIndex;		// 材質インデックス
		Vector2			uv[4];				// UV値 (Vector2[4])
		uint32_t		colors[4];			// 頂点カラー

		// Work data
		Vector3			normal;				// 面法線
		//
		MqoEdge*		edges[3];			// 辺
		Vector3			vertexNormals[3];			// 頂点法線 (これを使う時点では必ず三角形になっている)
	};

	struct MqoVertex
	{
		// Mqo data
		Vector3					position;
		List<MqoFacePointRef>	referenced;	// 頂点を参照している MqoFace の1点 (完全に同一要素を持つ点のうち1つ)

		// Work data
		MqoFaceRef*				faces = nullptr;	// この頂点を参照している面の LinkedList の先頭ノード
	};

	struct MqoFacePointRef
	{
		//MqoFace*		face;
		int				faceIndex;
		int				pointIndex;
		int				meshVertexNumber;	// 頂点バッファに落とすときの頂点番号
	};

	// Edge 検索用の参考情報
	struct MqoFaceRef
	{
		MqoFace*				face = nullptr;		// 対応する MqoFace
		MqoFaceRef*				next = nullptr;		// 次のノード
	};

	struct MqoEdge
	{
		MqoFace*				ownerFace = nullptr;	// この Edge を持つ面
		int						index0 = 0;				// MqoFace 上の始点の頂点インデックス (0-2)
		int						index1 = 0;				// MqoFace 上の終点の頂点インデックス (0-2)
		MqoEdge*				adjacent = nullptr;		// 隣接辺 (単方向参照)
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
	void ReadFaceChunk(StreamReader* reader, int vertexIndexOffset);

	void MakeMqoFaceRefsAndEdge();
	void MakeMqoFacePointNormals();

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
	bool					m_flipZCoord;

	List<MqoFaceRef>		m_mqoFaceRefBuffer;
	List<MqoEdge>			m_mqoEdgeBuffer;
};

/*
	- Edge を求める。基本的に Edge 単位で考える。
	- Edge は接続相手の Edge を覚えておく。単一方向参照。
	- 相手側との法線差から、スムージングされた法線を求めて頂点ごとに覚えておく。覚えておくのは MqoFace 側。
*/

} // namespace detail
LN_NAMESPACE_END
