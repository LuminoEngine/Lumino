
#include "../Internal.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include FT_STROKER_H
#include <Lumino/Base/Hash.h>
#include "FreeTypeFont.h"
#include "BitmapFont.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// Font
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Font* Font::Create(FontManager* manager)
{
	return LN_NEW FreeTypeFont(manager);
}

//=============================================================================
// FreeTypeGlyphData
//=============================================================================


void FreeTypeGlyphBitmap::ReleaseGlyph()
{
	if (CopyGlyph) {
		FT_Done_Glyph(CopyGlyph);
		CopyGlyph = NULL;
	}
	if (CopyOutlineGlyph) {
		FT_Done_Glyph(CopyOutlineGlyph);
		CopyOutlineGlyph = NULL;
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void FreeTypeGlyphData::ReleaseGlyph()
{
	if (CopyGlyph) {
		FT_Done_Glyph(CopyGlyph);
		CopyGlyph = NULL;
	}
	if (CopyOutlineGlyph) {
		FT_Done_Glyph(CopyOutlineGlyph);
		CopyOutlineGlyph = NULL;
	}
}

//=============================================================================
// FreeTypeFont
//=============================================================================


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

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
FreeTypeFont::FreeTypeFont(FontManager* manager)
	: m_manager(manager)
	, m_fontName()
	, m_fontSize(16)
	, m_edgeSize(0)
	, m_isBold(false)
	, m_isItalic(false)
	, m_isAntiAlias(false)
	, m_modified(false)
	, m_ftFaceID(0)
	, m_ftFace(NULL)
	, m_ftCacheMapIndex(0)
	, m_ftStroker(NULL)
	, m_ftImageType()
	, m_utf32Buffer()
	, m_glyphData()
	, m_glyphBitmap()
	, m_outlineBitmap()
{
	LN_SAFE_ADDREF(m_manager);
	m_glyphData.CopyGlyph = NULL;
	m_glyphData.CopyOutlineGlyph = NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FreeTypeFont::~FreeTypeFont()
{
	Dispose();
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Font* FreeTypeFont::Copy() const
{
	RefPtr<FreeTypeFont> font(LN_NEW FreeTypeFont(m_manager));
	font->SetName(m_fontName);
	font->SetSize(m_fontSize);
	font->SetEdgeSize(m_edgeSize);
	font->SetBold(m_isBold);
	font->SetItalic(m_isItalic);
	font->SetAntiAlias(m_isAntiAlias);
	font.SafeAddRef();
	return font;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size FreeTypeFont::GetTextSize(const char* text, int length)
{
	LN_VERIFY_RETURNV(text != NULL, Size::Zero);
	if (length == 0) { return Size::Zero; }
	length = (length < 0) ? strlen(text) : length;

	// char → UTF32 に変換するのに必要なバイト数で領域確保
	size_t size = Text::Encoding::GetConversionRequiredByteCount(Text::Encoding::GetSystemMultiByteEncoding(), Text::Encoding::GetUTF32Encoding(), length * sizeof(wchar_t));
	m_utf32Buffer.Resize(size);
	
	// UTF32 に変換
	Text::EncodingConversionResult result;
	Text::Encoding::Convert(
		text, length * sizeof(wchar_t), Text::Encoding::GetSystemMultiByteEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Text::Encoding::GetUTF32Encoding(),
		&result);

	return GetTextSize((UTF32*)m_utf32Buffer.GetData(), result.BytesUsed / sizeof(UTF32));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size FreeTypeFont::GetTextSize(const wchar_t* text, int length)
{
	LN_VERIFY_RETURNV(text != NULL, Size::Zero);
	if (length == 0) { return Size::Zero; }
	length = (length < 0) ? wcslen(text) : length;

	// wchar_t → UTF32 に変換するのに必要なバイト数で領域確保
	size_t size = Text::Encoding::GetConversionRequiredByteCount(Text::Encoding::GetWideCharEncoding(), Text::Encoding::GetUTF32Encoding(), length);
	m_utf32Buffer.Resize(size);

	// UTF32 に変換
	Text::EncodingConversionResult result;
	Text::Encoding::Convert(
		text, length, Text::Encoding::GetWideCharEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Text::Encoding::GetUTF32Encoding(),
		&result);

	return GetTextSize((UTF32*)m_utf32Buffer.GetData(), result.BytesUsed / sizeof(UTF32));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size FreeTypeFont::GetTextSize(const UTF32* text, int length)
{
	UpdateFont();

	FT_Bool useKerning = FT_HAS_KERNING(m_ftFace);
	FT_UInt previous = 0;

	int w = 0;
	const UTF32* code = text;
	int count = 0;
	for (; *code; ++code)
	{
		FT_UInt glyph_index = FTC_CMapCache_Lookup(
			m_manager->GetFTCacheMapCache(),
			m_ftFaceID,
			m_ftCacheMapIndex,
			*code);

		// カーニング
		if (useKerning && previous != 0 && glyph_index)
		{
			FT_Vector delta;
			FT_Get_Kerning(
				m_ftFace,
				previous,
				glyph_index,
				ft_kerning_default,
				&delta);
			w += delta.x >> 6;
		}
		previous = glyph_index;

		FT_Glyph glyph;
		FT_Error err = FTC_ImageCache_Lookup(
			m_manager->GetFTCImageCache(),
			&m_ftImageType,
			glyph_index,
			&glyph,
			NULL);
		LN_THROW(err == 0, InvalidOperationException, "failed FTC_ImageCache_Lookup : %d\n", err);

		w += glyph->advance.x >> 16;
	

		count++;
		if (count >= length)
			break;
	}

	//glyphData->NextBaseY = (m_ftFace->height + m_ftFace->descender) *
	//							m_ftFace->size->metrics.y_ppem / m_ftFace->units_per_EM;
	//glyphData->GlyphOffsetX = glyphData->NextBaseX + glyph_bitmap->left;
	//glyphData->GlyphOffsetY = glyphData->NextBaseY - glyph_bitmap->top;

	return Size(w, m_ftFace->size->metrics.height >> 6);//m_ftImageType.height/*m_ftFace->size->metrics.height >> 6*/ );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontGlyphLocation* FreeTypeFont::AdvanceKerning(UTF32 utf32code, FontGlyphLocation* prevData_)
{
	FreeTypeGlyphLocation* prevData = static_cast<FreeTypeGlyphLocation*>(prevData_);
	FreeTypeGlyphLocation* locData;

	// ひとつ前のデータがあればそれをベースに
	if (prevData)
	{
		locData = prevData;
	}
	// 最初の文字であればデータリセット
	else
	{
		UpdateFont();
		m_fontGlyphLocation.BitmapTopLeftPosition = Point::Zero;
		m_fontGlyphLocation.OutlineBitmapTopLeftPosition = Point::Zero;
		m_fontGlyphLocation.OuterTopLeftPosition = Point::Zero;
		m_fontGlyphLocation.NextBaseX = 0;
		m_fontGlyphLocation.NextBaseY = 0;
		m_fontGlyphLocation.PreviousGlyphIndex = 0;
		locData = &m_fontGlyphLocation;
	}

	// 文字に対する glyph index を取得する
	FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		m_manager->GetFTCacheMapCache(),
		m_ftFaceID,
		m_ftCacheMapIndex,
		utf32code);



	// ひとつ前のデータがある場合、ひとつ前の文字からの位置を求める
	if (prevData != NULL)
	{
		// カーニング
		if ((FT_HAS_KERNING(m_ftFace)) && prevData->PreviousGlyphIndex && glyphIndex)
		{
			FT_Vector delta;
			FT_Get_Kerning(
				m_ftFace,
				prevData->PreviousGlyphIndex, glyphIndex,
				ft_kerning_default, &delta);
			locData->NextBaseX += delta.x >> 6;	// ベースラインをずらす
		}
	}

	/* load glyph image into the slot (erase previous one) */
	FT_Glyph glyph;
	FT_Error err = FTC_ImageCache_Lookup(
		m_manager->GetFTCImageCache(),
		&m_ftImageType,
		glyphIndex,
		&glyph,
		NULL);
	LN_THROW(err == 0, InvalidOperationException, "failed FTC_ImageCache_Lookup : %d\n", err);

	// キャッシュが効いている時は FT_Load_Glyph を呼ばないと slot->metrics が更新されない。
	// つまり、レイアウトが正常に行われない。
	// ドキュメントにも slot->metrics は FT_Load_Glyph で更新される、とある。
	// ここではホントに metrics だけわかればいいので、ビットマップとかを作る必要は無い。
	// 後で必要最小限のフラグを探す。TODO:
	err = FT_Load_Glyph(m_ftFace, glyphIndex, m_ftImageType.flags);
	LN_THROW(err == 0, InvalidOperationException, "failed FTC_ImageCache_Lookup : %d\n", err);

	// 太字フォント
	if (m_isBold)
	{
		// アウトラインフォントである必要がある
		LN_THROW((glyph->format == FT_GLYPH_FORMAT_OUTLINE), InvalidOperationException, "glyph->format != FT_GLYPH_FORMAT_OUTLINE");

		FT_Pos strength = (800 - 400) / 8;	// 太さ
		err = FT_Outline_Embolden(&m_ftFace->glyph->outline, strength);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Outline_Embolden : %d\n", err);
	}

	FT_GlyphSlot slot = m_ftFace->glyph;

	// 転送座標オフセットを進める
	locData->NextBaseY = (m_ftFace->height + m_ftFace->descender) * m_ftFace->size->metrics.y_ppem / m_ftFace->units_per_EM;
	locData->BitmapTopLeftPosition.X = locData->NextBaseX + slot->bitmap_left;
	locData->BitmapTopLeftPosition.Y = locData->NextBaseY - (slot->metrics.horiBearingY >> 6);//slot->bitmap_top;// (slot->metrics.horiBearingY * m_ftFace->size->metrics.y_ppem / m_ftFace->units_per_EM)/*- slot->bitmap_top*/;

	locData->BitmapSize.Set(slot->bitmap.width, slot->bitmap.rows);

	// 直前の glyph_index として記憶
	locData->PreviousGlyphIndex = glyphIndex;

	// 次の文字へ
	locData->NextBaseX += slot->advance.x >> 6;

	// アウトライン有り
	if (m_ftStroker != NULL)
	{
		locData->OutlineBitmapTopLeftPosition.X = locData->BitmapTopLeftPosition.X - m_edgeSize;
		locData->OutlineBitmapTopLeftPosition.Y = locData->BitmapTopLeftPosition.Y - m_edgeSize;
		locData->OuterTopLeftPosition = locData->OutlineBitmapTopLeftPosition;
	}
	// アウトライン無し
	else
	{
		locData->OutlineBitmapTopLeftPosition = Point::Zero;
		locData->OuterTopLeftPosition = locData->BitmapTopLeftPosition;
	}
	
	return locData;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontGlyphBitmap* FreeTypeFont::LookupGlyphBitmap(UTF32 utf32code)
{
	FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		m_manager->GetFTCacheMapCache(),
		m_ftFaceID,
		m_ftCacheMapIndex,
		utf32code);

	FT_Glyph glyph;
	FT_Error err = FTC_ImageCache_Lookup(
		m_manager->GetFTCImageCache(),
		&m_ftImageType,
		glyphIndex,
		&glyph,
		NULL);
	LN_THROW(err == 0, InvalidOperationException, "failed FTC_ImageCache_Lookup : %d\n", err);

	// 太字フォント
	if (m_isBold)
	{
		// アウトラインフォントである必要がある
		LN_THROW((glyph->format == FT_GLYPH_FORMAT_OUTLINE), InvalidOperationException, "glyph->format != FT_GLYPH_FORMAT_OUTLINE");

		FT_Pos strength = (800 - 400) / 8;	// 太さ
		err = FT_Outline_Embolden(&m_ftFace->glyph->outline, strength);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Outline_Embolden : %d\n", err);
	}
	FT_Render_Mode renderMode = (m_isAntiAlias) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

	FT_BitmapGlyph glyph_bitmap;
	if (glyph->format == FT_GLYPH_FORMAT_BITMAP) {
		// FT_LOAD_NO_BITMAP が OFF だとここに入ってくる
		glyph_bitmap = (FT_BitmapGlyph)glyph;
	}
	else
	{
		err = FT_Glyph_Copy(glyph, &m_fontGlyphBitmap.CopyGlyph);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_Copy : %d\n", err);

		/* FT_RENDER_MODE_NORMALで普通の(256階調)アンチエイリアス
		* FT_RENDER_MODE_LCDで液晶用アンチエイリアス(サブピクセルレンダリング)
		*/
		err = FT_Glyph_To_Bitmap(&m_fontGlyphBitmap.CopyGlyph, renderMode, NULL, 1);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_To_Bitmap : %d\n", err);

		glyph_bitmap = (FT_BitmapGlyph)m_fontGlyphBitmap.CopyGlyph;
	}

	FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
	RefreshBitmap(m_glyphBitmap, ft_bitmap);
	m_fontGlyphBitmap.GlyphBitmap = m_glyphBitmap;

	// 枠線
	if (m_ftStroker)
	{
		err = FT_Glyph_Copy(glyph, &m_fontGlyphBitmap.CopyOutlineGlyph);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_Copy : %d\n", err);

		err = FT_Glyph_StrokeBorder(&m_fontGlyphBitmap.CopyOutlineGlyph, m_ftStroker, 0, 1);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_StrokeBorder : %d\n", err);

		err = FT_Glyph_To_Bitmap(&m_fontGlyphBitmap.CopyOutlineGlyph, renderMode, NULL, 1);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_To_Bitmap : %d\n", err);

		glyph_bitmap = (FT_BitmapGlyph)m_fontGlyphBitmap.CopyOutlineGlyph;



		FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
		RefreshBitmap(m_outlineBitmap, ft_bitmap);
		m_fontGlyphBitmap.OutlineBitmap = m_outlineBitmap;
		m_fontGlyphBitmap.OutlineOffset = m_edgeSize;
	}
	else
	{
		m_fontGlyphBitmap.OutlineBitmap = NULL;
		m_fontGlyphBitmap.OutlineOffset = 0;
	}

	return &m_fontGlyphBitmap;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#if 0
FontGlyphData* FreeTypeFont::LookupGlyphData(UTF32 utf32code, FontGlyphData* prevData_)
{
	FreeTypeGlyphData* prevData = static_cast<FreeTypeGlyphData*>(prevData_);
	FreeTypeGlyphData* glyphData;

	m_glyphData.ReleaseGlyph();
	m_glyphData.GlyphBitmap = m_glyphBitmap;

	// ひとつ前のデータがあればそれをベースに
	if (prevData)
	{
		glyphData = prevData;
	}
	// 最初の文字であればデータリセット
	else
	{
		UpdateFont();
		glyphData = &m_glyphData;
		glyphData->NextBaseX = 0;
		glyphData->Previous = 0;
		glyphData->CopyGlyph = NULL;
		glyphData->CopyOutlineGlyph = NULL;
	}

	//-----------------------------------------------------
	// グリフ取得

	/* キャッシュを使うときは…
	FT_Set_Char_Size のかわりに FTC_Manager_LookupSize
	FT_Get_Char_Index のかわりに FTC_CMapCache_Lookup
	FT_Load_Glyph のかわりに FTC_ImageCache_Lookup
	*/

	FT_UInt glyph_index = FTC_CMapCache_Lookup(
		m_manager->GetFTCacheMapCache(),
		m_ftFaceID,
		m_ftCacheMapIndex,
		utf32code);

	FT_Glyph glyph;
	FT_Error err = FTC_ImageCache_Lookup(
		m_manager->GetFTCImageCache(),
		&m_ftImageType,
		glyph_index,
		&glyph,
		NULL);
	LN_THROW(err == 0, InvalidOperationException, "failed FTC_ImageCache_Lookup : %d\n", err);

	// 太字フォント
	if (m_isBold)
	{
		// アウトラインフォントである必要がある
		LN_THROW((glyph->format == FT_GLYPH_FORMAT_OUTLINE), InvalidOperationException, "glyph->format != FT_GLYPH_FORMAT_OUTLINE");

		FT_Pos strength = (800 - 400) / 8;	// 太さ
		err = FT_Outline_Embolden(&m_ftFace->glyph->outline, strength);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Outline_Embolden : %d\n", err);
	}

	FT_Render_Mode renderMode = (m_isAntiAlias) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

	FT_BitmapGlyph glyph_bitmap;
	if (glyph->format == FT_GLYPH_FORMAT_BITMAP) {
		// FT_LOAD_NO_BITMAP が OFF だとここに入ってくる
		glyph_bitmap = (FT_BitmapGlyph)glyph;
	}
	else
	{
		err = FT_Glyph_Copy(glyph, &glyphData->CopyGlyph);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_Copy : %d\n", err);

		/* FT_RENDER_MODE_NORMALで普通の(256階調)アンチエイリアス
		* FT_RENDER_MODE_LCDで液晶用アンチエイリアス(サブピクセルレンダリング)
		*/
		err = FT_Glyph_To_Bitmap(&glyphData->CopyGlyph, renderMode, NULL, 1);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_To_Bitmap : %d\n", err);

		glyph_bitmap = (FT_BitmapGlyph)glyphData->CopyGlyph;
	}

	FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
	RefreshBitmap(m_glyphBitmap, ft_bitmap);

	if (m_ftStroker)
	{
		err = FT_Glyph_Copy(glyph, &glyphData->CopyOutlineGlyph);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_Copy : %d\n", err);

		err = FT_Glyph_StrokeBorder(&glyphData->CopyOutlineGlyph, m_ftStroker, 0, 1);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_StrokeBorder : %d\n", err);

		err = FT_Glyph_To_Bitmap(&glyphData->CopyOutlineGlyph, renderMode, NULL, 1);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Glyph_To_Bitmap : %d\n", err);

		glyph_bitmap = (FT_BitmapGlyph)glyphData->CopyOutlineGlyph;



		FT_Bitmap* ft_bitmap = &glyph_bitmap->bitmap;
		RefreshBitmap(m_outlineBitmap, ft_bitmap);

		m_glyphData.OutlineBitmap = m_outlineBitmap;
		m_glyphData.OutlineOffset = m_edgeSize;
	}
	else
	{
		m_glyphData.OutlineBitmap = NULL;
		m_glyphData.OutlineOffset = 0;
	}

	//-----------------------------------------------------
	// 位置調整

	// 転送座標オフセットを進める
	glyphData->NextBaseY = (m_ftFace->height + m_ftFace->descender) *
		m_ftFace->size->metrics.y_ppem / m_ftFace->units_per_EM;
	glyphData->GlyphOffsetX = glyphData->NextBaseX + glyph_bitmap->left;
	glyphData->GlyphOffsetY = glyphData->NextBaseY - glyph_bitmap->top;

	// ひとつ前のデータがある場合、ひとつ前の文字からの位置を求める
	if (prevData)
	{
		// カーニング
		if ((FT_HAS_KERNING(m_ftFace)) && prevData->Previous && glyph_index)
		{
			FT_Vector delta;
			FT_Get_Kerning(m_ftFace,
				prevData->Previous, glyph_index,
				ft_kerning_default, &delta);
			glyphData->NextBaseX += delta.x >> 6;
		}
	}
	// 初回文字 (リセット)
	else
	{
		// 初回の生成で負の値は許さない (j 等が -1 となってしまい、想定した描画領域からはみ出す)
		//if ( glyphData->GlyphOffsetX < 0 ) glyphData->GlyphOffsetX = 0;
		//if ( glyphData->GlyphOffsetY < 0 ) glyphData->GlyphOffsetY = 0;
	}

	// 直前の glyph_index として記憶
	glyphData->Previous = glyph_index;

	// 次の文字へ
	glyphData->NextBaseX += glyph_bitmap->root.advance.x >> 16;

	// 行高さ
	glyphData->MetricsHeight = m_ftFace->size->metrics.height >> 6;

	return glyphData;
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::UpdateFont()
{
	if (m_modified)
	{
		m_ftFaceID = (FTC_FaceID)Hash::CalcHash(m_fontName);
		FTC_Manager ftc_manager = m_manager->GetFTCacheManager();
		m_manager->m_requesterFaceName = m_fontName;

		FT_Error err = FTC_Manager_LookupFace(ftc_manager, m_ftFaceID, &m_ftFace);
		LN_THROW(err == 0, InvalidOperationException, "failed FTC_Manager_LookupFace : %d\n", err);

		if (m_isItalic)
		{
			// イタリック体の場合は Transform で傾ける
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

		// m_fontSize に対する本当の文字サイズを取得する
		FTC_ScalerRec scaler;
		scaler.face_id = m_ftFaceID;
		scaler.width = 0;
		scaler.height = m_fontSize << 6;
		scaler.pixel = 0;
		scaler.x_res = RESOLUTION_X;
		scaler.y_res = RESOLUTION_Y;
		FT_Size ft_size;
		err = FTC_Manager_LookupSize(ftc_manager, &scaler, &ft_size);
		LN_THROW(err == 0, InvalidOperationException, "failed FTC_Manager_LookupSize : %d\n", err);

		m_lineHeight = ft_size->metrics.height >> 6;

		Dispose();
		if (m_edgeSize > 0)
		{
			// エッジの描画情報
			FT_Stroker_New(m_manager->GetFTLibrary(), &m_ftStroker);
			FT_Stroker_Set(m_ftStroker,
				(int)(m_edgeSize * 64),
				FT_STROKER_LINECAP_ROUND,	// 線分の両端は半円でレンダリングする
				FT_STROKER_LINEJOIN_ROUND,	// 線分の接合点は半円でレンダリングする
				0);
		}


		// font_typeを設定
		m_ftImageType.face_id = m_ftFaceID;
		m_ftImageType.width = 0;
		m_ftImageType.height = m_fontSize;
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
		if (m_edgeSize > 0 || m_isBold) {
			m_ftImageType.flags = FT_LOAD_NO_BITMAP;
		}
		// アウトライン OFF
		else {
			m_ftImageType.flags = FT_LOAD_RENDER;
		}
		// アンチエイリアス ON
		if (m_isAntiAlias) {
			//m_ftImageType.flags = ; そのまま
		}
		// アンチエイリアス OFF
		else {
			m_ftImageType.flags |= FT_LOAD_MONOCHROME;
		}

		// グリフ格納用ビットマップ (仮確保)
		m_glyphBitmap.Attach(LN_NEW Bitmap(Size(m_fontSize, m_fontSize), PixelFormat_A8));
		m_outlineBitmap.Attach(LN_NEW Bitmap(Size(m_fontSize, m_fontSize), PixelFormat_A8));

		// ラスタライズで使用する
		//mPixelList = LN_NEW PixelData[m_fontSize * m_fontSize * 4];

		m_modified = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::Dispose()
{
	m_glyphData.ReleaseGlyph();

	//SAFE_DELETE_ARRAY(mPixelList);
	if (m_ftStroker)
	{
		FT_Stroker_Done(m_ftStroker);
		m_ftStroker = NULL;
	}

	// キャッシュを使用している場合、FT_Done_Face( m_ftFace ); などで解放の必要はない。
	// 解放してしまうと、CacheManager の解放時に NULL アクセスが起こる。
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::RefreshBitmap(Bitmap* bitmap, FT_Bitmap* ftBitmap)
{
	int width = ftBitmap->width;
	int height = ftBitmap->rows;

	// サイズ
	bitmap->m_size.Width = width;
	bitmap->m_size.Height = height;
	bitmap->m_pitch = abs(ftBitmap->pitch);
	if (ftBitmap->pitch < 0) {
		bitmap->m_upFlow = false;
	}

	// フォーマット
	if (ftBitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
		bitmap->m_format = PixelFormat_A1;
	}
	else if (ftBitmap->pixel_mode == FT_PIXEL_MODE_GRAY) {
		bitmap->m_format = PixelFormat_A8;
	}
	else {
		LN_THROW(0, InvalidOperationException, "Invalid pixel format.");
	}

	// ビットマップデータを参照モードでセットする
	bitmap->m_bitmapData.Attach(
		ftBitmap->buffer,
		Bitmap::GetPixelFormatByteCount(bitmap->m_format, bitmap->m_size));
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::_reserveBitmapBuffer(int newWidth, int newHeight, int newEdgeSize)
{
	// ビットマップを格納するバッファサイズが足りなければ再作成
	int w = (newWidth + newEdgeSize * 2);
	int h = (newWidth + newEdgeSize * 2);

	Base::ReferenceBuffer* bitmapData = m_glyphBitmap.getBitmapData();
	if (w * h > bitmapData->getSize())
	{
		bitmapData->reserve(w * h);
		bitmapData->clear();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::_copyBitmap(Base::ReferenceBuffer* dest, FT_Bitmap* ftBitmap)
{
	int width = ftBitmap->width;
	int height = ftBitmap->rows;

	// ビットマップバッファのコピー
	dest->clear();
	lnByte* buf = dest->getPointer();
	for (int y = 0; y < height; ++y)
	{
		lnByte* line = ftBitmap->pitch < 0 ?
			&ftBitmap->buffer[(-ftBitmap->pitch * height) - ftBitmap->pitch * y] :
			&ftBitmap->buffer[ftBitmap->pitch * y];
		for (int x = 0; x < width; ++x)
		{
			buf[y * width + x] = line[x];
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::renderSpans(FT_OutlineGlyph glyph)
{
	mPixelCount = 0;

	FT_Raster_Params params;
	memset(&params, 0, sizeof(params));
	params.gray_spans = rasterCallback;
	params.user = this;
	if (m_isAntiAlias) params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	else                params.flags = FT_RASTER_FLAG_DIRECT;
	// ↑FT_RASTER_FLAG_DIRECT を使用する場合はコールバックを登録する必要があるが、
	// 2.4現在、アンチエイリアス時のみしかサポートしていない。

	FT_Outline_Render(m_manager->getFTLibrary(), &glyph->outline, &params);

	int width = mPixelMaxX - mPixelMinX + 1;
	int height = mPixelMaxY - mPixelMinY + 1;
#if 0
	if (width != mFontGlyphData.GlyphBitmap.mWidth ||
		height != mFontGlyphData.GlyphBitmap.mHeight)
	{
		mFontGlyphData.GlyphBitmap.create(width, height, LN_BITMAPFORMAT_A8);
		mFontGlyphData.GlyphBitmap.mWidth = width;
		mFontGlyphData.GlyphBitmap.mHeight = height;
	}
#else
	// ビットマップを格納するバッファサイズが足りなければ再作成
	_reserveBitmapBuffer(width, height, m_edgeSize);

	m_glyphBitmap.mWidth = width;
	m_glyphBitmap.mHeight = height;
#endif
	// ビットマップバッファに格納
	Base::ReferenceBuffer* bitmapData = m_glyphBitmap.getBitmapData();
	lnByte* buf = bitmapData->getPointer();
	for (int i = 0; i < mPixelCount; ++i)
	{
		//printf("%d ", (m_glyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ));
		//printf( "x:%d y:%d w:%d h:%d minX:%d minY:%d\n",
		//    (mPixelList[i].X - mPixelMinX), 
		//    (m_glyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ),
		//    width, height,
		//    mPixelMinX, mPixelMinY);

		buf[
			(width * (height - 1 - (mPixelList[i].Y - mPixelMinY))) +
				(mPixelList[i].X - mPixelMinX)
		] = (mtest) ? 0 : mPixelList[i].Coverage;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::rasterCallback(
	const int y,
	const int count,
	const FT_Span * const spans,
	void * const user)
{
	FreeTypeFont* font = (FreeTypeFont*)user;

	font->mPixelMinY = std::min(y, font->mPixelMinY);
	font->mPixelMaxY = std::max(y, font->mPixelMaxY);

	for (int i = 0; i < count; ++i)
	{
		for (int cx = 0; cx < spans[i].len; ++cx)
		{
			font->mPixelList[font->mPixelCount].X = spans[i].x + cx;
			font->mPixelList[font->mPixelCount].Y = y;
			font->mPixelList[font->mPixelCount].Coverage = spans[i].coverage;
			font->mPixelMinX = std::min((int)font->mPixelList[font->mPixelCount].X, font->mPixelMinX);
			font->mPixelMaxX = std::max((int)font->mPixelList[font->mPixelCount].X, font->mPixelMaxX);
			++font->mPixelCount;
		}
	}
}
#endif
	
} // namespace Graphics
} // namespace Lumino
