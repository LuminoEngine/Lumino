
#pragma once
#include "../Base/Delegate.h"
#include "../Threading/Thread.h"
#include "Common.h"
#include "StreamReader.h"
#include "StreamWriter.h"

LN_NAMESPACE_BEGIN
namespace detail { class ProcessImpl; }

/**
	@brief		外部プロセスを起動し、通信を行うクラスです。

	@section	プロセスを起動し、終了を待つ
	~~~~~~~~~~~~~~~{.cpp}
	Process proc;
	proc.Start("Merge", "file1.c", "file2.c");
	proc.WaitForExit();
	~~~~~~~~~~~~~~~
*/
class Process
{
public:
	Process();
	~Process();
	
public:

	/**
		@brief		開始するプロセスの作業ディレクトリを指定します。 (規定値:現在のカレントディレクトリ)
		@details	Start() の前に設定する必要があります。
	*/
	void SetWorkingDirectory(const PathName& directoryPath);
	
	/**
		@brief		標準入力をリダイレクトするかを設定します。 (規定値:false)
		@details	Start() の前に設定する必要があります。
	*/
	void SetRedirectStandardInput(bool enabled);

	/**
		@brief		標準出力をリダイレクトするかを設定します。 (規定値:false)
		@details	Start() の前に設定する必要があります。
	*/
	void SetRedirectStandardOutput(bool enabled);

	/**
		@brief		標準エラー出力をリダイレクトするかを設定します。 (規定値:false)
		@details	Start() の前に設定する必要があります。
	*/
	void SetRedirectStandardError(bool enabled);

	/**
		@brief		標準入力のエンコーディングを設定します。
		@details	Start() の前に設定する必要があります。
					規定値は Encoding::GetGetSystemMultiByteEncoding() です。
	*/
	void SetStandardInputEncoding(Encoding* encoding) LN_NOEXCEPT{ m_standardInputEncoding = encoding; }
		
	/**
		@brief		標準出力のエンコーディングを設定します。
		@details	Start() の前に設定する必要があります。
					規定値は Encoding::GetGetSystemMultiByteEncoding() です。
	*/
	void SetStandardOutputEncoding(Encoding* encoding) LN_NOEXCEPT { m_standardOutputEncoding = encoding; }

	/**
		@brief		標準エラー出力のエンコーディングを設定します。
		@details	Start() の前に設定する必要があります。
					規定値は Encoding::GetGetSystemMultiByteEncoding() です。
	*/
	void SetStandardErrorEncoding(Encoding* encoding) LN_NOEXCEPT{ m_standardErrorEncoding = encoding; }
		
	/**
		@brief		標準出力を Stream へリダイレクトする場合、リダイレクト先の Stream を設定します。
		@details	リダイレクト先の Stream が設定されている場合、GetStandardOutput() を呼び出すことはできません。
					Start() の前に設定する必要があります。
	*/
	void SetStandardOutputStream(Stream* stream) LN_NOEXCEPT{ m_standardOutputExternalStream = stream; }

#ifdef LN_CPP11
	/**
		@brief		標準出力に行が書き込まれたときに呼び出されるコールバック関数を設定します。
		@details	非同期読み取りは Start() の後、BeginOutputReadLine() を呼び出すことで開始します。
					コールバックは Start() を実行したスレッドとは別のスレッドから非同期的に呼び出されます。
	*/
	void SetOutputDataReceivedCallback(const Delegate<void(String)>& callback);

	/**
		@brief		標準エラー出力に行が書き込まれたときに呼び出されるコールバック関数を設定します。
		@details	非同期読み取りは Start() の後、BeginErrorReadLine() を呼び出すことで開始します。
					コールバックは Start() を実行したスレッドとは別のスレッドから非同期的に呼び出されます。
	*/
	void SetErrorDataReceivedCallback(const Delegate<void(String)>& callback);
#else
	/**
		@brief		標準出力に行が書き込まれたときに呼び出されるコールバック関数を設定します。
		@details	非同期読み取りは Start() の後、BeginOutputReadLine() を呼び出すことで開始します。
					コールバックは Start() を実行したスレッドとは別のスレッドから非同期的に呼び出されます。
	*/
	void SetOutputDataReceivedCallback(const Delegate01<String>& callback);

	/**
		@brief		標準エラー出力に行が書き込まれたときに呼び出されるコールバック関数を設定します。
		@details	非同期読み取りは Start() の後、BeginErrorReadLine() を呼び出すことで開始します。
					コールバックは Start() を実行したスレッドとは別のスレッドから非同期的に呼び出されます。
	*/
	void SetErrorDataReceivedCallback(const Delegate01<String>& callback);
#endif

