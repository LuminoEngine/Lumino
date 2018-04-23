#include "Internal.hpp"
#include <Lumino/IO/Process.hpp>
#if defined(LN_OS_WIN32)
#include "Process_Win32.hpp"
#else
#include "Process_Unix.hpp"
#endif

namespace ln {

//==============================================================================
// Process

Process::Process()
	: m_impl(makeRef<detail::ProcessImpl>())
	, m_startInfo()
{
}

Process::~Process()
{
}

void Process::start()
{
	m_impl->start(m_startInfo);
}

bool Process::wait(int timeoutMilliseconds)
{
	return m_impl->waitForExit(timeoutMilliseconds);
}

ProcessStatus Process::status()
{
	return m_impl->getStatus(nullptr);
}

int Process::exitCode()
{
	int n = 1;
	m_impl->getStatus(&n);
	return n;
}

#if 0
//==============================================================================
// Process
//==============================================================================
//------------------------------------------------------------------------------
Process::Process()
	: m_impl(LN_NEW detail::ProcessImpl())
	, m_workingDirectory()
	, m_redirectStandardInput(false)
	, m_redirectStandardOutput(false)
	, m_redirectStandardError(false)
	, m_standardInputEncoding(Encoding::getSystemMultiByteEncoding())
	, m_standardOutputEncoding(Encoding::getSystemMultiByteEncoding())
	, m_standardErrorEncoding(Encoding::getSystemMultiByteEncoding())
	, m_standardOutputExternalStream(nullptr)
	, m_standardInputWriter()
	, m_standardOutputReader()
	, m_standardErrorReader()
	, m_runningReadThread(false)
	, m_runningErrorReadThread(false)
#ifdef LN_OS_WIN32
#else
    , m_pid(NULL)
#endif
{
}

//------------------------------------------------------------------------------
Process::~Process()
{
	disposeProcess();
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void Process::setWorkingDirectory(const PathName& directoryPath)
{
	m_workingDirectory = directoryPath;
}

//------------------------------------------------------------------------------
void Process::setRedirectStandardInput(bool enabled)
{
	m_redirectStandardInput = enabled;
}

//------------------------------------------------------------------------------
void Process::setRedirectStandardOutput(bool enabled)
{
	m_redirectStandardOutput = enabled;
}

//------------------------------------------------------------------------------
void Process::setRedirectStandardError(bool enabled)
{
	m_redirectStandardError = enabled;
}

#ifdef LN_CPP11
//------------------------------------------------------------------------------
void Process::setOutputDataReceivedCallback(const Delegate<void(String)>& callback)
{
	m_outputDataReceivedCallback = callback;
}

//------------------------------------------------------------------------------
void Process::setErrorDataReceivedCallback(const Delegate<void(String)>& callback)
{
	m_errorDataReceivedCallback = callback;
}
#else
//------------------------------------------------------------------------------
void process::SetOutputDataReceivedCallback(const Delegate01<String>& callback)
{
	m_outputDataReceivedCallback = callback;
}

//------------------------------------------------------------------------------
void process::SetErrorDataReceivedCallback(const Delegate01<String>& callback)
{
	m_errorDataReceivedCallback = callback;
}
#endif

//------------------------------------------------------------------------------
void Process::start(const PathName& program, const String& args)
{
	detail::ProcessStartInfo si;
	detail::ProcessStartResult result;
	si.program = program;
	si.args = args;
	si.workingDirectory = m_workingDirectory;
	si.redirectStandardInput = m_redirectStandardInput;
	si.redirectStandardOutput = m_redirectStandardOutput;
	si.redirectStandardError = m_redirectStandardError;
	si.standardInputEncoding = m_standardInputEncoding;
	si.standardOutputEncoding = m_standardOutputEncoding;
	si.standardErrorEncoding = m_standardErrorEncoding;
	m_impl->start(si, &result);
	m_standardInputWriter = result.standardInputWriter;
	m_standardOutputReader = result.standardOutputReader;
	m_standardErrorReader = result.standardErrorReader;
}

//------------------------------------------------------------------------------
void Process::start(const PathName& program, const StringArray& argsList)
{
	// コマンドライン引数を作る
	String args;
	for (int i = 0; i < argsList.getCount(); ++i)
	{
		if (!args.isEmpty()) {
			args += _LT(' ');
		}

		// スペースが含まれていれば引数を " で囲む
		String tmp = argsList[i];
		if (tmp.contains(_LT(' ')) || tmp.contains(_LT('\t')))
		{
			if (!tmp.startsWith(_LT('\"')) && !tmp.endsWith(_LT('\"')))
			{
				tmp = _LT('\"') + tmp + _LT('\"');
			}
		}

		args += tmp;
	}

	start(program, args);
}

//------------------------------------------------------------------------------
bool Process::waitForExit(int timeoutMSec)
{
	return m_impl->waitForExit(timeoutMSec);
}

//------------------------------------------------------------------------------
StreamWriter* Process::getStandardInput() const
{
	if (LN_REQUIRE(m_standardInputWriter != nullptr)) return nullptr;
	return m_standardInputWriter;
}

//------------------------------------------------------------------------------
StreamReader* Process::getStandardOutput() const
{
	if (LN_REQUIRE(m_standardOutputReader != nullptr)) return nullptr;
	return m_standardOutputReader;
}

//------------------------------------------------------------------------------
StreamReader* Process::getStandardError() const
{
	if (LN_REQUIRE(m_standardErrorReader != nullptr)) return nullptr;
	return m_standardErrorReader;
}

//------------------------------------------------------------------------------
int Process::execute(const PathName& program, const String& args, String* outStdOutput, String* outStdError)
{
	Process proc;
	proc.setRedirectStandardOutput(outStdOutput != nullptr);
	proc.setRedirectStandardError(outStdError != nullptr);
	proc.start(program, args);
	if (outStdOutput != nullptr) {
		*outStdOutput = proc.getStandardOutput()->readToEnd();
	}
	if (outStdError != nullptr) {
		*outStdError = proc.getStandardError()->readToEnd();
	}
	proc.waitForExit();
	return proc.getExitCode();
}

//------------------------------------------------------------------------------
int Process::getExitCode()
{
	return m_impl->getExitCode();
}

//------------------------------------------------------------------------------
void Process::beginOutputReadLine()
{
	if (LN_REQUIRE(m_standardOutputReader != nullptr)) return;

	// 読み取りスレッドを立てる
#ifdef LN_CPP11
	m_readStdOutputThread.start(Delegate<void()>(this, &Process::thread_ReadStdOutput));
#else
	m_readStdOutputThread.start(LN_CreateDelegate(this, &process::Thread_ReadStdOutput));
#endif
	m_runningReadThread = true;
}

//------------------------------------------------------------------------------
void Process::beginErrorReadLine()
{
	if (LN_REQUIRE(m_standardErrorReader != nullptr)) return;

	// 読み取りスレッドを立てる
#ifdef LN_CPP11
	m_readStdErrorThread.start(Delegate<void()>(this, &Process::thread_ReadStdError));
#else
	m_readStdErrorThread.start(LN_CreateDelegate(this, &process::Thread_ReadStdError));
#endif
	m_runningErrorReadThread = true;
}

//------------------------------------------------------------------------------
void Process::disposeProcess()
{
	m_impl->dispose();

	// 読み取りスレッドの終了を待つ
	if (m_runningReadThread)
	{
		m_runningReadThread = false;
		m_readStdOutputThread.wait();
	}
	if (m_runningErrorReadThread)
	{
		m_runningErrorReadThread = false;
		m_readStdErrorThread.wait();
	}
}

//------------------------------------------------------------------------------
void Process::thread_ReadStdOutput()
{
	String strLine;
	while (m_standardOutputReader->readLine(&strLine))
	{
		if (!m_outputDataReceivedCallback.isEmpty()) {
			m_outputDataReceivedCallback.call(strLine);
		}
	}
}

//------------------------------------------------------------------------------
void Process::thread_ReadStdError()
{
	String strLine;
	while (m_standardErrorReader->readLine(&strLine))
	{
		if (!m_errorDataReceivedCallback.isEmpty()) {
			m_errorDataReceivedCallback.call(strLine);
		}
	}
}
#endif

} // namespace ln
