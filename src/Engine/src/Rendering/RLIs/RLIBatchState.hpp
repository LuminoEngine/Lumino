#pragma once
#include "../RenderStage.hpp"

namespace ln {
namespace detail {

// RLIMaterial は本当に最終的に使われるマテリアル情報だが、こちらは Batch に付く情報。
// RLIBatchState は上位レイヤーから要求された情報を保持し、複数の renderPass 間で参照される const 情報。
// RLIBatchState は最終的に RenderPass の都合で設定がオーバーライドされることがあり、その結果が RLIMaterial となる。
struct RLIBatchState
{
public:
	Material* material;		// ソースマテリアル。Mesh を描画するときは、Mesh にアタッチされた Material.
	BlendMode blendMode;
	CullMode cullingMode;
	bool depthTestEnabled;
	bool depthWriteEnabled;
	ShadingModel shadingModel;
	PrimitiveTopology primitiveTopology;



	// DrawElement が持っている CombinedWorldTransform への参照。
	// null の場合は Matrix::Identity とみなす。
	// 単純に RenderFeatureBatch のサイズを増やしたくないのでポインタで用意してある。
	// RenderFeatureBatch の寿命は SceneRenderer の内部だけであり、その間は DrawElement(が持っている CombinedWorldTransform) が消えることはない。
	// Sprite や SpriteText など DynamicVertexBuffer を作るものたちは、Vertex を作るときに Transform するので、これは null となる。
	// Mesh (サブセット単位) などはこれに値がセットされる。
	const Matrix* m_worldTransform;

	SubsetInfo m_subsetInfo;
	RenderPass* m_renderPass;




	RLIBatchState();
	void reset();
	bool equals(const RLIBatchState* other) const;

	// TODO: 移行用一時処理
	void mergeFrom(const GeometryStageParameters* geometoryParams, Material* finalMaterial);
};

} // namespace detail
} // namespace ln

