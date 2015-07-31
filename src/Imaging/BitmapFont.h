
#pragma once
#include <Lumino/Imaging/Font.h>
#include <Lumino/Imaging/FontManager.h>

namespace Lumino
{
namespace Imaging
{
	
/**
	@brief		ビットマップフォントのクラスです。
*/
class BitmapFont
    : public Font
{
public:
	BitmapFont(FontManager* manager, Bitmap* bitmap);
	virtual ~BitmapFont();

public:
	virtual void SetName(const String& fontName) { LN_THROW(0, InvalidOperationException); }
	virtual const String& GetName() const { LN_THROW(0, InvalidOperationException); return String::GetEmpty(); }
	virtual void SetSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int GetSize() const { LN_THROW(0, InvalidOperationException); return 0; }
	virtual void SetEdgeSize(int size) { LN_THROW(0, InvalidOperationException); }
	virtual int GetEdgeSize() const { LN_THROW(0, InvalidOperationException); return 0; }
	virtual void SetBold(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsBold() const { LN_THROW(0, InvalidOperationException); return false; }
	virtual void SetItalic(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsItalic() const { LN_THROW(0, InvalidOperationException); return false; }
	virtual void SetAntiAlias(bool enabled) { LN_THROW(0, InvalidOperationException); }
	virtual bool IsAntiAlias() const { LN_THROW(0, InvalidOperationException); return false; }
	virtual Font* Copy() const;
	virtual int GetLineHeight() { return m_charHeight; }
	virtual FontGlyphLocation* AdvanceKerning(UTF32 utf32code, FontGlyphLocation* prevData);
	virtual FontGlyphBitmap* LookupGlyphBitmap(UTF32 utf32code);
	virtual FontManager* GetManager() const { return m_manager; }

private:
    void Dispose();

private:
	FontManager*			m_manager;
	RefPtr<Bitmap>			m_fontBitmap;
	int						m_charWidth;			///< 1文字分の幅
	int						m_charHeight;		///< 1文字分の高さ
	FontGlyphLocation		m_fontGlyphLocation;	///< AdvanceKerning() で返すデータ
	RefPtr<Bitmap>			m_glyphBitmap;
	FontGlyphBitmap			m_fontGlyphBitmap;		///< LookupGlyphBitmap() で返すデータ
};

} // namespace Imaging
} // namespace Lumino
