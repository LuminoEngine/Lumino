
#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include <Lumino/Graphics/Bitmap.h>
#include <Lumino/Graphics/Text/Font.h>
#include "FontManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail {

struct FreeTypeGlyphLocation
	: public FontGlyphLocation
{
	int				NextBaseX;			///< 内部データ
	int				NextBaseY;			///< 内部データ
	uint32_t		PreviousGlyphIndex;	///< ひとつ前の文字の GlyphIndex
};

struct FreeTypeGlyphBitmap
	: public FontGlyphBitmap
{
	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている

	FreeTypeGlyphBitmap()
	{
		CopyGlyph = NULL;
		CopyOutlineGlyph = NULL;
	}
	~FreeTypeGlyphBitmap()
	{
		ReleaseGlyph();
	}

	void ReleaseGlyph();
};

// [Obsolete]
struct FreeTypeGlyphData
	: public FontGlyphData
{
	//int             BitmapRealDataSize;    ///< 内部データ
	int				NextBaseX;      ///< 内部データ
	int				NextBaseY;      ///< 内部データ
	uint32_t		Previous;       ///< 内部データ
	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている

	void ReleaseGlyph();
};

class FreeTypeFont
	: public RawFont
{
public:
	FreeTypeFont();
	virtual ~FreeTypeFont();
	void initialize();
	virtual void Dispose_() override;

public:
	virtual void setName(const String& fontName) { m_fontData.Family = fontName; m_modified = true; }
	virtual const String& getName() const { return m_fontData.Family; }
	virtual void setSize(int size) { m_fontData.Size = size; m_modified = true; }
	virtual int getSize() const  { return m_fontData.Size; }
	//virtual void SetEdgeSize(int size) { m_fontData.EdgeSize = size; m_modified = true; }
	//virtual int GetEdgeSize() const  { return m_fontData.EdgeSize; }
	virtual void SetBold(bool enabled) { m_fontData.IsBold = enabled; m_modified = true; }
	virtual bool IsBold() const  { return m_fontData.IsBold; }
	virtual void SetItalic(bool enabled) { m_fontData.IsItalic = enabled; m_modified = true; }
	virtual bool IsItalic() const { return m_fontData.IsItalic; }
	virtual void SetAntiAlias(bool enabled) { m_fontData.IsAntiAlias = enabled; m_modified = true; }
	virtual bool IsAntiAlias() const { return m_fontData.IsAntiAlias; }

	virtual RawFontPtr copy() const;
	virtual int GetLineSpacing() { UpdateFont(); return m_lineHeight; }	// TODO: [Obsolete]
	//virtual Size GetTextSize(const char* text, int length);	// TODO: GetTextSize もいらなそう。UTF32 のだけあればいいかも？
	//virtual Size GetTextSize(const wchar_t* text, int length);
	//virtual Size GetTextSize(const UTF32* text, int length);
	virtual FontGlyphLocation* AdvanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData);
	virtual FontGlyphBitmap* LookupGlyphBitmap(UTF32 utf32code, int strokeSize);
	//virtual FontGlyphData* LookupGlyphData(UTF32 utf32code, FontGlyphData* prevData);

	virtual void GetGlobalMetrics(FontGlobalMetrics* outMetrics) override;
	virtual bool IsOutlineSupported() const override { return true; }
	virtual void DecomposeOutline(UTF32 utf32code, RawFont::VectorGlyphInfo* outInfo) override;
	virtual Vector2 GetKerning(UTF32 prev, UTF32 next) override;
	virtual void GetGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) override;

	virtual FontManager* getManager() const { return m_manager; }

	FTC_FaceID GetFTCFaceId() const { return m_ftFaceID; }
	FT_Face GetFTFace() const { return m_ftFace; }
	FT_Int GetFTCacheMapIndex() const { return m_ftCacheMapIndex; }

LN_INTERNAL_ACCESS:	// TODO
	void UpdateFont();
	void RefreshBitmap(Bitmap* bitmap, FT_Bitmap* ftBitmap);
	void UpdateImageFlags();
	void TryUpdateStroke(int newEdgeSize);
	bool IsOutLineMetrix() const;
	void GetOutlineTextMetrix();
	void GetBitmapTextMetrix();

	// FT_Outline_Funcs callbacks
	struct DecomposingState
	{
		FreeTypeFont*				thisData;
		List<OutlineInfo>*			outlines;
		List<FontOutlineVertex>*	vertices;
		Vector2						lastVertex;
		float						vectorScale;
		int							tessellationStep;
		float						delta1;
		float						delta2;
		float						delta3;
	};
	static int ftMoveToCallback(FT_Vector* to, DecomposingState* state);
	static int ftLineToCallback(FT_Vector* to, DecomposingState* state);
	static int ftConicToCallback(FT_Vector* control, FT_Vector* to, DecomposingState* state);
	static int ftCubicToCallback(FT_Vector* control1, FT_Vector* control2, FT_Vector* to, DecomposingState* state);
	static Vector2 FTVectorToLNVector(const FT_Vector* ftVec);

private:
	FontManager*		m_manager;
	//String				m_fontName;
	//int					m_fontSize;
	//int					m_edgeSize;
	//bool				m_isBold;
	//bool				m_isItalic;
	//bool				m_isAntiAlias;
	detail::FontData	m_fontData;
	int					m_edgeSize;
	bool				m_modified;

	FTC_FaceID			m_ftFaceID;			///< キャッシュから FT_Face を検索するためのキー値
	FT_Face				m_ftFace;			///< フォント本体
	FT_Int				m_ftCacheMapIndex;	///< m_ftFace 内で現在アクティブな FT_CharMap の番号
	FT_Stroker			m_ftStroker;		///< エッジの生成情報
	FTC_ImageTypeRec	m_ftImageType;		///< キャッシュからグリフビットマップを取りだすための情報
	int					m_lineHeight;

	ByteBuffer			m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)

	FreeTypeGlyphLocation	m_fontGlyphLocation;
	FreeTypeGlyphBitmap		m_fontGlyphBitmap;

	FT_Outline_Funcs		m_ftOutlineFuncs;

	// TODO: いらないかも
	FreeTypeGlyphData	m_glyphData;		///< LookupGlyphData() の戻り値として公開されるデータ
	RefPtr<Bitmap>		m_glyphBitmap;		///< LookupGlyphData() で生成する一時グリフデータ
	RefPtr<Bitmap>		m_outlineBitmap;	///< LookupGlyphData() で生成する一時グリフデータ
};

} // namespace detail
LN_NAMESPACE_END
