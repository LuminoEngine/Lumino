
#include "../Internal.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include <Lumino/Base/Hash.h>
#include <Lumino/Graphics/Text/Font.h>
#include "../../IO/Archive.h"
#include "FontManager.h"
#include "FreeTypeFont.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// FontData
//==============================================================================

//------------------------------------------------------------------------------
FontData::FontData()
	: Family()
	, Size(16)
	//, EdgeSize(0)
	, isBold(false)
	, isItalic(false)
	, isAntiAlias(true)
{}

//------------------------------------------------------------------------------
bool FontData::operator < (const FontData& right)
{
	if (Family < right.Family) { return true; }
	if (Family > right.Family) { return false; }
	if (Size < right.Size) { return true; }
	if (Size > right.Size) { return false; }
	//if (EdgeSize < right.EdgeSize) { return true; }
	//if (EdgeSize > right.EdgeSize) { return false; }
	if (isBold < right.isBold) { return true; }
	if (isBold > right.isBold) { return false; }
	if (isItalic < right.isItalic) { return true; }
	if (isItalic > right.isItalic) { return false; }
	if (isAntiAlias < right.isAntiAlias) { return true; }
	if (isAntiAlias > right.isAntiAlias) { return false; }
	return false;
}

//------------------------------------------------------------------------------
uint64_t FontData::calcHash() const
{
	uint32_t v[2];
	v[0] = Hash::calcHash(Family.c_str());

	uint8_t* v2 = (uint8_t*)&v[1];
	v2[0] = Size;
	//v2[1] = fontData.EdgeSize;
	v2[3] =
		(((isBold) ? 1 : 0)) |
		(((isItalic) ? 1 : 0) << 1) |
		(((isAntiAlias) ? 1 : 0) << 2);

	return *((uint64_t*)&v);
}

//uint32_t CalcHash() const
//{
//	uint32_t v = Hash::CalcHash(Family.c_str());
//	v += Size;
//	//v += 10 * EdgeSize;
//	v += 100 * (int)isBold;
//	v += 1000 * (int)isItalic;
//	v += 10000 * (int)isAntiAlias;
//	return v;
//}

	
//==============================================================================
// FontManager
//==============================================================================

//------------------------------------------------------------------------------
FontManager::FontManager()
{
	m_defaultFontName = _T("Meiryo UI");
}

//------------------------------------------------------------------------------
FontManager::~FontManager()
{
}

//------------------------------------------------------------------------------
void FontManager::initialize(ArchiveManager* archiveManager, GraphicsManager* graphicsManager)
{
	m_archiveManager = archiveManager;
	m_graphicsManager = graphicsManager;
	m_charToUTF32Converter.setDestinationEncoding(Encoding::getUTF32Encoding());
	m_charToUTF32Converter.getSourceEncoding(Encoding::getSystemMultiByteEncoding());
	m_wcharToUTF32Converter.setDestinationEncoding(Encoding::getUTF32Encoding());
	m_wcharToUTF32Converter.getSourceEncoding(Encoding::getWideCharEncoding());

	m_TCharToUTF32Converter.setDestinationEncoding(Encoding::getUTF32Encoding());
	m_TCharToUTF32Converter.getSourceEncoding(Encoding::getTCharEncoding());
	m_UTF32ToTCharConverter.setDestinationEncoding(Encoding::getTCharEncoding());
	m_UTF32ToTCharConverter.getSourceEncoding(Encoding::getUTF32Encoding());

	// FreeType 初期化
	FT_Error err = FT_Init_FreeType(&m_ftLibrary);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font library : %d\n", err);

	// キャッシマネージャ
	err = FTC_Manager_New(
		m_ftLibrary,
		0, 0, 0,
		callbackFaceRequester,
		this,
		&m_ftCacheManager);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font cache manager : %d\n", err);

	// キャッシュマップ
	err = FTC_CMapCache_New(m_ftCacheManager, &m_ftCMapCache);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font cache map : %d\n", err);

	// イメージキャッシュ
	err = FTC_ImageCache_New(m_ftCacheManager, &m_ftImageCache);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font image cache : %d\n", err);

	// デフォルトフォント
	m_defaultFont = Ref<Font>::makeRef();
	m_defaultFont->initialize(m_graphicsManager, nullptr);
	m_defaultFont->setFamily(m_defaultFontName);
	m_defaultFont->setAntiAlias(true);


	m_defaultRawFont = newObject<FreeTypeFont>();
	m_defaultRawFont->setName(m_defaultFontName);
	m_defaultRawFont->setSize(12);
	m_defaultRawFont->setAntiAlias(true);
	//m_defaultFont = RawFont::CreateBuiltInBitmapFontInternal(this, 7);

	// キャッシュ
	m_rawFontCache = Ref<CacheManager>::makeRef(32, 0);

	// 組み込みフォント
	m_builtinFontList.resize(1);
	{
		Ref<RawFont> raw = RawFont::createBuiltInBitmapFontInternal2(7);
		Ref<Font> font = Ref<Font>::makeRef();
		font->initialize(m_graphicsManager, raw);
		m_builtinFontList[(int)BuiltinFontSize::XXSmall] = font;
	}
}

