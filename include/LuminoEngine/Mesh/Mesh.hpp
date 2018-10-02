#pragma once
#include "../Graphics/Common.hpp"
#include "../Rendering/Vertex.hpp"

namespace ln {
class VertexBuffer;
class IndexBuffer;
class AbstractMaterial;

enum MeshResourceVertexBufferGroup
{
	Basic = 0,
	BlendWeights = 1,
	AdditionalUVs = 2,
	SdefInfo = 3,
	MmdExtra = 4,
};

// LOD
// Triangle
class MeshResource
{
public:
	void resizeVertexBuffer(int vertexCount);
	void resizeIndexBuffer(int indexCount);

private:
	GraphicsResourceUsage m_usage;
	std::array<Ref<VertexBuffer>, 5> m_vertexBuffers; // sizeof MeshResourceVertexBufferGroup
	Ref<IndexBuffer> m_indexBuffer;
};

// ひとつのメッシュモデルデータ内にいくつかのメッシュノードが含まれているとき、それを名前検索するために使用する。
// 例えば、フィールドのモデルに ビジュアル用のメッシュとコリジョン用のメッシュが含まれている場合、名前検索でコリジョンを取り出して Phyiscs モジュールに渡したりする。
// また、LOD の管理も行う。
class MeshContainer
	: public Object
{

};

class MeshModel
	: public Object
{
public:

private:
	List<Ref<MeshContainer>> m_meshContainers;
	List<Ref<AbstractMaterial>> m_materials;
};

} // namespace ln

