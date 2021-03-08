#pragma once
#include "../RenderStage.hpp"

namespace ln {
namespace detail {

// RLIMaterial は本当に最終的に使われるマテリアル情報だが、こちらは Batch に付く情報。
// RLIBatchMaterial は上位レイヤーから要求された情報を保持し、複数の renderPass 間で参照される const 情報。
// RLIBatchMaterial は最終的に RenderPass の都合で設定がオーバーライドされることがあり、その結果が RLIMaterial となる。
struct RLIBatchMaterial
{
public:
	Material* material;		// ソースマテリアル。Mesh を描画するときは、Mesh にアタッチされた Material.
	BlendMode blendMode;
	CullMode cullingMode;
	bool depthTestEnabled;
	bool depthWriteEnabled;
	ShadingModel shadingModel;
	PrimitiveTopology primitiveTopology;

	RLIBatchMaterial();
	void reset();
	bool equals(const RLIBatchMaterial* other) const;

	// TODO: 移行用一時処理
	void mergeFrom(const GeometryStageParameters* geometoryParams, Material* finalMaterial);
};

} // namespace detail
} // namespace ln

