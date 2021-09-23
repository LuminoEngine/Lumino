
#include "Internal.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H /* <freetype/freetype.h> */
#include FT_CACHE_H /* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H /* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H /* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include <LuminoFont/Font.hpp>
#include "../../RuntimeCore/src/Asset/AssetManager.hpp"
#include "FontCore.hpp"
#include "FreeTypeFont.hpp"
#include "FontManager.hpp"

#define LN_DEFAULTFONT_PROPORTIONAL 1

namespace ln {
namespace detail {

//==============================================================================
// FontDesc
   
FontDesc::FontDesc()
    : Family()
    , Size(Font::DefaultSize)
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
	uint8_t flags = (((isBold) ? 1 : 0)) |
		(((isItalic) ? 1 : 0) << 1) |
		(((isAntiAlias) ? 1 : 0) << 2);

	return
		static_cast<uint64_t>(CRCHash::compute(Family.c_str())) << 32 |
		static_cast<uint16_t>(Size) << 16 |
		flags;



    //uint32_t v[2];
    //v[0] = CRCHash::compute(Family.c_str());

    //uint8_t* v2 = (uint8_t*)&v[1];
    //v2[0] = Size;
    //v2[1] =
    //    (((isBold) ? 1 : 0)) |
    //    (((isItalic) ? 1 : 0) << 1) |
    //    (((isAntiAlias) ? 1 : 0) << 2);

    //return *((uint64_t*)&v);
}

bool FontDesc::equals(const FontDesc& other) const
{
	return
		(Family == other.Family) &&
		(Size == other.Size) &&
		(isBold == other.isBold) &&
		(isItalic == other.isItalic) &&
		(isAntiAlias == other.isAntiAlias);
}

//==============================================================================
// FontManager

FontManager* FontManager::initialize(const Settings& settings)
{
    if (instance()) return instance();

    auto m = Ref<FontManager>(LN_NEW detail::FontManager(), false);
    if (!m->init(settings)) return nullptr;

    EngineContext2::instance()->registerModule(m);
    EngineContext2::instance()->fontManager = m;
    return m;
}

void FontManager::terminate()
{
    if (instance()) {
        instance()->dispose();
        EngineContext2::instance()->unregisterModule(instance());
        EngineContext2::instance()->fontManager = nullptr;
    }
}

FontManager::FontManager()
{
}

bool FontManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "FontManager Initialization started.";

    m_assetManager = settings.assetManager;
    m_charToUTF32Converter.setDestinationEncoding(TextEncoding::utf32Encoding());
    m_charToUTF32Converter.getSourceEncoding(TextEncoding::systemMultiByteEncoding());
    m_wcharToUTF32Converter.setDestinationEncoding(TextEncoding::utf32Encoding());
    m_wcharToUTF32Converter.getSourceEncoding(TextEncoding::wideCharEncoding());
    m_TCharToUTF32Converter.setDestinationEncoding(TextEncoding::utf32Encoding());
    m_TCharToUTF32Converter.getSourceEncoding(TextEncoding::tcharEncoding());
    m_UTF32ToTCharConverter.setDestinationEncoding(TextEncoding::tcharEncoding());
    m_UTF32ToTCharConverter.getSourceEncoding(TextEncoding::utf32Encoding());

    m_fontCoreCache.init();

    // FreeType
    {
        FT_Error err = FT_Init_FreeType(&m_ftLibrary);
        if (LN_ENSURE(err == 0, "failed init font library : %d\n", err)) return false;

        err = FTC_Manager_New(m_ftLibrary, 0, 0, 0, callbackFaceRequester, this, &m_ftCacheManager);
        if (LN_ENSURE(err == 0, "failed init font cache manager : %d\n", err)) return false;

        err = FTC_CMapCache_New(m_ftCacheManager, &m_ftCMapCache);
        if (LN_ENSURE(err == 0, "failed init font cache map : %d\n", err)) return false;

        err = FTC_ImageCache_New(m_ftCacheManager, &m_ftImageCache);
        if (LN_ENSURE(err == 0, "failed init font image cache : %d\n", err)) return false;
    }

