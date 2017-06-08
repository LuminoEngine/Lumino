
#pragma once
#include "Common.h"
#include "../Graphics/Color.h"
#include "../Graphics/RenderState.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Graphics/ContextInterface.h"
#include "../Graphics/Material.h"

LN_NAMESPACE_BEGIN
class Pen;
class Font;
class Material;
class MeshResource;
class StaticMeshModel;
class DrawList;
class RenderView;
class RenderDiag;

class CameraComponent;


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
class IRenderFeature;
class PrimitiveRenderFeature;
class BlitRenderer;
class MeshRenderFeature;
class SpriteRenderFeature;
class TextRenderer;
class VectorTextRenderer;
class ShapesRenderFeature;
class NanoVGRenderFeature;
class FrameRectRenderFeature;
class DrawElementBatch;
class RenderingPass2;
class CombinedMaterial;
class DrawElementList;
class SceneRenderer;

struct DefaultStatus
{
	RenderTargetTexture*	defaultRenderTarget;
	DepthBuffer*			defaultDepthBuffer;
	//BlendMode				blendMode;
	//CullingMode				cullingMode;
	//bool					depthTestEnabled;
	//bool					depthWriteEnabled;
};

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
	PrimitiveRenderFeature* BeginPrimitiveRenderer();
	BlitRenderer* BeginBlitRenderer();
	MeshRenderFeature* BeginMeshRenderer();
	SpriteRenderFeature* BeginSpriteRenderer();
	TextRenderer* BeginTextRenderer();
	VectorTextRenderer* BeginVectorTextRenderer();
	ShapesRenderFeature* BeginShapesRenderer();
	NanoVGRenderFeature* BeginNanoVGRenderer();
	FrameRectRenderFeature* BeginFrameRectRenderer();

	void SetViewInfo(const Size& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix);
	void ApplyStatus(DrawElementBatch* state, const DefaultStatus& defaultStatus);
	DrawElementBatch* GetCurrentStatus() const { return m_currentStatePtr; }
	detail::SpriteRenderFeature* GetSpriteRenderer();

	void Flush();

LN_INTERNAL_ACCESS:
	void SwitchActiveRenderer(detail::IRenderFeature* renderer);

	IRenderFeature*					m_current;
	Details::Renderer*				m_baseRenderer;
	RefPtr<PrimitiveRenderFeature>	m_primitiveRenderer;
	RefPtr<BlitRenderer>			m_blitRenderer;
	RefPtr<MeshRenderFeature>		m_meshRenderer;
	RefPtr<SpriteRenderFeature>		m_spriteRenderer;
	RefPtr<TextRenderer>			m_textRenderer;
	RefPtr<VectorTextRenderer>		m_vectorTextRenderer;
	RefPtr<ShapesRenderFeature>		m_shapesRenderer;
	RefPtr<NanoVGRenderFeature>		m_nanoVGRenderer;
	RefPtr<FrameRectRenderFeature>	m_frameRectRenderer;
	DrawElementBatch*				m_currentStatePtr;

	friend class ::ln::DrawList;
};

class DrawElement
{
public:
	struct DrawArgs
	{
		DrawElementList* oenerList;
		InternalContext* context;
		detail::SceneRenderer* renderer;
		RenderTargetTexture* defaultRenderTarget;
		DepthBuffer* defaultDepthBuffer;
		RenderDiag* diag;
	};

	int					batchIndex;
	DrawingSectionId	drawingSectionId;
	detail::Sphere		boundingSphere;		// 位置はワールド座標
	int					subsetIndex;
	float				zDistance;
	DrawElementMetadata	metadata;
	int					m_stateFence;

	DrawElementList*	m_ownerDrawElementList;

	DrawElement();
	virtual ~DrawElement();

	const Matrix& GetTransform(DrawElementList* oenerList) const;

	virtual void MakeElementInfo(DrawElementList* oenerList, const CameraInfo& cameraInfo, ElementInfo* outInfo);
	virtual void MakeSubsetInfo(DrawElementList* oenerList, CombinedMaterial* material, SubsetInfo* outInfo);

