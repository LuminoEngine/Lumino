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

class IArchive	// TODO: AssetStorage
	: public RefObject
{
protected:
	IArchive() {}
	virtual ~IArchive() {}

public:
	virtual const PathName& getDirectoryPath() const = 0;

	/**
		@brief	アーカイブ内に指定したパスのファイルが存在するか確認します。
	*/
	virtual bool existsFile(const PathName& fileFullPath) = 0;

	/**
		@brief	アーカイブ内のファイルを読み取るためのストリームを作成します。
	*/
	virtual bool tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring) = 0;

};

/**
	@brief	アーカイブファイルに読み取りアクセスするクラスです。
	@note	アーカイブファイル内のファイル名文字コードは UTF-16。
			ファイルマップに展開するときは環境依存の Char に変換される。
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
	void open(const PathName& filePath, const String& key);

	virtual const PathName& getDirectoryPath() const override { return m_virtualDirectoryPath; }

	/**
		@brief	アーカイブ内に指定したパスのファイルが存在するか確認します。
	*/
	virtual bool existsFile(const PathName& fileFullPath) override;

	/**
		@brief	アーカイブ内のファイルを読み取るためのストリームを作成します。
	*/
	virtual bool tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring) override;

private:

	/// ArchiveStream から呼ばれる
	size_t readArchiveStream(byte_t* buffer, size_t count, FILE* stream, uint64_t dataOffset, uint64_t seekPos);

	// 数値を 16 にそろえるために加算する数値「( 16 - ( v_ % 16 ) ) % 16」の最適化 ( 5 は 11、27 は 5 等 )
	int padding16(int v) const { return (v != 0) ? (16 - (v & 0x0000000f)) & 0x0000000f : 16; }
	uint32_t padding16(uint32_t v) const { return (v != 0) ? (16 - (v & 0x0000000f)) & 0x0000000f : 16; }

	/// パディングを考慮して整数を読み込む
	uint32_t readU32Padding16();

	/// パディングを考慮して整数を読み込む (ファイル名長さ、ファイルサイズ用)
	void readU32Padding16(uint32_t* v0, uint32_t* v1);

	/// パディングを考慮してデータを読み込む
	void readPadding16(byte_t* buffer, int count);

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
    Mutex				m_mutex;				///< readArchiveStream() をスレッドセーフにする
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
	virtual bool canRead() const { return true; }
	virtual bool canWrite() const { return false; }
	virtual int64_t getLength() const { return m_dataSize; }
	virtual int64_t getPosition() const { return m_seekPoint; }
	virtual size_t read(void* buffer, size_t byteCount);
	virtual void write(const void* data, size_t byteCount) {}
	virtual void seek(int64_t offset, SeekOrigin origin);
	virtual void flush() {}

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
	virtual const PathName& getDirectoryPath() const override { static PathName dummy; return dummy; }
	virtual bool existsFile(const PathName& fileFullPath) override;
	virtual bool tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring) override;
};



namespace detail {

class DirectoryAssetsStorage
	: public IArchive
{
public:
	DirectoryAssetsStorage(const PathName& directoryPath);
	virtual ~DirectoryAssetsStorage();
	virtual const PathName& getDirectoryPath() const override { return m_directoryFullPath; }
	virtual bool existsFile(const PathName& fileFullPath) override;
	virtual bool tryCreateStream(const PathName& fileFullPath, Ref<Stream>* outStream, bool isDeferring) override;

private:
	PathName	m_directoryFullPath;
};

class ArchiveManager
    : public RefObject
{
public:
	ArchiveManager();
	virtual ~ArchiveManager();
	void initialize(FileAccessPriority accessPriority);
	void dispose();

	void registerArchive(const PathName& filePath, const String& password);
	void addAssetsDirectory(const StringRef& directoryPath);
	bool existsFile(const StringRef& filePath);
	Ref<Stream> createFileStream(const StringRef& filePath, bool isDeferring);
	PathName findLocalFilePath(const StringRef& filePath);

private:
	void refreshArchiveList();

	FileAccessPriority		m_fileAccessPriority;
	Ref<IArchive>			m_installDirAssetsStorage;
	List<Ref<Archive>>		m_archiveList;
	List<Ref<IArchive>>		m_directoryArchiveList;
	List<IArchive*>			m_activeArchiveList;
};

} // namespace detail
LN_NAMESPACE_END
