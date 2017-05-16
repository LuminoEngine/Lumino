
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
	struct MqoFacePointGroup;

	// 面上の点
	struct MqoFacePoint
	{
		int					vertexIndex;			// オリジナルの頂点番号
		Vector3				normal;					// 頂点法線
		MqoFacePointGroup*	group = nullptr;		// 面上の点がどのグループに属しているか
	};

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
		//Vector3			vertexNormals[3];			// 頂点法線 (これを使う時点では必ず三角形になっている)
		//MqoFacePointGroup*		groups[3] = {};			// 面上の点がどのグループに属しているか
		MqoFacePoint	points[3];
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
		//MqoFace*				face;
		int						faceIndex;
		int						pointIndex;
		int						meshVertexNumber;		// 頂点バッファに落とすときの頂点番号

		// MqoFacePointGroup で使われる情報
		//MqoFacePointRef*		next = nullptr;			// リスト構造の次のノード
	};	

	// Edge 検索用の参考情報
	struct MqoFaceRef
	{
		MqoFace*				face = nullptr;			// 対応する MqoFace
		MqoFaceRef*				next = nullptr;			// リスト構造の次のノード
	};

	struct MqoEdge
	{
		MqoFace*				ownerFace = nullptr;	// この Edge を持つ面
		MqoFacePoint*			point0 = nullptr;
		MqoFacePoint*			point1 = nullptr;
		//int						index0 = 0;				// MqoFace 上の始点の頂点インデックス (0-2)
		//int						index1 = 0;				// MqoFace 上の終点の頂点インデックス (0-2)
		MqoEdge*				adjacent = nullptr;			// 隣接辺 (単方向参照)
		MqoFacePoint*			adjacentPoint0 = nullptr;	// point0 に対する隣接辺上の point
		MqoFacePoint*			adjacentPoint1 = nullptr;	// point1 に対する隣接辺上の point
	};

	struct MqoFacePointRef2
	{
		MqoFace*				face;
		int						pointIndex;
		MqoFacePointRef*		next = nullptr;			// リスト構造の次のノード
	};

	// スムージングの判定で、法線を共有する Face 上の点をまとめる (関係する頂点が1つしかなくてもグループを作る)
	struct MqoFacePointGroup
	{
		//MqoFacePointRef*		points = nullptr;		// 点のリスト
		int						vertexIndex;	// 頂点番号
		Vector3					normal;					// スムージングされた法線
		int						pointsCount = 0;
		int						outputVertexIndex = -1;	// 最終的な頂点バッファに出力した頂点番号 (0 以上になることで、既に出力されていることも表す)
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
	void MakeMqoFacePointGroup(MqoEdge* edge);

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

	List<MqoFaceRef>				m_mqoFaceRefBuffer;
	List<MqoEdge>					m_mqoEdgeBuffer;
	CacheBuffer<MqoFacePointRef2>	m_mqoFacePointRefBuffer_ForGroup;
	CacheBuffer<MqoFacePointGroup>	m_mqoFacePointGroupBuffer;
};

} // namespace detail
LN_NAMESPACE_END
