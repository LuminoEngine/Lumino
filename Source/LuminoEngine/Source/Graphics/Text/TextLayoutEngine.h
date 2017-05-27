
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Text/Font.h>

LN_NAMESPACE_BEGIN
namespace detail {

///// テキストの配置方法
//enum TextAlignment
//{
//	TextAlignment_Left = 0,			///< 左揃え
//	TextAlignment_Center,			///< 中央揃え
//	TextAlignment_Right,			///< 右揃え
//	TextAlignment_Justify,			///< 両端揃え
//};
//
///// 描画領域にテキストが収まりきらない場合に、テキストを切り取る方法
//enum TextTrimming
//{
//	TextTrimming_None,				///< 切り取りを行わない。
//	TextTrimming_CharacterEllipsis,	///< 略記号(...) を描画する。
//};
//
///// テキストの描画方向
//enum FlowDirection
//{
//	FlowDirection_LeftToRight,		///< テキストを左から右に描画する。
//	FlowDirection_RightToLeft,		///< テキストを左から右に描画する。
//	FlowDirection_TopToBottom,		///< テキストを左から右に描画する。(試験中)
//};


enum class LayoutTextOptions
{
	All,
	RenderSizeOnly,
};

class TextLayoutEngine
{
public:
	TextLayoutEngine();
	~TextLayoutEngine();

public:
	void SetFont(RawFont* font) { m_font = font; }
	RawFont* GetFont() const { return m_font; }
	//void SetForeColor(Graphics::Color color) { m_foreColor = color; }
	//void SetStrokeColor(Graphics::Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; }
	int GetStrokeSize() const { return m_strokeSize; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetDrawingArea(const RectI& area) { m_drawingArea = area; }

	void ResetSettings();

	void LayoutText(const UTF32* text, int length, LayoutTextOptions options, TextLayoutResult* outResult/*, bool takeOverKerning*/);

protected:
	/// ptLeftTop:DrawingArea の左上を原点とした、グリフの配置先左上座標 (配置方法によってはマイナス値になることもある)
	//virtual void OnPlaceChar(UTF32 ch, PointI ptLeftTop) = 0;

private:
	void LayoutTextHorizontal(const UTF32* text, int length);
	void LayoutLineHorizontal(const UTF32* text, int length, const RectI& lineArea, SizeI* outLineSize);

private:
	RawFont*			m_font;
	//Graphics::Color	m_foreColor;
	//Graphics::Color	m_strokeColor;
	int					m_strokeSize;
	TextAlignment		m_textAlignment;
	TextTrimming		m_textTrimming;
	FlowDirection		m_flowDirection;
	RectI				m_drawingArea;
	//ByteBuffer			m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)
	
	// レイアウト処理中に使う
	FontGlyphData*		m_prevGlyphData;
	TextLayoutResult*	m_result;
	LayoutTextOptions	m_layoutTextOptions;
};






class AbstractTextLayoutEngine
{
public:
	struct ResultItem
	{
		UTF32				ch;
		float				columnBaseline;	// 横書きの場合はX方向のベースラインの位置  https://www.freetype.org/freetype2/docs/tutorial/step2.html
		float				lineBaseline;	// 横書きの場合はY方向のベースラインの位置
	};

	struct ResultData
	{
		Size				areaSize;
		List<ResultItem>	items;
	};

protected:
	void Layout(RawFont* font, const UTF32* text, int length, const Rect& layoutArea, TextLayoutOptions options);
	virtual void OnPlacementChar(const ResultItem& item) = 0;

private:
	void LayoutTextHorizontal(const UTF32* text, int length);
	void LayoutLineHorizontal(const UTF32* text, int length);

	RawFont*			m_font;
	FontGlobalMertics	m_globalMetrics;
	TextLayoutOptions	m_options;
	float				m_currentLineBaseline;
};


class TextLayoutEngine2 : public AbstractTextLayoutEngine
{
public:
	void Layout(RawFont* font, const UTF32* text, int length, const Rect& layoutArea, TextLayoutOptions options, ResultData* outResult);

protected:
	virtual void OnPlacementChar(const ResultItem& item);

private:
	ResultData*			m_result;
};


} // namespace detail
LN_NAMESPACE_END
