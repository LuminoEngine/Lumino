
#pragma once
#include <map>
#include <Lumino/Base/Hash.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Text/EncodingConverter.h>

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

LN_NAMESPACE_BEGIN
class RawFont;

namespace detail {
class ArchiveManager;
class GraphicsManager;

// フォントの管理クラス
class FontManager
	: public RefObject
{
public:
	FontManager();
	virtual ~FontManager();
	void initialize(ArchiveManager* archiveManager, GraphicsManager* graphicsManager);
	void dispose();

	GraphicsManager* getGraphicsManager() const { return m_graphicsManager; }

	Ref<Font> getDefaultFont() const { return m_defaultFont; }

	// フォントファイルを追加する (ttf)
	void registerFontFile(const String& fontFilePath);

	void setDefaultRawFont(RawFont* font);
	RawFont* getDefaultRawFont() const { return m_defaultRawFont; }

	// FreeTypeFont で名前が空のときに使われる
	void setDefaultFontName(const String& name) { m_defaultFontName = name; }
	const String& getDefaultFontName() const { return m_defaultFontName; }

	FontPtr getBuiltinFont(BuiltinFontSize size) const;

	EncodingConverter* getCharToUTF32Converter() { return &m_charToUTF32Converter; }
	EncodingConverter* getWCharToUTF32Converter() { return &m_wcharToUTF32Converter; }
	EncodingConverter* getTCharToUTF32Converter() { return &m_TCharToUTF32Converter; }
	EncodingConverter* getUTF32ToTCharConverter() { return &m_UTF32ToTCharConverter; }

	RawFontPtr lookupRawFont(const detail::FontData& keyData);

LN_INTERNAL_ACCESS:
	void addFontResource_(RawFont* font) { m_fontResourceList.add(font); }
	void removeFontResource_(RawFont* font) { m_fontResourceList.remove(font); }
	FT_Library getFTLibrary() const { return m_ftLibrary; }
	FTC_Manager getFTCacheManager() const { return m_ftCacheManager; }
	FTC_CMapCache getFTCacheMapCache() const { return m_ftCMapCache; }
	FTC_ImageCache getFTCImageCache() const { return m_ftImageCache; }
	//void AddFreeTypeFont(FreeTypeFont* font) { m_freeTypeFontList.Add(font); }
	//void RemoveFreeTypeFont(FreeTypeFont* font) { m_freeTypeFontList.Remove(font); }

	// キャッシュ検索コールバック
	FT_Error faceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

	// faceRequester() の呼び出し元
	static FT_Error callbackFaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

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
	TSystemFontData* getWindowsSystemFontData(LPCTSTR name);
	unsigned char* lockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index);
	static void freeWindowsSystemFontData(TSystemFontData *fnt);
	static unsigned long streamIoFunc(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count);
	static void streamCloseFunc(FT_Stream stream);
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
	typedef std::map<intptr_t, TTFDataEntry>     TTFDataEntryMap;
	typedef std::pair<intptr_t, TTFDataEntry>    TTFDataEntryPair;
	TTFDataEntryMap			m_ttfDataEntryMap;

	ArchiveManager*			m_archiveManager;
	GraphicsManager*		m_graphicsManager;
	Ref<CacheManager>	m_rawFontCache;
	Ref<Font>			m_defaultFont;
	Ref<RawFont>			m_defaultRawFont;
	String					m_defaultFontName;
	List<Ref<Font>>		m_builtinFontList;
	List<RawFont*>			m_fontResourceList;

	EncodingConverter		m_charToUTF32Converter;
	EncodingConverter		m_wcharToUTF32Converter;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;

	FT_Library				m_ftLibrary;
	FTC_Manager				m_ftCacheManager;
	FTC_CMapCache			m_ftCMapCache;
	FTC_ImageCache			m_ftImageCache;

	// faceRequester() で Windows のシステムフォントを拾うための細工。
	// FreeType は FTC_Manager_LookupFace() に渡されたアドレスを直接辞書のキーとする。
	// (アドレスの中身までは見ない。そのため、文字列をキーにすることはできない)
	// つまり、faceRequester() にフォント名を伝えることはできない。なので、外部に一度とっておく必要がある。
	// この変数には、FTC_Manager_LookupFace() の直前でフォント名をセットしておく。
	// ローカル変数のポインタでよい。faceRequester() で NULL が格納される。
	const Char*		m_requesterFaceName;

};

} // namespace detail
LN_NAMESPACE_END
