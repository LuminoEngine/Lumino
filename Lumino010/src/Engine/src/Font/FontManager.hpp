#pragma once
#include <list>
#include <LuminoEngine/Font/Common.hpp>
#include "../Base/RefObjectCache.hpp"

typedef int FT_Error;
typedef void*  FT_Pointer;
typedef FT_Pointer FTC_FaceID;
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FTC_ManagerRec_* FTC_Manager;
typedef struct FTC_CMapCacheRec_* FTC_CMapCache;
typedef struct FTC_ImageCacheRec_* FTC_ImageCache;
typedef struct FT_FaceRec_*  FT_Face;

namespace ln {
class Font;
namespace detail {
class FontCore;
class GlyphIconFontManager;

// フォントファイルデータ。
// メモリ上のデータからFaceを作る場合、FT_Done_Face() するまでメモリを開放してはならないため、全データを持っておく。
struct FontFaceSource
{
	String familyName;
	String styleName;
	Ref<ByteBuffer> buffer;
	int faceIndex;
};

class FontManager
	: public RefObject
{
public:
	struct Settings
	{
        AssetManager* assetManager;
		Path engineAssetPath;
		Path fontFile;
	};

    FontManager();
	void init(const Settings& settings);
	void dispose();
	void registerFontFromFile(const StringRef& fontFilePath, bool defaultFamily);
    void registerFontFromStream(Stream* stream, bool defaultFamily);
	Ref<FontCore> lookupFontCore(const FontDesc& keyDesc, float dpiScale);

	FT_Library ftLibrary() const { return m_ftLibrary; }
	FTC_Manager ftCacheManager() const { return m_ftCacheManager; }
	FTC_CMapCache ftCacheMapCache() const { return m_ftCMapCache; }
	FTC_ImageCache ftCImageCache() const { return m_ftImageCache; }

	void addAliveFontCore(FontCore* font) { m_aliveFontCoreList.add(font); }
	void removeAliveFontCore(FontCore* font) { m_aliveFontCoreList.remove(font); }

    //void setDefaultFontDesc(const FontDesc& desc) { m_defaultFontDesc = desc; }
    void setDefaultFont(Font* font);
    FontDesc defaultFontDesc() const;
    Font* defaultFont() const;
	Font* emojiFont() const;
	ByteBuffer* getDefaultFontData() const;
	const Ref<GlyphIconFontManager>& glyphIconFontManager() const { return m_glyphIconFontManager; }

	const FontFaceSource* lookupFontFaceSourceFromFamilyName(const String& name);

private:
    static FT_Error callbackFaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);
    FT_Error faceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

    struct TTFDataEntry
    {
        Ref<ByteBuffer> dataBuffer;
        int collectionIndex;

        /* メモリ上のデータからFaceを作る場合、FT_Done_Face() するまでメモリを開放してはならない。
         * ファイルストリームを使うこともできるが、そうするとフォントが存在している間
         * ストリームを1つ占有してしまうことになる。
         * フォントファイルひとつ当たり 4M とか普通にあるけど、大体1000x1000ビットマップひとつ分。
         * 知らないところでストリームが開きっぱなしになるのはどうかと思うし、
         * (音声のストリーミングと違って、フォントのストリーミングなんて言っても普通はピンと来ないだろうし…)
         * 文字列描画は速度的にかなりクリティカルなもの。メモリ上に置きっぱなしでいいと思う。
         */
    };

    AssetManager* m_assetManager;
	ObjectCache<uint64_t, RefObject> m_fontCoreCache;
	List<FontCore*> m_aliveFontCoreList;
    EncodingConverter m_charToUTF32Converter;
    EncodingConverter m_wcharToUTF32Converter;
    EncodingConverter m_TCharToUTF32Converter;
    EncodingConverter m_UTF32ToTCharConverter;

    FT_Library m_ftLibrary;
    FTC_Manager m_ftCacheManager;
    FTC_CMapCache m_ftCMapCache;
    FTC_ImageCache m_ftImageCache;

    typedef std::unordered_map<intptr_t, TTFDataEntry> TTFDataEntryMap;
    TTFDataEntryMap m_ttfDataEntryMap;

    //FontDesc m_defaultFontDesc;
    Ref<Font> m_defaultFont;
	Ref<Font> m_emojiFont;
	Ref<GlyphIconFontManager> m_glyphIconFontManager;

	List<FontFaceSource> m_famlyNameToFontFaceSourceMap;
};

// Font Awesome や Emoji アイコンのフォント管理を行う。
// GlyphIcon オブジェクトでシーングラフのノードの1つとして描画すると、Font インスタンスが大量に作られる可能性がある。
// これらは基本的にサイズが異なるだけで多くのプロパティが同一であるため、できるだけ一元管理する。
class GlyphIconFontManager
	: public RefObject
{
public:
	GlyphIconFontManager();
	virtual ~GlyphIconFontManager();
	Result init(FontManager* fontManager);
	void dispose();

	Font* getFontAwesomeFont(const StringRef& style, int size);
	uint32_t getFontAwesomeCodePoint(const StringRef& glyphName);

private:
	FontManager* m_fontManager;
	std::unordered_map<String, uint32_t> m_fontAwesomeVariablesMap;
	std::unordered_map<int, Ref<Font>> m_fontAwesomeFontMap_Regular;
	std::unordered_map<int, Ref<Font>> m_fontAwesomeFontMap_Solid;
};

} // namespace detail
} // namespace ln

