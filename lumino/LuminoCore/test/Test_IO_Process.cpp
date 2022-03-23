﻿#include "Common.hpp"
#include <LuminoCore/IO/Process.hpp>

//==============================================================================
//# Process
class Test_IO_Process : public ::testing::Test { };

//# simple run test
TEST_F(Test_IO_Process, SimpleRun)
{
	//* [ ] 
	{
		Process proc1;
		proc1.setProgram(ln::Environment::executablePath());
		proc1.setArguments({_T("proctest1")});
		proc1.start();
		ASSERT_EQ(true, proc1.wait());
		ASSERT_EQ(ProcessStatus::Finished, proc1.status());
		ASSERT_EQ(2, proc1.exitCode());
	}
}

//# redirect test
TEST_F(Test_IO_Process, Redirect)
{
	//* [ ] 
	{
		Process proc1(ln::Environment::executablePath(), { _T("proctest2") });
		StreamWriter* w1 = proc1.openStdin();
		StreamReader* r1 = proc1.openStdout();
		StreamReader* r2 = proc1.openStderr();
		proc1.start();
		w1->writeLine(_TT("test"));	// 改行が必要
		String sr1 = r1->readToEnd();
		String sr2 = r2->readToEnd();
		ASSERT_EQ(_LT("[test]"), sr1);
		ASSERT_EQ(_LT("err:test"), sr2);
	}
	//* [ ] UTF8文字列が標準出力されたとき、文字コード変換エラーになっていた。
	{
		Process proc(ln::Environment::executablePath(), { _T("proctest3") }, TextEncoding::utf8Encoding());
		StreamReader* r1 = proc.openStdout();
		proc.start();
		String str = r1->readToEnd();
		ASSERT_EQ(_LT("あ"), str);
	}
}

TEST_F(Test_IO_Process, SimpleRun2) {
    auto proc1 = ProcessCommand2(ln::Environment::executablePath())
		.arg(_T("proctest1"))
		.start();
    ASSERT_EQ(true, proc1->wait());
    ASSERT_EQ(ProcessStatus::Finished, proc1->status());
    ASSERT_EQ(2, proc1->exitCode());
}

TEST_F(Test_IO_Process, Redirect2) {
    auto proc1 = ProcessCommand2(ln::Environment::executablePath())
                     .arg(_T("proctest2"))
                     .stdIn(ProcessStdio::piped())
                     .stdOut(ProcessStdio::piped())
                     .stdErr(ProcessStdio::piped())
                     .start();
    StreamWriter* w1 = proc1->stdIn();
    StreamReader* r1 = proc1->stdOut();
    StreamReader* r2 = proc1->stdErr();
    w1->writeLine(_TT("test"));
    String sr1 = r1->readToEnd();
    String sr2 = r2->readToEnd();
    ASSERT_EQ(_LT("[test]"), sr1);
    ASSERT_EQ(_LT("err:test"), sr2);
}

TEST_F(Test_IO_Process, AsyncRedirect) {
    String out;
    String err;
    auto proc1 = ProcessCommand2(ln::Environment::executablePath())
                     .arg(_T("proctest2"))
                     .stdIn(ProcessStdio::piped())
                     .stdOut(ProcessStdio::piped())
                     .stdErr(ProcessStdio::piped())
                     .onStdOutReceived([&](String line) {
                         out += line;
                     })
                     .onStdErrReceived([&](String line) {
                         err += line;
                     })
                     .start();
    StreamWriter* w1 = proc1->stdIn();
    w1->writeLine(_TT("test"));
    proc1->wait();
    ASSERT_EQ(_LT("[test]"), out);
    ASSERT_EQ(_LT("err:test"), err);
}

TEST_F(Test_IO_Process, Redirect4_LongTime) {
    auto proc1 = ProcessCommand2(ln::Environment::executablePath())
                     .arg(_T("proctest4_LongTime"))
                     .stdOut(ProcessStdio::piped())
                     .stdErr(ProcessStdio::piped())
                     .start();
    StreamReader* r1 = proc1->stdOut();
    StreamReader* r2 = proc1->stdErr();
    String sr1 = r1->readToEnd();
    String sr2 = r2->readToEnd();
    proc1->wait();
    ASSERT_EQ(U"0123456789", sr1.replace(U"\r", U"").replace(U"\n", U""));
    ASSERT_EQ(U"9876543210", sr2.remove(U"\r").remove(U"\n"));
    ASSERT_EQ(10, proc1->exitCode());
}

TEST_F(Test_IO_Process, Redirect5_BigData) {
    auto proc1 = ProcessCommand2(ln::Environment::executablePath())
                     .arg(_T("proctest5_BigData"))
                     .stdOut(ProcessStdio::piped())
                     .stdErr(ProcessStdio::piped())
                     .start();
    StreamReader* r1 = proc1->stdOut();
    StreamReader* r2 = proc1->stdErr();
    String sr1 = r1->readToEnd();
    String sr2 = r2->readToEnd();
    proc1->wait();
    const auto lines = sr1.remove(U"\r").split(U"\n");
    ASSERT_EQ(51, lines.size());    // last is empty
    ASSERT_EQ(5, proc1->exitCode());
}
