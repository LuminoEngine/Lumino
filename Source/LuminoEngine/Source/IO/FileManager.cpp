
#include "../Internal.h"
#include "Archive.h"
#include <Lumino/IO/FileManager.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// FileManager
//==============================================================================

//------------------------------------------------------------------------------
//FileManager& FileManager::getInstance()
//{
//	static FileManager instance;
//	return instance;
//}
//


//------------------------------------------------------------------------------
FileManager::FileManager(const Settings& settings)
	: m_fileAccessPriority(settings.AccessMode)//FileAccessPriority_DirectoryFirst)
	, m_dummyArchive(LN_NEW DummyArchive())
	, m_endRequested(false)
	, m_isASyncTaskListEmpty(true)
{
	m_archiveList.add(m_dummyArchive);
	m_dummyArchive->addRef();	// m_archiveList からの参照を示す

	m_asyncProcThread.start(createDelegate(this, &FileManager::thread_ASyncProc));
}

//------------------------------------------------------------------------------
FileManager::~FileManager()
{
	// 非同期処理のスレッドを停止する
	m_endRequested.setTrue();
	m_asyncProcThread.wait();

	for (IArchive* a : m_archiveList) {
		a->release();
	}
	m_archiveList.clear();

	LN_SAFE_RELEASE(m_dummyArchive);
}

//------------------------------------------------------------------------------
void FileManager::registerArchive(const PathName& filePath, const String& password)
{
	MutexScopedLock lock(m_mutex);

	Archive* archive = LN_NEW Archive();
	archive->open(filePath, password);
	m_archiveList.add(archive);

	refreshArchiveList();
}

//------------------------------------------------------------------------------
bool FileManager::existsFile(const char* filePath)
{
	return existsFile(PathName(filePath));
}

//------------------------------------------------------------------------------
bool FileManager::existsFile(const wchar_t* filePath)
{
	return existsFile(PathName(filePath));
}

//------------------------------------------------------------------------------
bool FileManager::existsFile(const PathName& filePath)
{
	for (IArchive* archive : m_archiveList)
	{
		if (archive->existsFile(filePath)) {
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
Stream* FileManager::createFileStream(const char* filePath, bool isDeferring)
{
	return createFileStream(PathName(filePath), isDeferring);
}

//------------------------------------------------------------------------------
Stream* FileManager::createFileStream(const wchar_t* filePath, bool isDeferring)
{
	return createFileStream(PathName(filePath), isDeferring);
}

//------------------------------------------------------------------------------
// Note: isDeferring は今のところ Sound の遅延読み込み用のもの。
// ディクス上のファイルから FileStream を作るときに使用する。
//------------------------------------------------------------------------------
Stream* FileManager::createFileStream(const PathName& filePath, bool isDeferring)
{
	PathName absPath = filePath.canonicalizePath();

	RefPtr<Stream> stream;
	for (IArchive* archive : m_archiveList)
	{
		if (archive->tryCreateStream(absPath, &stream, isDeferring)) {
			break;
		}
	}

	LN_THROW(stream != NULL, FileNotFoundException, absPath);	// ファイルが見つからなかった
	return stream.detachMove();
}

//------------------------------------------------------------------------------
CaseSensitivity FileManager::getFileSystemCaseSensitivity() const
{
#ifdef LN_OS_WIN32
	return CaseSensitivity::CaseInsensitive;
#else
	return CaseSensitivity::CaseSensitive;
#endif
}

//------------------------------------------------------------------------------
void FileManager::requestASyncTask(ASyncIOObject* task)
{
	if (m_endRequested.isTrue()) {
		return;		// 終了要求が来ている場合は追加しない
	}

	MutexScopedLock lock(m_asyncTaskListMutex);

	// Idle 状態でなければ追加できない
	LN_THROW(task->m_ayncIOState == ASyncIOState_Idle, InvalidOperationException);

	task->m_ayncIOState = ASyncIOState_Ready;
	task->addRef();
	m_asyncTaskList.add(task);
	m_isASyncTaskListEmpty.setFalse();
}

//------------------------------------------------------------------------------
void FileManager::waitForAllASyncTask()
{
	m_isASyncTaskListEmpty.wait();
}

//------------------------------------------------------------------------------
void FileManager::refreshArchiveList()
{
	// 一度ダミーをリストから外す
	if (m_archiveList.contains(m_dummyArchive))
	{
		m_archiveList.remove(m_dummyArchive);
		m_dummyArchive->release();
	}

	// ディレクトリ優先ならダミーを先頭に追加し直す
	if (m_fileAccessPriority == FileAccessPriority_DirectoryFirst) {
		m_archiveList.insert(0, m_dummyArchive);
		m_dummyArchive->addRef();
	}
	// アーカイブ優先ならダミーを末尾に追加し直す
	else if (m_fileAccessPriority == FileAccessPriority_ArchiveFirst) {
		m_archiveList.add(m_dummyArchive);
		m_dummyArchive->addRef();
	}
	// アーカイブのみであればダミーを追加する必要は無い
	else {
	}
}

//------------------------------------------------------------------------------
void FileManager::thread_ASyncProc()
{
	// 終了フラグが ON でも、リストに何か残っていればすべて処理する
	while (m_endRequested.isFalse() || m_isASyncTaskListEmpty.isFalse())
	{
		// 読み込みリクエストを取り出す
		ASyncIOObject* task = NULL;
		{
			MutexScopedLock lock(m_asyncTaskListMutex);
			if (!m_asyncTaskList.isEmpty())
			{
				task = m_asyncTaskList.getFront();
				task->m_ayncIOState = ASyncIOState_Processing;	// 処理中状態にする
				m_asyncTaskList.removeAt(0);				// 先頭要素を削除する (Queue の方がよかったかも…？)
			}
		}

		// 読み込みリクエスト実行
		if (task)
		{
			try
			{
				task->onASyncIOProc();							// 実行
				task->m_ayncIOState = ASyncIOState_Completed;		// 処理完了状態にする
			}
			catch (Exception& e)
			{
				task->m_ayncIOState = ASyncIOState_Failed;		// 処理失敗状態にする
				task->m_ayncIOException = e.copy();				// メインスレッドがエラー内容を確認できるように例外を保持しておく
			}
			catch (...)
			{
				task->m_ayncIOState = ASyncIOState_Failed;		// 処理失敗状態にする
			}

			// 自動削除する場合は削除する
			//if (task->m_autoDelete) {
			//	delete task;
			//}
			task->release();
		}

		// この時点でリストが空ならすべて処理が終わったことにする
		{
			MutexScopedLock lock(m_asyncTaskListMutex);
			if (m_asyncTaskList.isEmpty())
			{
				m_isASyncTaskListEmpty.setTrue();
			}
		}

		// 適当に待って、CPU 使用率が MAX にならないようにする
		Thread::sleep(10);
	}
}

LN_NAMESPACE_END
