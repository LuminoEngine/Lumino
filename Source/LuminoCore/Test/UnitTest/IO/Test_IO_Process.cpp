#include <TestConfig.h>
#include <Lumino/IO/Process.h>

class Test_IO_Process : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_Process, Example)
{
	// <Example> リダイレクト無しで起動。
	{
		Process proc;
		proc.Start(_T("LuminoCore_Test"), _T("--proctest1"));
		proc.WaitForExit();
		ASSERT_EQ(5, proc.GetExitCode());
	}

	// <Example> 標準出力をリダイレクトする。
	{
		Process proc;
		proc.SetRedirectStandardOutput(true);
		proc.Start(_T("LuminoCore_Test"), _T("--proctest1"));
		ASSERT_EQ(_T("stdout"), proc.GetStandardOutput()->ReadToEnd());
	}

	// <Example> 標準入力をリダイレクトする。
	{
		Process proc;
		proc.SetRedirectStandardInput(true);
		proc.Start(_T("LuminoCore_Test"), _T("--proctest2"));
		proc.GetStandardInput()->Write(_T("test\n"));	// 改行が必要
		proc.WaitForExit();
		ASSERT_EQ(4, proc.GetExitCode());				// 文字数が返ってくる
	}

	// <Example> 標準入力と標準出力をリダイレクトする。
	{
		Process proc;
		proc.SetRedirectStandardInput(true);
		proc.SetRedirectStandardOutput(true);
		proc.Start(_T("LuminoCore_Test"), _T("--proctest3"));
		proc.GetStandardInput()->Write(_T("test\n"));	// 改行が必要
		ASSERT_EQ(_T("[test]"), proc.GetStandardOutput()->ReadToEnd());
	}

	// <Example> プロセスをシンプルに実行する。
	{
		if (Process::Execute(_T("LuminoCore_Test"), _T("--proctest1")) != 5) {
			FAIL();
		}
	}

	// <Example> プロセスをシンプルに実行し、標準出力をリダイレクトする。
	{
		String stdOutput;
		if (Process::Execute(_T("LuminoCore_Test"), _T("--proctest1"), &stdOutput) == 5) {
			ASSERT_EQ(_T("stdout"), stdOutput);
		}
		else {
			FAIL();
		}
	}

	// <Example> 標準出力をリダイレクトし、非同期で読み取る。
	{
		String output;

		struct Test {
			String* output;
			void Callback(String str) {
				(*output) += str;
			}
		} t;
		t.output = &output;

		Process proc;
		proc.SetRedirectStandardOutput(true);
		proc.SetOutputDataReceivedCallback(Delegate<void(String)>(&t, &Test::Callback));
		proc.Start(_T("LuminoCore_Test"), _T("--proctest1"));
		proc.BeginOutputReadLine();
		proc.WaitForExit();
		ASSERT_EQ(_T("stdout"), output);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_Process, Start)
{
	// <Illegal> プログラム名が見つからなかった。
	{
		Process proc;
		ASSERT_THROW(
			proc.Start(_T("XXXX")),
			FileNotFoundException);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_Process, Issue)
{
	// <Issue> UTF8文字列が標準出力されたとき、文字コード変換エラーになっていた。
	{
		Process proc;
		proc.SetRedirectStandardOutput(true);
		proc.SetStandardOutputEncoding(Encoding::GetUTF8Encoding());	// エンコーディングを明示することで回避する
		proc.Start(_T("LuminoCore_Test"), _T("--proctest4_utf8"));
		String str = proc.GetStandardOutput()->ReadToEnd();
		ASSERT_EQ(_T("あ"), str);
	}
}
