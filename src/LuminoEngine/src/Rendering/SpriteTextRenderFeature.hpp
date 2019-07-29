#pragma once
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../Font/FontGlyphCache.hpp"
#include "../Font/TextLayoutEngine.hpp"
#include "RenderStage.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {



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
	void render(IGraphicsContext* context, const GlyphData* dataList, uint32_t dataCount, ITexture* glyphsTexture);

private:
	void prepareBuffers(int spriteCount);
	void putRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color);
	void flush(IGraphicsContext* context, ITexture* glyphsTexture, uint32_t startIndex, uint32_t primitiveCount);

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
	virtual void flush(GraphicsContext* context) override;
    virtual void updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo) override;
	virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;
    virtual bool drawElementTransformNegate() const override { return true; }

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

class AbstractSpriteTextDrawElement : public RenderDrawElement
{
public:
	virtual FontCore* getFontCore(float dpiScale) = 0;
};

class DrawTextElement : public AbstractSpriteTextDrawElement
{
public:
    Ref<detail::FormattedText> formattedText;
	detail::FlexGlyphRun* glyphRun = nullptr;	// TODO: RefObj

	virtual FontCore* getFontCore(float dpiScale) override
	{
		if (glyphRun) {
			return glyphRun->font;
		}
		else {
			return FontHelper::resolveFontCore(formattedText->font, dpiScale);
		}
	}

    virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
    {
		if (glyphRun) {
			static_cast<detail::SpriteTextRenderFeature*>(renderFeatures)->drawFlexGlyphRun(context, glyphRun, combinedWorldMatrix());
		}
		else {
			static_cast<detail::SpriteTextRenderFeature*>(renderFeatures)->drawText(context, formattedText, combinedWorldMatrix());
		}
    }
};

class DrawCharElement : public AbstractSpriteTextDrawElement
{
public:
	uint32_t codePoint;
	Ref<Font> font;
	Color color;

	virtual FontCore* getFontCore(float dpiScale) override
	{
		return FontHelper::resolveFontCore(font, dpiScale);
	}

	virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
	{
		static_cast<detail::SpriteTextRenderFeature*>(renderFeatures)->drawChar(context, codePoint, color, combinedWorldMatrix());
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