	virtual void DrawSubset(const DrawArgs& e) = 0;
	const detail::Sphere& GetBoundingSphere() const { return boundingSphere; }

	// (ローカル座標系)
	void MakeBoundingSphere(const Vector3& minPos, const Vector3& maxPos);

	virtual DynamicLightInfo** GetAffectedDynamicLightInfos();

	virtual void ReportDiag(RenderDiag* diag) = 0;

protected:
	//void OnJoindDrawList(const Matrix& transform);
	
private:
	//Matrix	m_transform;

	//friend class DrawList;
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

	void SetBlendMode(BlendMode mode);
	BlendMode GetBlendMode() const { return m_blendMode; }

	void SetCullingMode(CullingMode mode);
	CullingMode GetCullingMode() const { return m_cullingMode; }

	void SetDepthTestEnabled(bool enabled);
	bool IsDepthTestEnabled() const { return m_depthTestEnabled; }

	void SetDepthWriteEnabled(bool enabled);
	bool IsDepthWriteEnabled() const { return m_depthWriteEnabled; }


	void SetBrush(Brush* brush);
	Brush* GetBrush() const;

	Pen* GetPen() const { return nullptr; }	// TODO

	void SetFont(Font* font);
	Font* GetFont() const;		// not null (default font)
								

LN_INTERNAL_ACCESS:
	void ApplyStatus(InternalContext* context, CombinedMaterial* combinedMaterial, const DefaultStatus& defaultStatus);
	uint32_t GetHashCode() const;
	void Reset();
	bool IsHashDirty() const { return m_hashDirty; }

private:

	RefPtr<RenderTargetTexture>	m_renderTargets[Graphics::MaxMultiRenderTargets];
	RefPtr<DepthBuffer>			m_depthBuffer;
	RectI						m_scissorRect;
	BlendMode					m_blendMode;
	CullingMode					m_cullingMode;
	bool						m_depthTestEnabled;
	bool						m_depthWriteEnabled;

	RefPtr<Brush>				m_brush;
	RefPtr<Font>				m_font;

	mutable size_t				m_hashCode;
	mutable bool				m_hashDirty;
};

class DrawElementBatch
{
public:
	DrawElementBatch();

	void SetTransfrom(const Matrix& value);
	const Matrix& GetTransfrom() const { return m_transfrom; }

	void SetCombinedMaterial(CombinedMaterial* value);
	CombinedMaterial* GetCombinedMaterial() const { return m_combinedMaterial; }


	void SetBuiltinEffect(const BuiltinEffectData& data);

	void SetStandaloneShaderRenderer(bool enabled);
	bool IsStandaloneShaderRenderer() const;

	bool Equal(const BatchState& state, Material* material, const Matrix& transfrom, const BuiltinEffectData& effectData) const;
	void Reset();
	void ApplyStatus(InternalContext* context, const DefaultStatus& defaultStatus);
	size_t GetHashCode() const;
	size_t GetBuiltinEffectDataHashCode() const;

	intptr_t				m_rendererId;

	BatchState				state;



private:
	Matrix					m_transfrom;			// WorldTransform. 変わったらシェーダの ln_World* も変える必要がある。
	CombinedMaterial*		m_combinedMaterial;
	bool					m_standaloneShaderRenderer;
	mutable size_t			m_hashCode;
	mutable bool			m_hashDirty;

	BuiltinEffectData		m_builtinEffectData;

};

class BatchStateBlock
{
public:
	DrawElementBatch	state;

