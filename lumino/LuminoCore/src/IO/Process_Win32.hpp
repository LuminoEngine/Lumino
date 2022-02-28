#include "Internal.hpp"
#include <LuminoCore/IO/Stream.hpp>
#include <LuminoCore/IO/Process.hpp>
#include <LuminoCore/Base/Platform.hpp>

namespace ln {
namespace detail {

/*
	Note:
		Pipe は初期値継承不可で作る。
		start 時に、子側に渡すハンドルを継承可能で複製する。
		こうするとリソース管理が少し簡単になる。

QProcess
----------
stdout/stderr の読み取りは別スレッドで行われているようだ。
ReadFile しているのは QWindowsPipeReader::startAsyncReadLocked()
*/

//==============================================================================
// PipeImpl

class PipeImpl : public Stream {
public:
    PipeImpl();
    virtual ~PipeImpl();

    void init();
    int readBytes(void* buffer, int length);
    int writeBytes(const void* buffer, int length);
    HANDLE readHandle() const { return m_readHandle; }
    HANDLE writeHandle() const { return m_writeHandle; }
    void closeRead();
    void closeWrite();
    void closeBoth();

    // Stream interface
    virtual bool canRead() const override { return true; }
    virtual bool canWrite() const override { return true; }
    virtual int64_t length() const override {
        LN_UNREACHABLE();
        return 0;
    }
    virtual int64_t position() const override {
        LN_UNREACHABLE();
        return 0;
    }
    virtual size_t read(void* buffer, size_t byteCount) override { return readBytes(buffer, byteCount); }
    virtual void write(const void* buffer, size_t byteCount) override { writeBytes(buffer, byteCount); }
    virtual void seek(int64_t offset, SeekOrigin origin) override { LN_UNREACHABLE(); }
    virtual void flush() override {}

private:
    HANDLE m_readHandle;
    HANDLE m_writeHandle;
};

PipeImpl::PipeImpl()
    : m_readHandle(INVALID_HANDLE_VALUE)
    , m_writeHandle(INVALID_HANDLE_VALUE) {
}

void PipeImpl::init() {
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    attr.bInheritHandle = FALSE;

    BOOL result = ::CreatePipe(&m_readHandle, &m_writeHandle, &attr, 0);
    if (LN_ENSURE(result)) return;
}

PipeImpl::~PipeImpl() {
    closeBoth();
}

int PipeImpl::writeBytes(const void* buffer, int length) {
    DWORD written = 0;
    BOOL result = ::WriteFile(m_writeHandle, buffer, length, &written, NULL);
    if (LN_ENSURE(result)) return 0;
    return written;
}

int PipeImpl::readBytes(void* buffer, int length) {
    DWORD bytesRead = 0;
    BOOL result = ::ReadFile(m_readHandle, buffer, length, &bytesRead, NULL);
    if (result || ::GetLastError() == ERROR_BROKEN_PIPE) {
        return bytesRead;
    }
    else {
        LN_ENSURE(0);
        return 0;
    }
}

void PipeImpl::closeRead() {
    if (m_readHandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_readHandle);
        m_readHandle = INVALID_HANDLE_VALUE;
    }
}

void PipeImpl::closeWrite() {
    if (m_writeHandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_writeHandle);
        m_writeHandle = INVALID_HANDLE_VALUE;
    }
}

void PipeImpl::closeBoth() {
    closeRead();
    closeWrite();
}

//==============================================================================
// ProcessImpl

class ProcessImpl : public RefObject {
public:
    ProcessImpl();
    virtual ~ProcessImpl();

    void start(const ProcessStartInfo& startInfo);
    void startWithShell(const ProcessStartInfo& startInfo);
    bool waitForExit(int timeoutMSec);
    ProcessStatus getStatus(int* outExitCode);

    void closeHandle();

