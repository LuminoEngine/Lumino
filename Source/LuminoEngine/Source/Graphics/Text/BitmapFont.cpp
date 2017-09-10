
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
Ref<RawFont> RawFont::createBuiltInBitmapFontInternal2(int size)
{
	MemoryStream stream(g_BuiltInBitmapFont_size7_Data, g_BuiltInBitmapFont_size7_Len);
	Ref<Bitmap> bitmap(LN_NEW Bitmap(&stream), false);
	auto font = newObject<detail::BitmapFont>(bitmap);
	return Ref<RawFont>::staticCast(font);
}

//------------------------------------------------------------------------------
void RawFont::registerFontFile(const StringRef& filePath)
{
	detail::GraphicsManager::getInstance()->getFontManager()->registerFontFile(filePath);
}

namespace detail {

//==============================================================================
// BitmapFont
//==============================================================================

//------------------------------------------------------------------------------
BitmapFont::BitmapFont()
	: m_manager(NULL)
	, m_fontBitmap()
	, m_charWidth(0)
	, m_charHeight(0)
{
}

//------------------------------------------------------------------------------
void BitmapFont::initialize(Bitmap* bitmap)
{
	RawFont::initialize();
	m_manager = detail::EngineDomain::getGraphicsManager()->getFontManager();

	m_name = String::sprintf(_T("%d"), rand());	// TODO: 名前がユーザー指定されていなければランダムに作る
	m_fontBitmap = bitmap;

	m_charWidth = m_fontBitmap->getSize().width / 16;
	m_charHeight = m_fontBitmap->getSize().height / 16;

	// グリフ用の一時ビットマップ
	m_glyphBitmap.attach(LN_NEW Bitmap(SizeI(m_charWidth, m_charHeight), PixelFormat::R8G8B8A8));

	m_fontGlyphBitmap.GlyphBitmap = m_glyphBitmap;
	m_fontGlyphBitmap.OutlineBitmap = NULL;
	m_fontGlyphBitmap.OutlineOffset = 0;
}

//------------------------------------------------------------------------------
BitmapFont::~BitmapFont()
{
}

#if 0
//------------------------------------------------------------------------------
void BitmapFont::getTextSize(const char* text, int len, Geometry::RectI* rect)
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
void BitmapFont::getTextSize(const wchar_t* text, int len, Geometry::RectI* rect)
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
RawFontPtr BitmapFont::copy() const
{
	auto ptr = newObject<BitmapFont>(m_fontBitmap);
	return RawFontPtr::staticCast(ptr);
}

//------------------------------------------------------------------------------
FontGlyphLocation* BitmapFont::advanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData)
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
		m_fontGlyphLocation.BitmapTopLeftPosition = PointI::Zero;
		m_fontGlyphLocation.OutlineBitmapTopLeftPosition = PointI::Zero;
		m_fontGlyphLocation.OuterTopLeftPosition = PointI::Zero;
		locData = &m_fontGlyphLocation;
	}
	locData->BitmapSize.set(m_charWidth, m_charHeight);

	return locData;
}

//------------------------------------------------------------------------------
FontGlyphBitmap* BitmapFont::lookupGlyphBitmap(UTF32 utf32code, int strokeSize)
{
	// ASCII 部分だけグリフに出来る。それ以外は '?'
	if (utf32code > 0xFF) {
		utf32code = '?';
	}

	// 一時ビットマップへ転送してそれを返す
	RectI dstRect(0, 0, m_charWidth, m_charHeight);
	RectI srcRect((utf32code % 16) * m_charWidth, (utf32code / 16) * m_charHeight, m_charWidth, m_charHeight);
	m_glyphBitmap->bitBlt(dstRect, m_fontBitmap, srcRect, Color32::White, false);

	return &m_fontGlyphBitmap;
}

//------------------------------------------------------------------------------
void BitmapFont::getGlobalMetrics(FontGlobalMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics != nullptr)) return;
	outMetrics->ascender = m_charHeight;
	outMetrics->descender = 0;
	outMetrics->lineSpace = m_charHeight;
}

//------------------------------------------------------------------------------
void BitmapFont::getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics != nullptr)) return;
	outMetrics->size.width = (float)m_charWidth;
	outMetrics->size.height = (float)m_charHeight;
	outMetrics->advance.x = m_charWidth;
	outMetrics->advance.y = m_charHeight;
}

} // namespace detail
LN_NAMESPACE_END
