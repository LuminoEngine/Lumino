
#pragma once
#include "Common.h"
#include "../Graphics/Color.h"
#include "../Graphics/RenderState.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Graphics/ContextInterface.h"
#include "../Graphics/Material.h"

LN_NAMESPACE_BEGIN
namespace detail { class ShadowCasterPass; }
namespace detail { class RenderTargetTextureCache; }
class Pen;
class Font;
class CommonMaterial;
class MeshResource;
class StaticMeshModel;
class SkinnedMeshModel;
class DrawList;
class RenderView;
class RenderDiag;

enum class DrawElementCategory
{
	SceneObject,
	Information,
};

struct DrawElementMetadata
{
	DrawElementCategory	category = DrawElementCategory::SceneObject;
	int	priority = 0;

	bool equals(const DrawElementMetadata& rhs) const
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
class RenderStage;
class RenderStageCache;
class VisualNodeParameters;

// TODO: Obsolete
struct SceneGlobalRenderSettings
{
	//bool				fogEnabled = false;
	Color				fogColor = Color(1, 1, 1, 1);
	float				fogDensity = 0;

	// Reflection
	// flare
	// sky
};

struct DefaultStatus
{
	RenderTargetTexture*	defaultRenderTarget[4];
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
	Color		m_ambient;			// アンビエントカラー [Obsolete]
	Color		m_specular;			// スペキュラカラー [Obsolete]
	Color		m_groundColor;		// 半球ライティング用
	Vector3		m_position;
	Vector3		m_direction;		// 向き
	float		m_intensity;
	float		m_range;
	float		m_attenuation;
	float		m_spotAngle;
	float		m_spotPenumbra;
	float		m_shadowZFar;

	detail::ShadowCasterPass*	m_shadowCasterPass;
	//Matrix		transform;

	float		tempDistance;		// 作業用変数

	static const int MaxLights = 3;		// MMD based
};

class CombinedMaterialCache
	: public SimpleOneTimeObjectCache<CombinedMaterial>
{
protected:
	virtual Ref<CombinedMaterial> createObject() override;
};

class InternalContext
	: public RefObject
{
public:

	InternalContext();
	void initialize(detail::GraphicsManager* manager);
	detail::CoreGraphicsRenderFeature* getRenderStateManager();
	detail::CoreGraphicsRenderFeature* beginBaseRenderer();
	PrimitiveRenderFeature* beginPrimitiveRenderer();
	BlitRenderer* beginBlitRenderer();
	MeshRenderFeature* beginMeshRenderer();
	SpriteRenderFeature* beginSpriteRenderer();
	TextRenderer* beginTextRenderer();
	VectorTextRenderer* beginVectorTextRenderer();
	ShapesRenderFeature* beginShapesRenderer();
	NanoVGRenderFeature* beginNanoVGRenderer();
	FrameRectRenderFeature* beginFrameRectRenderer();

	void setViewInfo(const Size& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix);
	void applyStatus(RenderStage* state, const DefaultStatus& defaultStatus);
	RenderStage* getCurrentStatus() const { return m_currentStatePtr; }
	detail::SpriteRenderFeature* getSpriteRenderer();

	void flush();

LN_INTERNAL_ACCESS:
	void switchActiveRenderer(detail::IRenderFeature* renderer);
	void applyFrameBufferStatusInternal(RenderStage* stage, const DefaultStatus& defaultStatus);
	void applyGeometryStatusInternal(RenderStage* stage, CommonMaterial* priorityMaterial = nullptr);
	IRenderFeature* getCurrentRenderFeature() const { return m_current; }

	IRenderFeature*					m_current;
	detail::CoreGraphicsRenderFeature*	m_baseRenderer;
	Ref<PrimitiveRenderFeature>	m_primitiveRenderer;
	Ref<BlitRenderer>			m_blitRenderer;
	Ref<MeshRenderFeature>		m_meshRenderer;
	Ref<SpriteRenderFeature>		m_spriteRenderer;
	Ref<TextRenderer>			m_textRenderer;
	Ref<VectorTextRenderer>		m_vectorTextRenderer;
	Ref<ShapesRenderFeature>		m_shapesRenderer;
	Ref<NanoVGRenderFeature>		m_nanoVGRenderer;
	Ref<FrameRectRenderFeature>	m_frameRectRenderer;
	RenderStage*				m_currentStatePtr;

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
	int					subsetIndex;
	float				zDistance;
	DrawElementMetadata	metadata;
	int					m_stateFence;
	CommonMaterial*		priorityMaterial = nullptr;	// マテリアルを指定する drawMesh() のオーバーライドなどで設定される
	detail::IRenderFeature* renderFeature = nullptr;
	DrawElementList*	m_ownerDrawElementList;
	int					subsetCount = 1;
	ShaderTechniqueClass_MeshProcess	vertexProcessing = ShaderTechniqueClass_MeshProcess::StaticMesh;

	DrawElement();
	virtual ~DrawElement();

	const Matrix& getTransform(DrawElementList* oenerList) const;

	virtual CommonMaterial*	getPriorityMaterial(int subsetIndex) { return priorityMaterial; }

	virtual void makeElementInfo(DrawElementList* oenerList, const CameraInfo& cameraInfo, RenderView* renderView, ElementInfo* outInfo);
	virtual void makeSubsetInfo(DrawElementList* oenerList, RenderStage* stage, SubsetInfo* outInfo);

	virtual void drawSubset(const DrawArgs& e, int subsetIndex) = 0;
	const detail::Sphere& getLocalBoundingSphere() const { return m_localBoundingSphere; }
	void setLocalBoundingSphere(const detail::Sphere& value) { m_localBoundingSphere = value; }

	// (ローカル座標系)
	void makeBoundingSphere(const Vector3& minPos, const Vector3& maxPos);
	void makeBoundingSphere(float radius);

	virtual DynamicLightInfo** getAffectedDynamicLightInfos();

	virtual void reportDiag(RenderDiag* diag) = 0;

protected:
	//void OnJoindDrawList(const Matrix& transform);
	
private:
	//Matrix	m_transform;

	//friend class DrawList;
	detail::Sphere		m_localBoundingSphere;
};

class LightingDrawElement
	: public DrawElement
{
public:
	LightingDrawElement();
	virtual ~LightingDrawElement() = default;
	virtual DynamicLightInfo** getAffectedDynamicLightInfos() { return m_affectedDynamicLightInfos; }

private:
	DynamicLightInfo*	m_affectedDynamicLightInfos[DynamicLightInfo::MaxLights];
};




class BatchState
{
public:
	BatchState();

