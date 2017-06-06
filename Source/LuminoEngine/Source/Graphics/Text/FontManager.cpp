
#include "../Internal.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include <Lumino/Base/Hash.h>
#include <Lumino/Graphics/Text/Font.h>
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
	, IsBold(false)
	, IsItalic(false)
	, IsAntiAlias(true)
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
	if (IsBold < right.IsBold) { return true; }
	if (IsBold > right.IsBold) { return false; }
	if (IsItalic < right.IsItalic) { return true; }
	if (IsItalic > right.IsItalic) { return false; }
	if (IsAntiAlias < right.IsAntiAlias) { return true; }
	if (IsAntiAlias > right.IsAntiAlias) { return false; }
	return false;
}

//------------------------------------------------------------------------------
uint64_t FontData::CalcHash() const
{
	uint32_t v[2];
	v[0] = Hash::CalcHash(Family.c_str());

	uint8_t* v2 = (uint8_t*)&v[1];
	v2[0] = Size;
	//v2[1] = fontData.EdgeSize;
	v2[3] =
		(((IsBold) ? 1 : 0)) |
		(((IsItalic) ? 1 : 0) << 1) |
		(((IsAntiAlias) ? 1 : 0) << 2);

	return *((uint64_t*)&v);
}

//uint32_t CalcHash() const
//{
//	uint32_t v = Hash::CalcHash(Family.c_str());
//	v += Size;
//	//v += 10 * EdgeSize;
//	v += 100 * (int)IsBold;
//	v += 1000 * (int)IsItalic;
//	v += 10000 * (int)IsAntiAlias;
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
void FontManager::Initialize(FileManager* fileManager, GraphicsManager* graphicsManager)
{
	m_fileManager = fileManager;
	m_graphicsManager = graphicsManager;
	m_charToUTF32Converter.SetDestinationEncoding(Encoding::GetUTF32Encoding());
	m_charToUTF32Converter.SetSourceEncoding(Encoding::GetSystemMultiByteEncoding());
	m_wcharToUTF32Converter.SetDestinationEncoding(Encoding::GetUTF32Encoding());
	m_wcharToUTF32Converter.SetSourceEncoding(Encoding::GetWideCharEncoding());

	m_TCharToUTF32Converter.SetDestinationEncoding(Encoding::GetUTF32Encoding());
	m_TCharToUTF32Converter.SetSourceEncoding(Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetDestinationEncoding(Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetSourceEncoding(Encoding::GetUTF32Encoding());

	// FreeType 初期化
	FT_Error err = FT_Init_FreeType(&m_ftLibrary);
	LN_THROW(err == 0, InvalidOperationException, "failed initialize font library : %d\n", err);

	// キャッシマネージャ
	err = FTC_Manager_New(
		m_ftLibrary,
		0, 0, 0,
		CallbackFaceRequester,
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
	m_defaultFont = RefPtr<Font>::MakeRef();
	m_defaultFont->Initialize(m_graphicsManager, nullptr);
	m_defaultFont->SetFamily(m_defaultFontName);
	m_defaultFont->SetAntiAlias(true);


	m_defaultRawFont = LN_NEW FreeTypeFont(this);
	m_defaultRawFont->SetName(m_defaultFontName);
	m_defaultRawFont->SetSize(12);
	m_defaultRawFont->SetAntiAlias(true);
	//m_defaultFont = RawFont::CreateBuiltInBitmapFontInternal(this, 7);

	// キャッシュ
	m_rawFontCache = RefPtr<CacheManager>::MakeRef(32, 0);

	// 組み込みフォント
	m_builtinFontList.Resize(1);
	{
		RefPtr<RawFont> raw(RawFont::CreateBuiltInBitmapFontInternal(this, 7), false);
		RefPtr<Font> font = RefPtr<Font>::MakeRef();
		font->Initialize(m_graphicsManager, raw);
		m_builtinFontList[(int)BuiltinFontSize::XXSmall] = font;
	}
}

//------------------------------------------------------------------------------
void FontManager::Finalize()
{
	//for (FreeTypeFont* font : m_freeTypeFontList)
	//{
	//	font->
	//}

	m_builtinFontList.Clear();
	//LN_SAFE_RELEASE();
	//m_defaultRawFont = nullptr;
	m_defaultFont = nullptr;

	m_rawFontCache->FinalizeCache();
	LN_SAFE_RELEASE(m_defaultRawFont);

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
void FontManager::RegisterFontFile(const String& fontFilePath)
{
	// ファイルから全てのデータを読み込む
	RefPtr<Stream> file(m_fileManager->CreateFileStream(fontFilePath), false);
	ByteBuffer buffer((size_t)file->GetLength(), false);
	file->Read(buffer.GetData(), buffer.GetSize());

	// Face 作成 (ファミリ名・Face 数を調べるため。すぐ削除する)
	FT_Face face;
	FT_Error err = FT_New_Memory_Face(
		m_ftLibrary,
		(const FT_Byte*)buffer.GetData(),
		buffer.GetSize(),
		0,
		&face);
	LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

	// Fase ひとつだけ (.ttf)
	if (face->num_faces == 1)
	{
		String familyName(face->family_name);
		uint32_t key = Hash::CalcHash(familyName.c_str());
		if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
		{
			TTFDataEntry e;
			e.DataBuffer = buffer;
//			e.DataBuffer->AddRef();
			e.CollectionIndex = 0;
			m_ttfDataEntryMap.insert(TTFDataEntryPair(key, e));

			ln::Logger::WriteLine("Registered font file. [%s]", face->family_name);

			//// 初回登録の場合はデフォルトフォント名として登録する
			//if (m_ttfDataEntryMap.size() == 1) {
			//	m_defaultFont->SetName(familyName);
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
				(const FT_Byte*)buffer.GetData(),
				buffer.GetSize(),
				i,
				&face);
			LN_THROW(err == FT_Err_Ok, InvalidOperationException, "failed FT_New_Memory_Face : %d\n", err);

			String familyName(face->family_name);
			uint32_t key = Hash::CalcHash(familyName.c_str());
			if (m_ttfDataEntryMap.find(key) == m_ttfDataEntryMap.end())
			{
				TTFDataEntry e;
				e.DataBuffer = buffer;
//				e.DataBuffer->AddRef();
				e.CollectionIndex = i;
				m_ttfDataEntryMap.insert(TTFDataEntryPair(key, e));

				ln::Logger::WriteLine("Registered font file. [%s]", face->family_name);

				//// 初回登録の場合はデフォルトフォント名として登録する
				//if (m_ttfDataEntryMap.size() == 1) {
				//	m_defaultFont->SetName(familyName);
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
void FontManager::SetDefaultRawFont(RawFont* font)
{
	LN_REFOBJ_SET(m_defaultRawFont, font);
}

//------------------------------------------------------------------------------
FontPtr FontManager::GetBuiltinFont(BuiltinFontSize size) const
{
	return m_builtinFontList[(int)size];
}

//------------------------------------------------------------------------------
RawFontPtr FontManager::LookupRawFont(const detail::FontData& keyData)
{
	CacheKey key(keyData.CalcHash());
	RawFont* ptr = static_cast<RawFont*>(m_rawFontCache->FindObjectAddRef(key));
	if (ptr != nullptr) return RawFontPtr(ptr, false);	// found

	RawFontPtr ref;
	if (keyData.Family.IsEmpty())
	{
		ref = GetDefaultRawFont()->Copy();
	}
	else
	{
		auto ftFont = RefPtr<FreeTypeFont>::MakeRef(this);
		ref = ftFont;
	}

	ref->SetName(keyData.Family);
	ref->SetSize(keyData.Size);
	ref->SetBold(keyData.IsBold);
	ref->SetItalic(keyData.IsItalic);
	ref->SetAntiAlias(keyData.IsAntiAlias);

	m_rawFontCache->RegisterCacheObject(key, ref);
	return RawFontPtr::StaticCast(ref);
}

//------------------------------------------------------------------------------
FT_Error FontManager::FaceRequester(
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
			(const FT_Byte*)itr->second.DataBuffer.GetConstData(),
			itr->second.DataBuffer.GetSize(),
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
		TSystemFontData* systemFont = GetWindowsSystemFontData(m_requesterFaceName);
		m_requesterFaceName = NULL;
		if (systemFont == NULL){
			return FT_Err_Cannot_Open_Resource;
		}

		// リソースロック
		size_t size = 0;
		int index = 0;
		byte_t* data = LockWindowsSystemFontData(systemFont, &size, &index);
		if (data == NULL){
			FreeWindowsSystemFontData(systemFont);
			return FT_Err_Cannot_Open_Resource;
		}

		// FreeType の読み取りストリーム
		FT_Stream stream = (FT_Stream)malloc(sizeof(FT_StreamRec));
		if (stream == NULL){
			FreeWindowsSystemFontData(systemFont);
			return FT_Err_Out_Of_Memory;
		}
		memset(stream, 0, sizeof(FT_StreamRec));
		stream->base = 0;
		stream->size = size;
		stream->pos = 0;
		stream->descriptor.pointer = systemFont;
		stream->pathname.pointer = NULL;
		stream->read = StreamIoFunc;
		stream->close = StreamCloseFunc;

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
FT_Error FontManager::CallbackFaceRequester(
	FTC_FaceID face_id,
	FT_Library library,
	FT_Pointer request_data,
	FT_Face* aface)
{
	return ((FontManager*)request_data)->FaceRequester(face_id, library, request_data, aface);
}


#ifdef LN_OS_WIN32
// http://kikyou.info/diary/?200510#i10 を参考にした
#define TVP_TT_TABLE_ttcf  (('t' << 0) + ('t' << 8) + ('c' << 16) + ('f' << 24))
#define TVP_TT_TABLE_name  (('n' << 0) + ('a' << 8) + ('m' << 16) + ('e' << 24))

//------------------------------------------------------------------------------
FontManager::TSystemFontData* FontManager::GetWindowsSystemFontData(LPCTSTR name)
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
unsigned char* FontManager::LockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index)
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
void FontManager::FreeWindowsSystemFontData(TSystemFontData *fnt)
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
unsigned long FontManager::StreamIoFunc(
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
void FontManager::StreamCloseFunc(FT_Stream stream)
{
	FreeWindowsSystemFontData((TSystemFontData*)stream->descriptor.pointer);
	free(stream);
}
#endif

} // namespace detail
LN_NAMESPACE_END
