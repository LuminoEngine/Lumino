//#include "Internal.hpp"
#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <chrono>
#include <LuminoCore/IO/Stream.hpp>
#include <LuminoCore/IO/Process.hpp>

#if defined(LN_OS_MAC)
//#include <crt_externs.h>
//#define environ (*_NSGetEnviron())
extern char **environ;
#else
extern char **environ;
#endif

namespace ln {
namespace detail {

//==============================================================================
// PipeImpl

class PipeImpl : public Stream
{
public:
	PipeImpl();
	virtual ~PipeImpl();

	void init();
	int readBytes(void* buffer, int length);
	int writeBytes(const void* buffer, int length);
	int readFD() const { return m_readFD; }
	int writeFD() const { return m_writeFD; }
	void closeRead();
	void closeWrite();
	void closeBoth();

	// Stream interface
	virtual bool canRead() const override { return true; }
	virtual bool canWrite() const override { return true; }
	virtual int64_t length() const override { LN_UNREACHABLE(); return 0; }
	virtual int64_t position() const override { LN_UNREACHABLE(); return 0; }
	virtual size_t read(void* buffer, size_t byteCount) override { return readBytes(buffer, byteCount); }
	virtual void write(const void* buffer, size_t byteCount) override { writeBytes(buffer, byteCount); }
	virtual void seek(int64_t offset, SeekOrigin origin) override { LN_UNREACHABLE(); }
	virtual void flush() override {}

private:
	int m_readFD;
	int m_writeFD;
};

PipeImpl::PipeImpl()
	: m_readFD(-1)
	, m_writeFD(-1)
{
}

PipeImpl::~PipeImpl()
{
	closeBoth();
}

void PipeImpl::init()
{
	int fds[2];
	int result = pipe(fds);
	if (LN_ENSURE(result == 0)) return;

	m_readFD = fds[0];
	m_writeFD = fds[1];
}

int PipeImpl::readBytes(void* buffer, int length)
{
	int n;
	do
	{
		n = ::read(m_readFD, buffer, length);
	} while (n < 0 && errno == EINTR);

	if (LN_ENSURE(n >= 0)) return 0;

	return n;
}

int PipeImpl::writeBytes(const void* buffer, int length)
{
	int n;
	do
	{
		n = ::write(m_writeFD, buffer, length);
	} while (n < 0 && errno == EINTR);

	if (LN_ENSURE(n >= 0)) return 0;

	return n;
}

void PipeImpl::closeRead()
{
	if (m_readFD != -1)
	{
		close(m_readFD);
		m_readFD = -1;
	}
}

void PipeImpl::closeWrite()
{
	if (m_writeFD != -1)
	{
		close(m_writeFD);
		m_writeFD = -1;
	}
}

void PipeImpl::closeBoth()
{
	closeRead();
	closeWrite();
}

//==============================================================================
// ProcessImpl

class ProcessImpl : public RefObject
{
public:
	ProcessImpl();
	virtual ~ProcessImpl() = default;
	
	void start(const ProcessStartInfo& startInfo);
	void startWithShell(const ProcessStartInfo& startInfo);
	bool waitForExit(int timeoutMSec);
	ProcessStatus getStatus(int* outExitCode);
	
