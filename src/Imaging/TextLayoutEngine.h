
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Imaging/Font.h>

namespace Lumino
{
namespace Imaging
{

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


/// テキストの配置方法
LN_ENUM(TextAlignment)
{
	Left = 0,				///< 左揃え
	Center,					///< 中央揃え
	Right,					///< 右揃え
	Justify,				///< 両端揃え
};
LN_ENUM_DECLARE(TextAlignment)

/// 描画領域にテキストが収まりきらない場合に、テキストを切り取る方法
LN_ENUM(TextTrimming)
{
	None = 0,				///< 切り取りを行わない。
	CharacterEllipsis,		///< 略記号(...) を描画する。
};
LN_ENUM_DECLARE(TextTrimming)

/// テキストの描画方向
LN_ENUM(FlowDirection)
{
	LeftToRight = 0,		///< テキストを左から右に描画する。
	RightToLeft,			///< テキストを左から右に描画する。
	TopToBottom,			///< テキストを上から下に描画する。 (試験実装)
};
LN_ENUM_DECLARE(FlowDirection)

struct TextLayoutResultItem
{
	UTF32	Char;
	Imaging::FontGlyphLocation	Location;
};

struct TextLayoutResult
{
	Size						AreaSize;
	Array<TextLayoutResultItem>	Items;
};

class TextLayoutEngine
{
public:
	TextLayoutEngine();
	~TextLayoutEngine();

public:
	void SetFont(Font* font) { m_font = font; }
	//void SetForeColor(Graphics::Color color) { m_foreColor = color; }
	//void SetStrokeColor(Graphics::Color color) { m_strokeColor = color; }
	//void SetStrokeSize(int size) { m_strokeSize = size; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetDrawingArea(const Rect& area) { m_drawingArea = area; }

	void LayoutText(const UTF32* text, int length, TextLayoutResult* outResult/*, bool takeOverKerning*/);

protected:
	/// ptLeftTop:DrawingArea の左上を原点とした、グリフの配置先左上座標 (配置方法によってはマイナス値になることもある)
	//virtual void OnPlaceChar(UTF32 ch, Point ptLeftTop) = 0;

private:
	void LayoutTextHorizontal(const UTF32* text, int length);
	void LayoutLineHorizontal(const UTF32* text, int length, const Rect& lineArea, Size* outLineSize);

private:
	Bitmap*				m_targetBitmap;
	RefPtr<Font>		m_font;
	//Graphics::Color	m_foreColor;
	//Graphics::Color	m_strokeColor;
	//int				m_strokeSize;
	TextAlignment		m_textAlignment;
	TextTrimming		m_textTrimming;
	FlowDirection		m_flowDirection;
	Rect				m_drawingArea;
	//ByteBuffer			m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)
	FontGlyphData*		m_prevGlyphData;
	TextLayoutResult*			m_result;

};

} // namespace Imaging
} // namespace Lumino