//------------------------------------------------------------------------------
void FontManager::dispose()
{
	auto disposeList = m_fontResourceList;
	for (RawFont* font : disposeList)
	{
		font->disposeFontResource();
	}

	m_builtinFontList.clear();
	//LN_SAFE_RELEASE();
	//m_defaultRawFont = nullptr;
	m_defaultFont = nullptr;

	m_rawFontCache->finalizeCache();
	m_defaultRawFont.safeRelease();

	// 登録したTTFファイルのメモリバッファをすべて解放
	//TTFDataEntryMap::iterator itr = m_ttfDataEntryMap.begin();
	//for (; itr != m_ttfDataEntryMap.end(); ++itr)
	//{
	//	LN_SAFE_RELEASE(itr->second.DataBuffer);
	//}
	m_ttfDataEntryMap.clear();

	// キャッシュマネージャ
	if (m_ftCacheManager != NULL)
	{
		FTC_Manager_Done(m_ftCacheManager);
		m_ftCacheManager = NULL;
	}

	// FreeType
	if (m_ftLibrary != NULL)
	{
		FT_Done_FreeType(m_ftLibrary);
		m_ftLibrary = NULL;
	}
}

//------------------------------------------------------------------------------
void FontManager::registerFontFile(const String& fontFilePath)
{
	// ファイルから全てのデータを読み込む
	Ref<Stream> file = m_archiveManager->createFileStream(fontFilePath, false);
	ByteBuffer buffer((size_t)file->getLength(), false);
	file->read(buffer.getData(), buffer.getSize());

	// Face 作成 (ファミリ名・Face 数を調べるため。すぐ削除する)
	FT_Face face;
	FT_Error err = FT_New_Memory_Face(
		m_ftLibrary,
		(const FT_Byte*)buffer.getData(),
		buffer.getSize(),
		0,
		&face);
	LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

	// Fase ひとつだけ (.ttf)
	if (face->num_faces == 1)
	{
		String familyName(face->family_name);
		uint32_t key = Hash::calcHash(familyName.c_str());
		if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
		{
			TTFDataEntry e;
			e.DataBuffer = buffer;
//			e.DataBuffer->AddRef();
			e.CollectionIndex = 0;
			m_ttfDataEntryMap.insert(TTFDataEntryPair(key, e));

			ln::Logger::writeLine("Registered font file. [%s]", face->family_name);

			//// 初回登録の場合はデフォルトフォント名として登録する
			//if (m_ttfDataEntryMap.size() == 1) {
			//	m_defaultFont->setName(familyName);
			//}
		}
		FT_Done_Face(face);

	}
	// Fase が複数 (.ttc)
	else if (face->num_faces > 1)
	{
		int facesCount = face->num_faces;
		FT_Done_Face(face);	// 数を調べるためだけに使った。一度削除
		face = NULL;

		for (int i = 0; i < facesCount; i++)
		{
			err = FT_New_Memory_Face(
				m_ftLibrary,
				(const FT_Byte*)buffer.getData(),
				buffer.getSize(),
				i,
				&face);
			LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

			String familyName(face->family_name);
			uint32_t key = Hash::calcHash(familyName.c_str());
			if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
			{
				TTFDataEntry e;
				e.DataBuffer = buffer;
//				e.DataBuffer->AddRef();
				e.CollectionIndex = i;
				m_ttfDataEntryMap.insert(TTFDataEntryPair(key, e));

				ln::Logger::writeLine("Registered font file. [%s]", face->family_name);

				//// 初回登録の場合はデフォルトフォント名として登録する
				//if (m_ttfDataEntryMap.size() == 1) {
				//	m_defaultFont->setName(familyName);
				//}
			}
			FT_Done_Face(face);
		}
	}
	else {
		FT_Done_Face(face);
		LN_THROW(0, InvalidOperationException);
	}
}

//------------------------------------------------------------------------------
void FontManager::setDefaultRawFont(RawFont* font)
{
	LN_REFOBJ_SET(m_defaultRawFont, font);
}

//------------------------------------------------------------------------------
FontPtr FontManager::getBuiltinFont(BuiltinFontSize size) const
{
	return m_builtinFontList[(int)size];
}