	void closeHandle();

private:
	pid_t   m_pid;
	int m_exitCode;
	bool m_exited;
	//Ref<PipeImpl> m_stdinPipe;
	//Ref<PipeImpl> startInfo.stdoutPipe;
	//Ref<PipeImpl> startInfo.stderrPipe;
};
	
ProcessImpl::ProcessImpl()
	: m_pid(0)
	//, m_exitCode(0)
	, m_exitCode(0)
	, m_exited(false)
{
}

void ProcessImpl::start(const ProcessStartInfo& startInfo)
{
	std::string program = startInfo.program.str().toStdString();
	std::string workingDirectory = startInfo.workingDirectory.str().toStdString();
	std::vector<std::string> argvInstance(startInfo.args.size());
	std::vector<char*> argv(startInfo.args.size() + 2);

	argv[0] = const_cast<char*>(program.c_str());
	for (int i = 0; i < startInfo.args.size(); i++) {
		argvInstance[i] = startInfo.args[i].toStdString();
		argv[i + 1] = const_cast<char*>(argvInstance[i].c_str());
	}
	argv[startInfo.args.size() + 1] = NULL;

	pid_t pid = fork();
	if (LN_ENSURE(pid >= 0)) return;

	if (pid == 0)
	{
		// child process

		if (!workingDirectory.empty())
		{
			if (chdir(workingDirectory.c_str()) != 0)
			{
				perror("critical OS file missing.\n");
				exit(72);	// critical OS file missing 
			}
		}

		// redirection
		if (startInfo.stdinPipe) dup2(startInfo.stdinPipe->readFD(), STDIN_FILENO);
		if (startInfo.stdoutPipe) dup2(startInfo.stdoutPipe->writeFD(), STDOUT_FILENO);
		if (startInfo.stderrPipe) dup2(startInfo.stderrPipe->writeFD(), STDERR_FILENO);
		if (startInfo.stdinPipe) startInfo.stdinPipe->closeBoth();
		if (startInfo.stdoutPipe) startInfo.stdoutPipe->closeBoth();
		if (startInfo.stderrPipe) startInfo.stderrPipe->closeBoth();
		
		execve(argv[0], argv.data(), environ);
		
		// it only cames here when execve failed.
		fprintf(stderr, "execve failed. (errno:%d)\n", errno);
		exit(72);
	}
	else
	{
		// parent process

		if (startInfo.stdinPipe) startInfo.stdinPipe->closeRead();
		if (startInfo.stdoutPipe) startInfo.stdoutPipe->closeWrite();
		if (startInfo.stderrPipe) startInfo.stderrPipe->closeWrite();
	}
}

void ProcessImpl::startWithShell(const ProcessStartInfo& startInfo)
{
	start(startInfo);
}

bool ProcessImpl::waitForExit(int timeoutMSec)
{
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	int elapsedTime;

	do
	{
		int status;
		pid_t pid = waitpid(m_pid, &status, WNOHANG);
		if (pid == 0) {
			// no change state, running.
		}
		else
		{
			if (!m_exited)
			{
				m_exitCode = WEXITSTATUS(status);
				m_exited = true;
			}
			break;
		}
		
		usleep(1000);

		elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
		
	} while(timeoutMSec == -1 || elapsedTime < timeoutMSec);
	
	return m_exited;
}

ProcessStatus ProcessImpl::getStatus(int* outExitCode)
{
	if (outExitCode) *outExitCode = 0;

	int status;
	pid_t pid = waitpid(m_pid, &status, WNOHANG);
	if (pid == 0) {
		return ProcessStatus::Running;
	}
	else
	{
		if (!m_exited)
		{
			m_exitCode = WEXITSTATUS(status);
			m_exited = true;
		}
		if (outExitCode) *outExitCode = m_exitCode;
		return ProcessStatus::Finished;
	}

#if 0
	else if (WIFEXITED(status)) {
		if (!m_exited)
		{
			m_exitCode = WEXITSTATUS(status);
			m_exited = true;
		}
		if (outExitCode) *outExitCode = m_exitCode;
		return ProcessStatus::Finished;
	}
	else {
		// force termination.
		m_exited = true;
		return ProcessStatus::Crashed;
	}
#endif
}

void ProcessImpl::closeHandle() {
	// TODO:
}

} // namespace detail



#if 0
//==============================================================================
// Process
//==============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void process::start(const PathName& program, const String& args)
{}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool process::WaitForExit(int timeoutMSec)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ProcessStatus process::GetState()
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
void process::TryGetExitCode()
{
	//GetState();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void process::dispose()
{
	if (!m_disposed)
	{
		m_disposed = true;
	}
}
//#endif
#endif

} // namespace ln

