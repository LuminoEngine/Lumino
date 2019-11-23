#pragma once
//#include <LuminoCore/Base/EnumFlags.hpp>
//#include <LuminoEngine/Graphics/GeometryStructs.hpp>
//#include <LuminoEngine/Graphics/ColorStructs.hpp>
//#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>
//#include <LuminoEngine/Rendering/Vertex.hpp>
//#include "../Graphics/GraphicsDeviceContext.hpp"
#include "RenderStage.hpp"

namespace ln {
class MeshResource;
namespace detail {

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class MeshRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	MeshRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("MeshRenderFeatureStageParameters"))
	{
	}

	virtual bool equals(const RenderFeatureStageParameters* other) override
	{
		if (typeId() != other->typeId()) return false;
		if (this == other) return true;
		return true;
	}

	virtual void copyTo(RenderFeatureStageParameters* params) override
	{
		LN_CHECK(typeId() == params->typeId());
	}

private:
};

class MeshRenderFeature	// TODO: MeshSectionRenderFeature
	: public RenderFeature
{
public:
	MeshRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, MeshResource* mesh, int sectionIndex);

	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;

private:
	struct DrawMeshData
	{
		Ref<VertexLayout> vertexLayout;
		Ref<VertexBuffer> vertexBuffers[MaxVertexStreams];
		int vertexBuffersCount;
		Ref<IndexBuffer> indexBuffer;
		int startIndex;
		int primitiveCount;
		PrimitiveTopology primitiveType;
	};

	struct BatchData
	{
		int offset;
		int count;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	std::vector<DrawMeshData> m_meshes;
	BatchData m_batchData;
};

} // namespace detail
} // namespace ln