	//void setBlendMode(BlendMode mode);

	void setRenderTarget(int index, RenderTargetTexture* renderTarget);
	RenderTargetTexture* getRenderTarget(int index) const { return m_renderTargets[index]; }

	void setDepthBuffer(DepthBuffer* depthBuffer);
	DepthBuffer* getDepthBuffer() const { return m_depthBuffer; }

	void setViewportRect(const RectI& rect);
	const RectI& getViewportRect() const { return m_viewportRect; }

	void setScissorRect(const RectI& scissorRect);
	const RectI& getScissorRect() const { return m_scissorRect; }

	void setBlendMode(BlendMode mode);
	BlendMode getBlendMode() const { return m_blendMode; }

	void setCullingMode(CullingMode mode);
	CullingMode getCullingMode() const { return m_cullingMode; }

	void setDepthTestEnabled(bool enabled);
	bool isDepthTestEnabled() const { return m_depthTestEnabled; }

	void setDepthWriteEnabled(bool enabled);
	bool isDepthWriteEnabled() const { return m_depthWriteEnabled; }


	void setBrush(Brush* brush);
	Brush* getBrush() const;

	Pen* getPen() const { return nullptr; }	// TODO

	void setFont(Font* font);
	Font* getFont() const;		// not null (default font)
								

LN_INTERNAL_ACCESS:
	//void applyStatus(InternalContext* context, CombinedMaterial* combinedMaterial, const DefaultStatus& defaultStatus);
	uint32_t getHashCode() const;
	void reset();
	bool isHashDirty() const { return m_hashDirty; }

private:

	Ref<RenderTargetTexture>	m_renderTargets[Graphics::MaxMultiRenderTargets];
	Ref<DepthBuffer>			m_depthBuffer;
	RectI						m_viewportRect;
	RectI						m_scissorRect;
	BlendMode					m_blendMode;
	CullingMode					m_cullingMode;
	bool						m_depthTestEnabled;
	bool						m_depthWriteEnabled;

