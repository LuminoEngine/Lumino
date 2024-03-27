#pragma once
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>
#include "RLIBatchState.hpp"

namespace ln {
namespace detail {


// Rendering モジュールの後段の出力結果。
// RenderingContext(DrawElementListBuilder) によって作られた DrawElementList は SceneRenderer に入力されると、
// Zソートやカリング・ステートのグループ化を考慮して RenderFeatureBatch のリストに変換される。
// 最終的には各 RenderFeature が RenderFeatureBatch を実行することで、GraphicsCommandList へ描画命令が渡る。
// 
// RenderFeatureBatch が出来上がっている時点で、SpriteRenderer など Dynamic な VertexBuffer 等はすべて出来上がっている状態にしておく。
// RenderFeatureBatch 実行中は map や setData で、VertexBuffer や Texture 等への書きこみはできない。
// なぜこんな仕組みにしているかというと、特に Vulkan の RenderPass 実行中はデータ転送系のコマンドを実行できないため。
//
// RenderFeatureBatch は new してはならない。SceneRenderer から渡される LinearAllocator を使うこと。
// 
class RenderFeatureBatch
{
public:
	RenderFeatureBatch();

	RenderFeatureBatch* next() const { return m_next; }
	const detail::RenderStage* stage() const { return m_stage; }
	//detail::RenderDrawElementTypeFlags type() const { return m_type; }

	void setup(const Matrix* worldTransformPtr, Material* finalMaterial, const SubsetInfo& subsetInfo, RenderPass* renderPass);
	//void setWorldTransformPtr(const Matrix* value) { m_worldTransform = value; }
	const Matrix* worldTransformPtr() const { return m_material.m_worldTransform; }
	//void setFinalMaterial(Material* value);
	RLIBatchState* finalMaterial() { return &m_material; }
	//void setSubsetInfo(const SubsetInfo& value) { m_subsetInfo = value; }
	const SubsetInfo& subsetInfo() const { return m_material.m_subsetInfo; }
	//void setRenderPass(RenderPass* value) { m_renderPass = value; }
	RenderPass* renderPass() const { return m_material.m_renderPass; }
	RenderFeature* renderFeature() const { return m_stage->renderFeature; }

	// SpriteText で使っている。 TODO: これは MaskTexture みたいな位置づけにしてもいいかも。
	Texture* overrideTexture = nullptr;
	SamplerState* overrideSamplerState = nullptr;	// 共通の FontCache を AA 有無で描画したいときの区別
	SkeletonInstance* skeleton = nullptr;
	MorphInstance* morph = nullptr;

	bool instancing = false;

	// Extension 用。RenderPass を開始しない。
	bool ensureRenderPassOutside = false;

	void render(GraphicsCommandList* context);

private:
	RenderFeature* m_owner;
	RenderFeatureBatch* m_next;
	detail::RenderStage* m_stage;
	//detail::RenderDrawElementTypeFlags m_type;


	//Material* m_finalMaterial;
	RLIBatchState m_material;	// TODO: 直前の Batch と共有できるケースはあるので、ポインタにしておくのもあり

	friend class RenderFeatureBatchList;
};

} // namespace detail
} // namespace ln

