
#pragma once
#include "Common.h"
#include "Color.h"
#include "RenderState.h"
#include "Shader.h"
#include "Texture.h"
#include "ContextInterface.h"

LN_NAMESPACE_BEGIN
class Pen;
class Font;
class Material;
class MeshResource;
class StaticMeshModel;
class DrawList;



enum class DrawElementCategory
{
	SceneObject,
	Information,
};

struct DrawElementMetadata
{
	DrawElementCategory	category = DrawElementCategory::SceneObject;
	int	priority = 0;

	bool Equals(const DrawElementMetadata& rhs) const
	{
		return category == rhs.category && priority == rhs.priority;
	}

	static const DrawElementMetadata Default;
};

namespace detail {
class GraphicsManager;
class IRendererPloxy;
class PrimitiveRenderer;
class BlitRenderer;
class MeshRendererProxy;
class SpriteRenderer;
class TextRenderer;
class NanoVGRenderer;
class FrameRectRenderer;
class DrawElementBatch;
class RenderingPass2;
class CombinedMaterial;

class DynamicLightInfo
	: public RefObject
{
public:
	DynamicLightInfo();
	virtual ~DynamicLightInfo() = default;

	LightType	m_type;				// ライトの種類
	Color		m_diffuse;			// ディフューズカラー
	Color		m_ambient;			// アンビエントカラー
	Color		m_specular;			// スペキュラカラー
	Vector3		m_direction;		// 向き
	float		m_shadowZFar;

	Matrix		transform;

	float		tempDistance;		// 作業用変数

	static const int MaxLights = 3;		// MMD based
};

enum class DrawingSectionId
{
	None,
	NanoVG,
};


class CombinedMaterialCache
	: public SimpleOneTimeObjectCache<CombinedMaterial>
{
protected:
	virtual RefPtr<CombinedMaterial> CreateObject() override;
};

class InternalContext
	: public RefObject
{
public:
	InternalContext();
	void Initialize(detail::GraphicsManager* manager);
	Details::Renderer* GetRenderStateManager();
	Details::Renderer* BeginBaseRenderer();
	PrimitiveRenderer* BeginPrimitiveRenderer();
	BlitRenderer* BeginBlitRenderer();
	MeshRendererProxy* BeginMeshRenderer();
	SpriteRenderer* BeginSpriteRenderer();
	TextRenderer* BeginTextRenderer();
	NanoVGRenderer* BeginNanoVGRenderer();
	FrameRectRenderer* BeginFrameRectRenderer();

	void SetViewInfo(const Size& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix);
	void SetCurrentStatePtr(const DrawElementBatch* state);
	detail::SpriteRenderer* GetSpriteRenderer();

	void Flush();

private:
	void SwitchActiveRenderer(detail::IRendererPloxy* renderer);

	IRendererPloxy*				m_current;
	Details::Renderer*			m_baseRenderer;
	RefPtr<PrimitiveRenderer>	m_primitiveRenderer;
	RefPtr<BlitRenderer>		m_blitRenderer;
	RefPtr<MeshRendererProxy>	m_meshRenderer;
	RefPtr<SpriteRenderer>		m_spriteRenderer;
	RefPtr<TextRenderer>		m_textRenderer;
	RefPtr<NanoVGRenderer>		m_nanoVGRenderer;
	RefPtr<FrameRectRenderer>	m_frameRectRenderer;
	const DrawElementBatch*		m_currentStatePtr;

	friend class ::ln::DrawList;
};

class DrawElement
{
public:
	Matrix				transform;
	int					batchIndex;
	DrawingSectionId	drawingSectionId;
	detail::Sphere		boundingSphere;		// 位置はワールド座標
	int					subsetIndex;
	//ZSortDistanceBase	zSortDistanceBase;
	float				zDistance;
	DrawElementMetadata	metadata;

	DrawElement();
	virtual ~DrawElement();

	virtual void MakeElementInfo(const CameraInfo& cameraInfo, ElementInfo* outInfo);
	virtual void MakeSubsetInfo(CombinedMaterial* material, SubsetInfo* outInfo);

	//void Draw(InternalContext* context, RenderingPass2* pass);
	virtual void DrawSubset(InternalContext* context/*, int subsetIndex*/) = 0;
	const detail::Sphere& GetBoundingSphere() const { return boundingSphere; }

	// (ローカル座標系)
	void MakeBoundingSphere(const Vector3& minPos, const Vector3& maxPos);

