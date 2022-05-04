#pragma once
#include <LuminoGraphics/Rendering/RenderFeature.hpp>
#include "../RLIs/RLIBatchList.hpp"

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

	RequestBatchResult drawMesh(RenderFeatureBatchList* batchList, const RLIBatchState& batchState, GraphicsCommandList* context, MeshResource* mesh, int sectionIndex);
    RequestBatchResult drawMesh(RenderFeatureBatchList* batchList, const RLIBatchState& batchState, GraphicsCommandList* context, MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton, detail::MorphInstance* morph);
	RequestBatchResult drawMeshInstanced(RenderFeatureBatchList* batchList, const RLIBatchState& batchState, GraphicsCommandList* context, InstancedMeshList* list);

	RequestBatchResult attemptSubmitBatch(GraphicsCommandList* context, RenderFeatureBatchList* batchList, bool instanced);
	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsCommandList* context, RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) override;

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
		int offset;	// m_meshDrawList の index
		int count;	// m_meshDrawList の index から、何個描くか
		//bool instanced;
		detail::SkeletonInstance* skeleton;
		detail::MorphInstance* morph;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	Batch* acquireBatch(RenderFeatureBatchList* batchList, const RLIBatchState& batchState);

	std::vector<DrawMeshData> m_drawList;
};

} // namespace detail
} // namespace ln