	Ref<Brush>				m_brush;
	Ref<Font>				m_font;

	mutable size_t				m_hashCode;
	mutable bool				m_hashDirty;
};

class DrawElementBatch
{
public:
	DrawElementBatch();

	// list に入った後は freeze フラグつけておいたほうがいいかも。













	void setTransfrom(const Matrix& value);
	const Matrix& getTransfrom() const { return m_transfrom; }

	void setCombinedMaterial(CombinedMaterial* value);
	CombinedMaterial* getCombinedMaterial() const { return m_combinedMaterial; }


	void SetBuiltinEffect(const BuiltinEffectData& data);

	//void SetStandaloneShaderRenderer(bool enabled);
	//bool IsStandaloneShaderRenderer() const;
	void setRenderFeature(IRenderFeature* renderFeature);
	IRenderFeature* getRenderFeature() const { return m_renderFeature; }

	bool Equal(const DrawElementBatch& state, CommonMaterial* material, const BuiltinEffectData& effectData) const;
	void reset();
	//void applyStatus(InternalContext* context, const DefaultStatus& defaultStatus);
	size_t getHashCode() const;
	size_t getBuiltinEffectDataHashCode() const;


	BatchState				state;

private:
	Matrix					m_transfrom;			// WorldTransform. 変わったらシェーダの ln_World* も変える必要がある。
	CombinedMaterial*		m_combinedMaterial;
	IRenderFeature*			m_renderFeature;			// この state の下で描画を行う RenderFeature
	//bool					m_standaloneShaderRenderer;
	mutable size_t			m_hashCode;
	mutable bool			m_hashDirty;

	BuiltinEffectData		m_builtinEffectData;

};

class DrawElementList
{
public:
	typedef size_t	CommandHandle;

	DrawElementList();

	int getElementCount() { return m_commandDataCache.getDataCount(); }
	DrawElement* getElement(int index) { return reinterpret_cast<DrawElement*>(m_commandDataCache.getDataByIndex(index)); }
	DrawElement* getLastElement() const { return m_lastElement; }
	//DrawElementBatch* getBatch(int index) { return &m_batchList[index]; }

	void clearCommands();

	// 最後に確定させた Stage と、現在構築中の stage に差異があるかどうか
	bool checkChangeRenderStage(const RenderStage* stage) const;

	// 指定した stage の内容をコピーし、新しい Stage を確定させ、その id を返す
	int submitRenderStage(const RenderStage* stage);

	RenderStage* getRenderStage(int id) const;
	RenderStage* getLastRenderStage() const;

	template<typename T, typename... TArgs>
	T* addCommand(
		//const DrawElementBatch& state, CommonMaterial* availableMaterial, const BuiltinEffectData& effectData, bool forceStateChange,
		TArgs... args)
	{
		auto handle = m_commandDataCache.allocData(sizeof(T));
		T* t = new (m_commandDataCache.getData(handle))T(args...);
		//postAddCommandInternal(state, availableMaterial, effectData, forceStateChange, t);
		t->m_ownerDrawElementList = this;
		m_lastElement = t;
		return t;
	}
	
	//byte_t* allocExtData(size_t size) { return m_extDataCache.GetData(m_extDataCache.allocData(size)); }
	CommandDataCache::DataHandle allocExtData(size_t size) { return m_extDataCache.allocData(size); }
	void* getExtData(CommandDataCache::DataHandle handle) { return m_extDataCache.getData(handle); }

	//void ResolveCombinedMaterials();

	void addDynamicLightInfo(DynamicLightInfo* lightInfo);
	const List<Ref<DynamicLightInfo>>& getDynamicLightList() const { return m_dynamicLightList; }

	// default settings
	void setDefaultRenderTarget(RenderTargetTexture* value) { m_defaultRenderTarget = value; }
	RenderTargetTexture* getDefaultRenderTarget() const { return m_defaultRenderTarget; }
	void setDefaultDepthBuffer(DepthBuffer* value) { m_depthBuffer = value; }
	DepthBuffer* getDefaultDepthBuffer() const { return m_depthBuffer; }

private:
	//void postAddCommandInternal(const DrawElementBatch& state, CommonMaterial* availableMaterial, const BuiltinEffectData& effectData, bool forceStateChange, DrawElement* element);

