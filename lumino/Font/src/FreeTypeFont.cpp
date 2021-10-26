
#include "Internal.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H /* <freetype/freetype.h> */
#include FT_CACHE_H /* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H /* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H /* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include FT_STROKER_H
#include FT_SYNTHESIS_H
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoFont/Font.hpp>
#include "FontManager.hpp"
#include "FreeTypeFont.hpp"

#define FLValueToFloatPx(x) (0.015625f * static_cast<float>(x))	// x >> 6
#define FloatPxToFLValue(x) ((x) * 64)			// x << 6

namespace ln {
namespace detail {

//==============================================================================
// FreeTypeFontCached
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
	・FreeTypeFontCached クラスひとつに対して FT_Face はひとつ。

*/

FreeTypeFontCached::FreeTypeFontCached()
{
}

void FreeTypeFontCached::init(FontManager* manager, const FontDesc& desc)
{
	if (LN_REQUIRE(manager)) return;
	FontCore::init(manager);

	FT_Error err = 0;
	m_desc = desc;

	//const String* name = &m_desc.Family;
	//if (LN_REQUIRE(!name->isEmpty())) return;
	String name = m_desc.Family;
	if (name.isEmpty()) name = manager->defaultFontDesc().Family;
	if (LN_REQUIRE(!name.isEmpty())) return;

	m_ftFaceId = (FTC_FaceID)CRCHash::compute(name.c_str());
	FTC_Manager ftc_manager = manager->ftCacheManager();
	//m_manager->m_requesterFaceName = name->c_str();


#define RESOLUTION_X 96
#define RESOLUTION_Y 96
	//#define SCALE_X(x) (FT_MulFix(x, em_scale))
	//#define SCALE_Y(y) (FT_MulFix(y, em_scale))

	// m_fontSize に対する本当の文字サイズを取得する
	FTC_ScalerRec scaler;
	scaler.face_id = m_ftFaceId;
	scaler.width = 0;// m_desc.Size << 6;
	scaler.height = m_desc.Size << 6;
	scaler.pixel = 0;
	scaler.x_res = RESOLUTION_X;
	scaler.y_res = RESOLUTION_Y;
	FT_Size ft_size;
	err = FTC_Manager_LookupSize(ftc_manager, &scaler, &ft_size);	// FTC 使う場合は FT_Set_Char_Size の代わりにこれを使う。内部で FT_Set_Char_Size が呼ばれる。
	if (LN_ENSURE(err == 0, "failed FTC_Manager_LookupSize : %d\n", err)) return;

	//FTC_Manager_Reset(ftc_manager);



	err = FTC_Manager_LookupFace(ftc_manager, m_ftFaceId, &m_ftFace);
	if (LN_ENSURE(err == 0, "failed FTC_Manager_LookupFace : %d\n", err)) return;

	//FT_Set_Char_Size(m_ftFace, scaler.width, scaler.height, scaler.x_res, scaler.y_res);

	//err = FT_Set_Char_Size(m_ftFace, 0, m_desc.Size << 6, 0, RESOLUTION_Y);

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
	m_ftImageType.width = m_desc.Size;
	m_ftImageType.height = m_desc.Size;
	updateImageFlags();

	//float a1 = FLValueToFloatPx(m_ftFace->bbox.yMax);
	//float a2 = FLValueToFloatPx(m_ftFace->bbox.yMin);
	//float a3 = FLValueToFloatPx(m_ftFace->bbox.yMax - m_ftFace->bbox.yMin);
	//float h = a1 - a2;
	int width = std::ceil(FLValueToFloatPx(m_ftFace->bbox.xMax) - FLValueToFloatPx(m_ftFace->bbox.xMin));
	int height = std::ceil(FLValueToFloatPx(m_ftFace->bbox.yMax) - FLValueToFloatPx(m_ftFace->bbox.yMin));
	width += 2;		// antialias などが入ると微妙に増える。mplus で 22 ポイントにすると発生した。
	height += 2;
	m_internalCacheBitmap = makeObject<Bitmap2D>(width, height, PixelFormat::A8);

	// グリフ格納用ビットマップ (仮確保)
	//m_glyphBitmap.attach(LN_NEW RawBitmap(SizeI(m_desc.Size, m_desc.Size), PixelFormat::A8));
	//m_outlineBitmap.attach(LN_NEW RawBitmap(SizeI(m_desc.Size, m_desc.Size), PixelFormat::A8));

	// ラスタライズで使用する
	//mPixelList = LN_NEW PixelData[m_fontSize * m_fontSize * 4];

	// TODO:
	// 以下、FTC_ImageCache_Lookup を一度呼び出しておかないと、初回の Glyph の metrix が正しく取れなくなる。
	// どうも FreeType のキャッシュは ビットマップやグリフはいい感じにキャッシュしてくれるが、
	// それ以外のメトリクスなどの情報は上手く取れない。
	// 
	// https://stackoverflow.com/questions/15121595/tutorial-on-freetype-cache-subsystem
	// そのうちキャッシュを自前で作って、FreeType のキャッシュシステムは使わない方がよさそう。

	//BitmapGlyphInfo info;
	//info.glyphBitmap = nullptr;
	//lookupGlyphBitmap('a', &info);
	FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		this->manager()->ftCacheMapCache(),
		m_ftFaceId,
		m_ftCacheMapIndex,
		'a');
	if (LN_ENSURE(glyphIndex != 0, "failed FTC_CMapCache_Lookup")) return;

