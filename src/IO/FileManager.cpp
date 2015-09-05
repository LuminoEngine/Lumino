
#include "../Internal.h"
#include "Archive.h"
#include <Lumino/IO/FileManager.h>

namespace Lumino
{

//=============================================================================
// FileManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FileManager& FileManager::GetInstance()
{
	static FileManager instance;
	return instance;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FileManager::FileManager()
	: m_fileAccessPriority(FileAccessPriority_DirectoryFirst)
	, m_dummyArchive(LN_NEW DummyArchive())
	, m_endRequested(false)
	, m_isASyncTaskListEmpty(true)
{
	m_archiveList.Add(m_dummyArchive);
	m_dummyArchive->AddRef();	// m_archiveList からの参照を示す

	m_asyncProcThread.Start(LN_CreateDelegate(this, &FileManager::Thread_ASyncProc));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FileManager::~FileManager()
{
	// 非同期処理のスレッドを停止する
	m_endRequested.SetTrue();
	m_asyncProcThread.Wait();

	LN_FOREACH(IArchive* a, m_archiveList) {
		a->Release();
	}
	m_archiveList.Clear();

	LN_SAFE_RELEASE(m_dummyArchive);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FileManager::RegisterArchive(const PathName& filePath, const String& password)
{
	Threading::MutexScopedLock lock(m_mutex);

	Archive* archive = LN_NEW Archive();
	archive->Open(filePath, password);
	m_archiveList.Add(archive);

	RefreshArchiveList();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool FileManager::ExistsFile(const char* filePath)
{
	return ExistsFile(PathName(filePath));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool FileManager::ExistsFile(const wchar_t* filePath)
{
	return ExistsFile(PathName(filePath));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool FileManager::ExistsFile(const PathName& filePath)
{
	LN_FOREACH(IArchive* archive, m_archiveList)
	{
		if (archive->ExistsFile(filePath)) {
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Stream* FileManager::CreateFileStream(const char* filePath)
{
	return CreateFileStream(PathName(filePath));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Stream* FileManager::CreateFileStream(const wchar_t* filePath)
{
	return CreateFileStream(PathName(filePath));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Stream* FileManager::CreateFileStream(const PathName& filePath)
{
	PathName absPath = filePath.CanonicalizePath();

	Stream* stream = NULL;
	LN_FOREACH(IArchive* archive, m_archiveList)
	{
		if (archive->TryCreateStream(absPath, &stream)) {
			break;
		}
	}

	LN_THROW(stream != NULL, FileNotFoundException, absPath);	// ファイルが見つからなかった
	return stream;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CaseSensitivity FileManager::GetFileSystemCaseSensitivity() const
{
#ifdef LN_OS_WIN32
	return CaseSensitivity_CaseInsensitive;
#else
	return CaseSensitivity_CaseSensitive;
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FileManager::RequestASyncTask(ASyncIOObject* task)
{
	if (m_endRequested.IsTrue()) {
		return;		// 終了要求が来ている場合は追加しない
	}

	Threading::MutexScopedLock lock(m_asyncTaskListMutex);

	// Idle 状態でなければ追加できない
	LN_THROW(task->m_ayncIOState == ASyncIOState_Idle, InvalidOperationException);

	task->m_ayncIOState = ASyncIOState_Ready;
	task->AddRef();
	m_asyncTaskList.Add(task);
	m_isASyncTaskListEmpty.SetFalse();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FileManager::WaitForAllASyncTask()
{
	m_isASyncTaskListEmpty.Wait();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FileManager::RefreshArchiveList()
{
	// 一度ダミーをリストから外す
	if (m_archiveList.Contains(m_dummyArchive))
	{
		m_archiveList.Remove(m_dummyArchive);
		m_dummyArchive->Release();
	}

	// ディレクトリ優先ならダミーを先頭に追加し直す
	if (m_fileAccessPriority == FileAccessPriority_DirectoryFirst) {
		m_archiveList.Insert(0, m_dummyArchive);
		m_dummyArchive->AddRef();
	}
	// アーカイブ優先ならダミーを末尾に追加し直す
	else if (m_fileAccessPriority == FileAccessPriority_ArchiveFirst) {
		m_archiveList.Add(m_dummyArchive);
		m_dummyArchive->AddRef();
	}
	// アーカイブのみであればダミーを追加する必要は無い
	else {
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FileManager::Thread_ASyncProc()
{
	// 終了フラグが ON でも、リストに何か残っていればすべて処理する
	while (m_endRequested.IsFalse() || m_isASyncTaskListEmpty.IsFalse())
	{
		// 読み込みリクエストを取り出す
		ASyncIOObject* task = NULL;
		{
			Threading::MutexScopedLock lock(m_asyncTaskListMutex);
			if (!m_asyncTaskList.IsEmpty())
			{
				task = m_asyncTaskList.GetFront();
				task->m_ayncIOState = ASyncIOState_Processing;	// 処理中状態にする
				m_asyncTaskList.RemoveAt(0);				// 先頭要素を削除する (Queue の方がよかったかも…？)
			}
		}

		// 読み込みリクエスト実行
		if (task)
		{
			try
			{
				task->OnASyncIOProc();							// 実行
				task->m_ayncIOState = ASyncIOState_Completed;		// 処理完了状態にする
			}
			catch (Exception& e)
			{
				task->m_ayncIOState = ASyncIOState_Failed;		// 処理失敗状態にする
				task->m_ayncIOException = e.Copy();				// メインスレッドがエラー内容を確認できるように例外を保持しておく
			}
			catch (...)
			{
				task->m_ayncIOState = ASyncIOState_Failed;		// 処理失敗状態にする
			}

			// 自動削除する場合は削除する
			//if (task->m_autoDelete) {
			//	delete task;
			//}
			task->Release();
		}

		// この時点でリストが空ならすべて処理が終わったことにする
		{
			Threading::MutexScopedLock lock(m_asyncTaskListMutex);
			if (m_asyncTaskList.IsEmpty())
			{
				m_isASyncTaskListEmpty.SetTrue();
			}
		}

		// 適当に待って、CPU 使用率が MAX にならないようにする
		Threading::Thread::Sleep(10);
	}
}

} // namespace Lumino
