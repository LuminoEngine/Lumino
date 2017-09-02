#pragma once

#include "Common.h"
#include "Stream.h"
#include "PathName.h"

LN_NAMESPACE_BEGIN

/**
	@brief	ファイル用ストリーム
*/
class FileStream
    : public Stream
{
public:

	/**
		@brief	ファイルを開きます。
		@param	filePath	: ファイル名
		@param	openMode	: ファイルを開く方法 (FileOpenMode のフラグの組み合わせ)
	*/
	static Ref<FileStream> create(const Char* filePath, FileOpenMode openMode);

	/** ファイルを閉じます。*/
	void close();

	/** ファイルの絶対パスを取得します。*/
	const PathName& getFilePath() const { return m_filePath; }

public:
	// Stream interface
	virtual bool canRead() const;
	virtual bool canWrite() const;
	virtual int64_t getLength() const;
	virtual int64_t getPosition() const;
	virtual size_t read(void* buffer, size_t byteCount);
	virtual void write(const void* data, size_t byteCount);
	virtual void seek(int64_t offset, SeekOrigin origin);
	virtual void flush();

private:
	FileStream();
	virtual ~FileStream();
	void open(const Char* filePath, FileOpenMode openMode);
	void checkOpen() const;
	void open() const;

private:
	mutable FILE*	m_stream;
	PathName		m_filePath;
	FileOpenMode	m_openModeFlags;
};

LN_NAMESPACE_END
