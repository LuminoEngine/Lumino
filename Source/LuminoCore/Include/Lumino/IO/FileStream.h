#pragma once

#include "Common.h"
#include "Stream.h"
#include "PathName.h"

LN_NAMESPACE_BEGIN
class FileStream;
typedef RefPtr<FileStream>	FileStreamPtr;

/**
	@brief	ファイル用ストリーム
*/
class FileStream
    : public Stream
{
public:

	/**
		@brief	ファイルをバイナリモードで開く
		@param	filePath	: ファイル名
		@param	openMode	: ファイルを開く方法 (FileOpenMode のフラグの組み合わせ)
	*/
	static FileStreamPtr Create(const TCHAR* filePath, FileOpenMode openMode);

	/**
		@brief	ファイルを閉じる
	*/
	void Close();

	/** ファイルの絶対パスを取得します。*/
	const PathName& GetFilePath() const { return m_filePath; }

public:
	// override
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual int64_t GetLength() const;
	virtual int64_t GetPosition() const;
	virtual size_t Read(void* buffer, size_t byteCount);
	virtual void Write(const void* data, size_t byteCount);
	virtual void Seek(int64_t offset, SeekOrigin origin);
	virtual void Flush();

private:
	FileStream();
	virtual ~FileStream();
	void Open(const TCHAR* filePath, FileOpenMode openMode);
	void CheckOpen() const;
	void Open() const;

private:
	mutable FILE*	m_stream;
	PathName		m_filePath;
	FileOpenMode	m_openModeFlags;
};

LN_NAMESPACE_END
