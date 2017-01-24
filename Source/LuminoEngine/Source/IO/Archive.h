/**
	@file	Archive.h
*/
#pragma once

#include <map>
#include <string>
#include <Lumino/Threading/Mutex.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/PathName.h>

LN_NAMESPACE_BEGIN
class ArchiveStream;

class IArchive
	: public RefObject
{
protected:
	IArchive() {}
	virtual ~IArchive() {}

public:

	/**
		@brief	アーカイブ内に指定したパスのファイルが存在するか確認します。
	*/
	virtual bool ExistsFile(const PathName& fileFullPath) = 0;

	/**
		@brief	アーカイブ内のファイルを読み取るためのストリームを作成します。
	*/
	virtual bool TryCreateStream(const PathName& fileFullPath, RefPtr<Stream>* outStream, bool isDeferring) = 0;

};

/**
	@brief	アーカイブファイルに読み取りアクセスするクラスです。
	@note	アーカイブファイル内のファイル名文字コードは UTF-16。
			ファイルマップに展開するときは環境依存の TCHAR に変換される。
	@code
			// アーカイブファイル内に「Chara.png」「Map/Ground.png」というファイルがある場合…

			// 例 1 )
			// 実行ファイルと同じフォルダにあるアーカイブファイル "Image.lna" を使う
			archive.Open( "Image.lna" );

			archive.CreateStream( "Image/Chara.png" );
			archive.CreateStream( "Image/Map/Ground.png" );

			// 例 2 )
			// 実行ファイルと同じフォルダにある「Data」フォルダ内のアーカイブファイル "Image.lna" を使う
			archive.Open( "Data/Image.lna" );

			archive.CreateStream( "Data/Image/Chara.png" );
			archive.CreateStream( "Data/Image/Map/Ground.png" );
	@endcode
*/
class Archive
	: public IArchive
{
public:
	static const int Version_001 = 001;
	static const byte_t InternalKey[16];

public:
	Archive();
	virtual ~Archive();

public:

	/**
		@brief      アーカイブファイルを開いてアクセスの準備をします。
		@param[in]	filePath	: アーカイブファイルのパス
		@param[in]	key			: パスワード文字列
	*/
	void Open(const PathName& filePath, const String& key);

	/**
		@brief	アーカイブ内に指定したパスのファイルが存在するか確認します。
	*/
	virtual bool ExistsFile(const PathName& fileFullPath) override;

	/**
		@brief	アーカイブ内のファイルを読み取るためのストリームを作成します。
	*/
	virtual bool TryCreateStream(const PathName& fileFullPath, RefPtr<Stream>* outStream, bool isDeferring) override;

private:

	/// ArchiveStream から呼ばれる
	size_t ReadArchiveStream(byte_t* buffer, size_t count, FILE* stream, uint64_t dataOffset, uint64_t seekPos);

	// 数値を 16 にそろえるために加算する数値「( 16 - ( v_ % 16 ) ) % 16」の最適化 ( 5 は 11、27 は 5 等 )
	int Padding16(int v) const { return (v != 0) ? (16 - (v & 0x0000000f)) & 0x0000000f : 16; }
	uint32_t Padding16(uint32_t v) const { return (v != 0) ? (16 - (v & 0x0000000f)) & 0x0000000f : 16; }

	/// パディングを考慮して整数を読み込む
	uint32_t ReadU32Padding16();

	/// パディングを考慮して整数を読み込む (ファイル名長さ、ファイルサイズ用)
	void ReadU32Padding16(uint32_t* v0, uint32_t* v1);

	/// パディングを考慮してデータを読み込む
	void ReadPadding16(byte_t* buffer, int count);

private:

	/// ファイルひとつ分の情報
	struct Entry
	{
		uint32_t	Offset;		///< ストリーム先頭からファイルの位置までのオフセット
		uint32_t	Size;		///< ファイルサイズ
	};

	typedef std::map<PathName, Entry>	EntriesMap;
	typedef std::pair<PathName, Entry>	EntriesPair;

private:
	friend class ArchiveStream;

	// camellia key table type.
	static const int L_CAMELLIA_TABLE_BYTE_LEN = 272;
	static const int L_CAMELLIA_TABLE_WORD_LEN = (L_CAMELLIA_TABLE_BYTE_LEN / 4);
	typedef unsigned int KEY_TABLE_TYPE[L_CAMELLIA_TABLE_WORD_LEN];

    static const int KEY_SIZE = 128;

    PathName			m_virtualDirectoryPath;	///< アーカイブファイルをディレクトリに見立てた時の、そこまのパス ( [.lnaの親フルパス]/[拡張子を除いたアーカイブファイル名]/ )
	EntriesMap			m_entriesMap;		    ///< ファイル名に対応するファイル情報を格納する map
	FILE*				m_stream;			    ///< アーカイブファイルのストリーム
	int					m_fileCount;			///< アーカイブファイル内のファイル数
    String				m_key;				    ///< 復号キー (char)
	KEY_TABLE_TYPE		m_keyTable;
    Mutex				m_mutex;				///< ReadArchiveStream() をスレッドセーフにする
};

/**
	@brief	アーカイブからファイルを読むためのストリームクラスです。
*/
class ArchiveStream
    : public Stream
{
private:
	ArchiveStream(Archive* archive, FILE* stream, uint32_t dataOffset, uint32_t dataSize);
	virtual ~ArchiveStream();

public:
	virtual bool CanRead() const { return true; }
	virtual bool CanWrite() const { return false; }
	virtual int64_t GetLength() const { return m_dataSize; }
	virtual int64_t GetPosition() const { return m_seekPoint; }
	virtual size_t Read(void* buffer, size_t byteCount);
	virtual void Write(const void* data, size_t byteCount) {}
	virtual void Seek(int64_t offset, SeekOrigin origin);
	virtual void Flush() {}

private:
	friend class Archive;

    Archive*		m_archive;			///< このクラスを作成したアーカイブクラス
	FILE*			m_stream;			///< アーカイブ本体のファイルストリーム
	uint32_t		m_dataOffset;		///< ファイルの先頭からデータの先頭位置までのオフセット
	uint32_t		m_dataSize;			///< データサイズ
	int64_t			m_seekPoint;		///< シーク位置
};

/// FileManager の実装を簡易化するためのダミーアーカイブ (ディレクトリ直接参照)
class DummyArchive
	: public IArchive
{
public:
	virtual bool ExistsFile(const PathName& fileFullPath) override;
	virtual bool TryCreateStream(const PathName& fileFullPath, RefPtr<Stream>* outStream, bool isDeferring) override;
};

LN_NAMESPACE_END