	virtual DynamicLightInfo** GetAffectedDynamicLightInfos();
};

class LightingDrawElement
	: public DrawElement
{
public:
	LightingDrawElement();
	virtual ~LightingDrawElement() = default;
	virtual DynamicLightInfo** GetAffectedDynamicLightInfos() { return m_affectedDynamicLightInfos; }

private:
	DynamicLightInfo*	m_affectedDynamicLightInfos[DynamicLightInfo::MaxLights];
};


class BatchState
{
public:
	BatchState();

	//void SetBlendMode(BlendMode mode);

	void SetRenderTarget(int index, RenderTargetTexture* renderTarget);
	RenderTargetTexture* GetRenderTarget(int index) const { return m_renderTargets[index]; }

	void SetDepthBuffer(DepthBuffer* depthBuffer);
	DepthBuffer* GetDepthBuffer() const { return m_depthBuffer; }

	void SetScissorRect(const RectI& scissorRect);
	const RectI& GetScissorRect() const { return m_scissorRect; }

	void SetBrush(Brush* brush);
	Brush* GetBrush() const;

	Pen* GetPen() const { return nullptr; }	// TODO

	void SetFont(Font* font);
	Font* GetFont() const;


LN_INTERNAL_ACCESS:
	void ApplyStatus(InternalContext* context, CombinedMaterial* combinedMaterial, RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer);
	uint32_t GetHashCode() const;
	void Reset();
	bool IsHashDirty() const { return m_hashDirty; }

private:

	RefPtr<RenderTargetTexture>	m_renderTargets[Graphics::MaxMultiRenderTargets];
	RefPtr<DepthBuffer>			m_depthBuffer;
	RectI						m_scissorRect;

	RefPtr<Brush>				m_brush;
	RefPtr<Font>				m_font;

	mutable size_t				m_hashCode;
	mutable bool				m_hashDirty;
};


class DrawElementBatch
{
public:
	DrawElementBatch();

	void SetCombinedMaterial(CombinedMaterial* value);
	CombinedMaterial* GetCombinedMaterial() const { return m_combinedMaterial; }

	void SetStandaloneShaderRenderer(bool enabled);
	bool IsStandaloneShaderRenderer() const;

	bool Equal(const BatchState& state, Material* material) const;
	void Reset();
	void ApplyStatus(InternalContext* context, RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer);
	size_t GetHashCode() const;

	intptr_t				m_rendererId;

	BatchState				state;



private:
	CombinedMaterial*		m_combinedMaterial;
	bool					m_standaloneShaderRenderer;
	mutable size_t			m_hashCode;
	mutable bool			m_hashDirty;
};

class BatchStateBlock
{
public:
	DrawElementBatch	state;
	Matrix				transfrom;

	void Reset()
	{
		state.Reset();
		transfrom = Matrix::Identity;
	}
};

class DrawElementList
{
public:
	typedef size_t	CommandHandle;

	DrawElementList();

	int GetElementCount() { return m_commandDataCache.GetDataCount(); }
	DrawElement* GetElement(int index) { return reinterpret_cast<DrawElement*>(m_commandDataCache.GetDataByIndex(index)); }
	DrawElementBatch* GetBatch(int index) { return &m_batchList[index]; }

	void ClearCommands();

	template<typename T, typename... TArgs>
	T* AddCommand(const BatchState& state, Material* availableMaterial, TArgs... args)
	{
		auto handle = m_commandDataCache.AllocData(sizeof(T));
		T* t = new (m_commandDataCache.GetData(handle))T(args...);
		PostAddCommandInternal(state, availableMaterial, t);
		return t;
	}

	byte_t* AllocExtData(size_t size) { return m_extDataCache.GetData(m_extDataCache.AllocData(size)); }

	//void ResolveCombinedMaterials();

	void AddDynamicLightInfo(DynamicLightInfo* lightInfo);
	const List<RefPtr<DynamicLightInfo>>& GetDynamicLightList() const { return m_dynamicLightList; }

private:
	void PostAddCommandInternal(const BatchState& state, Material* availableMaterial, DrawElement* element);

	CommandDataCache		m_commandDataCache;
	CommandDataCache		m_extDataCache;
	List<DrawElementBatch>	m_batchList;

