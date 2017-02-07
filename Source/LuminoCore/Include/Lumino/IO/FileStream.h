#pragma once

#include "Common.h"
#include "Stream.h"
#include "PathName.h"

LN_NAMESPACE_BEGIN

/**
	@brief	ファイル用ストリーム
*/
template<typename TChar>
class GenericFileStream
    : public Stream
{
public:

	/**
		@brief	ファイルを開きます。
		@param	filePath	: ファイル名
		@param	openMode	: ファイルを開く方法 (FileOpenMode のフラグの組み合わせ)
	*/
	static RefPtr<GenericFileStream<TChar>> Create(const TChar* filePath, FileOpenMode openMode);

	/** ファイルを閉じます。*/
	void Close();

	/** ファイルの絶対パスを取得します。*/
	const GenericPathName<TChar>& GetFilePath() const { return m_filePath; }

public:
	// Stream interface
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual int64_t GetLength() const;
	virtual int64_t GetPosition() const;
	virtual size_t Read(void* buffer, size_t byteCount);
	virtual void Write(const void* data, size_t byteCount);
	virtual void Seek(int64_t offset, SeekOrigin origin);
	virtual void Flush();

private:
	GenericFileStream();
	virtual ~GenericFileStream();
	void Open(const TChar* filePath, FileOpenMode openMode);
	void CheckOpen() const;
	void Open() const;

private:
	mutable FILE*			m_stream;
	GenericPathName<TChar>	m_filePath;
	FileOpenMode			m_openModeFlags;
};

using FileStream = GenericFileStream<TCHAR>;

LN_NAMESPACE_END