	void Reset()
	{
		state.Reset();
		//transfrom = Matrix::Identity;
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
	T* AddCommand(const BatchState& state, Material* availableMaterial, const Matrix& transform, const BuiltinEffectData& effectData, TArgs... args)
	{
		auto handle = m_commandDataCache.AllocData(sizeof(T));
		T* t = new (m_commandDataCache.GetData(handle))T(args...);
		PostAddCommandInternal(state, availableMaterial, transform, effectData, t);
		t->m_ownerDrawElementList = this;
		return t;
	}

	//byte_t* AllocExtData(size_t size) { return m_extDataCache.GetData(m_extDataCache.AllocData(size)); }
	CommandDataCache::DataHandle AllocExtData(size_t size) { return m_extDataCache.AllocData(size); }
	void* GetExtData(CommandDataCache::DataHandle handle) { return m_extDataCache.GetData(handle); }

	//void ResolveCombinedMaterials();

	void AddDynamicLightInfo(DynamicLightInfo* lightInfo);
	const List<RefPtr<DynamicLightInfo>>& GetDynamicLightList() const { return m_dynamicLightList; }


	// default settings
	void SetDefaultRenderTarget(RenderTargetTexture* value) { m_defaultRenderTarget = value; }
	RenderTargetTexture* GetDefaultRenderTarget() const { return m_defaultRenderTarget; }
	void SetDefaultDepthBuffer(DepthBuffer* value) { m_depthBuffer = value; }
	DepthBuffer* GetDefaultDepthBuffer() const { return m_depthBuffer; }

private:
	void PostAddCommandInternal(const BatchState& state, Material* availableMaterial, const Matrix& transform, const BuiltinEffectData& effectData, DrawElement* element);

	CommandDataCache		m_commandDataCache;
	CommandDataCache		m_extDataCache;
	List<DrawElementBatch>	m_batchList;

	detail::CombinedMaterialCache	m_combinedMaterialCache;
	List<RefPtr<DynamicLightInfo>>	m_dynamicLightList;

	RefPtr<RenderTargetTexture>		m_defaultRenderTarget;
	RefPtr<DepthBuffer>				m_depthBuffer;
};



/**
	@brief	シーンの描画方法を定義し、描画コマンドを実行します。
*/
class SceneRenderer
	: public RefObject
{
public:
	SceneRenderer();
	virtual ~SceneRenderer();
	void Initialize(GraphicsManager* manager);

	void Render(
		RenderView* drawElementListSet,
		//DrawElementList* elementList,
		//const detail::CameraInfo& cameraInfo,
		RenderTargetTexture* defaultRenderTarget,
		DepthBuffer* defaultDepthBuffer,
		RenderDiag* diag);

protected:
	virtual void OnPreRender(DrawElementList* elementList);
	void AddPass(RenderingPass2* pass);

private:
	GraphicsManager*				m_manager;
	List<RefPtr<RenderingPass2>>	m_renderingPassList;
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
	: public SceneRenderer
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
	: public SceneRenderer
{
public:
	ForwardShadingRenderer();
	virtual ~ForwardShadingRenderer();
	void Initialize(GraphicsManager* manager);

protected:
	virtual void OnPreRender(DrawElementList* elementList);

private:
	void UpdateAffectLights(DrawElement* element, DrawElementList* elementList);

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


class RenderDiagItem
{
public:
	RenderDiagItem();
	virtual ~RenderDiagItem() = default;
	virtual String ToString() const;

private:
	enum class SubType
	{
		ScopeBegin,
		ScopeEnd,
		Command,
	};

	const TCHAR* m_name;
	SubType	m_subType;
	friend class RenderDiag;
};

class RenderDiag
	: public Object
{
public:
	void Clear();

	void BeginRenderView();
	void EndRenderView();
	void BeginDrawList();
	void EndDrawList();
	void ChangeRenderStage();
	void CallCommonElement(const TCHAR* typeName);

	template<typename TDiagItem>
	TDiagItem* CallElement(const TCHAR* typeName)
	{
		return Instantiate<TDiagItem>(typeName);
	}

	void Print();

private:

	class IDataCatch
	{
	public:
		virtual ~IDataCatch() = default;
		virtual void Clear() = 0;
	};

	template<typename T>
	class DataCatch : public IDataCatch
	{
	public:
		virtual void Clear() override
		{
			for (auto& p : m_using)
				m_storage.push_back(p);
			m_using.clear();
		}
		T* Instantiate()
		{
			if (!m_storage.empty())
			{
				auto p = m_storage.back();
				m_storage.pop_back();
				m_using.push_back(p);
				return p.get();
			}
			else
			{
				auto p = std::make_shared<T>();
				m_using.push_back(p);
				return p.get();
			}
		}

	private:
		std::vector<std::shared_ptr<T>>	m_storage;
		std::vector<std::shared_ptr<T>>	m_using;
	};

	template<typename T>
	T* Instantiate(const char* typeName)
	{
		assert(typeName != nullptr);

		DataCatch<T>* cache = nullptr;
		auto itr = m_cacheMap.find(typeName);
		if (itr != m_cacheMap.end())
		{
			cache = static_cast<DataCatch<T>*>(itr->second.get());
		}
		else
		{
			auto cachePtr = std::make_shared<DataCatch<T>>();
			m_cacheMap[typeName] = cachePtr;
			cache = cachePtr.get();
		}

		T* obj = cache->Instantiate();
		obj->m_name = typeName;
		m_items.push_back(obj);
		return obj;
	}

	std::unordered_map<std::string, std::shared_ptr<IDataCatch>>	m_cacheMap;
	std::vector<RenderDiagItem*>									m_items;
};

namespace detail {

class RenderDiagItem_BeginRenderView : public RenderDiagItem
{
public:
};
class RenderDiagItem_EndRenderView : public RenderDiagItem
{
public:
};
class RenderDiagItem_BeginDrawList : public RenderDiagItem
{
public:
};
class RenderDiagItem_EndDrawList : public RenderDiagItem
{
public:
};
class RenderDiagItem_Common : public RenderDiagItem
{
public:
};

} // namespace detail


/**
	@brief	ある視点を起点としたレンダリングのエントリーポイントとなるビューを表します。
*/
class RenderView
	: public RefObject
{
public:
	// 描画リストと視点情報のまとまり。
	// ある1つの視点から、複数の描画リストを結合して描画するために使用する。
	List<detail::DrawElementList*>	m_lists;
	detail::CameraInfo				m_cameraInfo;

	// 作業用
	List<detail::DrawElement*>				m_renderingElementList;


	const ln::Size& GetViewSize() const { return m_viewSize; }

LN_INTERNAL_ACCESS:
	void SetViewSize(const ln::Size& size) { m_viewSize = size; }

private:
	ln::Size    m_viewSize;

};


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
	void SetOpacity(float opacity);

	void SetBlendMode(BlendMode mode);

	void SetCullingMode(CullingMode mode);

	void SetDepthTestEnabled(bool enabled);

	void SetDepthWriteEnabled(bool enabled);

	/** @} */

	void SetTransform(const Matrix& transform);


	void SetBrush(Brush* brush);
	Brush* GetBrush() const;

	void SetFont(Font* font);

	void SetShader(Shader* shader);
	Shader* GetShader() const;

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


	void DrawSquare(float sizeX, float sizeZ, int slicesX, int slicesZ, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, Material* material = nullptr);
	
	void DrawArc(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, Material* material = nullptr);

	void DrawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, Material* material = nullptr);

	void DrawSphere(float radius, int slices = 8, int stacks = 8, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity);

	void DrawCylinder(float radius, float height, int slices = 8, int stacks = 1, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity);

	void DrawCone(float radius, float height, int slices = 8, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity);

	void DrawMesh(MeshResource* mesh, int subsetIndex, Material* material);
	//void DrawMesh(StaticMeshModel* mesh, int subsetIndex, Material* material);

	void Blit(Texture* source);
	void Blit(Texture* source, const Matrix& transform);
	void Blit(Texture* source, RenderTargetTexture* dest, const Matrix& transform);
	void Blit(Texture* source, RenderTargetTexture* dest, Material* material);

	void DrawGlyphRun(const PointF& position, GlyphRun* glyphRun);

	void DrawText_(const StringRef& text, const PointF& position);
	void DrawText_(const StringRef& text, const Rect& rect, StringFormatFlags flags);

	void DrawChar(TCHAR ch, const PointF& position);
	void DrawText2(const StringRef& text, const Rect& rect);

	void DrawSprite(
		const Vector3& position,
		const Size& size,
		const Vector2& anchor,
		Texture* texture,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType,
		Material* material = nullptr);

	void DrawRectangle(const Rect& rect);

	void DrawScreenRectangle();

LN_INTERNAL_ACCESS:
	DrawList();
	virtual ~DrawList();
	void Initialize(detail::GraphicsManager* manager);
	detail::GraphicsManager* GetManager() const { return m_manager; }
	detail::DrawElementList* GetDrawElementList() { return &m_drawElementList; }
	void SetDefaultMaterial(Material* material);
	void SetBuiltinEffectData(const detail::BuiltinEffectData& data);
	void BeginMakeElements();

	const detail::BatchStateBlock& GetState() const { return m_state; }
	void SetState(const detail::BatchStateBlock& state) { m_state = state; }
	void AddDynamicLightInfo(detail::DynamicLightInfo* lightInfo);
	void PushMetadata(const DrawElementMetadata* metadata);
	const DrawElementMetadata* GetMetadata();
	void PopMetadata();

	template<typename TElement> TElement* ResolveDrawElement(detail::DrawingSectionId sectionId, detail::IRenderFeature* renderer, Material* userMaterial);
	void DrawMeshResourceInternal(MeshResource* mesh, int subsetIndex, Material* material);
	//void DrawMeshSubsetInternal(StaticMeshModel* mesh, int subsetIndex, Material* material);
	void BlitInternal(Texture* source, RenderTargetTexture* dest, const Matrix& transform, Material* material);
	void DrawFrameRectangle(const Rect& rect);
	void RenderSubView(RenderView* listSet, detail::SceneRenderer* renderer = nullptr, RenderTargetTexture* defaultRenderTarget = nullptr, DepthBuffer* defaultDepthBuffer = nullptr);

	// TODO: 本質的に DrawList に持たせるべきではない。一応今は一時変数的な扱いでしかないので被害は少ないが・・・
	void SetCurrentCamera(CameraComponent* camera) { m_camera = camera; }
	CameraComponent* GetCurrentCamera() const { return m_camera; }

private:
	detail::GraphicsManager*		m_manager;
	
	detail::BatchStateBlock			m_state;
	RefPtr<Material>				m_defaultMaterial;

	detail::BuiltinEffectData		m_builtinEffectData;

	detail::DrawElementList			m_drawElementList;

	detail::DrawElement*			m_currentSectionTopElement;
	//detail::DrawElementBatch		m_stateInSection;
	const DrawElementMetadata*		m_metadata;
	int								m_currentStateFence;

	CameraComponent*							m_camera;

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


//------------------------------------------------------------------------------
template<typename TElement>
inline TElement* DrawList::ResolveDrawElement(detail::DrawingSectionId sectionId, detail::IRenderFeature* renderer, Material* userMaterial)
{
	Material* availableMaterial = (userMaterial != nullptr) ? userMaterial : m_defaultMaterial.Get();

	// これを決定してから比較を行う
	m_state.state.SetStandaloneShaderRenderer(renderer->IsStandaloneShader());

	m_state.state.m_rendererId = reinterpret_cast<intptr_t>(renderer);

	const DrawElementMetadata* userMetadata = GetMetadata();
	const DrawElementMetadata* metadata = (userMetadata != nullptr) ? userMetadata : &DrawElementMetadata::Default;

	// 何か前回追加された DrawElement があり、それと DrawingSectionId、State が一致するならそれに対して追記できる
	if (sectionId != detail::DrawingSectionId::None &&
		m_currentSectionTopElement != nullptr &&
		m_currentSectionTopElement->drawingSectionId == sectionId &&
		m_currentSectionTopElement->metadata.Equals(*metadata) &&
		m_currentSectionTopElement->m_stateFence == m_currentStateFence &&
		m_drawElementList.GetBatch(m_currentSectionTopElement->batchIndex)->Equal(m_state.state.state, availableMaterial, m_state.state.GetTransfrom(), m_builtinEffectData))
	{
		return static_cast<TElement*>(m_currentSectionTopElement);
	}

	// DrawElement を新しく作る
	TElement* element = m_drawElementList.AddCommand<TElement>(m_state.state.state, availableMaterial, m_state.state.GetTransfrom(), m_builtinEffectData);
	//element->OnJoindDrawList(m_state.transfrom);
	element->drawingSectionId = sectionId;
	element->metadata = *metadata;
	element->m_stateFence = m_currentStateFence;
	m_currentSectionTopElement = element;
	return element;
}


LN_NAMESPACE_END

