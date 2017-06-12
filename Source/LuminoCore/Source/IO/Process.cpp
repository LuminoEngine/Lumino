#include "../Internal.h"
#include <Lumino/IO/Process.h>
#if defined(LN_OS_WIN32)
#include "Process_Win32.h"
#else
#include "Process_Unix.h"
#endif

LN_NAMESPACE_BEGIN

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
	, m_standardInputEncoding(Encoding::GetSystemMultiByteEncoding())
	, m_standardOutputEncoding(Encoding::GetSystemMultiByteEncoding())
	, m_standardErrorEncoding(Encoding::GetSystemMultiByteEncoding())
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
	Dispose();
	LN_SAFE_DELETE(m_impl);
}

//------------------------------------------------------------------------------
void Process::SetWorkingDirectory(const PathName& directoryPath)
{
	m_workingDirectory = directoryPath;
}

//------------------------------------------------------------------------------
void Process::SetRedirectStandardInput(bool enabled)
{
	m_redirectStandardInput = enabled;
}

//------------------------------------------------------------------------------
void Process::SetRedirectStandardOutput(bool enabled)
{
	m_redirectStandardOutput = enabled;
}

//------------------------------------------------------------------------------
void Process::SetRedirectStandardError(bool enabled)
{
	m_redirectStandardError = enabled;
}

#ifdef LN_CPP11
//------------------------------------------------------------------------------
void Process::SetOutputDataReceivedCallback(const Delegate<void(String)>& callback)
{
	m_outputDataReceivedCallback = callback;
}

//------------------------------------------------------------------------------
void Process::SetErrorDataReceivedCallback(const Delegate<void(String)>& callback)
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
			args += _T(' ');
		}

		// スペースが含まれていれば引数を " で囲む
		String tmp = argsList[i];
		if (tmp.contains(_T(' ')) || tmp.contains(_T('\t')))
		{
			if (!tmp.startsWith(_T('\"')) && !tmp.endsWith(_T('\"')))
			{
				tmp = _T('\"') + tmp + _T('\"');
			}
		}

		args += tmp;
	}

	start(program, args);
}

//------------------------------------------------------------------------------
bool Process::WaitForExit(int timeoutMSec)
{
	return m_impl->WaitForExit(timeoutMSec);
}

//------------------------------------------------------------------------------
StreamWriter* Process::GetStandardInput() const
{
	if (LN_CHECK_STATE(m_standardInputWriter != nullptr)) return nullptr;
	return m_standardInputWriter;
}

//------------------------------------------------------------------------------
StreamReader* Process::GetStandardOutput() const
{
	if (LN_CHECK_STATE(m_standardOutputReader != nullptr)) return nullptr;
	return m_standardOutputReader;
}

//------------------------------------------------------------------------------
StreamReader* Process::GetStandardError() const
{
	if (LN_CHECK_STATE(m_standardErrorReader != nullptr)) return nullptr;
	return m_standardErrorReader;
}

//------------------------------------------------------------------------------
int Process::Execute(const PathName& program, const String& args, String* outStdOutput, String* outStdError)
{
	Process proc;
	proc.SetRedirectStandardOutput(outStdOutput != nullptr);
	proc.SetRedirectStandardError(outStdError != nullptr);
	proc.start(program, args);
	if (outStdOutput != nullptr) {
		*outStdOutput = proc.GetStandardOutput()->ReadToEnd();
	}
	if (outStdError != nullptr) {
		*outStdError = proc.GetStandardError()->ReadToEnd();
	}
	proc.WaitForExit();
	return proc.GetExitCode();
}

//------------------------------------------------------------------------------
int Process::GetExitCode()
{
	return m_impl->GetExitCode();
}

//------------------------------------------------------------------------------
void Process::BeginOutputReadLine()
{
	if (LN_CHECK_STATE(m_standardOutputReader != nullptr)) return;

	// 読み取りスレッドを立てる
#ifdef LN_CPP11
	m_readStdOutputThread.start(Delegate<void()>(this, &Process::Thread_ReadStdOutput));
#else
	m_readStdOutputThread.start(LN_CreateDelegate(this, &process::Thread_ReadStdOutput));
#endif
	m_runningReadThread = true;
}

//------------------------------------------------------------------------------
void Process::BeginErrorReadLine()
{
	if (LN_CHECK_STATE(m_standardErrorReader != nullptr)) return;

	// 読み取りスレッドを立てる
#ifdef LN_CPP11
	m_readStdErrorThread.start(Delegate<void()>(this, &Process::Thread_ReadStdError));
#else
	m_readStdErrorThread.start(LN_CreateDelegate(this, &process::Thread_ReadStdError));
#endif
	m_runningErrorReadThread = true;
}

//------------------------------------------------------------------------------
void Process::Dispose()
{
	m_impl->Dispose();

	// 読み取りスレッドの終了を待つ
	if (m_runningReadThread)
	{
		m_runningReadThread = false;
		m_readStdOutputThread.Wait();
	}
	if (m_runningErrorReadThread)
	{
		m_runningErrorReadThread = false;
		m_readStdErrorThread.Wait();
	}
}

//------------------------------------------------------------------------------
void Process::Thread_ReadStdOutput()
{
	String strLine;
	while (m_standardOutputReader->ReadLine(&strLine))
	{
		if (!m_outputDataReceivedCallback.isEmpty()) {
			m_outputDataReceivedCallback.call(strLine);
		}
	}
}

//------------------------------------------------------------------------------
void Process::Thread_ReadStdError()
{
	String strLine;
	while (m_standardErrorReader->ReadLine(&strLine))
	{
		if (!m_errorDataReceivedCallback.isEmpty()) {
			m_errorDataReceivedCallback.call(strLine);
		}
	}
}

LN_NAMESPACE_END
