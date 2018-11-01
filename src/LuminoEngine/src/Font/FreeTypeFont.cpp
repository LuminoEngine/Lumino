
#include "Internal.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H /* <freetype/freetype.h> */
#include FT_CACHE_H /* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H /* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H /* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include FT_STROKER_H
#include "FontManager.hpp"
#include "FreeTypeFont.hpp"

#define FLValueToFloatPx(x) (0.015625f * (x))	// x >> 6
#define FloatPxToFLValue(x) ((x) * 64)			// x << 6

namespace ln {
namespace detail {

//==============================================================================
// FreeTypeFont
/*
	時間計測…
	FTC_CMapCache_Lookup, FTC_ImageCache_Lookup
	文字設定              サイズ設定・グリフ取得

	同じフォントサイズ・同じ文字
	4, 1522
	1, 1
	0, 0
	0, 0
	0, 0

	同じフォントサイズ・文字++      2 週目はすべて 0ns
	5, 1167
	1, 78
	1, 52
	1, 46
	1, 33

	フォントサイズ++・同じ文字      2週目も同様
	4, 1147 (ns)
	0, 60
	0, 62
	0, 66
	0, 61

	フォントサイズ++・文字++        2週目も同様
	5, 1158
	1, 85
	1, 81
	1, 55
	1, 40

	サイズ、文字どちらか一方でも新しいものが来るといろいろ再構築されるみたい。
	ただし、上の各条件でさらにもう1周させた場合、
	フォントサイズが同じであれば 0ns だった。
	キャッシュの中ではサイズの方が偉いみたい。

	また、Face を作るのに必要な時間は FT_Open_Face で 300ns 程度。
	ファイルロード・メモリ確保入れると 3000ns くらいになる。

	設計方針…
	・フォントファイルひとつ分のメモリ領域は、同名フォントで共有
	・FreeTypeFont クラスひとつに対して FT_Face はひとつ。

*/

FreeTypeFont::FreeTypeFont()
{
}

void FreeTypeFont::initialize(FontManager* manager, const FontDesc& desc)
{
	if (LN_REQUIRE(manager)) return;
	FontCore::initialize(manager);

	m_desc = desc;

	const String* name = &m_desc.Family;
	if (LN_REQUIRE(!name->isEmpty())) return;

	m_ftFaceId = (FTC_FaceID)CRCHash::compute(name->c_str());
	FTC_Manager ftc_manager = manager->ftCacheManager();
	//m_manager->m_requesterFaceName = name->c_str();

	FT_Error err = FTC_Manager_LookupFace(ftc_manager, m_ftFaceId, &m_ftFace);
	if (LN_ENSURE(err == 0, "failed FTC_Manager_LookupFace : %d\n", err)) return;

	if (m_desc.isItalic)
	{
		// イタリック体の場合は transform で傾ける
		FT_Vector transform = { 0, 0 };
		FT_Matrix matrix;
		matrix.xx = 1 << 16;
		matrix.xy = 0x5800;
		matrix.yx = 0;
		matrix.yy = 1 << 16;
		FT_Set_Transform(m_ftFace, &matrix, &transform);
	}
	else
	{
		FT_Set_Transform(m_ftFace, NULL, NULL);
	}

	// m_ftFace->charmap は m_ftFace の中で現在アクティブな FT_CharMap。
	// グリフを取りだすときはそのインデックスを指定する必要があるので、ここで覚えておく。
	m_ftCacheMapIndex = FT_Get_Charmap_Index(m_ftFace->charmap);

#define RESOLUTION_X 72
#define RESOLUTION_Y 72
	//#define SCALE_X(x) (FT_MulFix(x, em_scale))
	//#define SCALE_Y(y) (FT_MulFix(y, em_scale))

	// m_fontSize に対する本当の文字サイズを取得する
	FTC_ScalerRec scaler;
	scaler.face_id = m_ftFaceId;
	scaler.width = 0;
	scaler.height = m_desc.Size << 6;
	scaler.pixel = 0;
	scaler.x_res = RESOLUTION_X;
	scaler.y_res = RESOLUTION_Y;
	FT_Size ft_size;
	err = FTC_Manager_LookupSize(ftc_manager, &scaler, &ft_size);
	if (LN_ENSURE(err == 0, "failed FTC_Manager_LookupSize : %d\n", err)) return;

	//m_lineHeight = ft_size->metrics.height >> 6;

	if (FT_IS_SCALABLE(m_ftFace)) {	// has outline?
		if (!getOutlineTextMetrix()) {
			return;
		}
	}
	else {
		if (!getBitmapTextMetrix()) {
			return;
		}
	}
	//int hh = m_ftFace->height;


	//m_glyphData.releaseGlyph();

	//if (m_edgeSize > 0)
	//{
	//	// エッジの描画情報
	//	FT_Stroker_New(m_manager->getFTLibrary(), &m_ftStroker);
	//	FT_Stroker_Set(m_ftStroker,
	//		(int)(m_edgeSize * 64),
	//		FT_STROKER_LINECAP_ROUND,	// 線分の両端は半円でレンダリングする
	//		FT_STROKER_LINEJOIN_ROUND,	// 線分の接合点は半円でレンダリングする
	//		0);
	//}


	// font_typeを設定
	m_ftImageType.face_id = m_ftFaceId;
	m_ftImageType.width = 0;
	m_ftImageType.height = m_desc.Size;
	updateImageFlags();

	// グリフ格納用ビットマップ (仮確保)
	//m_glyphBitmap.attach(LN_NEW RawBitmap(SizeI(m_desc.Size, m_desc.Size), PixelFormat::A8));
	//m_outlineBitmap.attach(LN_NEW RawBitmap(SizeI(m_desc.Size, m_desc.Size), PixelFormat::A8));

	// ラスタライズで使用する
	//mPixelList = LN_NEW PixelData[m_fontSize * m_fontSize * 4];
}

void FreeTypeFont::dispose()
{
	// 実体の寿命は FreeType の CacheManager が管理するので、ここでは参照を外すだけでOK
	m_ftFace = nullptr;

	//if (m_ftStroker)
	//{
	//	FT_Stroker_Done(m_ftStroker);
	//	m_ftStroker = nullptr;
	//}

	FontCore::dispose();
}

void FreeTypeFont::getGlobalMetrics(FontGlobalMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics)) return;
	if (LN_REQUIRE(m_ftFace)) return;
	outMetrics->ascender = FLValueToFloatPx(m_ftFace->size->metrics.ascender);
	outMetrics->descender = FLValueToFloatPx(m_ftFace->size->metrics.descender);
	outMetrics->lineSpace = outMetrics->ascender - outMetrics->descender;
	outMetrics->outlineSupported = FT_IS_SCALABLE(m_ftFace);
	outMetrics->boundingMinX = FLValueToFloatPx(m_ftFace->bbox.xMin);
	outMetrics->boundingMaxX = FLValueToFloatPx(m_ftFace->bbox.xMax);
	outMetrics->boundingMinY = FLValueToFloatPx(m_ftFace->bbox.yMin);
	outMetrics->boundingMaxY = FLValueToFloatPx(m_ftFace->bbox.yMax);
	// FIXME: Bitmap font の場合の bbox は FT_Bitmap_Size を使うべきらしい。
	// (FT_FaceRec_ のコメント)
}