//------------------------------------------------------------------------------
RawFontPtr FontManager::lookupRawFont(const detail::FontData& keyData)
{
	CacheKey key(keyData.calcHash());
	RawFont* ptr = static_cast<RawFont*>(m_rawFontCache->findObjectAddRef(key));
	if (ptr != nullptr) return RawFontPtr(ptr, false);	// found

	RawFontPtr ref;
	if (keyData.Family.isEmpty())
	{
		ref = getDefaultRawFont()->copy();
	}
	else
	{
		auto ftFont = newObject<FreeTypeFont>();
		ref = ftFont;
	}

	ref->setName(keyData.Family);
	ref->setSize(keyData.Size);
	ref->setBold(keyData.isBold);
	ref->setItalic(keyData.isItalic);
	ref->setAntiAlias(keyData.isAntiAlias);

	m_rawFontCache->registerCacheObject(key, ref);
	return RawFontPtr::staticCast(ref);
}

//------------------------------------------------------------------------------
FT_Error FontManager::faceRequester(
	FTC_FaceID face_id,
	FT_Library library,
	FT_Pointer request_data,
	FT_Face* aface)
{
	// キャッシュ検索のコールバック。map から取るようにしている。

	// face_id は、FTC_Manager_LookupFace() に渡した ID が入ってくる。
	// ID は自分で好きなように決められる。とりあえず文字列 (ファミリ名)としている

	//String family_name( (lnChar*)face_id );
	intptr_t key = (intptr_t)face_id;
	TTFDataEntryMap::iterator itr = m_ttfDataEntryMap.find(key/*family_name*/);
	if (itr != m_ttfDataEntryMap.end())
	{
		FT_Face face;
		FT_Error err = FT_New_Memory_Face(
			m_ftLibrary,
			(const FT_Byte*)itr->second.DataBuffer.getConstData(),
			itr->second.DataBuffer.getSize(),
			itr->second.CollectionIndex,
			&face);
		LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

		err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		if (err != FT_Err_Ok)
		{
			FT_Done_Face(face);
			LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_Select_Charmap : %d\n", err);
		}

		*aface = face;
		return FT_Err_Ok;
	}
#ifdef LN_OS_WIN32
	else if (m_requesterFaceName != NULL)
	{
		// 名前からシステムフォント検索
		TSystemFontData* systemFont = getWindowsSystemFontData(m_requesterFaceName);
		m_requesterFaceName = NULL;
		if (systemFont == NULL){
			return FT_Err_Cannot_Open_Resource;
		}

		// リソースロック
		size_t size = 0;
		int index = 0;
		byte_t* data = lockWindowsSystemFontData(systemFont, &size, &index);
		if (data == NULL){
			freeWindowsSystemFontData(systemFont);
			return FT_Err_Cannot_Open_Resource;
		}

		// FreeType の読み取りストリーム
		FT_Stream stream = (FT_Stream)malloc(sizeof(FT_StreamRec));
		if (stream == NULL){
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

//------------------------------------------------------------------------------
FT_Error FontManager::callbackFaceRequester(
	FTC_FaceID face_id,
	FT_Library library,
	FT_Pointer request_data,
	FT_Face* aface)
{
	return ((FontManager*)request_data)->faceRequester(face_id, library, request_data, aface);
}


#ifdef LN_OS_WIN32
// http://kikyou.info/diary/?200510#i10 を参考にした
#define TVP_TT_TABLE_ttcf  (('t' << 0) + ('t' << 8) + ('c' << 16) + ('f' << 24))
#define TVP_TT_TABLE_name  (('n' << 0) + ('a' << 8) + ('m' << 16) + ('e' << 24))

//------------------------------------------------------------------------------
FontManager::TSystemFontData* FontManager::getWindowsSystemFontData(LPCTSTR name)
{
	DWORD result;
	TSystemFontData *p = (TSystemFontData*)malloc(sizeof(TSystemFontData));
	if (p == NULL){
		return NULL;
	}
	p->fontdata = NULL;
	p->index = 0;
	p->hdc = GetDC(NULL);
	if (p->hdc == NULL){
		free(p);
		return NULL;
	}
	// 名前以外適当
	p->hFont = CreateFont(
		12, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		name);
	if (p->hFont == NULL){
		ReleaseDC(NULL, p->hdc);
		free(p);
		return NULL;
	}
	p->hOldFont = (HFONT)::SelectObject(p->hdc, p->hFont);
	// フォントデータが得られそうかチェック
	result = ::GetFontData(p->hdc, TVP_TT_TABLE_name, 0, NULL, 0);
	if (result == GDI_ERROR){
		SelectObject(p->hdc, p->hOldFont);
		DeleteObject(p->hFont);
		ReleaseDC(NULL, p->hdc);
		free(p);
		return NULL;
	}
	return p;
}

//------------------------------------------------------------------------------
unsigned char* FontManager::lockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index)
{
	unsigned char *name_content = NULL; // Windows から取得した name タグの内容
	unsigned char *name_content_ft = NULL; // FreeType から取得した name タグの内容
	DWORD result;

	DWORD fontsize;
	BOOL IsTTC = FALSE;
	unsigned char buf[4];
	int name_content_size;

	FT_Face face = NULL;

	int i;

	if (fnt == NULL || index == NULL){
		return NULL;
	}
	if (fnt->fontdata != NULL){
		*size = fnt->size;
		*index = fnt->index;
		return fnt->fontdata;
	}
	// フォント名称取得処理
	result = GetFontData(fnt->hdc, TVP_TT_TABLE_name, 0, NULL, 0);
	if (result == GDI_ERROR){
		// エラー; GetFontData では扱えなかった
		return NULL;
	}
	name_content_size = result;
	name_content = (unsigned char*)malloc(name_content_size);
	name_content_ft = (unsigned char*)malloc(name_content_size);
	GetFontData(fnt->hdc, TVP_TT_TABLE_name, 0, name_content, name_content_size);

	// フォントサイズ取得処理
	result = GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, &buf, 1);
	if (result == 1){
		// TTC ファイルだと思われる
		result = GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, NULL, 0);
		IsTTC = TRUE;
	}
	else{
		result = GetFontData(fnt->hdc, 0, 0, NULL, 0);
		fnt->index = 0;
	}
	if (result == GDI_ERROR){
		// エラー; GetFontData では扱えなかった
		free(name_content);
		free(name_content_ft);
		return NULL;
	}
	fontsize = result;
	fnt->size = fontsize;
	fnt->fontdata = (unsigned char*)malloc(fontsize);
	if (fnt->fontdata == NULL){
		free(name_content);
		free(name_content_ft);
		return NULL;
	}
	if (IsTTC){
		GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, fnt->fontdata, fontsize);
	}
	else{
		GetFontData(fnt->hdc, 0, 0, fnt->fontdata, fontsize);
	}
	if (!IsTTC){
		*size = fnt->size;
		*index = fnt->index;
		free(name_content);
		free(name_content_ft);
		return fnt->fontdata;
	}
	i = 0;
	while (1){
		FT_ULong length;
		FT_Error err;
		if (FT_New_Memory_Face(
			m_ftLibrary,
			fnt->fontdata,
			fnt->size,
			i,
			&face)){
			free(name_content);
			free(name_content_ft);
			return NULL;
		}
		// FreeType から、name タグのサイズを取得する
		length = 0;
		err = FT_Load_Sfnt_Table(face, TTAG_name, 0, NULL, &length);
		if (err){
			FT_Done_Face(face);
			free(name_content);
			free(name_content_ft);
			return NULL;
		}
		// FreeType から得た name タグの長さを Windows から得た長さと比較
		if (length == (DWORD)name_content_size){
			// FreeType から name タグを取得
			err = FT_Load_Sfnt_Table(face, TTAG_name, 0, name_content_ft, &length);
			if (err){
				FT_Done_Face(face);
				free(name_content);
				free(name_content_ft);
				return NULL;
			}
			// FreeType から読み込んだ name タグの内容と、Windows から読み込んだ
			// name タグの内容を比較する。
			// 一致していればその index のフォントを使う。
			if (!memcmp(name_content, name_content_ft, name_content_size)){
				FT_Done_Face(face);
				fnt->index = i;
				break;
			}
		}
		FT_Done_Face(face);
		i++;
	}

	*size = fnt->size;
	*index = fnt->index;
	free(name_content);
	free(name_content_ft);
	return fnt->fontdata;
}