	CommandDataCache		m_commandDataCache;
	CommandDataCache		m_extDataCache;
	DrawElement*			m_lastElement = nullptr;

	//List<DrawElementBatch>	m_batchList;

	//detail::CombinedMaterialCache	m_combinedMaterialCache;
	std::unique_ptr<detail::RenderStageCache>	m_renderStageCache;
	List<Ref<DynamicLightInfo>>	m_dynamicLightList;

	Ref<RenderTargetTexture>		m_defaultRenderTarget;
	Ref<DepthBuffer>				m_depthBuffer;
};

} // namespace detail


class RenderDiagItem
{
public:
	RenderDiagItem();
	virtual ~RenderDiagItem() = default;
	virtual String toString() const;

private:
	enum class SubType
	{
		ScopeBegin,
		ScopeEnd,
		Command,
	};

	const Char* m_name;
	SubType	m_subType;
	friend class RenderDiag;
};

class RenderDiag
	: public Object
{
public:
	void clear();

	void beginRenderView();
	void endRenderView();
	void beginDrawList();
	void endDrawList();
	void changeRenderStage();
	void callCommonElement(const Char* typeName);

	template<typename TDiagItem>
	TDiagItem* callElement(const Char* typeName)
	{
		return instantiate<TDiagItem>(typeName);
	}

	void print();

private:

	class IDataCatch
	{
	public:
		virtual ~IDataCatch() = default;
		virtual void clear() = 0;
	};

	template<typename T>
	class DataCatch : public IDataCatch
	{
	public:
		virtual void clear() override
		{
			for (auto& p : m_using)
				m_storage.push_back(p);
			m_using.clear();
		}
		T* instantiate()
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
	T* instantiate(const Char* typeName)
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

		T* obj = cache->instantiate();
		obj->m_name = typeName;
		m_items.push_back(obj);
		return obj;
	}

	std::unordered_map<String, std::shared_ptr<IDataCatch>>	m_cacheMap;
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
	@brief	
*/
class DrawList	// TODO: Renderer
	: public Object
{
public:

	// TODO: MeshResource など、一度コマンド化したら実行されるまでは変更禁止状態にしたい。

	//--------------------------------------------------------------------------
	/** @name render targets */
	/** @{ */

	/** レンダリングターゲットを設定します。index 0 のレンダリングターゲットを変更すると、ビューポート領域とシザー領域がリセットされます。 */
	void setRenderTarget(int index, RenderTargetTexture* renderTarget);

	/** 現在設定されているレンダリングターゲットを取得します。*/
	RenderTargetTexture* getRenderTarget(int index) const;

	/** 深度バッファを設定します。*/
	void setDepthBuffer(DepthBuffer* depthBuffer);

	/** 現在設定されている深度バッファを取得します。*/
	DepthBuffer* getDepthBuffer() const;

	/** ビューポート領域を設定します。*/
	void setViewportRect(const RectI& rect);

	/** 現在設定されている ビューポート領域を取得します。*/
	const RectI& getViewportRect() const;

	/** @} */


	//--------------------------------------------------------------------------
	/** @name render status */
	/** @{ */
	void setOpacity(float opacity);

	void setBlendMode(Nullable<BlendMode> mode);

	void setCullingMode(Nullable<CullingMode> mode);

	void setDepthTestEnabled(Nullable<bool> enabled);

	void setDepthWriteEnabled(Nullable<bool> enabled);

	/** @} */

	void setTransform(const Matrix& transform);


	void setBrush(Brush* brush);
	Brush* getBrush() const;

	void setFont(Font* font);

	//void setShader(Shader* shader);
	//Shader* getShader() const;

	void clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);
	

	/**
		@brief		線分を描画します。
		@param[in]	position1	: 始点の位置
		@param[in]	color1		: 始点の色
		@param[in]	position2	: 終点の位置
		@param[in]	color2		: 終点の色
	*/
	void drawLinePrimitive(
		const Vector3& position1, const Color& color1,
		const Vector3& position2, const Color& color2);

	/**
		@brief
	*/
	void drawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,	// TODO: 順序
		const Vector3& position4, const Vector2& uv4, const Color& color4/*,
		ShaderPass* shaderPass*/);

	// TODO: plane かな
	void drawSquare(float sizeX, float sizeZ, int slicesX, int slicesZ, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, CommonMaterial* material = nullptr);
	
	void drawArc(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, CommonMaterial* material = nullptr);

	void drawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, CommonMaterial* material = nullptr);

	void drawSphere(float radius, int slices = 8, int stacks = 8, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity, CommonMaterial* material = nullptr);

	void drawCylinder(float radius, float height, int slices = 8, int stacks = 1, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity);

	void drawCone(float radius, float height, int slices = 8, const Color& color = Color::White, const Matrix& localTransform = Matrix::Identity);

	void drawMesh(MeshResource* mesh, int subsetIndex, CommonMaterial* material);
	//void drawMesh(StaticMeshModel* mesh, int subsetIndex, CommonMaterial* material);

	void drawSkinnedMesh(SkinnedMeshModel* mesh);

	void blit(Texture* source);
	void blit(Texture* source, const Matrix& transform);
	void blit(Texture* source, RenderTargetTexture* dest, const Matrix& transform);
	void blit(Texture* source, RenderTargetTexture* dest, CommonMaterial* material);

	void drawGlyphRun(const Point& position, GlyphRun* glyphRun);

	// 描画時、アルファブレンド無効であれば有効として描画される。
	void drawText_(const StringRef& text, const Point& position);
	void drawText_(const StringRef& text, const Rect& rect, StringFormatFlags flags);

	//void drawChar(uint32_t ch, const Point& position);
	void drawChar(uint32_t codePoint, const Rect& rect, StringFormatFlags flags);
	void drawText2(const StringRef& text, const Rect& rect);

	void drawSprite(
		const Vector3& position,
		const Size& size,
		const Vector2& anchor,
		Texture* texture,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType,
		CommonMaterial* material = nullptr);

	void drawRectangle(const Rect& rect);

	void drawScreenRectangle();

