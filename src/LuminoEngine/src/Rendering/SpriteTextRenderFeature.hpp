#pragma once
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../Font/FontGlyphCache.hpp"
#include "../Font/TextLayoutEngine.hpp"
#include "RenderStage.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//#ifdef LN_RENDERING_MIGRATION
#if 1
	
// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class SpriteTextRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	SpriteTextRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("SpriteTextRenderFeatureStageParameters"))
	{
	}

	virtual bool equals(const RenderFeatureStageParameters* other) override
	{
		if (typeId() != other->typeId()) return false;
		if (this == other) return true;
		return true;
	}

	virtual void copyTo(RenderFeatureStageParameters* params) override
	{
		LN_CHECK(typeId() == params->typeId());
	}

private:
};

class SpriteTextRenderFeature
	: public RenderFeature
	, public TextLayoutEngine
{
public:
	SpriteTextRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult drawText(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, const FormattedText* text, const Matrix& transform);
	RequestBatchResult drawChar(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Font* font, uint32_t codePoint, const Color& color, const Matrix& transform);
	RequestBatchResult drawFlexGlyphRun(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Font* font, const FlexGlyphRun* glyphRun, const Matrix& transform);

protected:
	// RenderFeature interface
	virtual void beginRendering() override;
    virtual void endRendering() override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
    virtual bool drawElementTransformNegate() const override { return true; }

	// TextLayoutEngine interface
	virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

private:
	struct GlyphData
	{
		Matrix transform;
		Vector2 position;
		Color color;
	};

	struct BatchData
	{
		int spriteOffset;
		int spriteCount;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	// TODO: バッファの準備周りは spriteRenderer と同じ。共通化した方がいいかも
	void prepareBuffers(GraphicsContext* context, int spriteCount);
	RequestBatchResult updateCurrentFontAndFlushIfNeeded(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Font* newFont);
	void beginLayout();
	void addLayoutedGlyphItem(uint32_t codePoint, const Vector2& pos, const Color& color, const Matrix& transform);
	RequestBatchResult resolveCache(detail::RenderFeatureBatchList* batchList, GraphicsContext* context);
	void endLayout(GraphicsContext* context);
	static void putRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color);

	//
	//void flushInternal(GraphicsContext* context, FontGlyphTextureCache* cache);

	RenderingManager* m_manager;
	FontCore* m_currentFont;

	// sprite-batching
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	int m_buffersReservedSpriteCount;
	BatchData m_batchData;
	Vertex* m_mappedVertices;

	FontGlyphTextureCacheRequest m_cacheRequest;	// 1フレーム中繰り返し使う可変バッファ
	std::vector<GlyphData> m_glyphLayoutDataList;	// 1フレーム中繰り返し使う可変バッファ
	Texture* m_cacheTexture;
    std::vector<Ref<FontCore>> m_renderingFonts;    // バッチ生成～描画終了まで参照を持って、終了時にキャッシュ終了を通知したい

	// for onPlacementGlyph
	const FormattedText* m_drawingFormattedText;
	Matrix m_drawingTransform;

	//Ref<InternalSpriteTextRender> m_internal;
	//GraphicsContext* m_drawingGraphicsContext;
	//const FormattedText* m_drawingFormattedText;
 //   Matrix m_drawingTransform;
	//FontCore* m_drawingFont;
	////FontGlyphTextureCache* m_drawingFontGlyphCache;
	//std::vector<InternalSpriteTextRender::GlyphData> m_glyphLayoutDataList;
};

#else

class InternalSpriteTextRender
    : public RefObject
{
public:
	struct GlyphData
	{
		Matrix transform;
		Vector2 position;
		Color color;

		// Cache から取りだすデータ
		int outlineOffset;
		RectI srcRect;
	};

	InternalSpriteTextRender();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }
	void render(ICommandList* context, const GlyphData* dataList, uint32_t dataCount, ITexture* glyphsTexture);

