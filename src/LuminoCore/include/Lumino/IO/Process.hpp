
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
	virtual ~Process();
	
public:

	void setProgram(const String& program) { m_startInfo.program = program; }

	void setArguments(const List<String>& arguments) { m_startInfo.args = arguments; }

	/**
		@brief		開始するプロセスの作業ディレクトリを指定します。 (規定値:現在のカレントディレクトリ)
		@details	Start() の前に設定する必要があります。
	*/
	void setWorkingDirectory(const Path& directoryPath) { m_startInfo.workingDirectory = directoryPath; }
	
	///**
	//	@brief		標準入力をリダイレクトするかを設定します。 (規定値:false)
	//	@details	Start() の前に設定する必要があります。
	//*/
	//void setRedirectStandardInput(bool enabled);

	///**
	//	@brief		標準出力をリダイレクトするかを設定します。 (規定値:false)
	//	@details	Start() の前に設定する必要があります。
	//*/
	//void setRedirectStandardOutput(bool enabled);

	///**
	//	@brief		標準エラー出力をリダイレクトするかを設定します。 (規定値:false)
	//	@details	Start() の前に設定する必要があります。
	//*/
	//void setRedirectStandardError(bool enabled);

	///**
	//	@brief		標準入力のエンコーディングを設定します。
	//	@details	Start() の前に設定する必要があります。
	//				規定値は Encoding::GetGetSystemMultiByteEncoding() です。
	//*/
	//void setStandardInputEncoding(Encoding* encoding) LN_NOEXCEPT{ m_standardInputEncoding = encoding; }
	//	
	///**
	//	@brief		標準出力のエンコーディングを設定します。
	//	@details	Start() の前に設定する必要があります。
	//				規定値は Encoding::GetGetSystemMultiByteEncoding() です。
	//*/
	//void setStandardOutputEncoding(Encoding* encoding) LN_NOEXCEPT { m_standardOutputEncoding = encoding; }

	///**
	//	@brief		標準エラー出力のエンコーディングを設定します。
	//	@details	Start() の前に設定する必要があります。
	//				規定値は Encoding::GetGetSystemMultiByteEncoding() です。
	//*/
	//void setStandardErrorEncoding(Encoding* encoding) LN_NOEXCEPT{ m_standardErrorEncoding = encoding; }
	//	
	///**
	//	@brief		標準出力を Stream へリダイレクトする場合、リダイレクト先の Stream を設定します。
	//	@details	リダイレクト先の Stream が設定されている場合、GetStandardOutput() を呼び出すことはできません。
	//				Start() の前に設定する必要があります。
	//*/
	//void setStandardOutputStream(Stream* stream) LN_NOEXCEPT{ m_standardOutputExternalStream = stream; }

	/**
		@brief		プログラムのファイルパスとコマンドライン引数を指定してプロセスを起動します。
		@param[in]	program	: プログラム名または実行ファイルパス
		@param[in]	args	: コマンドライン引数
		@exception	FileNotFoundException	program が見つからなかった。
	*/
	void start();

	///**
	//	@brief		プログラムのファイルパスとコマンドライン引数を指定してプロセスを起動します。
	//	@param[in]	program	: プログラム名または実行ファイルパス
	//	@param[in]	args	: コマンドライン引数
	//	@exception	FileNotFoundException	program が見つからなかった。
	//	@details	指定されたコマンドライン引数の各要素は、要素に空白が含まれる場合その前後に " を付加した状態でプロセスの起動に使用します。
	//*/
	//void start(const Path& program, const List<String>& args);

	/**
		@brief		プロセスの終了を待機します。
		@param[in]	timeoutMSec	: タイムアウト時間 (ミリ秒)
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

	///**
	//	@brief		標準入力の書き込みに使用されるストリームを取得します。
	//*/
	//StreamWriter* getStandardInput() const;

	///**
	//	@brief		標準出力の読み取りに使用されるストリームを取得します。
	//	@details	この関数は同期的な読み取りを行う際に使用します。
	//				取得した StreamReader の ReadLine() や ReadToEnd() は、子プロセスが標準出力に書き込みを行うか、プロセスが終了するまで待機し、
	//				プロセスが終了することで EOF となります。
	//*/
	//StreamReader* getStandardOutput() const;

	///**
	//	@brief		標準エラー出力の読み取りに使用されるストリームを取得します。
	//	@see		GetStandardOutput()
	//*/
	//StreamReader* getStandardError() const;

public:

	/**
		@brief		プロセスを起動し、終了するまで待機します。
		@param[in]	program			: プログラム名または実行ファイルパス
		@param[in]	args			: コマンドライン引数
		@param[out]	outStdOutput	: 標準出力をリダイレクトし文字列として受け取る場合、その文字列を格納する変数のポインタを指定する
		@param[out]	outStdError		: 標準エラー出力をリダイレクトし文字列として受け取る場合、その文字列を格納する変数のポインタを指定する
		@return		プロセスの終了コード
		@exception	FileNotFoundException	: program が見つからなかった。
	*/
	static int execute(const Path& program, const String& args = String(), String* outStdOutput = nullptr, String* outStdError = nullptr);

private:
	//void disposeProcess();
	//void thread_ReadStdOutput();
	//void thread_ReadStdError();

	Ref<detail::ProcessImpl> m_impl;

	detail::ProcessStartInfo	m_startInfo;

//	Path					m_workingDirectory;
//	bool						m_redirectStandardInput;
//	bool						m_redirectStandardOutput;
//	bool						m_redirectStandardError;
//	TextEncoding*					m_standardInputEncoding;
//	TextEncoding*					m_standardOutputEncoding;
//	TextEncoding*					m_standardErrorEncoding;
//	Stream*						m_standardOutputExternalStream;
//	Ref<StreamWriter>		m_standardInputWriter;
//	Ref<StreamReader>		m_standardOutputReader;
//	Ref<StreamReader>		m_standardErrorReader;
//
//	bool						m_runningReadThread;
//	bool						m_runningErrorReadThread;
//
//#ifdef _WIN32
//#else
//    pid_t   m_pid;	// TODO:PImplにしたのでいらないかも
//#endif
};


} // namespace ln
