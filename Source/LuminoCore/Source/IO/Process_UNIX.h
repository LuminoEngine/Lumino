#include "../Internal.h"
#include <sys/types.h> 
#include <sys/wait.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/IO/Process.h>

#if defined(LN_OS_MAC)
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#else
extern char **environ;
#endif

LN_NAMESPACE_BEGIN
namespace detail {
	
//==============================================================================
// ProcessImpl
//==============================================================================
class ProcessImpl
{
public:
	ProcessImpl();
	~ProcessImpl();
	
	void Start(const ProcessStartInfo& startInfo, ProcessStartResult* outResult);
	bool WaitForExit(int timeoutMSec);
	ProcessStatus GetState();
	int GetExitCode();
	void TryGetExitCode();
	void Dispose();
	
private:
	pid_t   m_pid;
	//InternalPipeStream*		m_stdinPipeStream;
	//InternalPipeStream*		m_stdoutPipeStream;
	//InternalPipeStream*		m_stderrPipeStream;
	int						m_exitCode;
	bool					m_crashed;
	bool					m_disposed;
};
	
	
//------------------------------------------------------------------------------
ProcessImpl::ProcessImpl()
	: m_pid(0)
{
}

//------------------------------------------------------------------------------
ProcessImpl::~ProcessImpl()
{
}

//------------------------------------------------------------------------------
void ProcessImpl::Start(const ProcessStartInfo& startInfo, ProcessStartResult* outResult)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// 子プロセス側はこの if に入る
		StringA utf8Path;
		utf8Path.AssignCStr(startInfo.program.c_str());
		
		StringA utf8Args = startInfo.args.ToStringA();
		List<StringA> argList = utf8Args.Split(" ");
		
		char** argv = new char *[argList.GetCount() + 2];
		argv[0] = ::strdup(utf8Path.c_str());     // 書き込み可能なポインタを渡さなければならないので strdup
		for (int i = 0; i < argList.GetCount(); ++i) {
			argv[i + 1] = ::strdup(argList[i].c_str());
		}
		argv[argList.GetCount() + 1] = NULL;
		
		execve(argv[0], argv, environ);
		
		// ここには execve が失敗したときだけ来る。
	}
	else
	{
		// 親プロセス側
	}

}

//------------------------------------------------------------------------------
bool ProcessImpl::WaitForExit(int timeoutMSec)
{
	ElapsedTimer timer;
	bool exit = false;
	do
	{
		int status;
		pid_t pid = waitpid(m_pid, &status, WNOHANG);	// WNOHANG: 状態変化が起こった子プロセスがない場合にすぐに復帰する。
		LN_THROW(pid >= 0, IOException);
		if (pid == 0) {
			// 状態変化していなかった。実行中。
		}
		else
		{
			if (WIFEXITED(status))
			{
				// 子プロセスが正常終了の場合
				m_exitCode = WEXITSTATUS(status); //子プロセスの終了コード
				exit = true;
			}
			else
			{
				// 子プロセスが異常終了の場合
			}
		}
		
		Thread::Sleep(1);
		
	} while(timeoutMSec == -1 || timer.GetElapsedTime() < timeoutMSec);
	
	if (exit)
	{
		// いろいろ閉じる
		Dispose();
	}
	else {
		// タイムアウトした
	}
	return true;
}

//------------------------------------------------------------------------------
ProcessStatus ProcessImpl::GetState()
{
	LN_NOTIMPLEMENTED();
	return ProcessStatus::Finished;
}

//------------------------------------------------------------------------------
int ProcessImpl::GetExitCode()
{
	return m_exitCode;
}

//------------------------------------------------------------------------------
void ProcessImpl::TryGetExitCode()
{
}

//------------------------------------------------------------------------------
void ProcessImpl::Dispose()
{
	if (!m_disposed)
	{
		m_disposed = true;
	}
}


} // namespace detail



#if 0
//=============================================================================
// Process
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Process::Start(const PathName& program, const String& args)
{}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Process::WaitForExit(int timeoutMSec)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ProcessStatus Process::GetState()
{
	// ※ http://linuxjm.osdn.jp/html/LDP_man-pages/man2/wait.2.html には wait を実行しないとゾンビ状態になる、とあるが、
	//    mono のソースを見る限りだと waitpid でも大丈夫なようだ。（TODO: 要動作確認）
	//	  wait は終了処理で実行しておく。
	
	int status;
	pid_t pid = waitpid(m_pid, &status, WNOHANG);	// WNOHANG: 状態変化が起こった子プロセスがない場合にすぐに復帰する。
	if (pid == 0) {
		// 状態変化していなかった。実行中。
		return ProcessStatus::Running;
	}
	
	// 正常終了
	if (WIFEXITED(status)) {
		// 終了コード
		m_exitCode = WEXITSTATUS(status);
		return ProcessStatus::Finished;
	}
	// 異常終了
	else {
		return ProcessStatus::Crashed;
	}
	//waitid
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Process::TryGetExitCode()
{
	//GetState();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Process::Dispose()
{
	if (!m_disposed)
	{
		m_disposed = true;
	}
}
//#endif
#endif

LN_NAMESPACE_END

