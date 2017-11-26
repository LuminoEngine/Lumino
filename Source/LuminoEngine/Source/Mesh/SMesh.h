
#pragma once

LN_NAMESPACE_BEGIN
class CommonMaterial;

namespace tr {

/*

Half-Edge 構造の基本要素
--------------------
### 頂点
形状中の頂点を表す。
- 頂点位置ベクトル (必須)
- 頂自身を始点とする0個以上の半稜線 (適宜)

### 稜線
形状中の面同士の境界線を表す。（いわゆる「辺」）
必ず両端点に頂点があり、それぞれを始点とする半稜線を 1 個ずつ持つ。
- 両側の半稜線 (必須)
描画プリミティブを作る上では重要な情報ではない。

### 半稜線
稜線に 2 個ずつ属している要素で、稜線	の端点のうちの片方を始点とする。
- 親稜線 (必須)
- 始点頂点 (必須)
- 前後の半稜線 (必須)
- 属する面 (適宜)

### 面
複数の半稜線の連結によって構成される（半稜線のリスト）
- 構成する最初の半稜線 1 個 (必須)

Lumino の追加要素
--------------------
### 頂点

### 稜線

### 半稜線

### 面
- マテリアル

描画
--------------------
面を描画するには、面が持つ1つの半稜線を始点として連続する稜線をたどる。
始点まで戻ってきたら面が完成する。

*/

class SrVertex;
class SrEdge;
class SrLoop;
class SrFace;
class SrMesh;
class SrMeshOperation;

/** 頂点情報 */
class SrVertex
	: public Object
{
public:
	Vector3				position;
	//List<SrLoop*>	refHalfEdges;

	const List<SrEdge*>& getLinkEdges() const { return m_linkEdges; }

private:
	void setPosition(const Vector3& pos) { position = pos; }
	void addLinkEdge(SrEdge* edge) { m_linkEdges.add(edge); }

	List<SrEdge*>	m_linkEdges;

	friend class SrMesh;
	friend class SrMeshOperation;
};

/** 稜線情報 */
class SrEdge
	: public Object
{
public:
	std::array<SrVertex*, 2>	m_vertices;

	const List<SrFace*>& getLinkFaces() const { return m_linkFaces; }

	const Vector3& getNormal() const { return m_normal; }

private:
	void addLinkFace(SrFace* face);
	void setNormal(const Vector3& n) { m_normal = n; }

	List<SrFace*>	m_linkFaces;
	Vector3			m_normal;

	friend class SrMesh;
};

/** 半稜線情報 */
class SrLoop
	: public Object
{
public:
	SrVertex*			vertex;
	SrLoop*				nextLoop = nullptr;
	SrLoop*				prevLoop = nullptr;
	SrEdge*				edge;	// 必須
	SrFace*				face;	// nullptr である場合、まだ面は張られていない

	Vector3				normal;
	Vector2				uv;
	Color				color;

	//const List<SrEdge*>& getLinkEdges() const { return m_linkEdges; }

private:
	//void addLinkEdge(SrEdge* edge) { m_linkEdges.add(edge); }

	//List<SrEdge*>	m_linkEdges;

	friend class SrMesh;
};

/** 面情報 */
class SrFace
	: public Object
{
public:
	List<SrLoop*>	m_loops;

	CommonMaterial*		m_material;

public:
	const Vector3& getNormal() const { return m_normal; }

	const List<SrLoop*>& getLoops() const { return m_loops; }

private:
	void setNormal(const Vector3& n) { m_normal = n; }

	Vector3	m_normal;

	friend class SrMesh;
};


/** メッシュ */
class SrMesh
	: public Object
{
public:
	void addVertices(int count);
	SrVertex* getVertex(int index);


	SrEdge* makeEdge(SrVertex* v1, SrVertex* v2);
	SrLoop* makeLoop(SrEdge* edge, SrVertex* from, SrVertex* next);
	SrFace* makeFace(const int* indices, int count);

	/** 2つの頂点を結んでいる辺を検索する */
	SrEdge* findEdge(SrVertex* v1, SrVertex* v2);

	void calculateNormals(float smoothRadius = 0.0f);

	Ref<MeshResource> generateMeshResource();

private:
	List<Ref<SrVertex>>	m_vertices;
	List<Ref<SrFace>>	m_faces;
	List<Ref<SrEdge>>	m_edges;
	List<Ref<SrLoop>>	m_loops;
};

/** メッシュ */
class SrMeshModel
	: public Object
{
public:
	List<Ref<CommonMaterial>>	materials;

public:
	SrMesh* addMesh();
	void addMaterial(CommonMaterial* material);

	CommonMaterial* getMaterial(int index);

	void calculateNormals();

	Ref<StaticMeshModel> generateStaticMeshModel();

private:
	List<Ref<SrMesh>>	m_meshes;
};

/** メッシュ編集 */
class SrMeshOperation
{
public:
	static void moveVertex(SrVertex* v, const Vector3& pos);
};

} // namespace tr
LN_NAMESPACE_END
