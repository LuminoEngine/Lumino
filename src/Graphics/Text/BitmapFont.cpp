
#include "../Internal.h"
#include <algorithm>
#include <Lumino/Graphics/Bitmap.h>
#include "../GraphicsManager.h"
#include "FontManager.h"
#include "BitmapFont.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// RawFont
//==============================================================================

static const byte_t g_BuiltInBitmapFont_size7_Data[] =
{
#include "../../Graphics/Resource/BitmapFont1_h7_6x15.png.h"
};
static const size_t g_BuiltInBitmapFont_size7_Len = LN_ARRAY_SIZE_OF(g_BuiltInBitmapFont_size7_Data);

//------------------------------------------------------------------------------
RawFont* RawFont::CreateBuiltInBitmapFontInternal(detail::FontManager* manager, int size)
{
	MemoryStream stream(g_BuiltInBitmapFont_size7_Data, g_BuiltInBitmapFont_size7_Len);
	RefPtr<Bitmap> bitmap(LN_NEW Bitmap(&stream), false);
	return LN_NEW detail::BitmapFont(manager, bitmap);
}

//------------------------------------------------------------------------------
void RawFont::RegisterFontFile(const StringRef& filePath)
{
	detail::GraphicsManager::GetInstance()->GetFontManager()->RegisterFontFile(filePath);
}

namespace detail {

//==============================================================================
// BitmapFont
//==============================================================================

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
BitmapFont::BitmapFont(FontManager* manager, Bitmap* bitmap)
	: m_manager(NULL)
	, m_fontBitmap()
	, m_charWidth(0)
	, m_charHeight(0)
{
	LN_REFOBJ_SET(m_manager, manager);
	m_name = String::SPrintf(_T("%d"), rand());	// TODO: 名前がユーザー指定されていなければランダムに作る
	m_fontBitmap = bitmap;

	m_charWidth = m_fontBitmap->GetSize().width / 16;
	m_charHeight = m_fontBitmap->GetSize().height / 16;

	// グリフ用の一時ビットマップ
	m_glyphBitmap.Attach(LN_NEW Bitmap(SizeI(m_charWidth, m_charHeight), PixelFormat::R8G8B8A8));

	m_fontGlyphBitmap.GlyphBitmap = m_glyphBitmap;
	m_fontGlyphBitmap.OutlineBitmap = NULL;
	m_fontGlyphBitmap.OutlineOffset = 0;
}

//------------------------------------------------------------------------------
BitmapFont::~BitmapFont()
{
	LN_SAFE_RELEASE(m_manager);
}

#if 0
//------------------------------------------------------------------------------
void BitmapFont::getTextSize(const char* text, int len, Geometry::Rect* rect)
{
	len = (len <= -1) ? len = strlen(text) : len;

	const char* pos = text;
	const char* end = text + len;
	int maxLineLength = 0;
	int lineCount = 0;
	while (pos < end)
	{
		++lineCount;

		// 次の改行または末尾を探す
		const char* nlPos = Base::StringUtils::findNextNewLine(pos, end);

		// 1行の長さ
		int lineLen = nlPos - pos;
		maxLineLength = std::max(lineLen, maxLineLength);

		// 改行文字列分進める (end や \0 等を指しているときは 0)
		pos += Base::StringUtils::checkNewLineSequence(nlPos, end);
		pos++;
	}

	// 一番長い行 * 行数が描画領域
	rect->set(0, 0, m_charWidth * maxLineLength, m_charHeight * lineCount);
}

//------------------------------------------------------------------------------
void BitmapFont::getTextSize(const wchar_t* text, int len, Geometry::Rect* rect)
{
	len = (len <= -1) ? len = wcslen(text) : len;

	const wchar_t* pos = text;
	const wchar_t* end = text + len;
	int maxLineLength = 0;
	int lineCount = 0;
	while (pos < end)
	{
		++lineCount;

		// 次の改行または末尾を探す
		const wchar_t* nlPos = Base::StringUtils::findNextNewLine(pos, end);

		// 1行の長さ
		int lineLen = nlPos - pos;
		maxLineLength = std::max(lineLen, maxLineLength);

		// 改行文字列分進める (end や \0 等を指しているときは 0)
		pos += Base::StringUtils::checkNewLineSequence(nlPos, end);
		pos++;
	}

	// 一番長い行 * 行数が描画領域
	rect->set(0, 0, m_charWidth * maxLineLength, m_charHeight * lineCount);
}
#endif

//------------------------------------------------------------------------------
RawFontPtr BitmapFont::Copy() const
{
	RefPtr<BitmapFont> font(LN_NEW BitmapFont(m_manager, m_fontBitmap), false);
	return RawFontPtr::StaticCast(font);
}

//------------------------------------------------------------------------------
FontGlyphLocation* BitmapFont::AdvanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData)
{
	FontGlyphLocation* locData;

	// ひとつ前のデータがあればそれをベースに
	if (prevData)
	{
		locData = prevData;

		// 横方向に送る
		locData->BitmapTopLeftPosition.x += m_charWidth;
		locData->OuterTopLeftPosition = locData->BitmapTopLeftPosition;
	}
	// 最初の文字であればデータリセット
	else
	{
		m_fontGlyphLocation.BitmapTopLeftPosition = Point::Zero;
		m_fontGlyphLocation.OutlineBitmapTopLeftPosition = Point::Zero;
		m_fontGlyphLocation.OuterTopLeftPosition = Point::Zero;
		locData = &m_fontGlyphLocation;
	}
	locData->BitmapSize.Set(m_charWidth, m_charHeight);

	return locData;
}

//------------------------------------------------------------------------------
FontGlyphBitmap* BitmapFont::LookupGlyphBitmap(UTF32 utf32code, int strokeSize)
{
	// ASCII 部分だけグリフに出来る。それ以外は '?'
	if (utf32code > 0xFF) {
		utf32code = '?';
	}

	// 一時ビットマップへ転送してそれを返す
	Rect dstRect(0, 0, m_charWidth, m_charHeight);
	Rect srcRect((utf32code % 16) * m_charWidth, (utf32code / 16) * m_charHeight, m_charWidth, m_charHeight);
	m_glyphBitmap->BitBlt(dstRect, m_fontBitmap, srcRect, Color32::White, false);

	return &m_fontGlyphBitmap;
}


} // namespace detail
LN_NAMESPACE_END
