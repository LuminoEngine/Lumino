#include "Internal.hpp"
#include <LuminoCore/IO/Process.hpp>
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
    , m_stdinEncoding(nullptr)
    , m_stdoutEncoding(nullptr)
    , m_stderrEncoding(nullptr) {
    //m_startInfo.useShellExecute = true;
    createRedirectStreams();
}

Process::Process(const String& program, const List<String>& arguments, TextEncoding* encoding)
    : m_impl(makeRef<detail::ProcessImpl>())
    , m_startInfo()
    , m_stdinEncoding(encoding)
    , m_stdoutEncoding(encoding)
    , m_stderrEncoding(encoding) {
    setProgram(program);
    setArguments(arguments);
    createRedirectStreams();
}

Process::~Process() {
}

StreamWriter* Process::openStdin() {
    if (!m_stdinWriter) {
        m_startInfo.stdinPipe = makeRef<detail::PipeImpl>();
        m_startInfo.stdinPipe->init();
        TextEncoding* encoding = m_stdinEncoding;
        if (!encoding) {
            encoding = TextEncoding::systemMultiByteEncoding();
        }
        m_stdinWriter = makeRef<StreamWriter>(m_startInfo.stdinPipe, encoding);
    }

    return m_stdinWriter;
}

StreamReader* Process::openStdout() {
    if (!m_stdoutReader) {
        m_startInfo.stdoutPipe = makeRef<detail::PipeImpl>();
        m_startInfo.stdoutPipe->init();
        TextEncoding* encoding = m_stdoutEncoding;
        if (!encoding) {
            encoding = TextEncoding::systemMultiByteEncoding();
        }
        m_stdoutReader = makeRef<StreamReader>(m_startInfo.stdoutPipe, encoding);
    }

    return m_stdoutReader;
}

StreamReader* Process::openStderr() {
    if (!m_stderrReader) {
        m_startInfo.stderrPipe = makeRef<detail::PipeImpl>();
        m_startInfo.stderrPipe->init();
        TextEncoding* encoding = m_stdoutEncoding;
        if (!encoding) {
            encoding = TextEncoding::systemMultiByteEncoding();
        }
        m_stderrReader = makeRef<StreamReader>(m_startInfo.stderrPipe, encoding);
    }

    return m_stderrReader;
}

//StreamWriter* Process::stdinWriter() const
//{
//	return m_stdinWriter;
//}
//
//StreamReader* Process::stdoutReader() const
//{
//	return m_stdoutReader;
//}
//
//StreamReader* Process::stderrReader() const
//{
//	return m_stderrReader;
//}

void Process::start() {
    if (m_startInfo.useShellExecute) {
        if (LN_REQUIRE(!m_startInfo.stdinPipe && !m_startInfo.stdoutPipe && !m_startInfo.stderrPipe, "Use with setUseShellExecute(false).")) return;
        // TODO: 環境変数指定も NG
        m_impl->startWithShell(m_startInfo);
    }
    else {
        m_impl->start(m_startInfo);
    }
}

bool Process::wait(int timeoutMilliseconds) {
    return m_impl->waitForExit(timeoutMilliseconds);
}

ProcessStatus Process::status() {
    return m_impl->getStatus(nullptr);
}

int Process::exitCode() {
    int n = 1;
    m_impl->getStatus(&n);
    return n;
}

int Process::execute(const Path& program, const List<String>& args, String* outStdOutput, String* outStdError) {
    Process proc(program, args);
    StreamReader* stdoutReader = (outStdOutput) ? proc.openStdout() : nullptr;
    StreamReader* stderrReader = (outStdError) ? proc.openStderr() : nullptr;
    proc.start();
    proc.wait();
    if (stdoutReader && outStdOutput) *outStdOutput = stdoutReader->readToEnd();
    if (stderrReader && outStdError) *outStdError = stderrReader->readToEnd();
    return proc.exitCode();
}