	FT_Glyph glyph;
	err = FTC_ImageCache_Lookup(
		this->manager()->ftCImageCache(),
		&m_ftImageType,
		glyphIndex,
		&glyph,
		NULL);
	if (LN_ENSURE(err == 0, "failed FTC_ImageCache_Lookup : %d\n", err)) return;

}

void FreeTypeFontCached::dispose()
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

void FreeTypeFontCached::getGlobalMetrics(FontGlobalMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics)) return;
	if (LN_REQUIRE(m_ftFace)) return;
	outMetrics->ascender = FLValueToFloatPx(m_ftFace->size->metrics.ascender);
	outMetrics->descender = FLValueToFloatPx(m_ftFace->size->metrics.descender);
	outMetrics->lineSpace = outMetrics->ascender - outMetrics->descender;
	outMetrics->outlineSupported = FT_IS_SCALABLE(m_ftFace);
}

void FreeTypeFontCached::getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics)) return;

	// get glyph index
	FT_UInt glyphIndex = FTC_CMapCache_Lookup(manager()->ftCacheMapCache(), m_ftFaceId, m_ftCacheMapIndex, utf32Code);
	if (LN_ENSURE(glyphIndex != 0)) return;

	// グリフメトリクスにアクセスするため、グリフスロット(m_ftFace->glyph) に glyphIndex で示すグリフの情報をロードする
	FT_Error err = FT_Load_Glyph(m_ftFace, glyphIndex, FT_LOAD_DEFAULT);
	if (LN_ENSURE(err == 0)) return;

	//FT_Error err = FT_Load_Char(m_ftFace, utf32Code, FT_LOAD_DEFAULT);
	//if (LN_ENSURE(err == 0)) return;

	outMetrics->size.width = FLValueToFloatPx(m_ftFace->glyph->metrics.width);
	outMetrics->size.height = FLValueToFloatPx(m_ftFace->glyph->metrics.height);
	outMetrics->bearingX = FLValueToFloatPx(m_ftFace->glyph->metrics.horiBearingX);
	outMetrics->bearingY = FLValueToFloatPx(m_ftFace->glyph->metrics.horiBearingY);
	outMetrics->advance.x = FLValueToFloatPx(m_ftFace->glyph->advance.x);
	outMetrics->advance.y = FLValueToFloatPx(m_ftFace->glyph->advance.y);
}

