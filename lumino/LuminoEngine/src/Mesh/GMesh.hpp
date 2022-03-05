#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>

namespace ln {
class MeshResource;
namespace detail {
class LinearAllocator;
class GVertex;
class GEdge;
class GLoop;
class GFace;
class GMeshOperations;

/** 頂点 */
struct GVertex
{
	Vector3 position;

	// この頂点を参照ている Edge
	std::vector<GEdge*> linkEdges;
};

/**
 * 稜線 (辺)
 *
 * 形状中の面同士の境界線を表す。
 * 必ず両端点に頂点がある。
 * 主な役割は半稜線をグループ化すること。
 *
 * 検索すると見つかる論文のサンプルなどでは両端の半稜線の参照を持つことがあるが、
 * GMesh の構造としては 1辺が3つ以上の面を構成する場合も想定したいので、複数の半稜線の参照を持っている。
 */
struct GEdge
{
	GVertex* v1;
	GVertex* v2;

	// この頂点を参照ている Loop
	std::vector<GLoop*> linkLoops;
};

/** 
 * 半稜線
 *
 * 前後の Loop や関係する Edge、Face の参照を持つ。
 * 「稜線」と名前がついているが、頂点に関係する情報も持つ。
 * なぜ 法線や UV などを Vertex 側に持たせないかは、サイコロの法線や UV 展開を考えるとわかりやすい。
 * 位置は多くの頂点で共有されるが、法線や UV は共有できないこともある。
 */
struct GLoop
{
	// Structure data
	GVertex*			vertex = nullptr;
	GLoop*				nextLoop = nullptr;
	GLoop*				prevLoop = nullptr;
	GEdge*				edge = nullptr;	// この GLoop が属する GEdge
	GFace*				face = nullptr;	// この GLoop が属する GFace

	// Geometry data
	Vector3				normal;
	Vector2				uv;
	Color				color;

//private:
//	friend class GMeshOperations;
//	Vector3 normal;
};

/** 面
 *
 * 複数の半稜線の連結によって構成される。
 *
 * 面は時計回りを表とする。
 */
struct GFace
{
	GLoop* startLoop = nullptr;	// 開始 Loop。next をたどって、またこの loop まで戻ってきたら1つの面となる。

	template<typename TCallback>
	void foreachLoops(TCallback callback)
	{
		int i = 0;
		GLoop* loop = startLoop;
		if (loop)
		{
			do
			{
				callback(loop, i);
				i++;
				loop = loop->nextLoop;
			} while (loop != startLoop);
		}
	}

	//// 四角形以上は、TriangleStrip と同様に列挙される
	//template<typename TCallback>
	//void foreachTriangles(TCallback callback)
	//{
	//	int i = 0;
	//	GLoop* loopPrev2 = nullptr;
	//	GLoop* loopPrev1 = nullptr;
	//	GLoop* loop = startLoop;
	//	while (loop != startLoop)
	//	{
	//		if (loopPrev2 && loopPrev1 && loop) {
	//			callback(loop, i);
	//			i++;
	//		}
	//		loopPrev2 = loopPrev1;
	//		loopPrev1 = loop;
	//		loop = loop->nextLoop;
	//	}
	//}

	// 四角形以上は、TriangleFan と同様に列挙される
	template<typename TCallback>
	void foreachTriangleFans(TCallback callback)
	{
		int i = 0;
		GLoop* loopPrev1 = nullptr;
		GLoop* loop = startLoop;
		if (loop)
		{
			do
			{
				if (i >= 2 && loopPrev1 && loop) {
					callback(startLoop, loopPrev1, loop);
				}
				i++;
				loopPrev1 = loop;
				loop = loop->nextLoop;
			} while (loop != startLoop);
		}
	}


private:
	friend class GMeshOperations;
	Vector3 normal;
};

/**
 * Editable geometric mesh.
 *
 * 
 */
class GMesh
	: public RefObject
{
public:
	GMesh(MeshManager* manager);
	void setVertexCount(int count);
	void reserveFaceCount(int count);

	GVertex* vertex(int index) const { return m_vertices[index]; }

	const std::vector<GLoop*>& loops() const { return m_loops; }

	// 指定した 2 頂点を結ぶ Edge を作成する (事前に findEdge で重複しないように確認しておくこと)
	GEdge* makeEdge(GVertex* v1, GVertex* v2);

	GLoop* makeLoop(GEdge* parentEdge, GVertex* from, GVertex* next);

	// 指定したインデックスの頂点から構成される面を作成する
	GFace* makeFace(const int* indices, int count);

	// 指定した2頂点を結ぶ Edge を探す
	GEdge* findEdge(GVertex* v1, GVertex* v2) const;


private:
	template<typename T>
	T* newData()
	{
		void* data = m_linearAllocator->allocate(sizeof(GVertex));
		return new (data) T();
	}

	std::vector<GVertex*> m_vertices;
	std::vector<GEdge*> m_edges;
	std::vector<GLoop*> m_loops;
	std::vector<GFace*> m_faces;
	

	Ref<LinearAllocator> m_linearAllocator;

	// Allocator
	//std::vector<std::shared_ptr<GVertex>> m_verticesDataBuffer;
	//std::vector<std::shared_ptr<GFace>> m_verticesDataBuffer;

	friend class GMeshOperations;
};

class GMeshOperations
{
public:
	static void calculateNormals(GMesh* mesh, float smoothRadius);
	static Ref<MeshResource> generateMeshResource(GMesh* mesh);
};

} // namespace detail
} // namespace ln