void FreeTypeFont::getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics)
{
	LN_NOTIMPLEMENTED();
}

Vector2 FreeTypeFont::getKerning(UTF32 prev, UTF32 next)
{
	LN_NOTIMPLEMENTED();
	return Vector2();
}

void FreeTypeFont::decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo)
{
	LN_NOTIMPLEMENTED();
}

void FreeTypeFont::updateImageFlags()
{
	/* ビットマップまでキャッシュする場合はFT_LOAD_RENDER | FT_LOAD_TARGET_*
	* とする。ただし途中でTARGETを変更した場合等はキャッシュが邪魔する。
	* そういう時はFT_LOAD_DEFAULTにしてFTC_ImageCache_Lookup後に
	* FT_Glyph_To_Bitmapしたほうが都合がいいと思う。
	*/
	//m_ftImageType.flags = FT_LOAD_NO_BITMAP;

	/* m_ftImageType.flags に　FT_LOAD_FORCE_AUTOHINT を使用すると、
	* 一部のフォント(VL ゴシック等)で次の文字までのオフセットが正しく取れないことがある。
	*/

	// アウトライン ON
	if (/*m_edgeSize > 0 || */m_desc.isBold) {
		// TODO: メモリリークする
		m_ftImageType.flags = FT_LOAD_NO_BITMAP;
	}
	// アウトライン OFF
	else {
		// TODO: FT_LOAD_RENDER がついていると、アウトラインが取れない。
		m_ftImageType.flags = FT_LOAD_RENDER;
	}
	// アンチエイリアス ON
	if (m_desc.isAntiAlias) {
		//m_ftImageType.flags = ; そのまま
	}
	// アンチエイリアス OFF
	else {
		m_ftImageType.flags |= FT_LOAD_MONOCHROME;
	}
}

