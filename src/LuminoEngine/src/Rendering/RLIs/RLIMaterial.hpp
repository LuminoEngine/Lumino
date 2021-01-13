#pragma once
#include "../RenderStage.hpp"

namespace ln {
namespace detail {

// Material, RenderingContext(VisualNode), RenderFeature, SceneRenderPass のマージ結果。
//
// 優先度は SceneRenderPass > RenderFeature > RenderingContext (> VisualNode) > Material
// DrawElementList の構築時点では RenderingContext と Material のマージ結果を保持し、
// その後 SceneRenderer の中で、Batch リスト構築時に RenderFeature、Pass 描画時に SceneRenderPass の分をマージして、最終的なマテリアルとする。
//
// このような実装に至った背景
// ----------
// 以前は RenderState は DrawElementList の構築時点で確定すると思っていたが、
// RenderFeature の都合 (Instancing 使いたい、シャドウマップ作るときは両面カリングにしたい、等) で、
// より低いレイヤーでも変更する必要が出てきた。
// (特にシャドウマップは R32 フォーマットにしているが、適切に RenderState を指定しないと Vulkan の検証レイヤーに指摘される)
struct RLIMaterial	// 旧: GeometryStageParameters
{
public:
	Material* material;		// ソースマテリアル。Mesh を描画するときは、Mesh にアタッチされた Material.
	BlendMode blendMode;
	CullMode cullingMode;
	bool depthTestEnabled;
	bool depthWriteEnabled;
	ShadingModel shadingModel;
	PrimitiveTopology primitiveTopology;

	RLIMaterial();
	void reset();
	bool equals(const RLIMaterial* other) const;

	// TODO: 移行用一時処理
	void mergeFrom(const GeometryStageParameters* geometoryParams, Material* finalMaterial);
};

} // namespace detail
} // namespace ln

