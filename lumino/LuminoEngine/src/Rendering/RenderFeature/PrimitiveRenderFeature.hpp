#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../../../../Graphics/src/GraphicsManager.hpp"
#include "../../Mesh/MeshGeneraters/MeshGenerater.hpp"
#include "../RLIs/RLIBatchList.hpp"
#include "../RenderingManager.hpp"

namespace ln {
class MeshResource;
namespace detail {

// 単純なメッシュ形状を描画する。
// MeshRenderFeature が Mesh(VertexBuffer, IndexBuffer) を受け取って描画するのに対し、
// こちらは形状の情報（球なら中心位置と半径）を受け取って描画する。そのためデータサイズを非常に小さく抑えることができる。
// また、スプライトのようなバッファリングによるドローコール削減も狙う。
    /*
     0.4.0 までは CacheBuffer という、内部実装は vector なバッファを使っていたが、
     これだと常に最大使用量のメモリが確保されたままになるので、メモリ効率が良くない。
     LinearAllocator を使うことで平均化できるが、一方こちらはバッファがひとつながりではないので、
     CacheBuffer の時のように頂点バッファデータをキャッシュすることはできなくなる。
     そのため Sprite と同じように、まずは描画情報 (MeshGenerater) をリストで持って、
     flush 時に頂点データ化するという流れで描画を行う。
    */
class MeshGeneraterRenderFeature
	: public RenderFeature
{
public:
	MeshGeneraterRenderFeature();
	~MeshGeneraterRenderFeature();
	void init(RenderingManager* manager);



 //   //void drawMeshGenerater(const MeshGenerater* generator);
 //   template<class TFactory>
	//RequestBatchResult drawMeshGenerater(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, const TFactory& generator)
 //   {
	//	//// TODO: toporogy も RenderStage のパラメータに持っていく
 // //      if (m_lastPrimitiveType.hasValue() && m_lastPrimitiveType != generator.primitiveType()) {
	//	//	submitBatch(context, nullptr);
 // //      }
 // //      m_lastPrimitiveType = generator.primitiveType();

 //       LN_ENQUEUE_RENDER_COMMAND_2(
 //           PrimitiveRenderFeature_drawMeshGenerater, context,
 //           InternalPrimitiveRenderer*, m_internal,
 //           TFactory, generator,
 //           {
 //               m_internal->drawMeshGenerater(&generator);
 //           });

	//	return RequestBatchResult::Staging;
 //   }


    //void drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);


	RequestBatchResult drawMeshGenerater(
		RenderFeatureBatchList* batchList,
		const RLIBatchState& batchState,
		const MeshGenerater* generator);

	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) override;
    //virtual bool drawElementTransformNegate() const override { return true; }

private:
	struct BatchData
	{
		PrimitiveTopology topology;
		int indexOffset;
		int indexCount;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	void resetBatchData();
	void prepareBuffers(int vertexCount, int indexCount);

    //Optional<PrimitiveTopology> m_lastPrimitiveType;
	RenderingManager* m_manager;
    //Ref<InternalPrimitiveRenderer> m_internal;

	Ref<LinearAllocator> m_linearAllocator;
	List<MeshGenerater*> m_generators;
	//PrimitiveTopology m_primitiveType;
	//BatchData m_batchData;

	// RHI
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	int32_t m_indexCount;
};

class PrimitiveRenderFeature
	: public RenderFeature
{
public:
	PrimitiveRenderFeature();
	void init();

	RequestBatchResult drawPrimitive(
		RenderFeatureBatchList* batchList,
		const RLIBatchState& batchState,
		VertexLayout* vertexLayout,
		VertexBuffer* vertexBuffer,
		int startVertex,
		int primitiveCount);

	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) override;

private:
	struct PrimitveData
	{
		Ref<VertexLayout> vertexLayout;
		Ref<VertexBuffer> vertexBuffer;
		int startVertex;
		int primitiveCount;
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

	List<PrimitveData> m_primitives;
	//BatchData m_batchData;
};

} // namespace detail
} // namespace ln