	detail::CombinedMaterialCache	m_combinedMaterialCache;
	List<RefPtr<DynamicLightInfo>>	m_dynamicLightList;
};


class InternalRenderer
	: public RefObject
{
public:
	InternalRenderer();
	virtual ~InternalRenderer();
	void Initialize(GraphicsManager* manager);

	void Render(
		DrawElementList* elementList,
		const detail::CameraInfo& cameraInfo,
		RenderTargetTexture* defaultRenderTarget,
		DepthBuffer* defaultDepthBuffer);

protected:
	virtual void OnPreRender(DrawElementList* elementList);
	void AddPass(RenderingPass2* pass);

private:
	GraphicsManager*				m_manager;
	List<RefPtr<RenderingPass2>>	m_renderingPassList;
	List<DrawElement*>				m_renderingElementList;
};


class ScopedStateBlock2
{
public:
	ScopedStateBlock2(DrawList* renderer);
	~ScopedStateBlock2();
	void Apply();

private:
	DrawList*		m_renderer;
	BatchStateBlock	m_state;
};

//class DrawingSection
//{
//public:
//	virtual ~DrawingSection() = default;
//
//	DrawElementBatch*	stateInSection;
//};
//
//class DrawingSection_Line
//{
//public:
//	virtual ~DrawingSection_Line() = default;
//
//
//};

struct ElementRenderingPolicy
{
	Shader*	shader;		// null もありえる。Clear など。
	bool	visible;
};

class RenderingPass2
	: public Object
{
public:
	RenderingPass2();
	virtual ~RenderingPass2();
	//void Initialize(GraphicsManager* manager);

	virtual Shader* GetDefaultShader() const = 0;

	void SelectElementRenderingPolicy(DrawElement* element, CombinedMaterial* material, ElementRenderingPolicy* outPolicy);

	//virtual void RenderElement(DrawList* renderer, DrawElement* element);
	//virtual void RenderElementSubset(DrawList* renderer, DrawElement* element, int subsetIndex);

private:
};

class NonShadingRenderer
	: public InternalRenderer
{
public:
	NonShadingRenderer();
	virtual ~NonShadingRenderer();
	void Initialize(GraphicsManager* manager);

private:
};

class NonShadingRenderingPass
	: public RenderingPass2
{
public:
	NonShadingRenderingPass();
	virtual ~NonShadingRenderingPass();
	void Initialize(GraphicsManager* manager);
	virtual Shader* GetDefaultShader() const override;

private:
	RefPtr<Shader>	m_defaultShader;
};



class ForwardShadingRenderer
	: public InternalRenderer
{
public:
	ForwardShadingRenderer();
	virtual ~ForwardShadingRenderer();
	void Initialize(GraphicsManager* manager);

protected:
	virtual void OnPreRender(DrawElementList* elementList);

private:
	void UpdateAffectLights(DrawElement* element);

	List<DynamicLightInfo*>	m_selectingLights;	// UpdateAffectLights() の作業用変数
};


class ForwardShadingRenderingPass
	: public RenderingPass2
{
public:
	ForwardShadingRenderingPass();
	virtual ~ForwardShadingRenderingPass();
	void Initialize(GraphicsManager* manager);
	virtual Shader* GetDefaultShader() const override;

private:
	RefPtr<Shader>	m_defaultShader;
};


class InfomationRenderingPass
	: public NonShadingRenderingPass
{
public:
	InfomationRenderingPass();
	virtual ~InfomationRenderingPass();
	void Initialize(GraphicsManager* manager);
};


} // namespace detail

