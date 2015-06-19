
#include "../Internal.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include <Lumino/Base/Hash.h>
#include "FreeTypeFont.h"

namespace Lumino
{
namespace Imaging
{

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
	//, m_ftFace(NULL)
	//, m_ftCacheMapIndex(0)
	//, m_modified(false)
	//, m_fontName()
	//, m_fontSize(0)
	//, mColor(LColor::Gray)
	//, mEdgeColor(0, 0, 0, 1)
	//, m_edgeSize(0)
	//, m_isBold(false)
	//, m_isItalic(false)
	//, mIsAntiAlias(false)
	////, mCopyGlyph    ( NULL )
	////, mBitmapRealDataBytes   ( 0 )
	//, mPixelList(NULL)
	//, m_ftStroker(NULL)

	/*
	mFontHandle           ( NULL )
	, m_fontName             ( LN_DEFAULT_FONT_NAME )
	, m_fontSize             ( 22 )
	, mGlyphType            ( LN_GT_NOEDGE_NOANTI )
	, mEdgeColor            ( 0, 0, 0, 1 )
	, m_edgeSize             ( 0 )
	, mRealEdgeSize         ( 0 )
	, mOutlineQuality       ( FONT_ANTI_QUALITY )
	, mMemDC                ( NULL )
	, mDIBBitmap            ( NULL )
	, mDIBBitmapBuffer      ( NULL )
	, mDIBBitmapWidth       ( 0 )
	, mDIBBitmapHeight      ( 0 )
	, mOutlinePen           ( NULL )
	, mOutlineBrush         ( NULL )
	, mOutlineBGBrush       ( NULL )

	, mBitmapBufferSize     ( 0 )
	, mHDC                  ( NULL )

	, mOldFont              ( NULL )

	, mNeedUpdate           ( true )
	, mBeganGetGlyphOutline ( false )
	*/
{
	//memset( &mGlyphOutlineData, 0, sizeof( mGlyphOutlineData ) );
	//memset( &mGlyphBitmap, 0, sizeof( mGlyphBitmap ) );
	//mFontGlyphData.Bitmap = &mGlyphBitmap;

	mFontGlyphData.GlyphBitmap = &mGlyphBitmap;
	mFontGlyphData.CopyGlyph = NULL;

	LN_SAFE_ADDREF(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FreeTypeFont::~FreeTypeFont()
{
	dispose();
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size FreeTypeFont::GetTextSize(const char* text, int length)
{
	size_t size = Text::Encoding::GetConversionRequiredByteCount(Text::Encoding::GetSystemMultiByteEncoding(), Text::Encoding::GetUTF32Encoding(), length);
	m_utf32Buffer.Resize(size);
	
	Text::EncodingConversionResult result;
	Text::Encoding::Convert(
		text, length, Text::Encoding::GetSystemMultiByteEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Text::Encoding::GetUTF32Encoding(),
		&result);

	return GetTextSizeInternal((UTF32*)m_utf32Buffer.GetData(), m_utf32Buffer.GetSize() / sizeof(UTF32));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size FreeTypeFont::GetTextSize(const wchar_t* text, int length)
{
	size_t size = Text::Encoding::GetConversionRequiredByteCount(Text::Encoding::GetWideCharEncoding(), Text::Encoding::GetUTF32Encoding(), length);
	m_utf32Buffer.Resize(size);

	Text::EncodingConversionResult result;
	Text::Encoding::Convert(
		text, length, Text::Encoding::GetWideCharEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Text::Encoding::GetUTF32Encoding(),
		&result);

	return GetTextSizeInternal((UTF32*)m_utf32Buffer.GetData(), m_utf32Buffer.GetSize() / sizeof(UTF32));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Font* FreeTypeFont::copy()
{
	LRefPtr<FreeTypeFont> font(LN_NEW FreeTypeFont(m_manager));
	font->initialize(m_fontName.c_str(), m_fontSize, mColor, m_isBold, m_isItalic);
	font->setEdgeColor(mEdgeColor);
	font->setEdgeSize(m_edgeSize);
	font->setAntiAlias(mIsAntiAlias);
	font->addRef();
	return font;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontGlyphData* FreeTypeFont::makeGlyphData(lnU32 utf32code, FontGlyphData* prevData_)
{
	FreeTypeFontGlyphData* prevData = static_cast<FreeTypeFontGlyphData*>(prevData_);
	FreeTypeFontGlyphData* glyphData;

	// ひとつ前のデータがあればそれをベースに
	if (prevData)
	{
		glyphData = prevData;

		if (glyphData->CopyGlyph) {
			FT_Done_Glyph(glyphData->CopyGlyph);
			glyphData->CopyGlyph = NULL;
		}
		if (glyphData->CopyOutlineGlyph) {
			FT_Done_Glyph(glyphData->CopyOutlineGlyph);
			glyphData->CopyOutlineGlyph = NULL;
		}
	}
	// 最初の文字であればデータリセット
	else
	{
		updateFont();

		glyphData = &mFontGlyphData;
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
		m_manager->getFTCacheMapCache(),
		m_ftFaceID,
		m_ftCacheMapIndex,
		utf32code);

	FT_Glyph glyph;
	FT_Error err = FTC_ImageCache_Lookup(
		m_manager->getFTCImageCache(),
		&m_ftImageType,
		glyph_index,
		&glyph,
		NULL);
	LN_THROW(err == 0, InvalidOperationException, "failed FTC_ImageCache_Lookup : %d\n", err);

	// 太字フォント
	if (m_isBold)
	{
		// アウトラインフォントである必要がある
		LN_THROW_InvalidOperation((glyph->format == FT_GLYPH_FORMAT_OUTLINE), "glyph->format != FT_GLYPH_FORMAT_OUTLINE");

		FT_Pos strength = (800 - 400) / 8;	// 太さ
		err = FT_Outline_Embolden(&m_ftFace->glyph->outline, strength);
		LN_THROW(err == 0, InvalidOperationException, "failed FT_Outline_Embolden : %d\n", err);
	}

	FT_Render_Mode renderMode = (mIsAntiAlias) ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

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
	_refreshBitmap(&mGlyphBitmap, ft_bitmap);

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
		_refreshBitmap(&mOutlineBitmap, ft_bitmap);

		mFontGlyphData.OutlineBitmap = &mOutlineBitmap;
		mFontGlyphData.OutlineOffset = m_edgeSize;
	}
	else
	{
		mFontGlyphData.OutlineBitmap = NULL;
		mFontGlyphData.OutlineOffset = 0;
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::postGlyphData(FontGlyphData* glyphData_)
{
	FreeTypeFontGlyphData* glyphData = static_cast<FreeTypeFontGlyphData*>(glyphData_);

	if (glyphData)
	{
		if (glyphData->CopyGlyph) {
			FT_Done_Glyph(glyphData->CopyGlyph);
			glyphData->CopyGlyph = NULL;
		}
		if (glyphData->CopyOutlineGlyph) {
			FT_Done_Glyph(glyphData->CopyOutlineGlyph);
			glyphData->CopyOutlineGlyph = NULL;
		}
	}
	//if ( glyph_data )
	//{
	//    //SAFE_DELETE_ARRAY( glyph_data->Bitmap->Buffer );
	//    //SAFE_DELETE( glyph_data->Bitmap );
	//    //SAFE_DELETE( glyph_data );
	//}
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::UpdateFont()
{
	m_ftFaceID = (FTC_FaceID)Hash::CalcHash(m_fontName);
	FTC_Manager ftc_manager = m_manager->GetFTCacheManager();
	m_manager->mRequesterFaceName = m_fontName;

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


	if (m_modified)
	{
		dispose();
		if (m_edgeSize > 0)
		{
			FT_Stroker_New(m_manager->getFTLibrary(), &m_ftStroker);
			FT_Stroker_Set(m_ftStroker,
				(int)(m_edgeSize * 64),
				FT_STROKER_LINECAP_ROUND,
				FT_STROKER_LINEJOIN_ROUND,
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
		if (mIsAntiAlias) {
			//m_ftImageType.flags = ; そのまま
		}
		// アンチエイリアス OFF
		else {
			m_ftImageType.flags |= FT_LOAD_MONOCHROME;
		}



		mPixelList = LN_NEW PixelData[m_fontSize * m_fontSize * 4];

		// グリフ格納用ビットマップ (ダミー確保)
		mGlyphBitmap.create(m_fontSize, m_fontSize, LN_BITMAPFORMAT_A8);
		mOutlineBitmap.create(m_fontSize, m_fontSize, LN_BITMAPFORMAT_A8);

		m_modified = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size FreeTypeFont::GetTextSizeInternal(const UTF32* text, int length)
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
void FreeTypeFont::dispose()
{
	//SAFE_DELETE_ARRAY( mGlyphBitmap.Buffer );
	SAFE_DELETE_ARRAY(mPixelList);
	if (m_ftStroker)
	{
		FT_Stroker_Done(m_ftStroker);
		m_ftStroker = NULL;
	}

	// キャッシュを使用している場合、解放の必要はない。
	// 解放してしまうと、CacheManager の解放時に NULL アクセスが起こる。
	//if ( m_ftFace )
	//{
	//    FT_Done_Face( m_ftFace );
	//    m_ftFace = NULL;
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::_refreshBitmap(Bitmap* bitmap, FT_Bitmap* ftBitmap)
{
	int width = ftBitmap->width;
	int height = ftBitmap->rows;

	// サイズ
	bitmap->mWidth = width;
	bitmap->mHeight = height;
	bitmap->mPitch = ftBitmap->pitch;
	if (ftBitmap->pitch < 0) {
		bitmap->mUpFlow = false;
	}

	// フォーマット
	if (ftBitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
		bitmap->mFormat = LN_BITMAPFORMAT_A1;
	}
	else if (ftBitmap->pixel_mode == FT_PIXEL_MODE_GRAY) {
		bitmap->mFormat = LN_BITMAPFORMAT_A8;
	}
	else {
		LN_THROW_InvalidOperation(0);
	}

	// ビットマップデータ
	bitmap->mBitmapData->setBytesBuffer(
		ftBitmap->buffer,
		Bitmap::getPixelFormatByteCount(bitmap->mFormat, width, height),
		true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FreeTypeFont::_reserveBitmapBuffer(int newWidth, int newHeight, int newEdgeSize)
{
	// ビットマップを格納するバッファサイズが足りなければ再作成
	int w = (newWidth + newEdgeSize * 2);
	int h = (newWidth + newEdgeSize * 2);

	Base::ReferenceBuffer* bitmapData = mGlyphBitmap.getBitmapData();
	if (w * h > bitmapData->getSize())
	{
		bitmapData->reserve(w * h);
		bitmapData->clear();
	}
}

#if 0
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
	if (mIsAntiAlias) params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
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

	mGlyphBitmap.mWidth = width;
	mGlyphBitmap.mHeight = height;
#endif
	// ビットマップバッファに格納
	Base::ReferenceBuffer* bitmapData = mGlyphBitmap.getBitmapData();
	lnByte* buf = bitmapData->getPointer();
	for (int i = 0; i < mPixelCount; ++i)
	{
		//printf("%d ", (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ));
		//printf( "x:%d y:%d w:%d h:%d minX:%d minY:%d\n",
		//    (mPixelList[i].X - mPixelMinX), 
		//    (mGlyphBitmap.Height - 1 - (mPixelList[i].Y - mPixelMinY) ),
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
	
} // namespace Imaging
} // namespace Lumino
