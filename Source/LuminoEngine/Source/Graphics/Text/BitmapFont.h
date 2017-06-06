
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
	void Initialize(Bitmap* bitmap);

	virtual void SetName(const String& fontName) { LN_THROW(0, InvalidOperationException); }
	virtual const String& GetName() const { return m_name; }
	virtual void SetSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int GetSize() const { return 0; }
	virtual void SetEdgeSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int GetEdgeSize() const { return 0; }
	virtual void SetBold(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsBold() const { return false; }
	virtual void SetItalic(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsItalic() const { return false; }
	virtual void SetAntiAlias(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsAntiAlias() const { return false; }
	virtual RawFontPtr Copy() const;
	virtual int GetLineSpacing() { return m_charHeight; }
	virtual FontGlyphLocation* AdvanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData);
	virtual FontGlyphBitmap* LookupGlyphBitmap(UTF32 utf32code, int strokeSize);
	virtual void GetGlobalMetrics(FontGlobalMetrics* outMetrics) override;
	virtual bool IsOutlineSupported() const override { return false; }
	virtual void DecomposeOutline(UTF32 utf32code, RawFont::VectorGlyphInfo* outInfo) override { }
	virtual Vector2 GetKerning(UTF32 prev, UTF32 next) override { return Vector2::Zero; }
	virtual void GetGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) override;
	virtual FontManager* GetManager() const { return m_manager; }

private:
	FontManager*			m_manager;
	String					m_name;					// 名前。グリフテクスチャのキャッシュで使用する
	RefPtr<Bitmap>			m_fontBitmap;
	int						m_charWidth;			// 1文字分の幅
	int						m_charHeight;			// 1文字分の高さ
	FontGlyphLocation		m_fontGlyphLocation;	// AdvanceKerning() で返すデータ
	RefPtr<Bitmap>			m_glyphBitmap;
	FontGlyphBitmap			m_fontGlyphBitmap;		// LookupGlyphBitmap() で返すデータ
};

} // namespace detail
LN_NAMESPACE_END
