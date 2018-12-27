#pragma once
#include <list>
#include <LuminoEngine/Font/Common.hpp>

typedef int FT_Error;
typedef void*  FT_Pointer;
typedef FT_Pointer FTC_FaceID;
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FTC_ManagerRec_* FTC_Manager;
typedef struct FTC_CMapCacheRec_* FTC_CMapCache;
typedef struct FTC_ImageCacheRec_* FTC_ImageCache;
typedef struct FT_FaceRec_*  FT_Face;

namespace ln {
namespace detail {
class FontCore;

// RefObject のキャッシュ管理。
// RefObject を作りたいときは、まず先に findObject を呼び出してキャッシュ探す。なければ呼び出し側で作って registerObject()。
// RefObject は普通に makeRef で作ってよい。また、作った直後は registerObject() で登録しておく。
// RefObject を release するとき、その直前で releaseObject() に渡しておく。
template<class TKey>
class ObjectCache
{
public:
    ObjectCache()
        : m_maxCacheObjectCount(64)
        , m_aliveList()
        , m_freeList()
    {}

    void dispose()
    {
        m_aliveList.clear();
        m_freeList.clear();
        m_maxCacheObjectCount = 0;
    }

    RefObject* findObject(const TKey& key)
    {
        if (LN_REQUIRE(!isDisposed())) return nullptr;

        for (auto itr = m_aliveList.begin(); itr != m_aliveList.end(); ++itr) {
            if (itr->key == key) {
                return itr->obj;
            }
        }

        for (auto itr = m_freeList.begin(); itr != m_freeList.end(); ++itr) {
            if (itr->key == key) {
                m_aliveList.splice(m_aliveList.end(), std::move(m_freeList), itr);
                return m_aliveList.back().obj;
            }
        }

        return nullptr;
    }

    void registerObject(const TKey& key, RefObject* obj)
    {
        if (LN_REQUIRE(!isDisposed())) return;

        if (obj) {
            m_aliveList.push_back({ key, obj });
        }
    }

    void releaseObject(RefObject* obj)
    {
        if (LN_REQUIRE(!isDisposed())) return;

        if (obj) {
            auto itr = m_aliveList.begin();
            for (; itr != m_aliveList.end(); ++itr) {
                if (itr->obj == obj) {
                    break;
                }
            }
            if (LN_REQUIRE(itr != m_aliveList.end())) return;   // not contained m_aliveList

            m_freeList.splice(m_freeList.end(), std::move(m_aliveList), itr);
            collectOldObject();
        }
    }

private:
    bool isDisposed() const
    {
        return m_maxCacheObjectCount <= 0;
    }

    void collectOldObject()
    {
        int count = m_freeList.size() - m_maxCacheObjectCount;
        if (count > 0) {
            for (int i = 0; i < count; i++) {
                m_freeList.pop_front();
            }
        }
    }

    struct Entry
    {
        TKey key;
        Ref<RefObject> obj;
    };

    int m_maxCacheObjectCount;
    std::list<Entry> m_aliveList;
    std::list<Entry> m_freeList;    // front:oldest, back:newest
};



class FontManager
	: public RefObject
{
public:
	struct Settings
	{
        AssetManager* assetManager;
	};

    FontManager();
	void initialize(const Settings& settings);
	void dispose();
	void registerFontFile(const StringRef& fontFilePath);
	Ref<FontCore> lookupFontCore(const FontDesc& keyDesc);

	FTC_Manager ftCacheManager() const { return m_ftCacheManager; }
	FTC_CMapCache ftCacheMapCache() const { return m_ftCMapCache; }
	FTC_ImageCache ftCImageCache() const { return m_ftImageCache; }

	void addAliveFontCore(FontCore* font) { m_aliveFontCoreList.add(font); }
	void removeAliveFontCore(FontCore* font) { m_aliveFontCoreList.remove(font); }

    void setDefaultFontDesc(const FontDesc& desc) { m_defaultFontDesc = desc; }
    FontDesc defaultFontDesc() const { return m_defaultFontDesc; }

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
	ObjectCache<uint32_t> m_fontCoreCache;
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

    FontDesc m_defaultFontDesc;
};

} // namespace detail
} // namespace ln

