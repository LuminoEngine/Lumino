#include <TestConfig.h>
#include <Lumino/IO/Process.h>

#ifdef LN_USTRING
#else
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
		proc.start(_T("LuminoCore_Test"), _T("--proctest1"));
		proc.waitForExit();
		ASSERT_EQ(5, proc.getExitCode());
	}

	// <Example> 標準出力をリダイレクトする。
	{
		Process proc;
		proc.setRedirectStandardOutput(true);
		proc.start(_T("LuminoCore_Test"), _T("--proctest1"));
		ASSERT_EQ(_T("stdout"), proc.getStandardOutput()->readToEnd());
	}

	// <Example> 標準入力をリダイレクトする。
	{
		Process proc;
		proc.setRedirectStandardInput(true);
		proc.start(_T("LuminoCore_Test"), _T("--proctest2"));
		proc.getStandardInput()->write(_T("test\n"));	// 改行が必要
		proc.waitForExit();
		ASSERT_EQ(4, proc.getExitCode());				// 文字数が返ってくる
	}

	// <Example> 標準入力と標準出力をリダイレクトする。
	{
		Process proc;
		proc.setRedirectStandardInput(true);
		proc.setRedirectStandardOutput(true);
		proc.start(_T("LuminoCore_Test"), _T("--proctest3"));
		proc.getStandardInput()->write(_T("test\n"));	// 改行が必要
		ASSERT_EQ(_T("[test]"), proc.getStandardOutput()->readToEnd());
	}

	// <Example> プロセスをシンプルに実行する。
	{
		if (Process::execute(_T("LuminoCore_Test"), _T("--proctest1")) != 5) {
			FAIL();
		}
	}

	// <Example> プロセスをシンプルに実行し、標準出力をリダイレクトする。
	{
		String stdOutput;
		if (Process::execute(_T("LuminoCore_Test"), _T("--proctest1"), &stdOutput) == 5) {
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
		proc.setRedirectStandardOutput(true);
		proc.setOutputDataReceivedCallback(Delegate<void(String)>(&t, &Test::Callback));
		proc.start(_T("LuminoCore_Test"), _T("--proctest1"));
		proc.beginOutputReadLine();
		proc.waitForExit();
		ASSERT_EQ(_T("stdout"), output);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_Process, start)
{
	// <Illegal> プログラム名が見つからなかった。
	{
		Process proc;
		ASSERT_THROW(
			proc.start(_T("XXXX")),
			FileNotFoundException);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_Process, Issue)
{
	// <Issue> UTF8文字列が標準出力されたとき、文字コード変換エラーになっていた。
	{
		Process proc;
		proc.setRedirectStandardOutput(true);
		proc.setStandardOutputEncoding(Encoding::getUTF8Encoding());	// エンコーディングを明示することで回避する
		proc.start(_T("LuminoCore_Test"), _T("--proctest4_utf8"));
		String str = proc.getStandardOutput()->readToEnd();
		ASSERT_EQ(_T("あ"), str);
	}
}
#endif