Vector2 FreeTypeFontCached::getKerning(UTF32 prev, UTF32 next)
{
	if (FT_HAS_KERNING(m_ftFace))
	{
		FT_UInt glyphIndex1 = FTC_CMapCache_Lookup(manager()->ftCacheMapCache(), m_ftFaceId, m_ftCacheMapIndex, prev);
		FT_UInt glyphIndex2 = FTC_CMapCache_Lookup(manager()->ftCacheMapCache(), m_ftFaceId, m_ftCacheMapIndex, next);
		if (glyphIndex1 == 0 || glyphIndex2 == 0)
		{
			// newline, whitespace ...
			return Vector2::Zero;
		}
		else
		{
			FT_Vector delta;
			FT_Error err = FT_Get_Kerning(m_ftFace, glyphIndex1, glyphIndex2, ft_kerning_default, &delta);
			if (LN_ENSURE(err == 0)) return Vector2::Zero;

			return Vector2(FLValueToFloatPx(delta.x), FLValueToFloatPx(delta.y));
		}
	}
	else
	{
		return Vector2::Zero;
	}
}

void FreeTypeFontCached::lookupGlyphBitmap(UTF32 utf32code, BitmapGlyphInfo* outInfo)
{
	if (LN_REQUIRE(outInfo)) return;

	Bitmap2D* blitTarget = outInfo->glyphBitmap;
	if (!blitTarget) {
		blitTarget = m_internalCacheBitmap;
		outInfo->glyphBitmap = m_internalCacheBitmap;
	}

	FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		manager()->ftCacheMapCache(),
		m_ftFaceId,
		m_ftCacheMapIndex,
		utf32code);
	if (LN_ENSURE(glyphIndex != 0, "failed FTC_CMapCache_Lookup")) return;

	FT_Glyph glyph;
	FT_Error err = FTC_ImageCache_Lookup(
		manager()->ftCImageCache(),
		&m_ftImageType,
		glyphIndex,
		&glyph,
		NULL);
	if (LN_ENSURE(err == 0, "failed FTC_ImageCache_Lookup : %d\n", err)) return;

	// 太字フォント
	if (m_desc.isBold)
	{
		// アウトラインフォントである必要がある
		if (LN_ENSURE(glyph->format == FT_GLYPH_FORMAT_OUTLINE)) return;

		FT_Pos strength = (800 - 400) / 8;	// 太さ
		err = FT_Outline_Embolden(&m_ftFace->glyph->outline, strength);
		if (LN_ENSURE(err == 0, "failed FT_Outline_Embolden : %d\n", err)) return;
	}
	FT_Render_Mode renderMode = (m_desc.isAntiAlias) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

	FT_BitmapGlyph glyph_bitmap;
	if (glyph->format == FT_GLYPH_FORMAT_BITMAP) {
		// FT_LOAD_NO_BITMAP が OFF だとここに入ってくる
        FT_Bitmap* bitmap = &((FT_BitmapGlyph)glyph)->bitmap;
        FT_BitmapToBitmap2D(bitmap, blitTarget);
        outInfo->size.width = bitmap->width;
        outInfo->size.height = bitmap->rows;
	}
	else
	{
		// FT_Glyph_To_Bitmap は入力された FT_Glyph を破棄して新たなインスタンスを作る。
		// そのため キャッシュから取り出した FT_Glyph を渡してしまうと、キャッシュ管理下にある FT_Glyph が破壊されてしまう。
		// (次に FTC_ImageCache_Lookup したときに、壊れた FT_Glyph が得られてしまう)
		// それを回避するため、一度 FT_Glyph_Copy で新しいインスタンスを作り、それを FT_Glyph_To_Bitmap で変換する。

		FT_Glyph copyGlyph;
		err = FT_Glyph_Copy(glyph, &copyGlyph);
		if (LN_ENSURE(err == 0, "failed FT_Glyph_Copy : %d\n", err)) return;

		// FT_RENDER_MODE_NORMALで普通の(256階調)アンチエイリアス
		// FT_RENDER_MODE_LCDで液晶用アンチエイリアス(サブピクセルレンダリング)
		err = FT_Glyph_To_Bitmap(&copyGlyph, renderMode, NULL, 1);
		if (LN_ENSURE(err == 0, "failed FT_Glyph_To_Bitmap : %d\n", err)) return;

		//glyph_bitmap = (FT_BitmapGlyph)m_fontGlyphBitmap.CopyGlyph;
        FT_Bitmap* bitmap = &((FT_BitmapGlyph)copyGlyph)->bitmap;
		FT_BitmapToBitmap2D(&((FT_BitmapGlyph)copyGlyph)->bitmap, blitTarget);
        outInfo->size.width = bitmap->width;
        outInfo->size.height = bitmap->rows;

		FT_Done_Glyph(copyGlyph);
	}


	//FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
	//refreshBitmap(m_glyphBitmap, ft_bitmap);
	//m_fontGlyphBitmap.GlyphBitmap = m_glyphBitmap;

	//return &m_fontGlyphBitmap;

}

