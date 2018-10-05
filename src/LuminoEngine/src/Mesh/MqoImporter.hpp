
#pragma once
#include <LuminoEngine/Mesh/Mesh.hpp>

namespace ln {
class Stream;
namespace detail {

class MqoParser
{
public:
	MqoParser();

	//Ref<tr::SrMeshModel> import(ModelManager* manager, const PathName& filePath);
	Ref<MeshModel> import(MeshManager* manager, const Path& filePath);

private:
	struct MqoFace
	{
		int				vertexCount;		// 面を構成する頂点数 (3 or 4) 読み込まれた後、四角形は3角形に分割される
		int				vertexIndices[4];	// 面を構成する頂点のインデックス（頂点数分の数が存在）
		int				materialIndex;		// 材質インデックス
		Vector2			uv[4];				// UV値
		uint32_t		colors[4];			// 頂点カラー
	};

	void loadMaterials(StreamReader* reader);
	void loadObject(StreamReader* reader);
	void readVertexChunk(StreamReader* reader);
	void readFaceChunk(StreamReader* reader);
	static void readInts(const StringRef& str, int* values, int valuesCount);
	static void readUInts(const StringRef& str, uint32_t* values, int valuesCount);
	static void readFloats(const StringRef& str, float* values, int valuesCount);

	void visitMaterialChunk();
	void visitMaterial(AbstractMaterial* material);
	void visitObjectChunk(const StringRef& name);
	void visitVertexChunk(int vertexCount);
	void visitVertex(const Vector3& vertex, int index);
	void visitFaceChunk(int faceCount);
	void visitFace(const MqoFace& mqoFace);

	MeshManager*			m_manager;
	Path				m_filePath;
	Path				m_parentDir;

	Ref<MeshModel>	m_model;
	//tr::SrMesh*				m_mesh;
	bool					m_flipZCoord;
};


} // namespace detail
} // namespace ln
