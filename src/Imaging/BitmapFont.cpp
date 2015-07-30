
#include "../Internal.h"
#include <algorithm>
#include "BitmapFont.h"

namespace Lumino
{
namespace Imaging
{
#if 0

//==============================================================================
// BitmapFont
//==============================================================================

//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
BitmapFont::BitmapFont()
	: mManager			(NULL)
	, mFontTexture		(NULL)
	, mCharWidth		(0)
	, mCharHeight		(0)
{
	mFontGlyphData.GlyphBitmap = &mGlyphBitmap;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
BitmapFont::~BitmapFont()
{
    dispose();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void BitmapFont::create(FreeTypeFontManager* manager, Texture* fontTexture)
{
	LN_REFOBJ_SET(mManager, manager);
	LN_REFOBJ_SET(mFontTexture, fontTexture);

	mCharWidth = (int)mFontTexture->getSize().X / 16;
	mCharHeight = (int)mFontTexture->getSize().Y / 16;

	// テクスチャの内容をフォントビットマップとしてコピーして持っておく
	TextureScopedLock lock(mFontTexture);
	mFontBitmap.create(
		lock.getData(), 
		lock.getDataSize(),
		mFontTexture->getRealSize().X,
		mFontTexture->getRealSize().Y,
		Util::convertSurfaceFormatToBitmapFormat(mFontTexture->getSurfaceFormat()),
		true);

	// グリフ用の一時ビットマップ
	mGlyphBitmap.create(mCharWidth, mCharHeight, LN_BITMAPFORMAT_A8R8G8B8);

	// 初期色
	mColor = LColor::Gray;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
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
	rect->set(0, 0, mCharWidth * maxLineLength, mCharHeight * lineCount);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
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
	rect->set(0, 0, mCharWidth * maxLineLength, mCharHeight * lineCount);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Font* BitmapFont::copy()
{
	LRefPtr<BitmapFont> font(LN_NEW BitmapFont());
	font->create(mManager, mFontTexture);
	font->setColor( mColor );
	font->addRef();
	return font;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
FontGlyphData* BitmapFont::makeGlyphData(lnU32 utf32code, FontGlyphData* prevData)
{
	FontGlyphData* glyphData;

	// ひとつ前のデータがあればそれをベースに
	if (prevData)
	{
		glyphData = prevData;

		// 横方向に送る
		glyphData->GlyphOffsetX += mCharWidth;
	}
	// 最初の文字であればデータリセット
	else
	{
		glyphData = &mFontGlyphData;
		glyphData->OutlineBitmap = NULL;
		glyphData->GlyphOffsetX = 0;
		glyphData->GlyphOffsetY = 0;
		glyphData->OutlineOffset = 0;
		glyphData->MetricsHeight = mCharHeight;	// Renderer で改行時、行高さとして使う
	}

	// ASCII 部分だけ
	lnU32 code = utf32code & 0xFF;

	// 一時ビットマップへ転送
	LRect destRect(0, 0, mCharWidth, mCharHeight);
	LRect srcRect((code % 16) * mCharWidth, (code / 16) * mCharHeight, mCharWidth, mCharHeight);
	Bitmap::bitblt(mFontGlyphData.GlyphBitmap, destRect, &mFontBitmap, srcRect, 0xFFFFFFFF, false);
			
	return glyphData;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void BitmapFont::dispose()
{
	LN_SAFE_RELEASE(mFontTexture);
	LN_SAFE_RELEASE(mManager);
}
#endif

} // namespace Imaging
} // namespace Lumino
