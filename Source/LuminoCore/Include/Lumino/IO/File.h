/**
	@file	File.h
*/
#pragma once

#include "../Base/RefObject.h"
#include "../Base/String.h"
#include "PathName.h"
#include "FileStream.h"

LN_NAMESPACE_BEGIN

/**
	@brief		ファイル操作を行うクラスです。
*/
class File
	: public Stream
{
public:

	/**
		@brief		ファイル名を指定してインスタンスを初期化します。
		@details	このコンストラクタは、実際にファイルストリームを開きません。
					開いていない状態では Stream クラスの機能を使用することはできません。
					開くには、Open() を使用します。
	*/
	File(const String& filePath);
	
	/**
		@brief		ファイル名を指定してインスタンスを初期化します。
		@details	このコンストラクタは、実際にファイルストリームを開きません。
					開いていない状態では Stream クラスの機能を使用することはできません。
					開くには、Open() を使用します。
	*/
	//explicit File(const PathName& filePath);

	virtual ~File();

public:

	/**
		@brief		コンストラクタで指定されたファイルパスを使用してファイルストリームを開きます。
		@param[in]	mode		: ファイルを開く方法
		@param		openMode	: ファイルを開く方法 (FileOpenMode のフラグの組み合わせ)
	*/
	void Open(FileOpenMode openMode);

	/**
		@brief		開いているファイルストリームを閉じます。
		@details	デストラクタからも呼び出されます。
	*/
	void Close();

	/**
		@brief		ファイルの絶対パスを取得します。
	*/
	PathName GetFilePath() const;

	/**
		@brief		拡張子を含むファイルの名前を取得します。
		@code
					File f("C:\ConsoleApplication1\Program.cs");
					f.GetFileName();	// => "Program.cs"
		@endcode
	*/
	String GetFileName() const;

	/**
		@brief		現在のファイルサイズをバイト単位で取得します。
	*/
	//uint64_t GetLength() const;

public:
	// override Stream
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual int64_t GetLength() const;
	virtual int64_t GetPosition() const;
	virtual size_t Read(void* buffer, size_t byteCount);
	virtual void Write(const void* data, size_t byteCount);
	virtual void Seek(int64_t offset, SeekOrigin origin);
	virtual void Flush();

private:
	PathName		m_filePath;
	FileStreamPtr	m_fileStream;
};

class TemporaryFile
	: public File
{
public:
	TemporaryFile();
	~TemporaryFile();

	void Open();

	/** デストラクタで一時ファイルを自動削除するかを指定します。*/
	void SetAutoRemove(bool enabled) { m_autoRemove = enabled; }

	/** デストラクタで一時ファイルを自動削除するかを確認します。*/
	bool IsAutoRemove() const { return m_autoRemove; }

private:
	bool	m_autoRemove;
};

LN_NAMESPACE_END
