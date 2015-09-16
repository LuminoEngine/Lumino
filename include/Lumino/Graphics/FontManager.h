
#pragma once
#include <map>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Text/EncodingConverter.h>

// TODO: これらを思いっきり公開してるのは何とかしたい…
typedef int  FT_Error;
typedef void*  FT_Pointer;
typedef FT_Pointer  FTC_FaceID;
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FTC_ManagerRec_*  FTC_Manager;
typedef struct FTC_CMapCacheRec_*  FTC_CMapCache;
typedef struct FTC_ImageCacheRec_*  FTC_ImageCache;
typedef struct FT_FaceRec_*  FT_Face;
typedef struct FT_StrokerRec_*  FT_Stroker;
typedef struct FT_OutlineGlyphRec_*  FT_OutlineGlyph;
typedef struct FT_StreamRec_*  FT_Stream;
typedef struct FT_GlyphRec_*  FT_Glyph;

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
class Font;

/**
	@brief		フォントの管理クラス
*/
class FontManager
	: public RefObject
{
public:
	static FontManager* Create(FileManager* fileManager);

public:

	/// フォントファイルを追加する (ttf) (初回登録の場合はデフォルトフォント名として登録する)
	void RegisterFontFile(const String& fontFilePath);

	/// デフォルトのフォントをセットする
	void SetDefaultFont(Font* font);

	/// デフォルトのフォントを取得する
	Font* GetDefaultFont() const { return m_defaultFont; }

	void Dispose();


	EncodingConverter* GetCharToUTF32Converter() { return &m_charToUTF32Converter; }
	EncodingConverter* GetWCharToUTF32Converter() { return &m_wcharToUTF32Converter; }


	EncodingConverter* GetTCharToUTF32Converter() { return &m_TCharToUTF32Converter; }
	EncodingConverter* GetUTF32ToTCharConverter() { return &m_UTF32ToTCharConverter; }

private:
	FontManager(FileManager* fileManager);
	virtual ~FontManager();

	friend class FreeTypeFont;
	FT_Library GetFTLibrary() const { return m_ftLibrary; }
	FTC_Manager GetFTCacheManager() const { return m_ftCacheManager; }
	FTC_CMapCache GetFTCacheMapCache() const { return m_ftCMapCache; }
	FTC_ImageCache GetFTCImageCache() const { return m_ftImageCache; }

	// キャッシュ検索コールバック
	FT_Error FaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

	// FaceRequester() の呼び出し元
	static FT_Error CallbackFaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

#ifdef LN_OS_WIN32
	typedef struct TSystemFontData
	{
		HDC hdc;
		HFONT hFont;
		HFONT hOldFont;
		unsigned char *fontdata;
		size_t size;
		int index;
	} TSystemFontData;

	/// Windowsに登録されているフォントのバイナリデータを名称から取得
	TSystemFontData* GetWindowsSystemFontData(LPCTSTR name);
	unsigned char* LockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index);
	static void FreeWindowsSystemFontData(TSystemFontData *fnt);
	static unsigned long StreamIoFunc(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count);
	static void StreamCloseFunc(FT_Stream stream);
#endif

	struct TTFDataEntry
	{
		ByteBuffer		DataBuffer;
		int				CollectionIndex;

		/* メモリ上のデータからFaceを作る場合、FT_Done_Face() するまでメモリを開放してはならない。
		* ファイルストリームを使うこともできるが、そうするとフォントが存在している間
		* ストリームを1つ占有してしまうことになる。
		* フォントファイルひとつ当たり 4M とか普通にあるけど、大体1000x1000ビットマップひとつ分。
		* 知らないところでストリームが開きっぱなしになるのはどうかと思うし、
		* (音声のストリーミングと違って、フォントのストリーミングなんて言っても普通はピンと来ないだろうし…)
		* 文字列描画は速度的にかなりクリティカルなもの。メモリ上に置きっぱなしでいいと思う。
		*/
	};
	typedef std::map<uint32_t, TTFDataEntry>     TTFDataEntryMap;
	typedef std::pair<uint32_t, TTFDataEntry>    TTFDataEntryPair;
	TTFDataEntryMap		m_ttfDataEntryMap;

	RefPtr<FileManager>	m_fileManager;
	Font*  m_defaultFont;

	EncodingConverter		m_charToUTF32Converter;
	EncodingConverter		m_wcharToUTF32Converter;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;

	FT_Library      m_ftLibrary;
	FTC_Manager     m_ftCacheManager;
	FTC_CMapCache   m_ftCMapCache;
	FTC_ImageCache  m_ftImageCache;


	// FaceRequester() で Windows のシステムフォントを拾うための細工。
	// FreeType は FTC_Manager_LookupFace() に渡されたアドレスを直接辞書のキーとする。
	// (アドレスの中身までは見ない。そのため、文字列をキーにすることはできない)
	// つまり、faceRequester() にフォント名を伝えることはできない。なので、外部に一度とっておく必要がある。
	// この変数には、FTC_Manager_LookupFace() の直前でフォント名をセットしておく。
	// ローカル変数のポインタでよい。FaceRequester() で NULL が格納される。
	const TCHAR*		m_requesterFaceName;

};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