    //void start(const ProcessStartInfo& startInfo);
    //bool waitForExit(int timeoutMSec);
    //ProcessStatus getState();
    //int getExitCode();
    //void TryGetExitCode();
    //void dispose();

private:
    PipeImpl* m_stdinPipe;
    PipeImpl* m_stdoutPipe;
    PipeImpl* m_stderrPipe;
    PROCESS_INFORMATION m_processInfo;
    //HANDLE					m_hInputRead;			// 標準出力の読み取り側 (子プロセス側)
    //HANDLE					m_hInputWrite;			// 標準出力の書き込み側 (このプロセス側)
    //HANDLE					m_hOutputRead;			// 標準出力の読み取り側 (このプロセス側)
    //HANDLE					m_hOutputWrite;			// 標準出力の書き込み側 (子プロセス側)
    //HANDLE					m_hErrorRead;			// 標準エラー出力の読み取り側 (このプロセス側)
    //HANDLE					m_hErrorWrite;			// 標準エラー出力の書き込み側 (子プロセス側)
    //PROCESS_INFORMATION		m_processInfo;

    //InternalPipeStream*		m_stdinPipeStream;
    //InternalPipeStream*		m_stdoutPipeStream;
    //InternalPipeStream*		m_stderrPipeStream;
    //int						m_exitCode;
    //bool					m_crashed;
    //bool					m_disposed;
};

ProcessImpl::ProcessImpl() {
    memset(&m_processInfo, 0, sizeof(m_processInfo));
}

ProcessImpl::~ProcessImpl() {
    closeHandle();
}