private:
	void prepareBuffers(int spriteCount);
	void putRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color);
	void flush(ICommandList* context, ITexture* glyphsTexture, uint32_t startIndex, uint32_t primitiveCount);

	RenderingManager* m_manager;
	Ref<IVertexDeclaration> m_vertexDeclaration;
	Ref<IVertexBuffer> m_vertexBuffer;
	Ref<IIndexBuffer> m_indexBuffer;

	uint32_t m_buffersReservedSpriteCount;
	uint32_t m_stagingSpriteOffset;
	uint32_t m_stagingSpriteCount;
};

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class SpriteTextRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	SpriteTextRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("SpriteTextRenderFeatureStageParameters"))
	{
	}

	virtual bool equals(const RenderFeatureStageParameters* other) override
	{
		if (typeId() != other->typeId()) return false;
		if (this == other) return true;
		return true;
	}

	virtual void copyTo(RenderFeatureStageParameters* params) override
	{
		LN_CHECK(typeId() == params->typeId());
	}

private:
};

class SpriteTextRenderFeature
	: public RenderFeature
	, public TextLayoutEngine
{
public:
	SpriteTextRenderFeature();
	void init(RenderingManager* manager);

	void drawText(GraphicsContext* context, const FormattedText* text, const Matrix& transform);
	void drawChar(GraphicsContext* context, uint32_t codePoint, const Color& color, const Matrix& transform);
	void drawFlexGlyphRun(GraphicsContext* context, const FlexGlyphRun* glyphRun, const Matrix& transform);

protected:
	// RenderFeature interface
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
    virtual void updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo) override;
    virtual bool drawElementTransformNegate() const override { return true; }

	// TextLayoutEngine interface
	virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

private:
	void addLayoutedGlyphItem(uint32_t codePoint, const Vector2& pos, const Color& color, const Matrix& transform);
	void flushInternal(GraphicsContext* context, FontGlyphTextureCache* cache);

	Ref<InternalSpriteTextRender> m_internal;
	GraphicsContext* m_drawingGraphicsContext;
	const FormattedText* m_drawingFormattedText;
    Matrix m_drawingTransform;
	FontCore* m_drawingFont;
	//FontGlyphTextureCache* m_drawingFontGlyphCache;
	std::vector<InternalSpriteTextRender::GlyphData> m_glyphLayoutDataList;
};
#endif


class AbstractSpriteTextDrawElement : public RenderDrawElement
{
public:
	//virtual FontCore* getFontCore(float dpiScale) = 0;
};

class DrawTextElement : public AbstractSpriteTextDrawElement
{
public:
    Ref<detail::FormattedText> formattedText;
	detail::FlexGlyphRun* glyphRun = nullptr;	// TODO: RefObj
	//Ref<Font> font;

	//virtual FontCore* getFontCore(float dpiScale) override
	//{
	//	if (glyphRun) {
	//		return glyphRun->font;
	//	}
	//	else {
	//		return FontHelper::resolveFontCore(formattedText->font, dpiScale);
	//	}
	//}

	virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
	{
		if (glyphRun) {
			return static_cast<detail::SpriteTextRenderFeature*>(renderFeature)->drawFlexGlyphRun(batchList, context, glyphRun->font, glyphRun, combinedWorldMatrix());
		}
		else {
			return static_cast<detail::SpriteTextRenderFeature*>(renderFeature)->drawText(batchList, context, formattedText, combinedWorldMatrix());
		}
    }
};

class DrawCharElement : public AbstractSpriteTextDrawElement
{
public:
	uint32_t codePoint;
	Ref<Font> font;
	Color color;

	//virtual FontCore* getFontCore(float dpiScale) override
	//{
	//	return FontHelper::resolveFontCore(font, dpiScale);
	//}

	virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
	{
		return static_cast<detail::SpriteTextRenderFeature*>(renderFeature)->drawChar(batchList, context, font, codePoint, color, combinedWorldMatrix());
	}
};


//class DrawFlexGlyphRunElement : public RenderDrawElement
//{
//public:
//	//Ref<detail::FlexText> flexText;
//
//	virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
//	{
//	}
//};


} // namespace detail
} // namespace ln

