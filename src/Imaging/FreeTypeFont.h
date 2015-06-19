
#pragma once
#include <Lumino/Imaging/Font.h>
#include <Lumino/Imaging/FontManager.h>

namespace Lumino
{
namespace Imaging
{
class FontManager;

struct FreeTypeFontGlyphData
	: public FontGlyphData
{
	//int             BitmapRealDataSize;    ///< 内部データ
	int				NextBaseX;      ///< 内部データ
	int				NextBaseY;      ///< 内部データ
	lnUInt			Previous;       ///< 内部データ
	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている
};


/**
	@brief		フォントのクラスです。
*/
class FreeTypeFont
	: public Font
{
public:
	FreeTypeFont(FontManager* manager);
	virtual ~FreeTypeFont();

public:
	virtual void SetName(const String& fontName) { m_fontName = fontName; m_modified = true; }
	virtual const String& GetName() const { return m_fontName; }
	virtual void SetSize(int size) { m_fontSize = size; m_modified = true; }
	virtual int GetSize() const  { return m_fontSize; }
	virtual void SetEdgeSize(int size) { m_edgeSize = size; m_modified = true; }
	virtual int GetEdgeSize() const  { return m_edgeSize; }
	virtual void SetBold(bool enabled) { m_isBold = enabled; m_modified = true; }
	virtual bool IsBold() const  { return m_isBold; }
	virtual void SetItalic(bool enabled) { m_isItalic = enabled; m_modified = true; }
	virtual bool IsItalic() const { return m_isItalic; }
	virtual void SetAntiAlias(bool enabled) { m_isAntiAlias = enabled; m_modified = true; }
	virtual bool IsAntiAlias() const { return m_isAntiAlias; }

	virtual Size GetTextSize(const char* text, int length);
	virtual Size GetTextSize(const wchar_t* text, int length);

	virtual Font* Copy() const = 0;

	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	virtual FontGlyphData* MakeGlyphData(UTF32 utf32code, FontGlyphData* prevData) = 0;

	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
	virtual void EndMakeGlyphData(FontGlyphData* glyphData) = 0;
	// ↑メンバに持ってればいいだけだし必要ないかも。スレッドセーフにする必要もないし。

private:
	void UpdateFont();
	Size GetTextSizeInternal(const UTF32* text, int length);

private:
	FontManager*	m_manager;
	String			m_fontName;
	int				m_fontSize;
	int				m_edgeSize;
	bool			m_isBold;
	bool			m_isItalic;
	bool			m_isAntiAlias;
	bool			m_modified;

	FTC_FaceID		m_ftFaceID;			///< キャッシュから FT_Face を検索するためのキー値
	FT_Face			m_ftFace;			///< フォント本体
	FT_Int			m_ftCacheMapIndex;	///< m_ftFace 内で現在アクティブな FT_CharMap の番号

	ByteBuffer		m_utf32Buffer;
};

} // namespace Imaging
} // namespace Lumino