void ProcessImpl::start(const ProcessStartInfo& startInfo) {
    BOOL bResult;
    HANDLE hProcess = ::GetCurrentProcess();

    m_stdinPipe = startInfo.stdinPipe;
    m_stdoutPipe = startInfo.stdoutPipe;
    m_stderrPipe = startInfo.stderrPipe;

    STARTUPINFOW startupInfo;
    startupInfo.cb = sizeof(STARTUPINFOW);
    ::GetStartupInfoW(&startupInfo);
    startupInfo.lpReserved = NULL;
    startupInfo.lpDesktop = NULL;
    startupInfo.lpTitle = NULL;
    startupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK;
    startupInfo.cbReserved2 = 0;
    startupInfo.lpReserved2 = NULL;

    BOOL inheritHandles = FALSE;
    DWORD creationFlags = 0;

    // stdin redirect
    if (startInfo.stdinPipe) {
        ::DuplicateHandle(hProcess, startInfo.stdinPipe->readHandle(), hProcess, &startupInfo.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
        inheritHandles = TRUE;
        creationFlags |= CREATE_NO_WINDOW;
    }
    else if (::GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE) {
        ::DuplicateHandle(hProcess, ::GetStdHandle(STD_INPUT_HANDLE), hProcess, &startupInfo.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
        inheritHandles = TRUE;
    }
    else {
        startupInfo.hStdInput = NULL;
    }

    // stdout redirect
    if (startInfo.stdoutPipe) {
        ::DuplicateHandle(hProcess, startInfo.stdoutPipe->writeHandle(), hProcess, &startupInfo.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
        inheritHandles = TRUE;
        creationFlags |= CREATE_NO_WINDOW;
    }
    else if (::GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE) {
        ::DuplicateHandle(hProcess, ::GetStdHandle(STD_OUTPUT_HANDLE), hProcess, &startupInfo.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
        inheritHandles = TRUE;
    }
    else {
        startupInfo.hStdOutput = NULL;
    }

    // stderr redirect
    if (startInfo.stderrPipe) {
        ::DuplicateHandle(hProcess, startInfo.stderrPipe->writeHandle(), hProcess, &startupInfo.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
        inheritHandles = TRUE;
        creationFlags |= CREATE_NO_WINDOW;
    }
    else if (::GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE) {
        ::DuplicateHandle(hProcess, ::GetStdHandle(STD_ERROR_HANDLE), hProcess, &startupInfo.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
        inheritHandles = TRUE;
    }
    else {
        startupInfo.hStdError = 0;
    }

    // close duplicate source
    if (startInfo.stdinPipe) startInfo.stdinPipe->closeRead();
    if (startInfo.stdoutPipe) startInfo.stdoutPipe->closeWrite();
    if (startInfo.stderrPipe) startInfo.stderrPipe->closeWrite();

    if (inheritHandles) {
        startupInfo.dwFlags = STARTF_USESTDHANDLES;
    }

    // make args
    std::wstring program = startInfo.program.str().toStdWString();
    std::wstring cmdArgs = program;
    for (auto& a : startInfo.args) {
        cmdArgs.append(L" ");
        cmdArgs.append(a.toStdWString());
    }

    // make working directory
    std::wstring workingDirectory;
    if (!startInfo.workingDirectory.isEmpty()) {
        workingDirectory = startInfo.workingDirectory.str().toStdWString();
    }

    // start process
    memset(&m_processInfo, 0, sizeof(m_processInfo));
    bResult = ::CreateProcessW(
        NULL,
        const_cast<wchar_t*>(cmdArgs.c_str()),
        NULL,
        NULL,
        inheritHandles,
        creationFlags, //CREATE_NO_WINDOW/* | DETACHED_PROCESS*/,
        NULL,
        (workingDirectory.empty()) ? NULL : workingDirectory.c_str(),
        &startupInfo,
        &m_processInfo);
    if (!bResult) {
        DWORD dwErr = ::GetLastError();
        if (dwErr == ERROR_FILE_NOT_FOUND) {
            LN_ENSURE(0, program.c_str());
            return;
        }
        LN_ENSURE(0, Win32Helper::getWin32ErrorMessage(dwErr));
        return;
    }

    //
    //if (startInfo.stdinPipe) {
    //	startInfo.stdinPipe->closeRead();
    //}
    //if (startInfo.stdoutPipe) {
    //	startInfo.stdoutPipe->closeWrite();
    //}
    //if (startInfo.stderrPipe) {
    //	startInfo.stderrPipe->closeWrite();
    //}

    ::CloseHandle(m_processInfo.hThread);
    if (startupInfo.hStdInput) CloseHandle(startupInfo.hStdInput);
    if (startupInfo.hStdOutput) CloseHandle(startupInfo.hStdOutput);
    if (startupInfo.hStdError) CloseHandle(startupInfo.hStdError);
}

void ProcessImpl::startWithShell(const ProcessStartInfo& startInfo) {
    SHELLEXECUTEINFOW shellExecuteInfo = { 0 };
    shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
    shellExecuteInfo.fMask |= SEE_MASK_NOCLOSEPROCESS;
    shellExecuteInfo.fMask |= SEE_MASK_FLAG_NO_UI;
    shellExecuteInfo.fMask |= SEE_MASK_FLAG_DDEWAIT;

    if (0 /*ウィンドウ非表示*/) {
        shellExecuteInfo.nShow = SW_HIDE;
    }
    else {
        shellExecuteInfo.nShow = SW_SHOWNORMAL;
    }

    std::wstring fileName;
    if (!startInfo.program.isEmpty()) {
        fileName = startInfo.program.str().toStdWString();
        shellExecuteInfo.lpFile = fileName.c_str();
    }

    std::wstring arguments;
    if (!startInfo.args.empty()) {
        for (auto& a : startInfo.args) {
            arguments.append(L" ");
            arguments.append(a.toStdWString());
        }
        shellExecuteInfo.lpParameters = arguments.c_str();
    }

    std::wstring workingDirectory;
    if (!startInfo.workingDirectory.isEmpty()) {
        workingDirectory = startInfo.workingDirectory.str().toStdWString();
        shellExecuteInfo.lpDirectory = workingDirectory.c_str();
    }

    if (!::ShellExecuteExW(&shellExecuteInfo)) {
        LN_ENSURE(0, Win32Helper::getWin32ErrorMessage(::GetLastError()));
    }

    // TODO: ファイルが見つからない場合でも、TRUE で戻ってくる。
    // その場合、hProcess が null になっている。

    memset(&m_processInfo, 0, sizeof(m_processInfo));

    m_processInfo.hProcess = shellExecuteInfo.hProcess;
}

bool ProcessImpl::waitForExit(int timeoutMSec) {
    if (m_processInfo.hProcess) {
        DWORD r = ::WaitForSingleObject(m_processInfo.hProcess, (timeoutMSec < 0) ? INFINITE : timeoutMSec);
        if (r == WAIT_TIMEOUT) {
            return false; // timeout (running)
        }
    }
    //if (m_stdoutPipe) {
    //    OVERLAPPED ovl;
    //    memset(&ovl, 0, sizeof(OVERLAPPED));
    //    DWORD dwWritten;
    //    ::GetOverlappedResult(m_stdoutPipe->readHandle(), &ovl, &dwWritten, TRUE);
    //}

    return true;
}

ProcessStatus ProcessImpl::getStatus(int* outExitCode) {
    if (outExitCode) *outExitCode = 1;

    if (m_processInfo.hProcess) {
        if (::WaitForSingleObject(m_processInfo.hProcess, 0) == WAIT_TIMEOUT) {
            return ProcessStatus::Running;
        }

        DWORD exitCode;
        if (!::GetExitCodeProcess(m_processInfo.hProcess, &exitCode)) {
            return ProcessStatus::Crashed;
        }
        if (outExitCode) *outExitCode = (int)exitCode;

        // クラッシュを確実に検出するのは難しい。
        // 現実的な方法としては、GetExitCodeProcess() は未処理例外の例外コードを返すのでそれをチェックすること。
        // https://social.msdn.microsoft.com/Forums/en-US/7e0746ab-d285-4061-9032-81400875243a/detecting-if-a-child-process-crashed
        bool crashed = (exitCode >= 0x80000000 && exitCode < 0xD0000000);

        return (crashed) ? ProcessStatus::Crashed : ProcessStatus::Finished;
    }
    else {
        return ProcessStatus::Finished;
    }
}

void ProcessImpl::closeHandle() {
    if (m_processInfo.hProcess) {
        ::CloseHandle(m_processInfo.hProcess);
        m_processInfo.hProcess = NULL;
    }
}

//
//
//
//
//
//
//class InternalPipeStream
//	: public Stream
//{
//public:
//	enum Side
//	{
//		ReadSide = 0,
//		WriteSide,
//	};
//
//public:
//	InternalPipeStream(Side side, HANDLE hPipe)
//		: m_side(side)
//		, m_hPipe(hPipe)	// クローズは Process クラスに任せる
//	{}
//
//public:
//	virtual bool canRead() const { return (m_side == ReadSide); }
//	virtual bool canWrite() const { return (m_side == WriteSide); }
//	virtual int64_t getLength() const { LN_UNREACHABLE(); return 0; }
//	virtual int64_t getPosition() const { LN_UNREACHABLE(); return 0; }
//	virtual size_t read(void* buffer, size_t byteCount)
//	{
//		if (LN_REQUIRE(m_side == ReadSide)) return 0;
//
//		DWORD bytesRead = 0;
//
//		// この行でブロックされる。
//		// ログが溜まったり、子プロセスが終了すると動き出す。
//		if (!::ReadFile(m_hPipe, buffer, (DWORD)byteCount, &bytesRead, NULL) || !bytesRead)
//		{
//			if (::GetLastError() == ERROR_BROKEN_PIPE) {
//				return 0;	// 子プロセスが終了したか、パイプが壊れている。継続不可
//			}
//		}
//		return bytesRead;
//	}
//	virtual void write(const void* data, size_t byteCount)
//	{
//		if (LN_REQUIRE(m_side == WriteSide)) return;
//
//		DWORD bytesWrite = 0;
//		BOOL bRes = ::WriteFile(m_hPipe, data, (DWORD)byteCount, &bytesWrite, NULL);
//		if (LN_ENSURE(bRes != FALSE, Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
//	}
//	virtual void seek(int64_t offset, SeekOrigin origin) { LN_UNREACHABLE(); }
//	virtual void flush() {}
//
//private:
//	Side	m_side;
//	HANDLE	m_hPipe;
//};
//
////==============================================================================
//// ProcessImpl
////==============================================================================
//
//class ProcessImpl
//{
//public:
//	ProcessImpl();
//	~ProcessImpl();
//
//	void start(const ProcessStartInfo& startInfo, ProcessStartResult* outResult);
//	bool waitForExit(int timeoutMSec);
//	ProcessStatus getState();
//	int getExitCode();
//	void TryGetExitCode();
//	void dispose();
//
//private:
//	HANDLE					m_hInputRead;			// 標準出力の読み取り側 (子プロセス側)
//	HANDLE					m_hInputWrite;			// 標準出力の書き込み側 (このプロセス側)
//	HANDLE					m_hOutputRead;			// 標準出力の読み取り側 (このプロセス側)
//	HANDLE					m_hOutputWrite;			// 標準出力の書き込み側 (子プロセス側)
//	HANDLE					m_hErrorRead;			// 標準エラー出力の読み取り側 (このプロセス側)
//	HANDLE					m_hErrorWrite;			// 標準エラー出力の書き込み側 (子プロセス側)
//	PROCESS_INFORMATION		m_processInfo;
//
//	InternalPipeStream*		m_stdinPipeStream;
//	InternalPipeStream*		m_stdoutPipeStream;
//	InternalPipeStream*		m_stderrPipeStream;
//	int						m_exitCode;
//	bool					m_crashed;
//	bool					m_disposed;
//};
//
////------------------------------------------------------------------------------
//ProcessImpl::ProcessImpl()
//	: m_hInputRead(nullptr)
//	, m_hInputWrite(nullptr)
//	, m_hOutputRead(nullptr)
//	, m_hOutputWrite(nullptr)
//	, m_hErrorRead(nullptr)
//	, m_hErrorWrite(nullptr)
//	, m_stdinPipeStream(nullptr)
//	, m_stdoutPipeStream(nullptr)
//	, m_stderrPipeStream(nullptr)
//	, m_exitCode(0)
//	, m_crashed(false)
//	, m_disposed(false)
//{
//	memset(&m_processInfo, 0, sizeof(m_processInfo));
//}
//
////------------------------------------------------------------------------------
//ProcessImpl::~ProcessImpl()
//{
//}
//
////------------------------------------------------------------------------------
//void ProcessImpl::start(const ProcessStartInfo& startInfo, ProcessStartResult* outResult)
//{
//	enum { R = 0, W = 1 };
//	BOOL bResult;
//	HANDLE hProcess = ::GetCurrentProcess();
//
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//
//	/*
//		パイプの継承設定を行う理由
//		http://www.ne.jp/asahi/hishidama/home/tech/c/windows/CreatePipe.html
//	*/
//
//	// 標準入力のパイプを作る
//	if (startInfo.redirectStandardInput)
//	{
//		HANDLE hPipe[2] = { 0, 0 };
//		bResult = ::CreatePipe(&hPipe[R], &hPipe[W], &sa, 0);
//		if (LN_ENSURE(bResult != FALSE, Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
//
//		// パイプのこのプロセス側を非継承で複製する
//		if (!::DuplicateHandle(hProcess, hPipe[W], hProcess, &m_hInputWrite, 0, FALSE, DUPLICATE_SAME_ACCESS))
//		{
//			DWORD dwErr = ::GetLastError();
//			::CloseHandle(hPipe[R]);
//			::CloseHandle(hPipe[W]);
//			LN_ENSURE(0, Win32Helper::getWin32ErrorMessage(dwErr));
//			return;
//		}
//		::CloseHandle(hPipe[W]);
//		m_hInputRead = hPipe[R];
//
//		// 標準出力の Writer を作る
//		m_stdinPipeStream = LN_NEW InternalPipeStream(InternalPipeStream::WriteSide, m_hInputWrite);
//		outResult->standardInputWriter.attach(LN_NEW StreamWriter(m_stdinPipeStream, startInfo.standardInputEncoding));
//	}
//
//	// 標準出力のパイプを作る
//	if (startInfo.redirectStandardOutput)
//	{
//		HANDLE hPipe[2] = { 0, 0 };
//		bResult = ::CreatePipe(&hPipe[R], &hPipe[W], &sa, 0);
//		if (LN_ENSURE(bResult != FALSE, Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
//
//		// パイプのこのプロセス側を非継承で複製する
//		if (!::DuplicateHandle(hProcess, hPipe[R], hProcess, &m_hOutputRead, 0, FALSE, DUPLICATE_SAME_ACCESS))
//		{
//			DWORD dwErr = ::GetLastError();
//			::CloseHandle(hPipe[R]);
//			::CloseHandle(hPipe[W]);
//			LN_ENSURE(0, Win32Helper::getWin32ErrorMessage(dwErr));
//			return;
//		}
//		::CloseHandle(hPipe[R]);
//		m_hOutputWrite = hPipe[W];
//
//		// 標準出力の Reader を作る
//		m_stdoutPipeStream = LN_NEW InternalPipeStream(InternalPipeStream::ReadSide, m_hOutputRead);
//		outResult->standardOutputReader.attach(LN_NEW StreamReader(m_stdoutPipeStream, startInfo.standardOutputEncoding));
//	}
//
//	// 標準エラー出力のパイプを作る
//	if (startInfo.redirectStandardError)
//	{
//		HANDLE hPipe[2] = { 0, 0 };
//		bResult = ::CreatePipe(&hPipe[R], &hPipe[W], &sa, 0);
//		if (LN_ENSURE(bResult != FALSE, Win32Helper::getWin32ErrorMessage(::GetLastError()))) return;
//
//		// パイプのこのプロセス側を非継承で複製する
//		if (!::DuplicateHandle(hProcess, hPipe[R], hProcess, &m_hErrorRead, 0, FALSE, DUPLICATE_SAME_ACCESS))
//		{
//			DWORD dwErr = ::GetLastError();
//			::CloseHandle(hPipe[R]);
//			::CloseHandle(hPipe[W]);
//			LN_ENSURE(0, Win32Helper::getWin32ErrorMessage(dwErr));
//			return;
//		}
//		::CloseHandle(hPipe[R]);
//		m_hErrorWrite = hPipe[W];
//
//		// 標準出力の Reader を作る
//		m_stderrPipeStream = LN_NEW InternalPipeStream(InternalPipeStream::ReadSide, m_hErrorRead);
//		outResult->standardErrorReader.attach(LN_NEW StreamReader(m_stderrPipeStream, startInfo.standardErrorEncoding));
//	}
//
//	// 子プロセスの標準出力の出力先を↑で作ったパイプにする
//	STARTUPINFO si;
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(si);
//	si.dwFlags = STARTF_USESTDHANDLES;
//	si.hStdInput = m_hInputRead;			// 子プロセスの標準入力はここから読み取る
//	si.hStdOutput = m_hOutputWrite;		// 子プロセスの標準出力はここへ
//	si.hStdError = m_hErrorWrite;		// 子プロセスの標準エラーはここへ
//	si.wShowWindow = SW_HIDE;
//
//	// exe 名と引数を連結してコマンドライン文字列を作る
//	std::wstring program = startInfo.program.getString().toStdWString();
//	std::wstring cmdArgs = program;
//	if (!startInfo.args.isEmpty())
//	{
//		cmdArgs += L" ";
//		cmdArgs += startInfo.args.toStdWString();
//	}
//
//	// カレントディレクトリ
//	std::wstring currentDirectory;
//	if (!startInfo.workingDirectory.isEmpty())
//	{
//		currentDirectory = startInfo.workingDirectory.getString().toStdWString();
//	}
//
//	// 子プロセス開始
//	memset(&m_processInfo, 0, sizeof(m_processInfo));
//	bResult = ::CreateProcess(
//		NULL, (LPTSTR)(LPCTSTR)cmdArgs.c_str(), NULL, NULL, TRUE,
//		CREATE_NO_WINDOW, NULL, (currentDirectory.empty()) ? NULL : currentDirectory.c_str(), &si, &m_processInfo);
//	if (bResult == FALSE)
//	{
//		DWORD dwErr = ::GetLastError();
//		if (dwErr == ERROR_FILE_NOT_FOUND) {
//			LN_ENSURE_FILE_NOT_FOUND(0, program.c_str());
//			return;
//		}
//		LN_ENSURE(0, Win32Helper::getWin32ErrorMessage(dwErr));
//		return;
//	}
//
//	// 子プロセスのスレッドハンドルは不必要なのでクローズしてしまう
//	::CloseHandle(m_processInfo.hThread);
//
//	// 子プロセス側は全てクローズしてしまう。
//	// ※実際に行われるのは参照カウントのデクリメント。
//	//   子側は終了したときに自動的に参照が外れるが、
//	//   親側は自分で Close しておかないとリソースリークになる。
//	//   また、例えば stdout の親側は ReadFile() で永遠にブロックしてしまう。
//	if (m_hInputRead != NULL) {
//		::CloseHandle(m_hInputRead);
//		m_hInputRead = NULL;
//	}
//	if (m_hOutputWrite != NULL) {
//		::CloseHandle(m_hOutputWrite);
//		m_hOutputWrite = NULL;
//	}
//	if (m_hErrorWrite != NULL) {
//		::CloseHandle(m_hErrorWrite);
//		m_hErrorWrite = NULL;
//	}
//}
//
////------------------------------------------------------------------------------
//bool ProcessImpl::waitForExit(int timeoutMSec)
//{
//	if (m_processInfo.hProcess != NULL)
//	{
//		// TODO:ここでやるべき？とりあえず暫定。
//		//if (m_standardOutputExternalStream != nullptr && m_stdoutPipeStream != nullptr)
//		//{
//		//	byte_t buf[1024];
//		//	size_t size;
//		//	while ((size = m_stdoutPipeStream->Read(buf, 1024)) > 0)
//		//	{
//		//		m_standardOutputExternalStream->Write(buf, size);
//		//	}
//		//}
//
//
//
//		// 終了した場合は制御を返し、WAIT_OBJECT_0 が返ってくる
//		DWORD r = ::WaitForSingleObject(m_processInfo.hProcess, (timeoutMSec < 0) ? INFINITE : timeoutMSec);
//		if (r == WAIT_TIMEOUT) {
//			return false;	// タイムアウト
//		}
//	}
//
//	// いろいろ閉じる
//	dispose();
//	return true;
//}
//
////------------------------------------------------------------------------------
//ProcessStatus ProcessImpl::getState()
//{
//	if (::WaitForSingleObject(m_processInfo.hProcess, 0) == WAIT_OBJECT_0) {
//		return ProcessStatus::Running;
//	}
//	TryGetExitCode();
//	return (m_crashed) ? ProcessStatus::Crashed : ProcessStatus::Finished;
//}
//
////------------------------------------------------------------------------------
//int ProcessImpl::getExitCode()
//{
//	TryGetExitCode();
//	return m_exitCode;
//}
//
////------------------------------------------------------------------------------
//void ProcessImpl::TryGetExitCode()
//{
//	if (m_processInfo.hProcess != NULL)
//	{
//		DWORD exitCode;
//		if (!::GetExitCodeProcess(m_processInfo.hProcess, &exitCode)) {
//			return;
//		}
//		m_exitCode = (int)exitCode;
//
//		// クラッシュを確実に検出するのは難しい。
//		// 現実的な方法としては、GetExitCodeProcess() は未処理例外の例外コードを返すのでそれをチェックすること。
//		// https://social.msdn.microsoft.com/Forums/en-US/7e0746ab-d285-4061-9032-81400875243a/detecting-if-a-child-process-crashed
//		m_crashed = (exitCode >= 0x80000000 && exitCode < 0xD0000000);
//	}
//}
//
////------------------------------------------------------------------------------
//void ProcessImpl::dispose()
//{
//	if (!m_disposed)
//	{
//		// 終了コードを覚えておく (閉じた後は取得できない)
//		TryGetExitCode();
//
//		// 子プロセスのハンドルを閉じる (子プロセスが終了するわけではないので注意)
//		if (m_processInfo.hProcess != NULL)
//		{
//			::CloseHandle(m_processInfo.hProcess);
//			m_processInfo.hProcess = NULL;
//		}
//
//		LN_SAFE_RELEASE(m_stdinPipeStream);
//		LN_SAFE_RELEASE(m_stdoutPipeStream);
//		LN_SAFE_RELEASE(m_stderrPipeStream);
//
//		// 書き込み側ハンドルは、WaitForSingleObject() の前でクローズしておく。
//		// こうしておかないと、子プロセスの ReadFile() がブロックし続けてしまい、
//		// スレッドが終了できなくなる。
//
//		// パイプを閉じる
//		if (m_hInputWrite != NULL) {
//			::CloseHandle(m_hInputWrite);
//			m_hInputWrite = NULL;
//		}
//		if (m_hOutputRead != NULL) {
//			::CloseHandle(m_hOutputRead);
//			m_hOutputRead = NULL;
//		}
//		if (m_hErrorRead != NULL) {
//			::CloseHandle(m_hErrorRead);
//			m_hErrorRead = NULL;
//		}
//
//		if (m_hInputRead != NULL) {
//			::CloseHandle(m_hInputRead);
//			m_hInputRead = NULL;
//		}
//		if (m_hOutputWrite != NULL) {
//			::CloseHandle(m_hOutputWrite);
//			m_hOutputWrite = NULL;
//		}
//		if (m_hErrorWrite != NULL) {
//			::CloseHandle(m_hErrorWrite);
//			m_hErrorWrite = NULL;
//		}
//
//		m_disposed = true;
//	}
//}

} // namespace detail
} // namespace ln