	/**
		@brief		プログラムのファイルパスとコマンドライン引数を指定してプロセスを起動します。
		@param[in]	program	: プログラム名または実行ファイルパス
		@param[in]	args	: コマンドライン引数
		@exception	FileNotFoundException	program が見つからなかった。
	*/
	void start(const PathName& program, const String& args = String());

	/**
		@brief		プログラムのファイルパスとコマンドライン引数を指定してプロセスを起動します。
		@param[in]	program	: プログラム名または実行ファイルパス
		@param[in]	args	: コマンドライン引数
		@exception	FileNotFoundException	program が見つからなかった。
		@details	指定されたコマンドライン引数の各要素は、要素に空白が含まれる場合その前後に " を付加した状態でプロセスの起動に使用します。
	*/
	void start(const PathName& program, const StringArray& args);

	/**
		@brief		プロセスの終了を待機します。
		@param[in]	timeoutMSec	: タイムアウト時間 (ミリ秒)
		@return		true:正常終了  false:タイムアウトした
	*/
	bool WaitForExit(int timeoutMSec = -1);

	/**
		@brief		プロセスの状態を取得します。
		@attention	リダイレクトを非同期で行っている場合、ProcessStatus::Finished を返してもまだリダイレクトスレッドは終了していないことがあります。
					完了させるには WaitForExit() を呼び出してください。
	*/
	ProcessStatus GetState();

	/**
		@brief		プロセスの終了コードを取得します。
		@details	GetState() が ProcessStatus::Finished を返さない限り、戻り値は有効ではありません。
	*/
	int GetExitCode();

	/**
		@brief		標準出力の非同期読み取り操作を開始します。
	*/
	void BeginOutputReadLine();

	/**
		@brief		標準出力の非同期読み取り操作を開始します。
	*/
	void BeginErrorReadLine();

	/**
		@brief		標準入力の書き込みに使用されるストリームを取得します。
	*/
	StreamWriter* GetStandardInput() const;

	/**
		@brief		標準出力の読み取りに使用されるストリームを取得します。
		@details	この関数は同期的な読み取りを行う際に使用します。
					取得した StreamReader の ReadLine() や ReadToEnd() は、子プロセスが標準出力に書き込みを行うか、プロセスが終了するまで待機し、
					プロセスが終了することで EOF となります。
	*/
	StreamReader* GetStandardOutput() const;

	/**
		@brief		標準エラー出力の読み取りに使用されるストリームを取得します。
		@see		GetStandardOutput()
	*/
	StreamReader* GetStandardError() const;

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
	static int Execute(const PathName& program, const String& args = String(), String* outStdOutput = nullptr, String* outStdError = nullptr);

private:
	void Dispose();
	void Thread_ReadStdOutput();
	void Thread_ReadStdError();

	detail::ProcessImpl*		m_impl;
	PathName					m_workingDirectory;
	bool						m_redirectStandardInput;
	bool						m_redirectStandardOutput;
	bool						m_redirectStandardError;
	Encoding*					m_standardInputEncoding;
	Encoding*					m_standardOutputEncoding;
	Encoding*					m_standardErrorEncoding;
	Stream*						m_standardOutputExternalStream;
	RefPtr<StreamWriter>		m_standardInputWriter;
	RefPtr<StreamReader>		m_standardOutputReader;
	RefPtr<StreamReader>		m_standardErrorReader;

#ifdef LN_CPP11
	Delegate<void(String)>		m_outputDataReceivedCallback;
	Delegate<void(String)>		m_errorDataReceivedCallback;
#else
	Delegate01<String>			m_outputDataReceivedCallback;
	Delegate01<String>			m_errorDataReceivedCallback;
#endif
	DelegateThread				m_readStdOutputThread;
	DelegateThread				m_readStdErrorThread;
	bool						m_runningReadThread;
	bool						m_runningErrorReadThread;

#ifdef _WIN32
#else
    pid_t   m_pid;	// TODO:PImplにしたのでいらないかも
#endif
};


namespace detail {

struct ProcessStartInfo
{
	PathName				program;
	String					args;
	PathName				workingDirectory;
	bool					redirectStandardInput;
	bool					redirectStandardOutput;
	bool					redirectStandardError;
	Encoding*				standardInputEncoding;
	Encoding*				standardOutputEncoding;
	Encoding*				standardErrorEncoding;
};

struct ProcessStartResult
{
	RefPtr<StreamWriter>	standardInputWriter;
	RefPtr<StreamReader>	standardOutputReader;
	RefPtr<StreamReader>	standardErrorReader;
};
	
} // namespace detail

LN_NAMESPACE_END
