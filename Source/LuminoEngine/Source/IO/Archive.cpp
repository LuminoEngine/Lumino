/*	
	[2015/4/13] ファイル名マップのキーは UTF-16 統一？環境に合わせる？その②
		
		TCHAR に合わせる。
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
#include <Lumino/IO/Common.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Text/Encoding.h>
#include "ArchiveMaker.h"
#include "Archive.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Archive
//==============================================================================
	
const byte_t Archive::InternalKey[16] = {
	0x6e, 0x36, 0x38, 0x64, 0x35, 0x6f, 0x68, 0x6d,
	0x33, 0x42, 0x69, 0x61, 0x34, 0x78, 0x37, 0x6c
};

//------------------------------------------------------------------------------
Archive::Archive()
	: m_stream(NULL)
	, m_fileCount(0)
{
}

//------------------------------------------------------------------------------
Archive::~Archive()
{
	if (m_stream)
	{
		fclose(m_stream);
		m_stream = NULL;

		m_entriesMap.clear();
	}
}

//------------------------------------------------------------------------------
void Archive::open(const PathName& filePath, const String& key)
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
		StringA k = m_key.toStringA();
		byte_t key_buf[KEY_SIZE] = { 0 };
		memcpy(key_buf, k.c_str(), k.getLength());
		memset(m_keyTable, 0, sizeof(m_keyTable));
		Camellia_Ekeygen(KEY_SIZE, key_buf, m_keyTable);
    }

	// アーカイブファイルを開く
	errno_t err = _tfopen_s(&m_stream, filePath, _T("rb"));
	LN_THROW(err == 0, FileNotFoundException);

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
		LN_THROW(0, InvalidFormatException);
	}

	// 内部キーのチェック (ユーザーキーは本当に正しいか？)
	byte_t internalKey[16];
	readPadding16(internalKey, 16);
	if (memcmp(internalKey, Archive::InternalKey, 16) != 0) {
		LN_THROW(0, InvalidFormatException, "invalid archive key.");
	}

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
		String tmpName;
		tmpName.convertFrom(nameBuf.getData(), nameBuf.getSize(), Encoding::getUTF16Encoding());
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
bool Archive::existsFile(const PathName& fileFullPath)
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
		const TCHAR* internalPath = fileFullPath.c_str() + m_virtualDirectoryPath.getString().getLength();
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
bool Archive::tryCreateStream(const PathName& fileFullPath, RefPtr<Stream>* outStream, bool isDeferring)
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
	const TCHAR* internalPath = fileFullPath.c_str() + m_virtualDirectoryPath.getString().getLength();
	LN_THROW((*internalPath != _T('\0')), FileNotFoundException, fileFullPath);	// ファイル名が空だった

	EntriesMap::iterator itr = m_entriesMap.find(internalPath);
	LN_THROW((itr != m_entriesMap.end()), FileNotFoundException, fileFullPath);	// ファイルが見つからなかった
	
	return LN_NEW ArchiveStream(this, m_stream, itr->second.mOffset, itr->second.mSize);
#endif
}

//------------------------------------------------------------------------------
size_t Archive::readArchiveStream(byte_t* buffer, size_t count, FILE* stream, uint64_t dataOffset, uint64_t seekPos)
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
uint32_t Archive::readU32Padding16()
{
	uint32_t v0, v1;
	readU32Padding16(&v0, &v1);
	return v0;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Archive::readU32Padding16( uint32_t* v0, uint32_t* v1 )
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

//------------------------------------------------------------------------------
void Archive::readPadding16(byte_t* buffer, int count)
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
ArchiveStream::ArchiveStream(Archive* archive, FILE* stream, uint32_t dataOffset, uint32_t dataSize)
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
	m_seekPoint = FileSystem::calcSeekPoint(m_seekPoint, m_dataSize, offset, origin);
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
bool DummyArchive::tryCreateStream(const PathName& fileFullPath, RefPtr<Stream>* outStream, bool isDeferring)
{
	if (!FileSystem::existsFile(fileFullPath)) {
		return false;
	}

	FileOpenMode mode = FileOpenMode::read;
	if (isDeferring) { mode |= FileOpenMode::Deferring; }
	RefPtr<FileStream> file = FileStream::create(fileFullPath, mode);
	*outStream = file;
	return true;
}

LN_NAMESPACE_END
