#pragma once
#include "FontCore.hpp"

namespace ln {
class Font;
namespace detail {
class FontCore;
class FlexText;
class FontRequester;

class TextRun
	: public RefObject
{
public:

};

// 簡易的な paragraph のイメージ。
class FormattedText
	: public RefObject
{
public:
	String text;
	Ref<Font> font;	// TODO: obsolete
	Color color;
	Rect area;
	TextAlignment textAlignment = TextAlignment::Forward;

	Ref<FontRequester> fontRequester;
};



struct FlexGlyph
{
	uint32_t codePoint;
	Vector3 pos;
	float opacity = 1.0f;
	float timeOffset;
};

struct FlexGlyphRun
{
	FlexText* owner = nullptr;
	int startIndex = 0;
	int glyphCount = 0;
	Ref<Font> font;	// TODO: これ Font でいい気がする
	Color color;
	// TODO: ルビとかのサブグリフ用に startIndex, count を用意しておく
};

class FlexText
	: public RefObject
{
public:
	void clear()
	{
		m_glyphs.clear();
		m_glyphRuns.clear();
	}

	void addGlyphRun(FlexGlyph* glyphs, int glyphsCount, Font* font, const Color& color)
	{
		FlexGlyphRun run;
		run.owner = this;
		run.startIndex = m_glyphs.size();
		run.glyphCount = glyphsCount;
		run.font = font;
		run.color = color;
		m_glyphs.resize(run.startIndex + glyphsCount);
		for (int i = 0; i < glyphsCount; i++) {
			m_glyphs[run.startIndex + i] = glyphs[i];
		}
		m_glyphRuns.push_back(run);
	}

	void copyFrom(const FlexText* other)
	{
		m_glyphs = other->m_glyphs;
		m_glyphRuns = other->m_glyphRuns;
	}

	const std::vector<FlexGlyph>& glyphs() const { return m_glyphs; }
	std::vector<FlexGlyphRun>& glyphRuns() { return m_glyphRuns; }

	void setTime(float value) { m_time = value; }
	float time() const { return m_time; }

private:
	std::vector<FlexGlyph> m_glyphs;	// 配置は TextLayoutEngine でやっておくこと。
	std::vector<FlexGlyphRun> m_glyphRuns;
	float m_time;
	//Ref<Font> m_font;
	// TODO: effectFunction
};

// TODO: 入力は paragraph 単位がいいだろう。単一の装飾とテキスト範囲を表す TextRun のを受け取るイメージ。
class TextLayoutEngine
{
public:
    TextLayoutEngine();

    void layout(FontCore* font, const Char* text, size_t length, const Rect& targetArea, float strokeSize, TextAlignment alignment);

protected:
	const Size& renderAreaSize() const { return m_renderAreaSize; }

    // pos : upper left
	// デフォルトは Font クラスから取得したメトリクスを直接使って測定する。もし RenderingContext 側でストロークサイズ(Pen)などを考慮したい場合、これをオーバーライドする。
	// outSizeOffset : 元のサイズから、上下左右にどれだけ大きくなるか。通常はストロークサイズをそのまま指定する。
	//virtual void onMeasureGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix, Size* outSizeOffset);
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) = 0;

private:
	enum class LayoutMode
	{
		Measure,
		Placement,
	};
	
	struct LayoutItem
	{
		UTF32 ch;
		Vector2 pos;
		Size size;
	};

	struct LayoutLine
	{
		List<LayoutItem> glyphs;
	};

    UTF32 readChar();
	void resetStream();
    void layoutTextHorizontal(LayoutMode mode);
    bool layoutLineHorizontal(float baselineY, LayoutMode mode);
	void placementTextHorizontal();
	void placementLineHorizontal(const LayoutLine& layoutLine);
	void calculateRenderAreaHorizontalOffset(LayoutLine* layoutLine);

    FontCore* m_font;
    const Char* m_text;
    size_t m_length;
    size_t m_pos;
    Rect m_targetArea;
	Size m_renderAreaSize;
	Vector2 m_renderAreaOffset;
	float m_strokeSize;
    TextAlignment m_alignment;
    FontGlobalMetrics m_globalMetrics;
	List<LayoutLine> m_layoutLines;
};

// テキスト描画領の域計算用
class MeasureTextLayoutEngine
    : public TextLayoutEngine
{
public:
    Size areaSize;

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;
};

class BitmapTextRenderer
    : public TextLayoutEngine
{
public:
    void render(Bitmap2D* bitmap, const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment);

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

private:
    Bitmap2D* m_bitmap;
    Rect m_rect;
    Color m_color;
    FontCore* m_font;
};

} // namespace detail
} // namespace ln

