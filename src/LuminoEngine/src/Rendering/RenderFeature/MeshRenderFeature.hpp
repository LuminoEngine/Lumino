#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../../Graphics/RHIs/GraphicsDeviceContext.hpp"
#include "../RenderStage.hpp"

namespace ln {
class MeshResource;
class MeshPrimitive;
class MeshSkeleton;
class InstancedMeshList;
namespace detail {

class MeshRenderFeature	// TODO: MeshSectionRenderFeature
	: public RenderFeature
{
public:
	MeshRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, MeshResource* mesh, int sectionIndex);
    RequestBatchResult drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton);
	RequestBatchResult drawMeshInstanced(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, InstancedMeshList* list);

	RequestBatchResult attemptSubmitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList, bool instanced);
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
		int instanceCount;
		PrimitiveTopology primitiveType;
	};

	struct BatchData
	{
		int offset;
		int count;
		bool instanced;
		detail::SkeletonInstance* skeleton;
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

