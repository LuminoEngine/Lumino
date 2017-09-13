
#pragma once

LN_NAMESPACE_BEGIN
class Material;

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
class SrHalfEdge;
class SrFace;
class SrMeshOperation;

/** 頂点情報 */
class SrVertex
	: public Object
{
public:
	Vector3				position;
	List<SrHalfEdge*>	refHalfEdges;

private:
	void setPosition(const Vector3& pos) { position = pos; }

	friend class SrMeshOperation;
};

/** 稜線情報 */
class SrEdge
	: public Object
{
public:
	SrHalfEdge*			leftHalfEdge;
	SrHalfEdge*			rightHalfEdge;
};

/** 半稜線情報 */
class SrHalfEdge
	: public Object
{
public:
	SrVertex*			vertex;
	SrHalfEdge*			nextHalfEdge;
	SrHalfEdge*			prevHalfEdge;
	SrEdge*				parentEdge;
	SrFace*				parentLoop;
};

/** 面情報 */
class SrFace
	: public Object
{
public:
	SrHalfEdge*			firstHalfEdge;
};

/** 面の角情報 */
class SrFaceLoop
	: public Object
{
public:
	SrFace*				parentFace;
	Vector3				uv;
	Color				color;
};

/** メッシュ */
class SrMesh
	: public Object
{
public:
	void addVertices(int count);
	SrVertex* getVertex(int index);

private:
	List<Ref<SrVertex>>	m_vertices;
};

/** メッシュ */
class SrMeshModel
	: public Object
{
public:
	List<Ref<Material>>	materials;

public:
	SrMesh* addMesh();
	void addMaterial(Material* material);

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
