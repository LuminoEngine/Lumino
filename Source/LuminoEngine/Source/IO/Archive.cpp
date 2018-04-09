/*	
	[2015/4/13] ファイル名マップのキーは UTF-16 統一？環境に合わせる？その②
		
		Char に合わせる。
		というのも、一部 (ほぼ全て？Ubuntuとか) の Unix 環境では、wchar_t が使いものにならないため。


	[2013/10/13] ファイル名マップのキーは UTF-16 統一？環境に合わせる？
		速度的な懸念。環境に合わせた方が高速な気がするが…。

		wchar_t のみ使用してくださいっていうなら、確かに高速。
		でも、Ruby とか Lua とか使うときはそんなことできない。
		SJIS or UTF-8 → UTF-16 or UTF-32 の変換が必ず必要。
		でも、この変換は「(UTF-32への変換でも)一度UTF-16を経由する」とかいう必要はなく、
		速度的にはそんなに変わらない。
		
		UTF-16か32かは UnicodeString 内に完全に隠されているので、
		あとは各文字コードから32への変換関数を実装すれば特に問題はない。
		(それが面倒なのだが…)
		
		あと、メモリ使用量が単に倍になるって言う点も心配。
		(棒人間では、約100ファイル、平均20文字(ASCII) → UTF-32 だと 6000kバイトくらい無駄)
		ビットマップに比べたら全然マシだけど…。

		→とりあえず完全にUnicodeString 任せで。
*/

#include <string>
#include <algorithm>
#include <camellia/camellia.h>
#include "../Internal.h"
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/Base/Environment.h>
#include <Lumino/Base/StdStringHelper.h>
#include <Lumino/IO/Common.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Text/Encoding.h>
#include "../EngineManager.h"
#include "ArchiveMaker.h"
#include "Archive.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// ArchiveFileAssetsStorage
//==============================================================================
	
const byte_t ArchiveFileAssetsStorage::InternalKey[16] = {
	0x6e, 0x36, 0x38, 0x64, 0x35, 0x6f, 0x68, 0x6d,
	0x33, 0x42, 0x69, 0x61, 0x34, 0x78, 0x37, 0x6c
};

//------------------------------------------------------------------------------
ArchiveFileAssetsStorage::ArchiveFileAssetsStorage()
	: m_stream(NULL)
	, m_fileCount(0)
{
}

//------------------------------------------------------------------------------
ArchiveFileAssetsStorage::~ArchiveFileAssetsStorage()
{
	if (m_stream)
	{
		fclose(m_stream);
		m_stream = NULL;

		m_entriesMap.clear();
	}
}

//------------------------------------------------------------------------------
void ArchiveFileAssetsStorage::open(const PathName& filePath, const String& key)
{
	m_key = key;
	m_fileCount = 0;

	// このアーカイブファイルをディレクトリに見立てたときのパスを作る
	// (絶対パスにして、拡張子を取り除く)
	PathName fullPath = filePath.canonicalizePath();
	m_virtualDirectoryPath = fullPath.getWithoutExtension();

    // 拡張キーの初期化
	memset(m_keyTable, 0, sizeof(m_keyTable));
	if (!m_key.isEmpty())
	{
		std::string k = StdStringHelper::makeStdString(m_key);
		byte_t key_buf[KEY_SIZE] = { 0 };
		memcpy(key_buf, k.c_str(), k.length());
		memset(m_keyTable, 0, sizeof(m_keyTable));
		Camellia_Ekeygen(KEY_SIZE, key_buf, m_keyTable);
    }

	// アーカイブファイルを開く
	errno_t err = _tfopen_s(&m_stream, filePath.c_str(), _T("rb"));
	if (LN_ENSURE_FILE_NOT_FOUND(err == 0, filePath.c_str())) return;

    // 終端から16バイト戻ってからそれを読むとファイル数
	fseek(m_stream, -16, SEEK_END);
	m_fileCount = readU32Padding16();

	// ヘッダ読み込み
	fseek( m_stream, 0, SEEK_SET );
    ArchiveHeader header;
    fread( &header, 1, sizeof( header ), m_stream );

    // マジックナンバーをチェック
    if ( memcmp( header.ID, "lna ", 4 ) != 0 )
	{
		fclose( m_stream );
		m_stream = NULL;
		LN_ENSURE_INVALID_FORMAT(0);
		return;
	}

	// 内部キーのチェック (ユーザーキーは本当に正しいか？)
	byte_t internalKey[16];
	readPadding16(internalKey, 16);
	if (memcmp(internalKey, ArchiveFileAssetsStorage::InternalKey, 16) != 0)
	{
		LN_ENSURE_INVALID_FORMAT(0, "invalid archive key.");
		return;
	}

	// TODO: UTF16ではなく String の内部エンコーディングに合わせる

	// ファイル情報を取得していく
	uint32_t name_len;
	Entry entry;
	for ( int i = 0; i < m_fileCount; ++i )
	{
		// ファイル名の長さとファイルサイズを読む
		readU32Padding16(&name_len, &entry.Size);

		// ファイル名を読み込むバッファを確保して読み込む
		ByteBuffer nameBuf(name_len * sizeof(UTF16));
		readPadding16(nameBuf.getData(), name_len * sizeof(UTF16));
		String tmpName = Encoding::fromBytes(nameBuf.getData(), nameBuf.getSize(), Encoding::getUTF16Encoding());
		PathName name(m_virtualDirectoryPath, tmpName);	// 絶対パスにする
		name = name.canonicalizePath();
			
		// ここでのファイルポインタがデータ本体の位置
		entry.Offset = ftell( m_stream );

		// 今のシステム用に正規化してからファイルマップに追加
		m_entriesMap.insert(EntriesPair(name, entry));

		// ファイルポインタをデータサイズ分進めて、次のファイルへ
		uint32_t ofs = padding16(entry.Size);
		fseek(m_stream, entry.Size + ofs, SEEK_CUR);
	}
}