void FreeTypeFontCached::decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo)
{
	LN_NOTIMPLEMENTED();
}

void FreeTypeFontCached::updateImageFlags()
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

bool FreeTypeFontCached::getOutlineTextMetrix()
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
	return true;
}

bool FreeTypeFontCached::getBitmapTextMetrix()
{
	LN_NOTIMPLEMENTED();
	return false;
}

void FreeTypeFontCached::FT_BitmapToBitmap2D(FT_Bitmap* ftBitmap, Bitmap2D* bitmap) const
{
	int width = ftBitmap->width;
	int height = ftBitmap->rows;
	if (LN_REQUIRE(bitmap->width() >= width && bitmap->height() >= height && bitmap->format() == PixelFormat::A8)) return;

	//// サイズ
	//bitmap->m_size.width = width;
	//bitmap->m_size.height = height;
	//bitmap->m_pitch = abs(ftBitmap->pitch);
	//if (ftBitmap->pitch < 0) {
	//	bitmap->m_upFlow = false;
	//}

	// フォーマット
	if (ftBitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
		size_t rowStride = (width / 8) + 1;
		for (int y = 0; y < height; y++) {
			const byte_t* srcRow = ftBitmap->buffer + rowStride * y;
			byte_t* dstRow = (bitmap->data() + bitmap->width() * y);	// A8 format
			for (int x = 0; x < width; x++) {
				size_t byte = (x) >> 3; // / 8;
				size_t bit = (x) & 7;   // % 8;
				if (srcRow[byte] & (0x80 >> bit)) {
					dstRow[x] = 0xFF;
				}
				else {
					dstRow[x] = 0x00;
				}
			}
		}
	}
	else if (ftBitmap->pixel_mode == FT_PIXEL_MODE_GRAY) {
		for (int y = 0; y < height; y++) {
			const byte_t* srcRow = ftBitmap->buffer + width * y;
			byte_t* dstRow = (bitmap->data() + bitmap->width() * y);	// A8 format
			memcpy(dstRow, srcRow, width);
		}
	}
	else {
		LN_NOTIMPLEMENTED();
		return;
	}
}



//==============================================================================
// FreeTypeFont

FreeTypeFont::FreeTypeFont()
	: m_desc()
	, m_face(nullptr)
	, m_internalCacheBitmapGray()
{
}

