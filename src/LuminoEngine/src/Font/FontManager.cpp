
#include "Internal.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H /* <freetype/freetype.h> */
#include FT_CACHE_H /* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H /* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H /* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include "FontManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// FontDesc
   
FontDesc::FontDesc()
    : Family()
    , Size(16)
    , isBold(false)
    , isItalic(false)
    , isAntiAlias(true)
{}

bool FontDesc::operator < (const FontDesc& right)
{
    if (Family < right.Family) return true;
    if (Family > right.Family) return false;
    if (Size < right.Size) return true;
    if (Size > right.Size) return false;
    if (isBold < right.isBold) return true;
    if (isBold > right.isBold) return false;
    if (isItalic < right.isItalic) return true;
    if (isItalic > right.isItalic) return false;
    if (isAntiAlias < right.isAntiAlias) return true;
    if (isAntiAlias > right.isAntiAlias) return false;
    return false;
}

uint64_t FontDesc::calcHash() const
{
    uint32_t v[2];
    v[0] = CRCHash::compute(Family.c_str());

    uint8_t* v2 = (uint8_t*)&v[1];
    v2[0] = Size;
    v2[1] =
        (((isBold) ? 1 : 0)) |
        (((isItalic) ? 1 : 0) << 1) |
        (((isAntiAlias) ? 1 : 0) << 2);

    return *((uint64_t*)&v);
}

//==============================================================================
// FontManager
	
FontManager::FontManager()
{
}

void FontManager::initialize(const Settings& settings)
{
    m_assetManager = settings.assetManager;
    m_charToUTF32Converter.setDestinationEncoding(TextEncoding::utf32Encoding());
    m_charToUTF32Converter.getSourceEncoding(TextEncoding::systemMultiByteEncoding());
    m_wcharToUTF32Converter.setDestinationEncoding(TextEncoding::utf32Encoding());
    m_wcharToUTF32Converter.getSourceEncoding(TextEncoding::wideCharEncoding());
    m_TCharToUTF32Converter.setDestinationEncoding(TextEncoding::utf32Encoding());
    m_TCharToUTF32Converter.getSourceEncoding(TextEncoding::tcharEncoding());
    m_UTF32ToTCharConverter.setDestinationEncoding(TextEncoding::tcharEncoding());
    m_UTF32ToTCharConverter.getSourceEncoding(TextEncoding::utf32Encoding());


    // FreeType
    {
        FT_Error err = FT_Init_FreeType(&m_ftLibrary);
        if (LN_ENSURE(err == 0, "failed initialize font library : %d\n", err)) return;

        err = FTC_Manager_New(m_ftLibrary, 0, 0, 0, callbackFaceRequester, this, &m_ftCacheManager);
        if (LN_ENSURE(err == 0, "failed initialize font cache manager : %d\n", err)) return;

        err = FTC_CMapCache_New(m_ftCacheManager, &m_ftCMapCache);
        if (LN_ENSURE(err == 0, "failed initialize font cache map : %d\n", err)) return;

        err = FTC_ImageCache_New(m_ftCacheManager, &m_ftImageCache);
        if (LN_ENSURE(err == 0, "failed initialize font image cache : %d\n", err)) return;
    }
}

void FontManager::dispose()
{
    if (m_ftCacheManager)
    {
        FTC_Manager_Done(m_ftCacheManager);
        m_ftCacheManager = nullptr;
    }

    if (m_ftLibrary)
    {
        FT_Done_FreeType(m_ftLibrary);
        m_ftLibrary = nullptr;
    }
}

FT_Error FontManager::callbackFaceRequester(
    FTC_FaceID face_id,
    FT_Library library,
    FT_Pointer request_data,
    FT_Face* aface)
{
    return ((FontManager*)request_data)->faceRequester(face_id, library, request_data, aface);
}

FT_Error FontManager::faceRequester(
    FTC_FaceID face_id,
    FT_Library library,
    FT_Pointer request_data,
    FT_Face* aface)
{
    // キャッシュ検索のコールバック。map から取るようにしている。

    // face_id は、FTC_Manager_LookupFace() に渡した ID が入ってくる。
    // ID は自分で好きなように決められる。

    intptr_t key = (intptr_t)face_id;
    auto itr = m_ttfDataEntryMap.find(key/*family_name*/);
    if (itr != m_ttfDataEntryMap.end())
    {
        FT_Face face;
        FT_Error err = FT_New_Memory_Face(
            m_ftLibrary,
            (const FT_Byte*)itr->second.dataBuffer->data(),
            itr->second.dataBuffer->size(),
            itr->second.collectionIndex,
            &face);
        if (LN_ENSURE(err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err)) return err;

        err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        if (err != FT_Err_Ok)
        {
            FT_Done_Face(face);
            if (LN_ENSURE(err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err)) return err;
        }

        *aface = face;
        return FT_Err_Ok;
    }
#if 0//def LN_OS_WIN32
    else if (m_requesterFaceName != NULL)
    {
        // 名前からシステムフォント検索
        TSystemFontData* systemFont = getWindowsSystemFontData(m_requesterFaceName);
        m_requesterFaceName = NULL;
        if (systemFont == NULL) {
            return FT_Err_Cannot_Open_Resource;
        }

        // リソースロック
        size_t size = 0;
        int index = 0;
        byte_t* data = lockWindowsSystemFontData(systemFont, &size, &index);
        if (data == NULL) {
            freeWindowsSystemFontData(systemFont);
            return FT_Err_Cannot_Open_Resource;
        }

        // FreeType の読み取りストリーム
        FT_Stream stream = (FT_Stream)malloc(sizeof(FT_StreamRec));
        if (stream == NULL) {
            freeWindowsSystemFontData(systemFont);
            return FT_Err_Out_Of_Memory;
        }
        memset(stream, 0, sizeof(FT_StreamRec));
        stream->base = 0;
        stream->size = size;
        stream->pos = 0;
        stream->descriptor.pointer = systemFont;
        stream->pathname.pointer = NULL;
        stream->read = streamIoFunc;
        stream->close = streamCloseFunc;

        FT_Open_Args args;
        memset(&args, 0, sizeof(args));
        args.flags = FT_OPEN_STREAM;
        args.stream = stream;
        args.driver = 0;
        args.num_params = 0;
        args.params = NULL;

        FT_Face face;
        FT_Error err = FT_Open_Face(m_ftLibrary, &args, index, &face);
        if (err != FT_Err_Ok) {
            // 失敗した場合もstreamは勝手に解放される
            return err;
        }

        // Charmapを設定しておく
        err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        if (err != FT_Err_Ok) {
            FT_Done_Face(face);
            return err;
        }

        *aface = (FT_Face)face;
        return FT_Err_Ok;
    }
#endif
    return -1;
}

} // namespace detail
} // namespace ln