/**
	@brief	
*/
class DrawList	// TODO: Renderer
	: public Object
{
public:

	// TODO: MeshResource など、一度コマンド化したら実行されるまでは変更禁止状態にしたい。

	//--------------------------------------------------------------------------
	/** @name Render targets */
	/** @{ */

	/** レンダリングターゲットを設定します。*/
	void SetRenderTarget(int index, RenderTargetTexture* renderTarget);

	/** 現在設定されているレンダリングターゲットを取得します。*/
	RenderTargetTexture* GetRenderTarget(int index) const;

	/** 深度バッファを設定します。*/
	void SetDepthBuffer(DepthBuffer* depthBuffer);

	/** 現在設定されている深度バッファを取得します。*/
	DepthBuffer* GetDepthBuffer() const;

	/** シザー領域を設定します。*/
	void SetViewport(const RectI& rect);

	/** 現在設定されているシザー領域を取得します。*/
	const RectI& GetViewport() const;

	/** @} */


	//--------------------------------------------------------------------------
	/** @name Render status */
	/** @{ */

	void SetBlendMode(BlendMode mode);

	void SetOpacity(float opacity);

	void SetDepthTestEnabled(bool enabled);

	void SetDepthWriteEnabled(bool enabled);

	/** @} */

	void SetTransform(const Matrix& transform);


	void SetBrush(Brush* brush);
	void SetFont(Font* font);

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);
	

	/**
		@brief		線分を描画します。
		@param[in]	position1	: 始点の位置
		@param[in]	color1		: 始点の色
		@param[in]	position2	: 終点の位置
		@param[in]	color2		: 終点の色
	*/
	void DrawLinePrimitive(
		const Vector3& position1, const Color& color1,
		const Vector3& position2, const Color& color2);

	/**
		@brief
		@details	デフォルトでは反時計回りが表面となります。
	*/
	void DrawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,	// TODO: 順序
		const Vector3& position4, const Vector2& uv4, const Color& color4/*,
		ShaderPass* shaderPass*/);

	void DrawMesh(MeshResource* mesh, int subsetIndex, Material* material);
	void DrawMesh(StaticMeshModel* mesh, int subsetIndex, Material* material);

	void Blit(Texture* source);
	void Blit(Texture* source, const Matrix& transform);
	void Blit(Texture* source, RenderTargetTexture* dest, const Matrix& transform);
	void Blit(Texture* source, RenderTargetTexture* dest, Material* material);


	void DrawText_(const StringRef& text, const PointF& position);
	void DrawText_(const StringRef& text, const RectF& rect, StringFormatFlags flags);

	void DrawSprite(
		const Vector3& position,
		const Size& size,
		const Vector2& anchor,
		Texture* texture,
		const RectF& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		Material* material = nullptr);

	void DrawRectangle(const RectF& rect);

LN_INTERNAL_ACCESS:
	DrawList();
	virtual ~DrawList();
	void Initialize(detail::GraphicsManager* manager);
	detail::GraphicsManager* GetManager() const { return m_manager; }
	detail::DrawElementList* GetDrawElementList() { return &m_drawElementList; }
	void BeginMakeElements();
	void EndMakeElements();
	void EndFrame();

	const detail::BatchStateBlock& GetState() const { return m_state; }
	void SetState(const detail::BatchStateBlock& state) { m_state = state; }
	void AddDynamicLightInfo(detail::DynamicLightInfo* lightInfo);
	void PushMetadata(const DrawElementMetadata* metadata);
	const DrawElementMetadata* PopMetadata();

	template<typename TElement> TElement* ResolveDrawElement(detail::DrawingSectionId sectionId, detail::IRendererPloxy* renderer, Material* userMaterial);
	void DrawMeshResourceInternal(MeshResource* mesh, int subsetIndex, Material* material);
	void DrawMeshSubsetInternal(StaticMeshModel* mesh, int subsetIndex, Material* material);
	void BlitInternal(Texture* source, RenderTargetTexture* dest, const Matrix& transform, Material* material);
	void DrawFrameRectangle(const RectF& rect);

private:
	detail::GraphicsManager*		m_manager;
	
	detail::BatchStateBlock			m_state;
	RefPtr<Material>				m_defaultMaterial;

	detail::DrawElementList			m_drawElementList;

	detail::DrawElement*			m_currentSectionTopElement;
	//detail::DrawElementBatch		m_stateInSection;
	const DrawElementMetadata*		m_metadata;

#if 0
	/** アルファブレンドの有無 (default: false) */
	void SetAlphaBlendEnabled(bool enabled);
	/** ブレンディングの演算方法 (default: Add) */
	void SetBlendOp(BlendOp op);
	/** ブレンディングの係数 (default: One) */
	void SetSourceBlend(BlendFactor blend);
	/** ブレンディングの係数 (default: Zero) */
	void SetDestinationBlend(BlendFactor blend);


	void SetBlendMode(BlendMode mode);
	void SetCullingMode(CullingMode mode);
	void SetFillMode(FillMode mode);
	void SetAlphaTestEnabled(bool enabled);

	void SetDepthStencilState(const DepthStencilState& state);

	void SetDepthTestEnabled(bool enabled);
	void SetDepthWriteEnabled(bool enabled);
	void SetDepthTestFunc(CompareFunc func);
	void SetStencilEnabled(bool enabled);
	void SetStencilFunc(CompareFunc func);
	void SetStencilReferenceValue(uint8_t value);
	void SetStencilFailOp(StencilOp op);
	void SetStencilDepthFailOp(StencilOp op);
	void SetStencilPassOp(StencilOp op);
#endif
};

LN_NAMESPACE_END