Result FreeTypeFont::init(FontManager* manager, const FontDesc& desc)
{
    // CSS (Web) のフォントポイントサイズに合わせる。
	static const int resolution = 72;

	if (LN_REQUIRE(manager)) return false;
	FontCore::init(manager);
	m_desc = desc;

	auto faceSource = manager->lookupFontFaceSourceFromFamilyName(m_desc.Family);
	if (!faceSource) {
		LN_ERROR();
		return false;
	}

	FT_Error err = FT_New_Memory_Face(manager->ftLibrary(), (const FT_Byte*)faceSource->buffer->data(), faceSource->buffer->size(), faceSource->faceIndex, &m_face);
	if (LN_ENSURE(err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err)) return false;

	if (m_face->num_fixed_sizes == 0) {
		// For 3D space text drawing.
		{
			FT_F26Dot6 size = Font::DefaultSize * 64;
			err = FT_Set_Char_Size(m_face, size, size, resolution, resolution);
			getGlobalMetricsInternal(&m_engineDefaultGlobalMetrix, true);
			if (LN_ENSURE(err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err)) return false;
		}

		// FT_Set_Char_Size() はポイントサイズと解像度をもとに m_face->size->metrics を作成する
		float size = static_cast<float>(m_desc.Size) * 64.0f;
		err = FT_Set_Char_Size(m_face, size, size, resolution, resolution);
		if (LN_ENSURE(err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err)) return false;
	}
	else {
		LN_LOG_VERBOSE << "\"" << m_face->family_name << "\" is fixed size font.";

		int bestMatch = 0;
		int diff = std::abs(m_desc.Size - m_face->available_sizes[0].width);
		for (int i = 1; i < m_face->num_fixed_sizes; ++i) {
			int ndiff = std::abs(m_desc.Size - m_face->available_sizes[i].width);
			if (ndiff < diff) {
				bestMatch = i;
				diff = ndiff;
			}
		}
		err = FT_Select_Size(m_face, bestMatch);
		if (LN_ENSURE(err == FT_Err_Ok, "failed FT_Select_Size : %d\n", err)) return false;
	}

	m_loadFlags = FT_LOAD_DEFAULT;

	LN_LOG_VERBOSE << "    x scale : " << (m_face->size->metrics.x_scale / 65536.0);
	LN_LOG_VERBOSE << "    y scale : " << (m_face->size->metrics.y_scale / 65536.0);
	LN_LOG_VERBOSE << "   ascender : " << FLValueToFloatPx(m_face->size->metrics.ascender);
	LN_LOG_VERBOSE << "  descender : " << FLValueToFloatPx(m_face->size->metrics.descender);
	LN_LOG_VERBOSE << "     height : " << FLValueToFloatPx(m_face->size->metrics.height);
	LN_LOG_VERBOSE << "max_advance : " << FLValueToFloatPx(m_face->size->metrics.max_advance);
	LN_LOG_VERBOSE << "     x ppem : " << (m_face->size->metrics.x_ppem);
	LN_LOG_VERBOSE << "     y ppem : " << (m_face->size->metrics.y_ppem);


	// lookupGlyphBitmap の結果を書き込むためのビットマップを作っておく
	{
		//FontGlobalMetrics metrix;
		//getGlobalMetrics(&metrix);

		//m_internalCacheBitmap = makeObject<Bitmap2D>(metrix.lineSpace, metrix.lineSpace, PixelFormat::A8);
	}
	
	return false;
}

void FreeTypeFont::dispose()
{
	if (m_face) {
		FT_Done_Face(m_face);
		m_face = nullptr;
	}

	FontCore::dispose();
}

void FreeTypeFont::getGlobalMetrics(FontGlobalMetrics* outMetrics)
{
	getGlobalMetricsInternal(outMetrics, false);
}

