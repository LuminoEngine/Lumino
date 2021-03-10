
#pragma once
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>

namespace ln {
class Stream;
class DiagnosticsManager;
namespace detail {
class GMesh;

/*
	Mqo ファイル Note
	--------
	- Z+ を手前とする右手座標系。
	- 面は時計回りが表となっている。

	四角形
	--------
	Vertex:
	0--1
	|  |
	2--3
	Index: 0 1 3 2
*/
class MqoParser
{
public:

	//Ref<tr::SrMeshModel> import(ModelManager* manager, const PathName& filePath);
	//Ref<MeshModel> import(MeshManager* manager, const Path& filePath);

protected:
	MqoParser();

	struct MqoFace
	{
		int				vertexCount;		// 面を構成する頂点数 (3 or 4) 読み込まれた後、四角形は3角形に分割される
		int				vertexIndices[4];	// 面を構成する頂点のインデックス（頂点数分の数が存在）
		int				materialIndex;		// 材質インデックス
		Vector2			uv[4];				// UV値
		uint32_t		colors[4];			// 頂点カラー
	};

	void parse(MeshManager* manager, const Path& filePath, DiagnosticsManager* diag);
	MeshManager* manager() const { return m_manager; }
	DiagnosticsManager* diag() const { return m_diag; }

	virtual void visitMaterialChunk() = 0;
	virtual void visitMaterial(Material* material) = 0;
	virtual void visitObjectChunk(const StringRef& name) = 0;
	virtual void visitVertexChunk(int vertexCount) = 0;
	virtual void visitVertex(const Vector3& vertex, int index) = 0;
	virtual void visitFaceChunk(int faceCount) = 0;
	virtual void visitFace(const MqoFace& mqoFace) = 0;

private:

	void loadMaterials(StreamReader* reader);
	void loadObject(StreamReader* reader);
	void readVertexChunk(StreamReader* reader);
	void readFaceChunk(StreamReader* reader);
	static void readInts(const StringRef& str, int* values, int valuesCount);
	static void readUInts(const StringRef& str, uint32_t* values, int valuesCount);
	static void readFloats(const StringRef& str, float* values, int valuesCount);


	MeshManager*			m_manager;
	DiagnosticsManager* m_diag;

	Path				m_filePath;
	Path				m_parentDir;

};

class MqoImporter
	: public MqoParser
{
public:
	MqoImporter();
	Ref<MeshModel> import(MeshManager* manager, const Path& filePath, DiagnosticsManager* diag);

protected:
	virtual void visitMaterialChunk() override;
	virtual void visitMaterial(Material* material) override;
	virtual void visitObjectChunk(const StringRef& name) override;
	virtual void visitVertexChunk(int vertexCount) override;
	virtual void visitVertex(const Vector3& vertex, int index) override;
	virtual void visitFaceChunk(int faceCount) override;
	virtual void visitFace(const MqoFace& mqoFace) override;

private:
	Ref<MeshModel>	m_model;
	MeshContainer* m_meshContainer;
	Ref<GMesh> m_mesh;
	//MeshResource* m_mesh;
	//tr::SrMesh*				m_mesh;
	bool					m_flipZCoord;

	//std::vector<Vector3> m_vertices;
};

} // namespace detail
} // namespace ln
