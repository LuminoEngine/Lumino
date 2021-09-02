
#pragma once
#include "Common.hpp"
#include "Path.hpp"
#include "StreamReader.hpp"
#include "StreamWriter.hpp"

namespace ln {
namespace detail {
class ProcessImpl;
class PipeImpl;

struct ProcessStartInfo
{
	Path				program;
	List<String>					args;
	Path				workingDirectory;
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

//struct ProcessStartResult
//{
//	Ref<StreamWriter>	standardInputWriter;
//	Ref<StreamReader>	standardOutputReader;
//	Ref<StreamReader>	standardErrorReader;
//};
	
} // namespace detail

/** プロセスの状態 */
enum class ProcessStatus
{
	Running,			/**< 実行中 */
	Finished,			/**< 正常終了した */
	Crashed,			/**< 異常終了した */
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
class Process : public RefObject
{
public:
	Process();
	Process(const String& program, const List<String>& arguments, TextEncoding* encoding = nullptr);
	virtual ~Process();
	
public:

	void setProgram(const String& program) { m_startInfo.program = program; }

	void setArguments(const List<String>& arguments) { m_startInfo.args = arguments; }

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
	static int execute(const Path& program, const List<String>& args = List<String>(), String* outStdOutput = nullptr, String* outStdError = nullptr);

private:
	void createRedirectStreams();


	Ref<detail::ProcessImpl> m_impl;

	detail::ProcessStartInfo	m_startInfo;

	TextEncoding*					m_stdinEncoding;
	TextEncoding*					m_stdoutEncoding;
	TextEncoding*					m_stderrEncoding;

	Ref<StreamWriter> m_stdinWriter;
	Ref<StreamReader> m_stdoutReader;
	Ref<StreamReader> m_stderrReader;

};


} // namespace ln
