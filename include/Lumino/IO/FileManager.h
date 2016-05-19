/**
	@file	FileManager.h
*/
#pragma once

#include <Lumino/IO/Common.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/ASyncIOObject.h>
#include <Lumino/Threading/Thread.h>
#include <Lumino/Threading/Mutex.h>

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class FileManager
    : public RefObject
{
public:
	/// 初期化データ
	struct Settings
	{
		FileAccessPriority	AccessMode;     ///< ファイルへのアクセス優先度

		Settings()
			: AccessMode(FileAccessPriority_DirectoryFirst)
		{}
	};

public:

	/**
		@brief		アーカイブファイルを登録します。
		@details	以降このクラスの機能を通して、アーカイブファイルをフォルダであるかのように
					内部のファイルに読み取りアクセスすることができます。
	*/
	void RegisterArchive(const PathName& filePath, const String& password);

	/**
		@brief		指定されたファイルが存在するかどうかを確認します。
		@details	初期化時に指定したアクセス優先度に従い、登録されているアーカイブがあればその内部も確認します。
	*/
	bool ExistsFile(const char* filePath);
	bool ExistsFile(const wchar_t* filePath);	///< @overload ExistsFile
	bool ExistsFile(const PathName& filePath);	///< @overload ExistsFile

	/**
		@brief		読み取り専用モードでファイルストリームを開きます。
		@details	初期化時に指定したアクセス優先度に従い、登録されているアーカイブがあればその内部も確認します。
	*/
	Stream* CreateFileStream(const char* filePath, bool isDeferring = false);
	Stream* CreateFileStream(const wchar_t* filePath, bool isDeferring = false);	///< @overload CreateInFileStream
	Stream* CreateFileStream(const PathName& filePath, bool isDeferring = false);	///< @overload CreateInFileStream

	/**
		@brief		現在の環境のファイルシステムが、パス文字列の大文字と小文字を区別するかを確認します。
	*/
	CaseSensitivity GetFileSystemCaseSensitivity() const;

	/**
		@brief		非同期処理をリクエストします。
		@details	処理が完了するまで task を解放しないでください。
	*/
	void RequestASyncTask(ASyncIOObject* task);

	/**
		@brief		リクエストされているすべての非同期読み込み/書き込み処理の終了を待機します。
	*/
	void WaitForAllASyncTask();

LN_INTERNAL_ACCESS:
	FileManager(const Settings& settings);
	virtual ~FileManager();

	void RefreshArchiveList();
	void Thread_ASyncProc();

private:
	typedef Array<IArchive*>		ArchiveList;
	typedef Array<ASyncIOObject*>	ASyncIOObjectList;

	FileAccessPriority			m_fileAccessPriority;
	ArchiveList					m_archiveList;
	IArchive*					m_dummyArchive;
	Threading::Mutex			m_mutex;

	ASyncIOObjectList			m_asyncTaskList;
	Threading::Mutex			m_asyncTaskListMutex;
	Threading::ConditionFlag	m_endRequested;
	Threading::ConditionFlag	m_isASyncTaskListEmpty;
	Threading::DelegateThread	m_asyncProcThread;
};

LN_NAMESPACE_END