LN_INTERNAL_ACCESS:
	DrawList();
	virtual ~DrawList();
	void initialize(detail::GraphicsManager* manager);
	detail::GraphicsManager* getManager() const { return m_manager; }
	detail::DrawElementList* getDrawElementList() { return &m_drawElementList; }
	//void setDefaultMaterial(CommonMaterial* material);
	//void setBuiltinEffectData(const detail::BuiltinEffectData& data);
	void setVisualNodeParameters(detail::VisualNodeParameters* params);
	void beginMakeElements();

	//const detail::BatchStateBlock& getState() const { return m_state; }
	//void setState(const detail::BatchStateBlock& state) { m_state = state; }
	void addDynamicLightInfo(detail::DynamicLightInfo* lightInfo);
	void pushMetadata(const DrawElementMetadata* metadata);
	const DrawElementMetadata* getMetadata();
	void popMetadata();

	template<typename TElement> TElement* resolveDrawElement(detail::IRenderFeature* renderFeature, CommonMaterial* userMaterial, bool append = false);
	void drawMeshResourceInternal(MeshResource* mesh, int subsetIndex, CommonMaterial* material);
	//void DrawMeshSubsetInternal(StaticMeshModel* mesh, int subsetIndex, CommonMaterial* material);
	void blitInternal(Texture* source, RenderTargetTexture* dest, const Matrix& transform, CommonMaterial* material);
	void drawFrameRectangle(const Rect& rect);
	//void renderSubView(RenderView* listSet, detail::SceneRenderer* renderer = nullptr, RenderTargetTexture* defaultRenderTarget = nullptr, DepthBuffer* defaultDepthBuffer = nullptr);

	void pushState();
	void popState();

