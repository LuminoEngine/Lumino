#pragma once
#include <thread>
#include <functional>
#include "Common.hpp"
#include "../Base/Array.hpp"
#include "Path.hpp"
#include "StreamReader.hpp"
#include "StreamWriter.hpp"

namespace ln {
class ProcessStdio;
class Process2;
namespace detail {
class ProcessImpl;
class PipeImpl;

struct ProcessStartInfo {
    Path program;
    std::vector<String> args;
    Path workingDirectory;
    Ref<detail::PipeImpl> stdinPipe;
    Ref<detail::PipeImpl> stdoutPipe;
    Ref<detail::PipeImpl> stderrPipe;
    //bool					redirectStandardInput;
    //bool					redirectStandardOutput;
    //bool					redirectStandardError;
    //Encoding*				standardInputEncoding;
    //Encoding*				standardOutputEncoding;
    //Encoding*				standardErrorEncoding;
    bool useShellExecute;
};

struct ProcessStartInfo2 {
    String command;
    std::vector<String> args;
    Path workingDirectory;
    Ref<ProcessStdio> stdinPipe;
    Ref<ProcessStdio> stdoutPipe;
    Ref<ProcessStdio> stderrPipe;
    bool useShellExecute;
};

//struct ProcessStartResult
//{
//	Ref<StreamWriter>	standardInputWriter;
//	Ref<StreamReader>	standardOutputReader;
//	Ref<StreamReader>	standardErrorReader;
//};

} // namespace detail

/** プロセスの状態 */
enum class ProcessStatus {
    Running,  /**< 実行中 */
    Finished, /**< 正常終了した */
    Crashed,  /**< 異常終了した */
};

/**
	@brief		外部プロセスを起動し、通信を行うクラスです。

	@section	プロセスを起動し、終了を待つ
	~~~~~~~~~~~~~~~{.cpp}
	Process proc;
	proc.Start("merge", "file1.c", "file2.c");
	proc.WaitForExit();
	~~~~~~~~~~~~~~~
*/
class Process : public RefObject {
public:
    Process();
    Process(const String& program, const List<String>& arguments, TextEncoding* encoding = nullptr);
    virtual ~Process();

public:
    void setProgram(const String& program) { m_startInfo.program = program; }

    void setArguments(const List<String>& arguments) { 
        for (auto& a : arguments) {
            m_startInfo.args.push_back(a);
        }
    }

    /**
		@brief		開始するプロセスの作業ディレクトリを指定します。 (規定値:現在のカレントディレクトリ)
		@details	Start() の前に設定する必要があります。
	*/
    void setWorkingDirectory(const Path& directoryPath) { m_startInfo.workingDirectory = directoryPath; }

    StreamWriter* openStdin();

    StreamReader* openStdout();

    StreamReader* openStderr();

    /**
		@brief		プログラムのファイルパスとコマンドライン引数を指定してプロセスを起動します。

		@exception	FileNotFoundException	program が見つからなかった。
	*/
    void start();

    /**
		@brief		プロセスの終了を待機します。
		@param[in]	timeoutMilliseconds	: タイムアウト時間 (ミリ秒)
		@return		true:正常終了  false:タイムアウトした
	*/
    bool wait(int timeoutMilliseconds = -1);

    /**
		@brief		プロセスの状態を取得します。
		@attention	リダイレクトを非同期で行っている場合、ProcessStatus::Finished を返してもまだリダイレクトスレッドは終了していないことがあります。
					完了させるには WaitForExit() を呼び出してください。
	*/
    ProcessStatus status();

    /**
		@brief		プロセスの終了コードを取得します。
		@details	GetState() が ProcessStatus::Finished を返さない限り、戻り値は有効ではありません。
	*/
    int exitCode();

    void setUseShellExecute(bool value) { m_startInfo.useShellExecute = value; }

public:
    /**
		@brief		プロセスを起動し、終了するまで待機します。
		@param[in]	program			: プログラム名または実行ファイルパス
		@param[in]	args			: コマンドライン引数
		@param[out]	outStdOutput	: 標準出力をリダイレクトし文字列として受け取る場合、その文字列を格納する変数のポインタを指定する
		@param[out]	outStdError		: 標準エラー出力をリダイレクトし文字列として受け取る場合、その文字列を格納する変数のポインタを指定する
		@return		プロセスの終了コード
	*/
    [[deprecated("There is a possibility of deadlock when getting a long standard output.")]]
    static int execute(const Path& program, const List<String>& args = List<String>(), String* outStdOutput = nullptr, String* outStdError = nullptr);

private:
    void createRedirectStreams();

    Ref<detail::ProcessImpl> m_impl;

    detail::ProcessStartInfo m_startInfo;

    TextEncoding* m_stdinEncoding;
    TextEncoding* m_stdoutEncoding;
    TextEncoding* m_stderrEncoding;