//------------------------------------------------------------------------------
bool ArchiveFileAssetsStorage::existsFile(const PathName& fileFullPath)
{
#if 1 // map のキーを絶対パスにしてみた。メモリ効率は悪いが、検索キー用に PathName を再度作らなくて良くなる。まぁ、携帯機に乗せるときに問題になるようなら改めて見直す…。
	EntriesMap::iterator itr = m_entriesMap.find(fileFullPath);
	if (itr != m_entriesMap.end()) {
		return true;
	}
	return false;
#else
	// まず、パスの先頭が m_virtualDirectoryPath と一致するかを確認する
	CaseSensitivity cs = FileManager::GetInstance().GetFileSystemCaseSensitivity();
	if (StringUtils::Compare(fileFullPath.c_str(), m_virtualDirectoryPath.GetCStr(), m_virtualDirectoryPath.getString().getLength(), cs) == 0)
	{
		// internalPath は m_virtualDirectoryPath の後ろの部分の開始位置
		const Char* internalPath = fileFullPath.c_str() + m_virtualDirectoryPath.getString().getLength();
		if (*internalPath != _T('\0'))
		{
			// 検索
			EntriesMap::iterator itr = m_entriesMap.find(internalPath);
			if (itr != m_entriesMap.end()) {
				return true;
			}
		}
	}
	return false;
#endif
}

//------------------------------------------------------------------------------
bool ArchiveFileAssetsStorage::tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring)
{
#if 1 // map のキーを絶対パスにしてみた。メモリ効率は悪いが、検索キー用に PathName を再度作らなくて良くなる。まぁ、携帯機に乗せるときに問題になるようなら改めて見直す…。
	EntriesMap::iterator itr = m_entriesMap.find(fileFullPath);
	if (itr == m_entriesMap.end()) {
		return false;
	}

	outStream->attach(LN_NEW ArchiveStream(this, m_stream, itr->second.Offset, itr->second.Size), false);
	return true;
#else
	// まず、パスの先頭が m_virtualDirectoryPath と一致するかを確認する
	CaseSensitivity cs = FileManager::GetInstance().GetFileSystemCaseSensitivity();
	if (StringUtils::Compare(fileFullPath.c_str(), m_virtualDirectoryPath.GetCStr(), m_virtualDirectoryPath.getString().getLength(), cs) != 0)
	{
		LN_THROW(0, FileNotFoundException, fileFullPath);
	}

	// internalPath は m_virtualDirectoryPath の後ろの部分の開始位置
	const Char* internalPath = fileFullPath.c_str() + m_virtualDirectoryPath.getString().getLength();
	LN_THROW((*internalPath != _T('\0')), FileNotFoundException, fileFullPath);	// ファイル名が空だった

	EntriesMap::iterator itr = m_entriesMap.find(internalPath);
	LN_THROW((itr != m_entriesMap.end()), FileNotFoundException, fileFullPath);	// ファイルが見つからなかった
	
	return LN_NEW ArchiveStream(this, m_stream, itr->second.mOffset, itr->second.mSize);
#endif
}

