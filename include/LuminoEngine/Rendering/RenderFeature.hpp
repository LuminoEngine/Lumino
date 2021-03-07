#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "../Graphics/Common.hpp"
#include "Common.hpp"
#include "../Shader/ShaderInterfaceFramework.hpp"

namespace ln {
class GraphicsContext;
class ShaderTechnique;
class RenderFeature;
class Material;

namespace detail {
class RenderingManager;
struct CameraInfo;
struct ElementInfo;
struct SubsetInfo;
class RenderFeatureBatchList;
class RenderDrawElement;
class RenderStage;
class SceneRendererPass;
class SkeletonInstance;

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
	void setFinalMaterial(Material* value) { m_finalMaterial = value; }
	Material* finalMaterial() const { return m_finalMaterial; }
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

	Material* m_finalMaterial;
	SubsetInfo m_subsetInfo;
	RenderPass* m_renderPass;

	friend class RenderFeatureBatchList;
};

class RenderFeatureBatchList
{
public:
	RenderFeatureBatchList(detail::RenderingManager* manager);
	void clear();
	void setCurrentStage(detail::RenderStage* stage);

	//void addClearBatch(ClearFlags flags, const Color& color, float depth, uint8_t stencil);

	template<class T>
	T* addNewBatch(RenderFeature* owner)
	{
		void* buffer = m_allocator->allocate(sizeof(T));
		T* batch = new (buffer)T();
		add(batch, owner);
		return batch;
	}

	RenderFeatureBatch* firstBatch() const { return m_head; }
	RenderFeatureBatch* lastBatch() const { return m_tail; }

	//
	//void prepareState(const CameraInfo& cameraInfo, RenderStage* stage, RenderDrawElement* element);

	//
	//void render(GraphicsContext* graphicsContext, SceneRendererPass* pass, const FrameBuffer& defaultFrameBuffer, const CameraInfo& cameraInfo);

    // TODO: とりいそぎ
    RenderTargetTexture* renderTarget = nullptr;
    DepthBuffer* depthBuffer = nullptr;

	const CameraInfo* m_mainCameraInfo;

private:
	void add(RenderFeatureBatch* batch, RenderFeature* owner);

	detail::RenderingManager* m_manager;
	Ref<detail::LinearAllocator> m_allocator;
	detail::RenderStage* m_currentStage;
	RenderFeatureBatch* m_head;
	RenderFeatureBatch* m_tail;
};

} // namespace detail

class RenderFeature
	: public Object
{
public:
	virtual void beginRendering() {}
	//virtual void endRendering() {}

    virtual void onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo);

	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) = 0;
	virtual void renderBatch(GraphicsContext* context, detail::RenderFeatureBatch* batch) = 0;

    // 主に PrimitiveRenderFeature など、バッチ描画を行うクラスのために用意したプロパティ。
    // true を返すようにオーバーライドすると、SceneRenderer は DrawElement が持っている Transform を ElementInfo に渡さなくなる。(単位行列になる)
    virtual bool drawElementTransformNegate() const;
    // TODO: ↑は↓に統合していい気がする

    // SceneRenderer で確定し、各 RenderFeature に渡されてくる入力パラメータを処理する。
    // 基本的にこのベースメソッドを呼び出すだけでよい。
    // SpritetextRenderFerture など、RenderFeature 固有のパラメータ (グリフテクスチャなど) で
    // パラメータをオーバーライドしたい場合はここで処理する。
    virtual void updateRenderParameters(
		detail::ShaderSecondaryDescriptor* descriptor,
		detail::RenderDrawElement* element,
		ShaderTechnique* tech,
		const detail::RenderViewInfo& renderViewInfo,
		const detail::SceneInfo& sceneInfo,
		const detail::ElementInfo& elementInfo,
		const detail::SubsetInfo& subsetInfo);

	static void updateRenderParametersDefault(
		detail::ShaderSecondaryDescriptor* descriptor,
		ShaderTechnique* tech,
		const detail::RenderViewInfo& renderViewInfo,
		const detail::SceneInfo& sceneInfo,
		const detail::ElementInfo& elementInfo,
		const detail::SubsetInfo& subsetInfo);

private:
};


namespace detail {

class ClearRenderFeature : public RenderFeature
{
public:
	RequestBatchResult clear(detail::RenderFeatureBatchList* batchList, ClearFlags flags, const Color& color, float depth, uint8_t stencil);

protected:
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, detail::RenderFeatureBatch* batch) override;

private:
	struct ClearInfo
	{
		ClearFlags flags;
		Color color;
		float depth;
		uint8_t stencil;
	};

	class ClearBatch : public detail::RenderFeatureBatch
	{
	public:
		ClearInfo data;
	};

	ClearInfo m_data;
};

} // namespace detail
} // namespace ln

