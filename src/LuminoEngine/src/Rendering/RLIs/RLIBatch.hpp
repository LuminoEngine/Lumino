#pragma once
#include "../RenderStage.hpp"
#include "RLIBatchMaterial.hpp"

namespace ln {
namespace detail {


// Rendering モジュールの後段の出力結果。
// RenderingContext(DrawElementListBuilder) によって作られた DrawElementList は SceneRenderer に入力されると、
// Zソートやカリング・ステートのグループ化を考慮して RenderFeatureBatch のリストに変換される。
// 最終的には各 RenderFeature が RenderFeatureBatch を実行することで、GraphicsContext へ描画命令が渡る。
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

	void setWorldTransformPtr(const Matrix* value) { m_worldTransform = value; }
	const Matrix* worldTransformPtr() const { return m_worldTransform; }
	void setFinalMaterial(Material* value);
	const RLIBatchMaterial* finalMaterial() const { return &m_material; }
	void setSubsetInfo(const SubsetInfo& value) { m_subsetInfo = value; }
	const SubsetInfo& subsetInfo() const { return m_subsetInfo; }
	void setRenderPass(RenderPass* value) { m_renderPass = value; }
	RenderPass* renderPass() const { return m_renderPass; }

	// SpriteText で使っている。 TODO: これは MaskTexture みたいな位置づけにしてもいいかも。
	Texture* overrideTexture = nullptr;
	SamplerState* overrideSamplerState = nullptr;	// 共通の FontCache を AA 有無で描画したいときの区別
	SkeletonInstance* skeleton = nullptr;
	MorphInstance* morph = nullptr;

	bool instancing = false;

	// Extension 用。RenderPass を開始しない。
	bool ensureRenderPassOutside = false;

	void render(GraphicsContext* context);

private:
	RenderFeature* m_owner;
	RenderFeatureBatch* m_next;
	detail::RenderStage* m_stage;
	//detail::RenderDrawElementTypeFlags m_type;

	// DrawElement が持っている CombinedWorldTransform への参照。
	// null の場合は Matrix::Identity とみなす。
	// 単純に RenderFeatureBatch のサイズを増やしたくないのでポインタで用意してある。
	// RenderFeatureBatch の寿命は SceneRenderer の内部だけであり、その間は DrawElement(が持っている CombinedWorldTransform) が消えることはない。
	// Sprite や SpriteText など DynamicVertexBuffer を作るものたちは、Vertex を作るときに Transform するので、これは null となる。
	// Mesh (サブセット単位) などはこれに値がセットされる。
	const Matrix* m_worldTransform;

	//Material* m_finalMaterial;
	RLIBatchMaterial m_material;	// TODO: 直前の Batch と共有できるケースはあるので、ポインタにしておくのもあり
	SubsetInfo m_subsetInfo;
	RenderPass* m_renderPass;

	friend class RenderFeatureBatchList;
};

} // namespace detail
} // namespace ln

