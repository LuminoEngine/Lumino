
#pragma once
#include <Lumino/Imaging/Font.h>

namespace Lumino
{
namespace Imaging
{

/// テキストの配置方法
enum TextAlignment
{
	TextAlignment_Left = 0,			///< 左揃え
	TextAlignment_Center,			///< 中央揃え
	TextAlignment_Right,			///< 右揃え
	TextAlignment_Justify,			///< 両端揃え
};

/// 描画領域にテキストが収まりきらない場合に、テキストを切り取る方法
enum TextTrimming
{
	TextTrimming_None,				///< 切り取りを行わない。
	TextTrimming_CharacterEllipsis,	///< 略記号(...) を描画する。
};

/// テキストの描画方向
enum FlowDirection
{
	FlowDirection_LeftToRight,		///< テキストを左から右に描画する。
	FlowDirection_RightToLeft,		///< テキストを左から右に描画する。
};

class TextLayoutEngine
{
public:
	TextLayoutEngine(Font* font);
	~TextLayoutEngine();

public:
	void SetFont(Font* font) { m_font = font; }
	void SetForeColor(Graphics::Color color) { m_foreColor = color; }
	void SetStrokeColor(Graphics::Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetDrawingArea(const Rect& area) { m_drawingArea = area; }

	void LayoutText(const UTF32* text, int length, bool takeOverKerning);

protected:
	/// ptLeftTop:DrawingArea の左上を原点とした、グリフの配置先左上座標 (配置方法によってはマイナス値になることもある)
	virtual void OnPlaceChar(UTF32 ch, Point ptLeftTop) = 0;

private:
	void LayoutTextHorizontal(const UTF32* text, int length);
	void LayoutLineHorizontal(const UTF32* text, int length, const Rect& lineArea);

private:
	Bitmap*			m_targetBitmap;
	RefPtr<Font>	m_font;
	Graphics::Color	m_foreColor;
	Graphics::Color	m_strokeColor;
	int				m_strokeSize;
	TextAlignment	m_textAlignment;
	TextTrimming	m_textTrimming;
	FlowDirection	m_flowDirection;
	Rect			m_drawingArea;
	ByteBuffer		m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)
	FontGlyphData*	m_prevGlyphData;

};

} // namespace Imaging
} // namespace Lumino
