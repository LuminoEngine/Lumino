#pragma once
#include "FontCore.hpp"

namespace ln {
namespace detail {

class FreeTypeFont
	: public FontCore
{
public:
    FreeTypeFont();
	void initialize(FontManager* manager, const FontDesc& desc);
	virtual void dispose() override;

	virtual void getGlobalMetrics(FontGlobalMetrics* outMetrics) override;
	virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) override;
	virtual Vector2 getKerning(UTF32 prev, UTF32 next) override;
	virtual void decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo) override;

private:
	void updateImageFlags();
	bool getOutlineTextMetrix();
	bool getBitmapTextMetrix();

	FontDesc m_desc;
	FTC_FaceID m_ftFaceId;			// キャッシュから FT_Face を検索するためのキー値
	FT_Face m_ftFace;				// フォント本体
	FT_Int m_ftCacheMapIndex;		// m_ftFace 内で現在アクティブな FT_CharMap の番号
	FTC_ImageTypeRec m_ftImageType;	// キャッシュからグリフビットマップを取りだすための情報
};

} // namespace detail
} // namespace ln

