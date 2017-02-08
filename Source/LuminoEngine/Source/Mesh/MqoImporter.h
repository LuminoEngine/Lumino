
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
		int			vertexCount;		// 面を構成する頂点数 (3 or 4)
		int			vertexIndices[4];	// 面を構成する頂点のインデックス（頂点数分の数が存在）
		int			materialIndex;		// 材質インデックス
		float		uv[8];				// UV値 (Vector2[4])
		uint32_t	colors[4];			// 頂点カラー
	};

	void LoadMaterials(StreamReader* reader);
	void LoadObject(StreamReader* reader);
	void ReadInts(const StringRef& str, int* values, int valuesCount);
	void ReadUInts(const StringRef& str, uint32_t* values, int valuesCount);
	void ReadFloats(const StringRef& str, float* values, int valuesCount);
	void ReadVertexChunk(StreamReader* reader);
	void ReadFaceChunk(StreamReader* reader);

	ModelManager*	m_manager;
	PathName		m_filePath;
	PathName		m_parentDir;
	List<Vector3>	m_mqoVertexList;
	List<MqoFace>	m_mqoFaceList;
};

} // namespace detail
LN_NAMESPACE_END