//------------------------------------------------------------------------------
size_t ArchiveFileAssetsStorage::readArchiveStream(byte_t* buffer, size_t count, FILE* stream, uint64_t dataOffset, uint64_t seekPos)
{
	MutexScopedLock lock(m_mutex);
	byte_t tmpSrcBuf[16];	// 復号前データ
	byte_t tmpDstBuf[16];	// 復号後データ
	size_t readSize = 0;

	uint64_t frontLeadCount = (seekPos % 16);						// 読み取り開始ブロックの、先頭の不要なバイト数
	uint64_t startBlockPos = dataOffset + seekPos - frontLeadCount;	// 読み取り開始ブロック(16byte単位)の開始Seek位置

	fseek(m_stream, (long)startBlockPos, SEEK_SET);

	// 前方に余分に読んだものがある場合 (前回の read が 16byte の倍数ではなければ、ブロックの途中から読む必要がある)
	if (frontLeadCount > 0)
	{
		fread(tmpSrcBuf, 1, 16, m_stream);	// アーカイブ内のデータは必ず 16byte にパディングされているので 16 読み取ってしまってよい
		Camellia_DecryptBlock(KEY_SIZE, tmpSrcBuf, m_keyTable, tmpDstBuf);

		uint32_t size = std::min(count, (size_t)(16 - frontLeadCount));	// 余分を除いたデータサイズ (min は count 以上書きこまないため)
		memcpy(buffer, (tmpDstBuf + frontLeadCount), size);
		buffer += size;
		count -= size;
		readSize += size;
	}

	// 読み取りデータが 16byte 満たされる間ループ。
	// 今回の読み込みで buffer への読み取りバイト数に到達する場合は終了。最後のブロックは抜けた直後の if に任せる。
	// count_ が 16 丁度の場合はもう一周。
	while (count >= 16)
	{
		fread(tmpSrcBuf, 1, 16, m_stream);
		Camellia_DecryptBlock(KEY_SIZE, tmpSrcBuf, m_keyTable, buffer);		// 直接 buffer に書き込んでしまう
		buffer += 16;
		count -= 16;
		readSize += 16;
	}

	// 後方に 16byte 未満が残っている
	uint32_t remCount = (count % 16);
	if (remCount > 0)
	{
		fread(tmpSrcBuf, 1, 16, m_stream);
		Camellia_DecryptBlock(KEY_SIZE, tmpSrcBuf, m_keyTable, tmpDstBuf);
		memcpy(buffer, tmpDstBuf, remCount);
		readSize += remCount;
	}
	else {
		// ここに来るなら全てのデータは読み終えているはず
		LN_ASSERT(count == 0);
	}

	return readSize;
}

//------------------------------------------------------------------------------
uint32_t ArchiveFileAssetsStorage::readU32Padding16()
{
	uint32_t v0, v1;
	readU32Padding16(&v0, &v1);
	return v0;
}

void ArchiveFileAssetsStorage::readU32Padding16( uint32_t* v0, uint32_t* v1 )
{
	byte_t b[16] = { 0 };

	// 復号する場合
	if (!m_key.isEmpty())
	{
		byte_t buf[16];
		fread(buf, 1, 16, m_stream);
		Camellia_DecryptBlock(KEY_SIZE, buf, m_keyTable, b);
	}
	// そのまま読み込む場合
	else
	{
		fread(b, 1, 16, m_stream);
	}

	if (Environment::isLittleEndian())
	{
		*v0 = *((uint32_t*)(b + 0));
		*v1 = *((uint32_t*)(b + 4));
	}
	else
	{
		uint32_t r = *((uint32_t*)b);
		*v0 = (((r) >> 24) +
			(((r)& 0x00ff0000) >> 8) +
			(((r)& 0x0000ff00) << 8) +
			((r) << 24));

		r = *((uint32_t*)(b + 4));
		*v1 = (((r) >> 24) +
			(((r)& 0x00ff0000) >> 8) +
			(((r)& 0x0000ff00) << 8) +
			((r) << 24));
	}
}

