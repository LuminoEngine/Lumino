
#pragma once
#include <Lumino/Graphics/Text/Font.h>

LN_NAMESPACE_BEGIN
namespace detail {
	
// ビットマップフォントのクラス
class BitmapFont
    : public RawFont
{
public:
	BitmapFont();
	virtual ~BitmapFont();
	void initialize(Bitmap* bitmap);

	virtual void setName(const String& fontName) { LN_THROW(0, InvalidOperationException); }
	virtual const String& getName() const { return m_name; }
	virtual void setSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int getSize() const { return 0; }
	//virtual void SetEdgeSize(int size) { LN_THROW(0, InvalidOperationException); }
	//virtual int GetEdgeSize() const { return 0; }
	virtual void setBold(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool isBold() const { return false; }
	virtual void setItalic(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool isItalic() const { return false; }
	virtual void setAntiAlias(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool isAntiAlias() const { return false; }
	virtual RawFontPtr copy() const;
	virtual int getLineSpacing() { return m_charHeight; }
	virtual FontGlyphLocation* advanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData);
	virtual FontGlyphBitmap* lookupGlyphBitmap(UTF32 utf32code, int strokeSize);
	virtual void getGlobalMetrics(FontGlobalMetrics* outMetrics) override;
	virtual bool isOutlineSupported() const override { return false; }
	virtual void decomposeOutline(UTF32 utf32code, RawFont::VectorGlyphInfo* outInfo) override { }
	virtual Vector2 getKerning(UTF32 prev, UTF32 next) override { return Vector2::Zero; }
	virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) override;
	virtual FontManager* getManager() const { return m_manager; }

private:
	FontManager*			m_manager;
	String					m_name;					// 名前。グリフテクスチャのキャッシュで使用する
	Ref<Bitmap>			m_fontBitmap;
	int						m_charWidth;			// 1文字分の幅
	int						m_charHeight;			// 1文字分の高さ
	FontGlyphLocation		m_fontGlyphLocation;	// advanceKerning() で返すデータ
	Ref<Bitmap>			m_glyphBitmap;
	FontGlyphBitmap			m_fontGlyphBitmap;		// lookupGlyphBitmap() で返すデータ
};

} // namespace detail
LN_NAMESPACE_END