bool FreeTypeFont::getOutlineTextMetrix()
{
	// TrueType OS/2 table
	TT_OS2* os2 = (TT_OS2*)FT_Get_Sfnt_Table(m_ftFace, ft_sfnt_os2);
	if (LN_ENSURE(os2 != nullptr, "FT_Get_Sfnt_Table")) return false;

	// TrueType horizontal header
	TT_HoriHeader* hori = (TT_HoriHeader*)FT_Get_Sfnt_Table(m_ftFace, ft_sfnt_hhea);
	if (LN_ENSURE(hori != nullptr, "FT_Get_Sfnt_Table")) return false;

	FT_Fixed x_scale = m_ftFace->size->metrics.x_scale;
	FT_Fixed y_scale = m_ftFace->size->metrics.y_scale;

	int tmAscent = (FT_MulFix(os2->usWinAscent, y_scale) + 32) >> 6;
	int tmDescent = (FT_MulFix(os2->usWinDescent, y_scale) + 32) >> 6;
	int tmInternalLeading = (FT_MulFix(os2->usWinAscent + os2->usWinDescent - m_ftFace->units_per_EM, y_scale) + 32) >> 6;
	int tmExternalLeading = std::max(0L, (FT_MulFix(hori->Line_Gap -
		((os2->usWinAscent + os2->usWinDescent) -
		(hori->Ascender - hori->Descender)), y_scale) + 32) >> 6);
	//m_lineHeight = tmInternalLeading + tmAscent + tmDescent + tmExternalLeading;


	//#define SCALE_X(x) (FT_MulFix(x, m_ftFace->size->metrics.x_scale))
	//#define SCALE_Y(y) (FT_MulFix(y, m_ftFace->size->metrics.y_scale))
	//
	//	TT_HoriHeader* hori = (TT_HoriHeader*)FT_Get_Sfnt_Table(m_ftFace, ft_sfnt_hhea);
	//
	//	int tmAscent = SCALE_Y(hori->Ascender);
	//	int tmDescent = SCALE_Y(hori->Descender);
	//	int tmInternalLeading = SCALE_Y(hori->Ascender + hori->Descender - m_ftFace->units_per_EM);
	//	m_lineHeight = tmInternalLeading + tmAscent + tmDescent;
	return false;
}

bool FreeTypeFont::getBitmapTextMetrix()
{
	LN_NOTIMPLEMENTED();
	return false;
}

} // namespace detail
} // namespace ln