void FreeTypeFont::getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics)) return;

    FT_Int32 loadFlags = FT_LOAD_DEFAULT;// | FT_LOAD_NO_HINTING;// FT_LOAD_NO_SCALE;

	{
		const FT_UInt glyphIndex = ::FT_Get_Char_Index(m_face, utf32Code);

		// load glyph info to GlyphSlot(m_face->glyph), for access to metrix.
		FT_Error err = FT_Load_Glyph(m_face, glyphIndex, loadFlags);
		if (LN_ENSURE(err == 0)) return;

		if (m_desc.isBold) {
			FT_GlyphSlot_Embolden(m_face->glyph);
		}

		if (m_desc.isItalic) {
			FT_GlyphSlot_Oblique(m_face->glyph);
		}
	}

    // ft_glyphslot_grid_fit_metrics で整数に切り上げられたりする

    outMetrics->size.width = FLValueToFloatPx(m_face->glyph->metrics.width);
    outMetrics->size.height = FLValueToFloatPx(m_face->glyph->metrics.height);
    outMetrics->bearingX = FLValueToFloatPx(m_face->glyph->metrics.horiBearingX);
    outMetrics->bearingY = FLValueToFloatPx(m_face->glyph->metrics.horiBearingY);
    outMetrics->advance.x = FLValueToFloatPx(m_face->glyph->advance.x);
    outMetrics->advance.y = FLValueToFloatPx(m_face->glyph->advance.y);

    //outMetrics->bearingY = std::ceil(outMetrics->bearingY);
    //outMetrics->advance.x = std::round(outMetrics->advance.x);

    if (loadFlags & FT_LOAD_NO_SCALE)
    {
        float scale = static_cast<float>(m_face->size->metrics.y_scale) / 65536.0f;
        outMetrics->size.width *= scale;
        outMetrics->size.height *= scale;
        outMetrics->bearingX *= scale;
        outMetrics->bearingY *= scale;
        outMetrics->advance.x *= scale;
        outMetrics->advance.y *= scale;
    }

    


}

Vector2 FreeTypeFont::getKerning(UTF32 prev, UTF32 next)
{
    //FT_Set_Pixel_Sizes(m_face, 0, 12);
	if (FT_HAS_KERNING(m_face))
	{
		const FT_UInt glyphIndex1 = ::FT_Get_Char_Index(m_face, prev);
		const FT_UInt glyphIndex2 = ::FT_Get_Char_Index(m_face, next);
		if (glyphIndex1 == 0 || glyphIndex2 == 0) {
			// newline, whitespace ...
			return Vector2::Zero;
		}
		else {
			FT_Vector delta;
			FT_Error err = FT_Get_Kerning(m_face, glyphIndex1, glyphIndex2, FT_KERNING_DEFAULT, &delta);
			if (LN_ENSURE(err == 0)) return Vector2::Zero;
			return Vector2(FLValueToFloatPx(delta.x), FLValueToFloatPx(delta.y));
		}
	}
	else
	{
		return Vector2::Zero;
	}
}

void FreeTypeFont::lookupGlyphBitmap(UTF32 utf32code, BitmapGlyphInfo* outInfo)
{
	if (LN_REQUIRE(outInfo)) return;
	LN_CHECK(outInfo->glyphBitmap == nullptr);	// TODO: 廃止予定
	FT_Error err = 0;

	{
		const FT_UInt glyphIndex = ::FT_Get_Char_Index(m_face, utf32code);

		FT_Int32 loadFlags = 0/*FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT | FT_LOAD_FORCE_AUTOHINT*/;
		if (outInfo->loadColor) {
			loadFlags |= FT_LOAD_COLOR;
		}

		// load glyph info to GlyphSlot(m_face->glyph), for access to metrix.
		err = FT_Load_Glyph(m_face, glyphIndex, loadFlags);
		if (LN_ENSURE(err == 0)) return;

		if (m_desc.isBold) {
			FT_GlyphSlot_Embolden(m_face->glyph);
		}

		if (m_desc.isItalic) {
			FT_GlyphSlot_Oblique(m_face->glyph);
		}
	}


	FT_Render_Mode renderMode = (m_desc.isAntiAlias) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;
	FT_GlyphSlot glyph = m_face->glyph;

	if (glyph->format == FT_GLYPH_FORMAT_BITMAP) {
		// FT_LOAD_NO_BITMAP が OFF だとここに入ってくる
		outInfo->glyphBitmap = FTBitmapToInternalCacheBitmap(&glyph->bitmap);
		outInfo->size.width = glyph->bitmap.width;
		outInfo->size.height = glyph->bitmap.rows;
	}
	else
	{
		err = ::FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
		if (LN_ENSURE(err == 0)) return;

		//FT_Bitmap* bitmap = &((FT_BitmapGlyph)glyph)->bitmap;
		outInfo->glyphBitmap = FTBitmapToInternalCacheBitmap(&glyph->bitmap);
		outInfo->size.width = glyph->bitmap.width;
		outInfo->size.height = glyph->bitmap.rows;

		//LN_NOTIMPLEMENTED();
	}
}