    // Default font
#ifdef LN_DEFAULTFONT_PROPORTIONAL
    {
        static const unsigned char data[] =
        {
#include "Resource/mplus-1c-regular-ascii-subset.ttf.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        registerFontFromStream(&stream, false);
    }
#else
    {
        static const unsigned char data[] =
        {
#include "Resource/mplus-1m-regular-ascii-subset.ttf.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        MemoryStream stream(data, size);
        registerFontFromStream(&stream, false);
    }
#endif

    FontDesc desc;
    desc.Family = _TT("mplus-1c-regular-ascii-subset");
    m_defaultFont = makeObject<Font>(desc);

    if (!settings.fontFile.isEmpty()) {
        registerFontFromFile(settings.fontFile, true);
    }





	m_glyphIconFontManager = makeRef<GlyphIconFontManager>();
	if (!m_glyphIconFontManager->init(this)) {
		return false;
	}

	auto emojiFontPath = ln::Path(settings.engineAssetPath, _TT("NotoColorEmoji.ttf"));
	if (FileSystem::existsFile(emojiFontPath)) {
		registerFontFromFile(emojiFontPath, false);
		FontDesc desc;
		desc.Family = _TT("Noto Color Emoji");
		m_emojiFont = makeObject<Font>(desc);
	}


    LN_LOG_DEBUG << "FontManager Initialization ended.";
    return true;
}

void FontManager::dispose()
{
	m_fontCoreCache.dispose();

	auto disposeList = m_aliveFontCoreList;
	for (FontCore* font : disposeList)
	{
		font->dispose();
	}
	LN_CHECK(m_aliveFontCoreList.isEmpty());


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

	m_ttfDataEntryMap.clear();
}

void FontManager::registerFontFromFile(const StringRef& fontFilePath, bool defaultFamily)
{
	auto file = m_assetManager->openFileStream(fontFilePath);
    registerFontFromStream(file, defaultFamily);
}

void FontManager::registerFontFromStream(Stream* stream, bool defaultFamily)
{
    auto buffer = makeRef<ByteBuffer>(stream->length());
    stream->read(buffer->data(), buffer->size());

    FT_Error err = 0;
    int numFaces = 0;
    String familyName;
	String styleName;

    // Face 作成 (ファミリ名・Face 数を調べるため。すぐ削除する)
    {
        FT_Face face;
        err = FT_New_Memory_Face(
            m_ftLibrary,
            (const FT_Byte*)buffer->data(),
            buffer->size(),
            0,
            &face);
        if (LN_ENSURE(err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err)) return;
        numFaces = face->num_faces;
        familyName = String::fromCString(face->family_name);
		styleName = String::fromCString(face->style_name);
        FT_Done_Face(face);
    }

    // Fase ひとつだけ (.ttf)
    if (numFaces == 1)
    {
        uint32_t key = CRCHash::compute(familyName.c_str());
        if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
        {
            TTFDataEntry e;
            e.dataBuffer = buffer;
            e.collectionIndex = 0;
            m_ttfDataEntryMap.insert({ key, e });
            LN_LOG_INFO << "Registered font file." << familyName;

            //// set default name
            //if (defaultFamily) {
            //    m_defaultFontDesc.Family = familyName;
            //}
        }

		m_famlyNameToFontFaceSourceMap.add({ familyName, styleName, buffer, 0 });
    }
    // Fase が複数 (.ttc)
    else if (numFaces > 1)
    {
        for (int i = 0; i < numFaces; i++)
        {
            FT_Face face;
            err = FT_New_Memory_Face(
                m_ftLibrary,
                (const FT_Byte*)buffer->data(),
                buffer->size(),
                i,
                &face);
            if (LN_ENSURE(err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err)) return;

            familyName = String::fromCString(face->family_name);
			styleName = String::fromCString(face->style_name);
            uint32_t key = CRCHash::compute(familyName.c_str());
            if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
            {
                TTFDataEntry e;
                e.dataBuffer = buffer;
                e.collectionIndex = 0;
                m_ttfDataEntryMap.insert({ key, e });
                LN_LOG_INFO << "Registered font file." << familyName;

                //// set default name
                //if (defaultFamily) {
                //    m_defaultFontDesc.Family = familyName;
                //}
            }

			m_famlyNameToFontFaceSourceMap.add({ familyName, styleName, buffer, i });

            FT_Done_Face(face);
        }
    }
    else {
        LN_UNREACHABLE();
    }

    if (defaultFamily) {
        LN_CHECK(m_defaultFont);
        m_defaultFont = m_defaultFont->clone();
        m_defaultFont->setFamily(familyName);
    }
}

Ref<FontCore> FontManager::lookupFontCore(const FontDesc& keyDesc, float dpiScale)
{
	// 検索用のキーを作る
	FontDesc actual = keyDesc;
	if (actual.Family.isEmpty()) {
		actual.Family = defaultFontDesc().Family;
	}
	actual.Size *= dpiScale;

	// 検索
	uint64_t key = actual.calcHash();
	RefObject* ptr = m_fontCoreCache.findObject(key);
	if (ptr) {
		return static_cast<FontCore*>(ptr);
	}
	else {
		auto font = makeRef<FreeTypeFont>();
		font->init(this, actual);
		m_fontCoreCache.registerObject(key, font);
		return font;
	}
}

void FontManager::setDefaultFont(Font* font)
{
    m_defaultFont = font;
}

FontDesc FontManager::defaultFontDesc() const
{
    return FontHelper::getFontDesc(m_defaultFont);
}

Font* FontManager::defaultFont() const
{
    return m_defaultFont;
}

Font* FontManager::emojiFont() const
{
	return m_emojiFont;
}

ByteBuffer* FontManager::getDefaultFontData() const
{
    if (!m_defaultFont) return nullptr;
    uint32_t key = CRCHash::compute(m_defaultFont->family().c_str());
    auto itr = m_ttfDataEntryMap.find(key);
    if (itr != m_ttfDataEntryMap.end()) {
        return itr->second.dataBuffer;
    }
    else {
        return nullptr;
    }
}

const FontFaceSource* FontManager::lookupFontFaceSourceFromFamilyName(const String& name)
{
	int index = m_famlyNameToFontFaceSourceMap.indexOfIf([&](auto& x) { return String::compare(name, x.familyName, CaseSensitivity::CaseInsensitive) == 0; });
	if (index >= 0)
		return &m_famlyNameToFontFaceSourceMap[index];
	else
		return nullptr;

	//auto itr = m_famlyNameToFontFaceSourceMap.find(name);
	//if (itr != m_famlyNameToFontFaceSourceMap.end())
	//	return &itr->second;
	//else
	//	return nullptr;
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

//==============================================================================
// GlyphIconFontManager

GlyphIconFontManager::GlyphIconFontManager()
	: m_fontManager(nullptr)
	, m_fontAwesomeVariablesMap({
#include "Resource/FontAwesome5Map.inl"
	})
{
}

GlyphIconFontManager::~GlyphIconFontManager()
{
}


Result GlyphIconFontManager::init(FontManager* fontManager)
{
	m_fontManager = fontManager;

//	{
//		static const unsigned char data[] = {
//#include "Resource/FontAwesome5Free-Regular-400.otf.inl"
//		};
//		static const size_t size = LN_ARRAY_SIZE_OF(data);
//		MemoryStream stream(data, size);
//		m_fontManager->registerFontFromStream(&stream, false);
//	}
	{
		static const unsigned char data[] = {
#include "Resource/FontAwesome5Free-Solid-900.otf.inl"
		};
		static const size_t size = LN_ARRAY_SIZE_OF(data);
		MemoryStream stream(data, size);
		m_fontManager->registerFontFromStream(&stream, false);
	}

	return true;
}

void GlyphIconFontManager::dispose()
{
	m_fontAwesomeFontMap_Regular.clear();
	m_fontAwesomeFontMap_Solid.clear();
}

Font* GlyphIconFontManager::getFontAwesomeFont(const StringRef& style, int size)
{
	const Char* familyName;
	std::unordered_map<int, Ref<Font>>* fontMap;
	if (String::compare(style, _TT("Solid"), CaseSensitivity::CaseInsensitive) == 0) {
		familyName = _TT("Font Awesome 5 Free");	// TODO: style Solid
		fontMap = &m_fontAwesomeFontMap_Solid;
	}
	else {
		familyName = _TT("Font Awesome 5 Free");
		fontMap = &m_fontAwesomeFontMap_Regular;
	}

	Font* font;
	auto itr = fontMap->find(size);
	if (itr != fontMap->end()) {
		font = itr->second;
	}
	else {
		auto newFont = makeObject<Font>(familyName, size);
		fontMap->insert({size, newFont});
		font = newFont;
	}
	return font;
}

uint32_t GlyphIconFontManager::getFontAwesomeCodePoint(const StringRef& glyphName)
{
	auto itr = m_fontAwesomeVariablesMap.find(glyphName);
	if (itr != m_fontAwesomeVariablesMap.end())
	{
		return itr->second;
	}
	else
	{
		LN_ENSURE(0, "not found FontAwesomeCodePoint.");
		return 0;
	}
}


} // namespace detail
} // namespace ln