    Ref<StreamWriter> m_stdinWriter;
    Ref<StreamReader> m_stdoutReader;
    Ref<StreamReader> m_stderrReader;
};

/** 
 * プロセスの標準I/Oストリームをどのように処理するかを設定します。
 */
class ProcessStdio : public RefObject {
public:
    static Ref<ProcessStdio> piped(TextEncoding* encoding = nullptr);

    TextEncoding* encoding() const { return m_encoding; }

private:
    Ref<TextEncoding> m_encoding;
};

using ProcessStdioReceivedHandler = std::function<void(ln::String)>;

/** 
 * 新しいプロセスを生成するための方法を制御します。
 * 
 * 非同期リダイレクト
 * ----------
 * onStdOutReceived または onStdErrReceived にコールバックを設定すると、ストリーム出力を非同期的に受信することができます。
 * このとき、Process::stdOut または Process::stdError は使用できません。
 * コールバックは受信用のスレッド (start() したスレッドとは別のスレッド) から呼び出されます。
 */
class ProcessCommand2 {
public:
    ProcessCommand2(const ln::String& command);

    ProcessCommand2& args(const ln::Array<ln::String> value);

    ProcessCommand2& arg(const ln::String& value);

    ProcessCommand2& workingDirectory(const ln::String& value);

    ProcessCommand2& stdIn(ProcessStdio* value) {
        m_stdIn = value;
        return *this;
    }

    ProcessCommand2& stdOut(ProcessStdio* value) {
        m_stdOut = value;
        return *this;
    }

    ProcessCommand2& stdErr(ProcessStdio* value) {
        m_stdErr = value;
        return *this;
    }

    /** リダイレクトされた標準出力を非同期的に受信するためのコールバック */
    ProcessCommand2& onStdOutReceived(ProcessStdioReceivedHandler value) {
        m_onStdOutReceived = value;
        return *this;
    }

    /** リダイレクトされた標準エラー出力を非同期的に受信するためのコールバック */
    ProcessCommand2& onStdErrReceived(ProcessStdioReceivedHandler value) {
        m_onStdErrReceived = value;
        return *this;
    }

    Ref<Process2> start();

private:
    static Ref<detail::PipeImpl> createPipe(ProcessStdio* io);

    detail::ProcessStartInfo m_startInfo;
    Ref<ProcessStdio> m_stdIn;
    Ref<ProcessStdio> m_stdOut;
    Ref<ProcessStdio> m_stdErr;
    ProcessStdioReceivedHandler m_onStdOutReceived;
    ProcessStdioReceivedHandler m_onStdErrReceived;

    friend class Process2;
};

/**
 * 
 * 
 * リダイレクトと wait の注意点
 * ----------
 * Pipe を使ったリダイレクトが有効である場合、子プロセスは EOF まで完全に読み取られた後に終了します。
 * この読み取りの責任はユーザープログラム側にあります。
 * 例えば次のプログラムでは子プロセスが完全に終了しないため、デッドロックが発生します。
 * 
 * ```
 * auto proc = ...
 * proc->wait();                    // Dead lock!
 * proc->stdoutStream()->read(...); // 終了後、標準出力を読み取ろうとした？
 * ```
 */
class Process2 : public RefObject {
public:
    /**
     * プロセスの終了を待機します。
     * 
     * @param[in]   timeoutMilliseconds	: タイムアウト時間 (ミリ秒)
     * @return      true:正常終了  false:タイムアウトした
     * 
     * wait はリダイレクトの機能と同時に使用することはできません。
	 */
    bool wait(int timeoutMilliseconds = -1);

    ProcessStatus status();

    int exitCode();


    //Stream* stdinStream();
    //Stream* stdoutStream();
    //Stream* stderrStream();
    StreamWriter* stdIn() const { return m_stdinWriter; }
    StreamReader* stdOut() const { return m_stdoutReader; }
    StreamReader* stdErr() const { return m_stderrReader; }

    static int exec(const String& command, String* outStdOutput = nullptr, String* outStdError = nullptr);

private:
    Process2();
    ~Process2();
    void start(const ProcessCommand2* cmd);
    void dispose();
    void beginOutputReadLine();
    void beginErrorReadLine();
    void endReadThreads();
    void thread_readStdOut();
    void thread_readStdErr();

    Ref<detail::ProcessImpl> m_impl;
    Ref<detail::PipeImpl> m_stdIn;
    Ref<detail::PipeImpl> m_stdOut;
    Ref<detail::PipeImpl> m_stdErr;
    Ref<StreamWriter> m_stdinWriter;
    Ref<StreamReader> m_stdoutReader;
    Ref<StreamReader> m_stderrReader;
    ProcessStdioReceivedHandler m_onStdOutReceived;
    ProcessStdioReceivedHandler m_onStdErrReceived;
    std::unique_ptr<std::thread> m_readStdOutThread;
    std::unique_ptr<std::thread> m_readStdErrThread;
    friend class ProcessCommand2;
};

} // namespace ln