private:

	//class StagingState : public RefObject
	//{
	//public:
	//	detail::DrawElementBatch		m_state;
	//	Ref<CommonMaterial>				m_material;		// TODO: これは内容を書き換えないようにしたほうがよさそう。今の使い方てきに。
	//	detail::BuiltinEffectData		m_builtinEffectData;
	//	Ref<Shader>					m_defaultMaterialShader;

	//	StagingState();

	//	void reset();
	//	void copyFrom(const StagingState* state);

	//private:
	//	LN_DISALLOW_COPY_AND_ASSIGN(StagingState);
	//};

	detail::RenderStage* getCurrentState() const;
	bool checkSubmitRenderStage(int* stageId);

	detail::GraphicsManager*		m_manager;
	List<Ref<detail::RenderStage>>		m_freeStateStack;
	List<Ref<detail::RenderStage>>		m_aliveStateStack;	// size >= 1
	//Ref<CommonMaterial>				m_defaultMaterial;


	detail::DrawElementList			m_drawElementList;

	//detail::DrawElement*			m_currentSectionTopElement;
	//detail::DrawElementBatch		m_stateInSection;
	const DrawElementMetadata*		m_metadata;
	int								m_currentStateFence;
	detail::IRenderFeature*			m_lastRenderFeature;

	//Ref<detail::RenderTargetTextureCache>		m_renderTargetPool;
};


//------------------------------------------------------------------------------
template<typename TElement>
inline TElement* DrawList::resolveDrawElement(detail::IRenderFeature* renderFeature, CommonMaterial* userMaterial, bool append)
{
	// stage の更新チェック
	int stageId;
	if (checkSubmitRenderStage(&stageId))
	{
		append = false;	// stage が変わったので追記はできない
	}

	// 必要であれば追記チェック
	if (append)
	{
		detail::DrawElement* element = m_drawElementList.getLastElement();
		if (element &&
			element->renderFeature == renderFeature &&	// TODO: 種類
			element->m_stateFence == m_currentStateFence &&
			element->priorityMaterial == userMaterial
			)
		{
			return static_cast<TElement*>(element);
		}
	}

	const DrawElementMetadata* userMetadata = getMetadata();
	const DrawElementMetadata* metadata = (userMetadata != nullptr) ? userMetadata : &DrawElementMetadata::Default;

	// DrawElement を新しく作る
	TElement* element = m_drawElementList.addCommand<TElement>();
	element->renderFeature = renderFeature;
	element->metadata = *metadata;
	element->batchIndex = stageId;
	element->priorityMaterial = userMaterial;
	element->m_stateFence = m_currentStateFence;
	return element;


	////CommonMaterial* availableMaterial = m_defaultMaterial;// = (userMaterial != nullptr) ? userMaterial : getCurrentState()->m_defaultMaterial.get();
	////if (getCurrentState()->m_defaultMaterial != nullptr) availableMaterial = getCurrentState()->m_defaultMaterial;
	//CommonMaterial* availableMaterial = userMaterial;
	//if (availableMaterial == nullptr) availableMaterial = getCurrentState()->m_material;
	//if (availableMaterial == nullptr) availableMaterial = m_defaultMaterial;


	//// これを決定してから比較を行う
	////getCurrentState()->m_state.SetStandaloneShaderRenderer(renderFeature->isStandaloneShader());
	//getCurrentState()->m_state.setRenderFeature(renderFeature);

	//bool forceStateChange = (m_lastRenderFeature != renderFeature);


	//const DrawElementMetadata* userMetadata = getMetadata();
	//const DrawElementMetadata* metadata = (userMetadata != nullptr) ? userMetadata : &DrawElementMetadata::Default;


	//// 何か前回追加された DrawElement があり、それと DrawingSectionId、State が一致するならそれに対して追記できる
	//if (m_currentSectionTopElement != nullptr &&
	//	append &&
	//	m_lastRenderFeature == renderFeature &&
	//	m_currentSectionTopElement->metadata.equals(*metadata) &&
	//	m_currentSectionTopElement->m_stateFence == m_currentStateFence &&
	//	m_drawElementList.getBatch(m_currentSectionTopElement->batchIndex)->Equal(getCurrentState()->m_state, availableMaterial, getCurrentState()->m_builtinEffectData))
	//{
	//	return static_cast<TElement*>(m_currentSectionTopElement);
	//}

	//m_lastRenderFeature = renderFeature;

	//// DrawElement を新しく作る
	//TElement* element = m_drawElementList.addCommand<TElement>(getCurrentState()->m_state, availableMaterial, getCurrentState()->m_builtinEffectData, forceStateChange);
	//element->metadata = *metadata;
	//element->m_stateFence = m_currentStateFence;
	//m_currentSectionTopElement = element;
	//return element;
}


LN_NAMESPACE_END