void Process::createRedirectStreams() {
    //if (!m_stdinWriter)
    //{
    //	m_startInfo.stdinPipe = makeRef<detail::PipeImpl>();
    //	m_startInfo.stdinPipe->init();
    //	m_stdinWriter = makeRef<StreamWriter>(m_startInfo.stdinPipe, m_stdinEncoding);
    //}

    //if (!m_stdoutReader)
    //{
    //	m_startInfo.stdoutPipe = makeRef<detail::PipeImpl>();
    //	m_startInfo.stdoutPipe->init();
    //	m_stdoutReader = makeRef<StreamReader>(m_startInfo.stdoutPipe, m_stdoutEncoding);
    //}

    //if (!m_stderrReader)
    //{
    //	m_startInfo.stderrPipe = makeRef<detail::PipeImpl>();
    //	m_startInfo.stderrPipe->init();
    //	m_stderrReader = makeRef<StreamReader>(m_startInfo.stderrPipe, m_stderrEncoding);
    //}
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

//==============================================================================
// ProcessCommand2

Ref<ProcessStdio> ProcessStdio::piped(TextEncoding* encoding) {
    auto io = Ref<ProcessStdio>(LN_NEW ProcessStdio(), false);
    io->m_encoding = encoding; 
    return io;
}

//==============================================================================
// ProcessCommand2
/*
start() method name?
----------
- Rust: spawn()
- Python: run()
- JavaScript: exec / spawn
- Ruby: fork()
- C#: start()
- Go: start() https://qiita.com/hironobu_s/items/77d99436457ef57889d6
- C++ reproc: run() https://github.com/DaanDeMeyer/reproc
- C++ Qt: start()
- C++ POCO: launch() https://docs.pocoproject.org/current/Poco.Process.html#18008
*/

ProcessCommand2::ProcessCommand2(const ln::String& command) {
    m_startInfo.program = command;
}

ProcessCommand2& ProcessCommand2::args(const ln::Array<ln::String> value) {
    for (const auto& v : value) {
        m_startInfo.args.push_back(v);
    }
    return *this;
}

ProcessCommand2& ProcessCommand2::arg(const ln::String& value) {
    m_startInfo.args.push_back(value);
    return *this;
}

ProcessCommand2& ProcessCommand2::workingDirectory(const ln::String& value) {
    m_startInfo.workingDirectory = value;
    return *this;
}

Ref<Process2> ProcessCommand2::start() {
    auto proc = Ref<Process2>(LN_NEW Process2(), false);
    proc->start(this);
    return proc;
}

//==============================================================================
// Process2

Process2::Process2()
    : m_impl() {
}

Process2::~Process2() {
    dispose();
}

void Process2::start(const ProcessCommand2* cmd) {
    detail::ProcessStartInfo startInfo;
    startInfo.program = cmd->m_startInfo.program;
    startInfo.args = cmd->m_startInfo.args;
    startInfo.workingDirectory = cmd->m_startInfo.workingDirectory;

    Ref<detail::PipeImpl> stdIn;
    if (cmd->m_stdIn) {
        stdIn = makeRef<detail::PipeImpl>();
        stdIn->init();
        TextEncoding* encoding = cmd->m_stdIn->encoding();
        if (!encoding) {
            encoding = TextEncoding::systemMultiByteEncoding();
        }
        m_stdinWriter = makeRef<StreamWriter>(stdIn, encoding);
    }

    Ref<detail::PipeImpl> stdOut;
    if (cmd->m_stdOut) {
        stdOut = makeRef<detail::PipeImpl>();
        stdOut->init();
        TextEncoding* encoding = cmd->m_stdOut->encoding();
        if (!encoding) {
            encoding = TextEncoding::systemMultiByteEncoding();
        }
        m_stdoutReader = makeRef<StreamReader>(stdOut, encoding);
    }

    Ref<detail::PipeImpl> stdErr;
    if (cmd->m_stdErr) {
        stdErr = makeRef<detail::PipeImpl>();
        stdErr->init();
        TextEncoding* encoding = cmd->m_stdErr->encoding();
        if (!encoding) {
            encoding = TextEncoding::systemMultiByteEncoding();
        }
        m_stderrReader = makeRef<StreamReader>(stdErr, encoding);
    }

    startInfo.stdinPipe = stdIn;
    startInfo.stdoutPipe = stdOut;
    startInfo.stderrPipe = stdErr;

    m_stdIn = stdIn;
    m_stdOut = stdOut;
    m_stdErr = stdErr;
    m_onStdOutReceived = cmd->m_onStdOutReceived;
    m_onStdErrReceived = cmd->m_onStdErrReceived;

    m_impl = makeRef<detail::ProcessImpl>();
    m_impl->start(startInfo);

    if (m_onStdOutReceived) {
        beginOutputReadLine();
    }
    if (m_onStdErrReceived) {
        beginErrorReadLine();
    }
}

void Process2::dispose() {
    if (m_impl) {
        m_impl->closeHandle();
        m_impl = nullptr;
    }

    // Wait for reading threads.
    endReadThreads();
}

bool Process2::wait(int timeoutMilliseconds) {
    bool exit = m_impl->waitForExit(timeoutMilliseconds);
    if (exit) {
        endReadThreads();
    }
    return exit;
}

ProcessStatus Process2::status() {
    return m_impl->getStatus(nullptr);
}

int Process2::exitCode() {
    int result = 1;
    ProcessStatus status = m_impl->getStatus(&result);
    if (status == ProcessStatus::Running) {
        LN_ERROR(U"Process is running. Use wait().");
    } 
    return result;
}

int Process2::exec(const String& command, String* outStdOutput, String* outStdError) {
    String program;
    String args;
    int i = command.indexOf(' ');
    if (i >= 0) {
        program = command.substr(0, i);
    }
    else {
        program = command;
    }

    ProcessCommand2 cmd(program);
    if (i >= 0) {
        cmd.arg(command.substr(i + 1));
    }
    if (outStdOutput) {
        cmd.stdOut(ProcessStdio::piped());
    }
    if (outStdError) {
        cmd.stdErr(ProcessStdio::piped());
    }

    auto proc = cmd.start();

    if (outStdOutput) {
        *outStdOutput = proc->stdOut()->readToEnd();
    }
    if (outStdError) {
        *outStdError = proc->stdErr()->readToEnd();
    }

    proc->wait();
    return proc->exitCode();
}

void Process2::beginOutputReadLine() {
    m_readStdOutThread = std::make_unique<std::thread>([this]() {
        thread_readStdOut();
    });
}

void Process2::beginErrorReadLine() {
    m_readStdErrThread = std::make_unique<std::thread>([this]() {
        thread_readStdErr();
    });
}

void Process2::endReadThreads() {
    if (m_readStdOutThread) {
        m_readStdOutThread->join();
        m_readStdOutThread = nullptr;
    }
    if (m_readStdErrThread) {
        m_readStdErrThread->join();
        m_readStdErrThread = nullptr;
    }
}

void Process2::thread_readStdOut() {
    String line;
    while (m_stdoutReader->readLine(&line)) {
        if (m_onStdOutReceived) {
            m_onStdOutReceived(line);
        }
    }
}

void Process2::thread_readStdErr() {
    String line;
    while (m_stderrReader->readLine(&line)) {
        if (m_onStdErrReceived) {
            m_onStdErrReceived(line);
        }
    }
}

} // namespace ln