//------------------------------------------------------------------------------
void FontManager::freeWindowsSystemFontData(TSystemFontData *fnt)
{
	if (fnt == NULL){
		return;
	}
	if (fnt->fontdata != NULL){
		free(fnt->fontdata);
	}
	SelectObject(fnt->hdc, fnt->hOldFont);
	DeleteObject(fnt->hFont);
	ReleaseDC(NULL, fnt->hdc);
	free(fnt);
}

//------------------------------------------------------------------------------
unsigned long FontManager::streamIoFunc(
	FT_Stream stream,
	unsigned long offset,
	unsigned char* buffer,
	unsigned long count)
{
	if (count != 0){
		TSystemFontData* font = (TSystemFontData*)stream->descriptor.pointer;
		unsigned char* p;
		unsigned long copycount;
		if (font == NULL || font->fontdata == NULL)
			return 0;
		p = font->fontdata;
		if (offset > stream->size)
			return 0;	// error
		copycount = offset + count > stream->size ?
			stream->size - offset :
			count;
		p += offset;
		if (buffer != NULL)
			memcpy(buffer, p, copycount);
		return copycount;
	}
	return 0;
}

//------------------------------------------------------------------------------
void FontManager::streamCloseFunc(FT_Stream stream)
{
	freeWindowsSystemFontData((TSystemFontData*)stream->descriptor.pointer);
	free(stream);
}
#endif

} // namespace detail
LN_NAMESPACE_END