void FreeTypeFont::decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo)
{
	LN_NOTIMPLEMENTED();
}

void FreeTypeFont::getGlobalMetricsInternal(FontGlobalMetrics* outMetrics, bool fromInit) const
{
	if (LN_REQUIRE(outMetrics)) return;
	if (LN_REQUIRE(m_face)) return;

	auto rawHeight = m_face->size->metrics.height;
	auto rawBBoxHeight = m_face->bbox.yMax - m_face->bbox.yMin;

	float height = static_cast<float>(m_face->size->metrics.height) / 64.0f;
	float bboxHeight = static_cast<float>(rawBBoxHeight) / 64.0f;

	float em_size = 1.0 * m_face->units_per_EM;
	float x_scale = 1.0f;//m_face->size->metrics.x_ppem / em_size;
	float y_scale = 1.0f;//m_face->size->metrics.y_ppem / em_size;
	float xMin = std::floor(FLValueToFloatPx(m_face->bbox.xMin));//std::floor(x_scale * m_face->bbox.xMin);
	float yMin = std::floor(FLValueToFloatPx(m_face->bbox.yMin)); //std::floor(y_scale * m_face->bbox.yMin);
	float xMax = std::ceil(FLValueToFloatPx(m_face->bbox.xMax)); //std::ceil(x_scale * m_face->bbox.xMax);
	float yMax = std::ceil(FLValueToFloatPx(m_face->bbox.yMax));//std::ceil(y_scale * m_face->bbox.yMax);
/*
	FT_FL
	FT_FloorFix;
	FT_MulFix;
*/
	outMetrics->ascender = FLValueToFloatPx(m_face->size->metrics.ascender);
	outMetrics->descender = FLValueToFloatPx(m_face->size->metrics.descender);
	outMetrics->lineSpace = FLValueToFloatPx(m_face->size->metrics.height);	// ascender - descender ではなく height を使う。FreeType 内部で端数が切り捨てられているので、1px足りないとかになる。
	
	// 最終的な lineSpace は、(desender - ascender) と height のうち大きい方を使う。
	// FontAwesome の "file" だと height の方が 1px 小さくなって、グリフビットマップの方が大きい状態になってた。
	// 一方 mplus などのフォントでは height の方が小さかったりした。FreeType の内部で小数点以下が切り捨てられているようだ。
	float diff = std::abs(outMetrics->descender - outMetrics->ascender);
	outMetrics->lineSpace = std::max(outMetrics->lineSpace, diff);
	
	outMetrics->outlineSupported = FT_IS_SCALABLE(m_face);

	if (fromInit) {
		outMetrics->virutalSpaceFactor = 1.0f;
	}
	else {
		float d1 = std::abs(m_engineDefaultGlobalMetrix.descender - m_engineDefaultGlobalMetrix.ascender);
		float d2 = std::abs(outMetrics->descender - outMetrics->ascender);
		outMetrics->virutalSpaceFactor = (1.0f / d1) * (d1 / d2);
	}
}

bool FreeTypeFont::getOutlineTextMetrix()
{
	return true;
}

bool FreeTypeFont::getBitmapTextMetrix()
{
	return true;
}

