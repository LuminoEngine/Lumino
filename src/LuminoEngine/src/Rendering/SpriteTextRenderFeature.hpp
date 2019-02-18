#pragma once
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

		// Cache から取りだすデータ
		int outlineOffset;
		RectI srcRect;
	};

	struct BrushData
	{
		Color color;
	};

	InternalSpriteTextRender();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }
	void render(IGraphicsDeviceContext* context, const GlyphData* dataList, int dataCount, ITexture* glyphsTexture, const BrushData& brushData);

private:
	void prepareBuffers(int spriteCount);
	void internalDrawRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color);
	void flush(IGraphicsDeviceContext* context, ITexture* glyphsTexture);

	RenderingManager* m_manager;
	Ref<IVertexDeclaration> m_vertexDeclaration;
	Ref<IVertexBuffer> m_vertexBuffer;
	Ref<IIndexBuffer> m_indexBuffer;
	size_t m_spriteCount;
	size_t m_buffersReservedSpriteCount;
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

	void drawText(GraphicsContext* context, const FormattedText* text);

protected:
	virtual void flush(GraphicsContext* context) override;
    virtual void updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo) override;
	virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

private:
	void flushInternal(GraphicsContext* context, FontGlyphTextureCache* cache);

	Ref<InternalSpriteTextRender> m_internal;
	GraphicsContext* m_drawingGraphicsContext;
	FontCore* m_drawingFont;
	FontGlyphTextureCache* m_drawingFontGlyphCache;
	std::vector<InternalSpriteTextRender::GlyphData> m_glyphLayoutDataList;
};

class DrawTextElement : public RenderDrawElement
{
public:
    Ref<detail::FormattedText> formattedText;

    virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
    {
        static_cast<detail::SpriteTextRenderFeature*>(renderFeatures)->drawText(context, formattedText);
    }
};

} // namespace detail
} // namespace ln

