#pragma once
#include "FontCore.hpp"

namespace ln {
class Bitmap2D;

namespace detail {

class FreeTypeFontCached
	: public FontCore
{
public:
    FreeTypeFontCached();
	void init(FontManager* manager, const FontDesc& desc);
	virtual void dispose() override;

	virtual void getGlobalMetrics(FontGlobalMetrics* outMetrics) override;
	virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) override;
	virtual Vector2 getKerning(UTF32 prev, UTF32 next) override;
	virtual void lookupGlyphBitmap(UTF32 utf32code, BitmapGlyphInfo* outInfo) override;
	virtual void decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo) override;

private:
	void updateImageFlags();
	bool getOutlineTextMetrix();
	bool getBitmapTextMetrix();
	void FT_BitmapToBitmap2D(FT_Bitmap* ftBitmap, Bitmap2D* bitmap) const;

	FontDesc m_desc;
	FTC_FaceID m_ftFaceId;			// キャッシュから FT_Face を検索するためのキー値
	FT_Face m_ftFace;				// フォント本体
	FT_Int m_ftCacheMapIndex;		// m_ftFace 内で現在アクティブな FT_CharMap の番号
	FTC_ImageTypeRec m_ftImageType;	// キャッシュからグリフビットマップを取りだすための情報
	Ref<Bitmap2D> m_internalCacheBitmap;
};

class FreeTypeFont
	: public FontCore
{
public:
	FreeTypeFont();
	Result init(FontManager* manager, const FontDesc& desc);
	virtual void dispose() override;

	virtual void getGlobalMetrics(FontGlobalMetrics* outMetrics) override;
	virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) override;
	virtual Vector2 getKerning(UTF32 prev, UTF32 next) override;
	virtual void lookupGlyphBitmap(UTF32 utf32code, BitmapGlyphInfo* outInfo) override;
	virtual void decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo) override;

private:
	void getGlobalMetricsInternal(FontGlobalMetrics* outMetrics, bool fromInit) const;
	bool getOutlineTextMetrix();
	bool getBitmapTextMetrix();
	Bitmap2D* FTBitmapToInternalCacheBitmap(FT_Bitmap* ftBitmap);
	void FTBitmapToBitmap2D(FT_Bitmap* ftBitmap, Bitmap2D* bitmap) const;

	FontDesc m_desc;
	FT_Face m_face;
	FT_Int32 m_loadFlags;
	FontGlobalMetrics m_engineDefaultGlobalMetrix;
	Ref<Bitmap2D> m_internalCacheBitmapGray;
	Ref<Bitmap2D> m_internalCacheBitmapRGBA;
};


} // namespace detail
} // namespace ln