Bitmap2D* FreeTypeFont::FTBitmapToInternalCacheBitmap(FT_Bitmap* ftBitmap)
{
	Bitmap2D* bmp = nullptr;
	if (ftBitmap->pixel_mode == FT_PIXEL_MODE_MONO ||
		ftBitmap->pixel_mode == FT_PIXEL_MODE_GRAY) {

		if (!m_internalCacheBitmapGray) {
			m_internalCacheBitmapGray = makeObject<Bitmap2D>(ftBitmap->width, ftBitmap->rows, PixelFormat::A8);
		}
		else if (m_internalCacheBitmapGray->width() < ftBitmap->width || m_internalCacheBitmapGray->height() < ftBitmap->rows) {
			m_internalCacheBitmapGray->resize(ftBitmap->width, ftBitmap->rows);
		}

		bmp = m_internalCacheBitmapGray;
	}
	else if (ftBitmap->pixel_mode == FT_PIXEL_MODE_BGRA) {

		if (!m_internalCacheBitmapRGBA) {
			m_internalCacheBitmapRGBA = makeObject<Bitmap2D>(ftBitmap->width, ftBitmap->rows, PixelFormat::RGBA8);
		}
		else if (m_internalCacheBitmapRGBA->width() < ftBitmap->width || m_internalCacheBitmapRGBA->height() < ftBitmap->rows) {
			m_internalCacheBitmapRGBA->resize(ftBitmap->width, ftBitmap->rows);
		}

		bmp = m_internalCacheBitmapRGBA;
	}

	FTBitmapToBitmap2D(ftBitmap, bmp);

	return bmp;
}

void FreeTypeFont::FTBitmapToBitmap2D(FT_Bitmap* ftBitmap, Bitmap2D* bitmap) const
{
	int width = ftBitmap->width;
	int height = ftBitmap->rows;
	//if (LN_REQUIRE(bitmap->format() == PixelFormat::A8)) return;
	if (LN_REQUIRE(bitmap->width() >= width && bitmap->height() >= height)) return;
	//if (LN_REQUIRE(bitmap->width() < width || bitmap->height() < height)) {
	//	bitmap->resize(std::max(bitmap->width(), width), std::max(bitmap->height(), height));
	//}

	//if (LN_REQUIRE(bitmap->width() >= width && bitmap->height() >= height && bitmap->format() == PixelFormat::A8)) return;

	//// サイズ
	//bitmap->m_size.width = width;
	//bitmap->m_size.height = height;
	//bitmap->m_pitch = abs(ftBitmap->pitch);
	//if (ftBitmap->pitch < 0) {
	//	bitmap->m_upFlow = false;
	//}

	// フォーマット
	if (ftBitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
		size_t rowStride = (width / 8) + 1;
		for (int y = 0; y < height; y++) {
			const byte_t* srcRow = ftBitmap->buffer + rowStride * y;
			byte_t* dstRow = (bitmap->data() + bitmap->width() * y);	// A8 format
			for (int x = 0; x < width; x++) {
				size_t byte = (x) >> 3; // / 8;
				size_t bit = (x) & 7;   // % 8;
				if (srcRow[byte] & (0x80 >> bit)) {
					dstRow[x] = 0xFF;
				}
				else {
					dstRow[x] = 0x00;
				}
			}
		}
	}
	else if (ftBitmap->pixel_mode == FT_PIXEL_MODE_GRAY) {
		for (int y = 0; y < height; y++) {
			const byte_t* srcRow = ftBitmap->buffer + width * y;
			byte_t* dstRow = (bitmap->data() + bitmap->width() * y);	// A8 format
			memcpy(dstRow, srcRow, width);
		}
	}
	else if (ftBitmap->pixel_mode == FT_PIXEL_MODE_BGRA) {
		for (int y = 0; y < height; y++) {
			const byte_t* srcRow = ftBitmap->buffer + sizeof(uint32_t) * width * y;
			byte_t* dstRow = (bitmap->data() + sizeof(uint32_t) * bitmap->width() * y);
			memcpy(dstRow, srcRow, sizeof(uint32_t) * width);

			for (int x = 0; x < width; x++) {
				byte_t* p = dstRow + x * 4;
				std::swap(p[0], p[2]);
			}
		}
	}
	else {
		LN_NOTIMPLEMENTED();
		return;
	}
}


} // namespace detail
} // namespace ln