void ArchiveFileAssetsStorage::readPadding16(byte_t* buffer, int count)
{
	// 復号する場合
	if (!m_key.isEmpty())
	{
		while (count > 0)
		{
			byte_t tmpSrcBuf[16];
			byte_t tmpDstBuf[16];

			// 16byte 読んで復号
			fread(tmpSrcBuf, 1, 16, m_stream);
			Camellia_DecryptBlock(KEY_SIZE, tmpSrcBuf, m_keyTable, tmpDstBuf);

			// buffer へ書き込む
			int size = std::min(count, 16);
			memcpy(buffer, tmpDstBuf, size);
			buffer += size;
			count -= size;
		}
	}
	// 復号しない場合
	else
	{
		fread(buffer, 1, 16, m_stream);
		fseek(m_stream, padding16(count), SEEK_CUR);
	}
}

//==============================================================================
// ArchiveStream
//==============================================================================

//------------------------------------------------------------------------------
ArchiveStream::ArchiveStream(ArchiveFileAssetsStorage* archive, FILE* stream, uint32_t dataOffset, uint32_t dataSize)
	: m_archive(archive)
	, m_stream(stream)
	, m_dataOffset(dataOffset)
	, m_dataSize(dataSize)
	, m_seekPoint(0)
{
	LN_SAFE_ADDREF(m_archive);
}

//------------------------------------------------------------------------------
ArchiveStream::~ArchiveStream()
{
	LN_SAFE_RELEASE(m_archive);
}

//------------------------------------------------------------------------------
size_t ArchiveStream::read(void* buffer, size_t byteCount)
{
    // 復号しながら読み込む
	size_t validSize = m_archive->readArchiveStream((byte_t*)buffer, byteCount, m_stream, m_dataOffset, m_seekPoint);

    // 読んだ分だけファイルポインタを移動
	// ※テキスト形式の場合は、fread() が返すバイト数とシーク位置が異なるときがある。(CR+LF変換)
	//   そのため、validSize ではなく read_size でポインタを進めておく。
	//   例えばこう
	//   [ FileIO::File::getFileSize(fp) = 162 ]
	//   [ fread( buf, 1, FileIO::File::getFileSize(fp), fp ) = 157 ]
	//   [ ftell(fp) = 162 ]
	// → [2015/4/13] Archive 内部でのテキスト対応は無しにした。
	m_seekPoint += validSize;
	if (m_seekPoint > m_dataSize) {
		m_seekPoint = m_dataSize;
	}

    return validSize;
}

//------------------------------------------------------------------------------
void ArchiveStream::seek(int64_t offset, SeekOrigin origin)
{
	m_seekPoint = detail::FileSystemInternal::calcSeekPoint(m_seekPoint, m_dataSize, offset, origin);
}

//==============================================================================
// DummyArchive
//==============================================================================

//------------------------------------------------------------------------------
bool DummyArchive::existsFile(const PathName& fileFullPath)
{
	return FileSystem::existsFile(fileFullPath);
}

//------------------------------------------------------------------------------
bool DummyArchive::tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring)
{
	if (!FileSystem::existsFile(fileFullPath)) {
		return false;
	}

	Flags<FileOpenMode> mode = FileOpenMode::read;
	if (isDeferring) { mode |= FileOpenMode::Deferring; }
	Ref<FileStream> file = FileStream::create(fileFullPath.c_str(), mode);
	*outStream = file;
	return true;
}






namespace detail {

//==============================================================================
// DirectoryAssetsStorage
//==============================================================================

DirectoryAssetsStorage::DirectoryAssetsStorage(const PathName& directoryPath)
	: m_directoryFullPath(directoryPath.canonicalizePath())
{
}

DirectoryAssetsStorage::~DirectoryAssetsStorage()
{
}

bool DirectoryAssetsStorage::existsFile(const PathName& fileFullPath)
{
	LN_ASSERT(fileFullPath.isAbsolute());

	// fileFullPath が m_directoryFullPath 以下のファイルを指していること
	// (実際に存在していても、Lumino に登録されていない場所は除外したい)
	if (StringTraits::compare(
		m_directoryFullPath.c_str(), m_directoryFullPath.getLength(),
		fileFullPath.c_str(), fileFullPath.getLength(),
		m_directoryFullPath.getLength(), FileSystem::getFileSystemCaseSensitivity()) == 0)
	{
		return FileSystem::existsFile(fileFullPath);
	}
	else
	{
		return false;
	}
}

bool DirectoryAssetsStorage::tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring)
{
	if (!existsFile(fileFullPath))
	{
		return false;
	}

	Flags<FileOpenMode> mode = FileOpenMode::read;
	if (isDeferring)
	{
		mode |= FileOpenMode::Deferring;
	}

	Ref<FileStream> file = FileStream::create(fileFullPath.c_str(), mode);
	*outStream = file;
	return true;
}

