
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Graphics.h>
#include <Lumino/Rendering/Common.h>
#include <Lumino/Graphics/Material.h>

LN_NAMESPACE_BEGIN

namespace detail {
class RenderStageCache;

// VisualNode が持つステート関係は全てここにまとめる。
// 内部的には、VisualNode からは RenderingContext の public ステート変更メソッドを呼ばない。
// これはユーザーが VisualNote::onRender() で自由に RenderingContext を使えるようにするため。
class VisualNodeParameters
	: public RefObject
{
public:
	Ref<CommonMaterial>			material;
	BuiltinEffectData			builtinEffectData;
	Matrix						transfrom;

	Nullable<BlendMode>			blendMode;
	Nullable<CullingMode>		cullingMode;
	Nullable<bool>				depthTestEnabled;
	Nullable<bool>				depthWriteEnabled;
	Nullable<ShadingModel>		shadingModel;
};

class RenderingContextParameters
	: public RefObject
{
public:
	void reset();
	void copyFrom(const RenderingContextParameters* other);

	void setRenderTarget(int index, RenderTargetTexture* renderTarget);
	RenderTargetTexture* getRenderTarget(int index) const { return m_renderTargets[index]; }

	void setDepthBuffer(DepthBuffer* depthBuffer);
	DepthBuffer* getDepthBuffer() const { return m_depthBuffer; }

	void setViewportRect(const RectI& rect);
	const RectI& getViewportRect() const { return m_viewportRect; }

	void setScissorRect(const RectI& scissorRect);
	const RectI& getScissorRect() const { return m_scissorRect; }

	void setBlendMode(const Nullable<BlendMode>& mode);
	const Nullable<BlendMode>& getBlendMode() const { return m_blendMode; }

	void setCullingMode(const Nullable<CullingMode>& mode);
	const Nullable<CullingMode>& getCullingMode() const { return m_cullingMode; }

	void setDepthTestEnabled(const Nullable<bool>& enabled);
	const Nullable<bool>& isDepthTestEnabled() const { return m_depthTestEnabled; }

	void setDepthWriteEnabled(const Nullable<bool>& enabled);
	const Nullable<bool>& isDepthWriteEnabled() const { return m_depthWriteEnabled; }

	void setTransform(const Matrix& value);
	const Matrix& getTransform() const { return m_transfrom; }

	void setMaterial(CommonMaterial* value);
	CommonMaterial* getMaterial() const { return m_material; }

	void setBrush(Brush* brush);
	Brush* getBrush() const;

	void setFont(Font* font);
	Font* getFont() const;

	//void setPriorityMaterial(CommonMaterial* value);
	//CommonMaterial* getPriorityMaterial() const { return m_priorityMaterial; }

private:
	uint32_t getHashCode() const;

	// base params
	Ref<RenderTargetTexture>	m_renderTargets[Graphics::MaxMultiRenderTargets];
	Ref<DepthBuffer>			m_depthBuffer;
	RectI						m_viewportRect;
	RectI						m_scissorRect;
	Nullable<BlendMode>			m_blendMode;
	Nullable<CullingMode>		m_cullingMode;
	Nullable<bool>				m_depthTestEnabled;
	Nullable<bool>				m_depthWriteEnabled;
	Matrix						m_transfrom;
	Ref<CommonMaterial>			m_material;			// setMaterial() で指定されたマテリアル

	// drawing params
	Ref<Brush>					m_brush;
	Ref<Font>					m_font;

	// DrawElement params
	//Ref<CommonMaterial>			m_priorityMaterial;	// 各種 draw() の引数で明示されたマテリアル

	mutable size_t				m_hashCode;
	mutable bool				m_hashDirty;
};

class RenderStage
	: public Object
{
public:
	Ref<VisualNodeParameters>	visualNodeParameters;
	RenderingContextParameters	renderingContextParameters;	// TODO: これも nullable でいいかも。null だったら、一つ前の stage のものを参照するとか

public:
	// frontend ----------------------------------------
	void reset();
	void copyFrom(const RenderStage* other);
	bool equals(const RenderStage* other) const;
	int getId() const { return m_id; }

	// backend  ----------------------------------------
	void combineParameters();
	RenderTargetTexture* getRenderTargetFinal(int index) const { return renderingContextParameters.getRenderTarget(index); }
	DepthBuffer* getDepthBufferFinal() const { return renderingContextParameters.getDepthBuffer(); }
	const RectI& getViewportRectFinal() const { return renderingContextParameters.getViewportRect(); }
	const RectI& getScissorRectFinal() const { return renderingContextParameters.getScissorRect(); }
	BlendMode getBlendModeFinal() const;
	CullingMode getCullingModeFinal() const;
	bool isDepthTestEnabledFinal() const;
	bool isDepthWriteEnabledFinal() const;
	const Matrix& getTransformFinal() const { return m_combinedWorldMatrix; }
	CommonMaterial* getMaterialFinal(CommonMaterial* defaultValue, CommonMaterial* priorityValue) const;
	Brush* getBrushFinal() const { return renderingContextParameters.getBrush(); }
	Font* getFontFinal() const { return renderingContextParameters.getFont(); }
	//CommonMaterial* getPriorityMaterialFinal() const { return renderingContextParameters.getPriorityMaterial(); }
	// BuiltinEffectData
	Shader* getShaderFinal(CommonMaterial* finalMaterial) const;	// getMaterialFinal() で確定した Material を渡すこと
	float getOpacityFinal() const { return (visualNodeParameters) ? visualNodeParameters->builtinEffectData.getOpacity() : BuiltinEffectData::DefaultData.getOpacity(); }
	const Color& getColorScaleFinal() const { return (visualNodeParameters) ? visualNodeParameters->builtinEffectData.getColorScale() : BuiltinEffectData::DefaultData.getColorScale(); }
	const Color& getBlendColorFinal() const { return (visualNodeParameters) ? visualNodeParameters->builtinEffectData.getBlendColor() : BuiltinEffectData::DefaultData.getBlendColor(); }
	const ToneF& getToneFinal() const { return (visualNodeParameters) ? visualNodeParameters->builtinEffectData.getTone() : BuiltinEffectData::DefaultData.getTone(); }
	ShadingModel getShadingModelFinal(CommonMaterial* finalMaterial) const;	// getMaterialFinal() で確定した Material を渡すこと

LN_CONSTRUCT_ACCESS:
	RenderStage();
	virtual ~RenderStage();
	void initialize(int id);
	
private:
	int		m_id;
	Matrix	m_combinedWorldMatrix;
};

class RenderStageCache
{
public:
	RenderStageCache();
	void initialize(int reserved);
	void clear();
	RenderStage* request();
	RenderStage* get(int id) const;

private:
	List<Ref<RenderStage>>	m_instanceList;
	int						m_used;
};

} // namespace detail
LN_NAMESPACE_END