//==============================================================================
// ArchiveManager
//==============================================================================

ArchiveManager::ArchiveManager()
	: m_fileAccessPriority(FileAccessPriority_DirectoryFirst)
	, m_installDirAssetsStorage(nullptr)
	, m_archiveList()
	, m_directoryArchiveList()
	, m_activeArchiveList()
{
}

ArchiveManager::~ArchiveManager()
{
}

void ArchiveManager::initialize(FileAccessPriority accessPriority)
{
	m_fileAccessPriority = accessPriority;

	auto storage = Ref<DummyArchive>::makeRef();
	m_directoryArchiveList.add(Ref<IAssetsStorage>::staticCast(storage));

	auto installDir = detail::EngineDomain::getEngineManager()->getInstallDir();
	if (!installDir.isEmpty())
	{
		PathName dir = installDir;
		dir.append(_T(LN_COMPILER_KEYWORD));
		dir.append(_T("Assets"));
		m_installDirAssetsStorage = Ref<DirectoryAssetsStorage>::makeRef(dir);
	}

	refreshArchiveList();
}

void ArchiveManager::dispose()
{
	m_archiveList.clear();
	m_directoryArchiveList.clear();
}

void ArchiveManager::registerArchive(const PathName& filePath, const String& password)
{
	auto archive = Ref<ArchiveFileAssetsStorage>::makeRef();
	archive->open(filePath, password);
	m_archiveList.add(archive);
	refreshArchiveList();
}

void ArchiveManager::addAssetsDirectory(const StringRef& directoryPath)
{
	auto storage = Ref<DirectoryAssetsStorage>::makeRef(directoryPath);
	m_directoryArchiveList.add(Ref<IAssetsStorage>::staticCast(storage));
	refreshArchiveList();
}

bool ArchiveManager::existsFile(const StringRef& filePath)
{
	for (IAssetsStorage* archive : m_activeArchiveList)
	{
		// TODO: PathName に SmallStringOptimaize を実装しよう
		PathName path(archive->getDirectoryPath(), filePath);
		if (archive->existsFile(path))
		{
			return true;
		}
	}
	return false;
}

Ref<Stream> ArchiveManager::createFileStream(const StringRef& filePath, bool isDeferring)
{
	Ref<Stream> stream;
	for (IAssetsStorage* archive : m_activeArchiveList)
	{
		// TODO: PathName に SmallStringOptimaize を実装しよう
		PathName path(archive->getDirectoryPath(), filePath);
		if (archive->tryCreateStream(path, &stream, isDeferring))
		{
			break;
		}
	}

	LN_ENSURE(stream != nullptr, "file not found: %s", filePath.getBegin());
	return stream;
}

// Assets フォルダとして登録されているフォルダの中の有効なファイルパスを取得する。
// ファイルがなければ空のパスを返す。
// これは、ローカルファイルからしかリソースアクセスできない (Stream 対応していない) API を使う場合の逃げ道。
PathName ArchiveManager::findLocalFilePath(const StringRef& filePath)
{
	for (IAssetsStorage* archive : m_directoryArchiveList)
	{
		// TODO: PathName に SmallStringOptimaize を実装しよう
		PathName path(archive->getDirectoryPath(), filePath);
		if (archive->existsFile(path))
		{
			return path;
		}
	}

	LN_ENSURE_FILE_NOT_FOUND(0, String(filePath).c_str());
	return PathName();
}

void ArchiveManager::refreshArchiveList()
{
	m_activeArchiveList.clear();

	switch (m_fileAccessPriority)
	{
	case ln::FileAccessPriority_ArchiveFirst:
		for (auto& ac : m_archiveList) m_activeArchiveList.add(ac);
		for (auto& ac : m_directoryArchiveList) m_activeArchiveList.add(ac);
		if (m_installDirAssetsStorage != nullptr) m_activeArchiveList.add(m_installDirAssetsStorage);
		break;
	case ln::FileAccessPriority_DirectoryFirst:
		for (auto& ac : m_directoryArchiveList) m_activeArchiveList.add(ac);
		for (auto& ac : m_archiveList) m_activeArchiveList.add(ac);
		if (m_installDirAssetsStorage != nullptr) m_activeArchiveList.add(m_installDirAssetsStorage);
		break;
	case ln::FileAccessPriority_ArchiveOnly:
		for (auto& ac : m_archiveList) m_activeArchiveList.add(ac);
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

} // namespace detail
LN_NAMESPACE_END
