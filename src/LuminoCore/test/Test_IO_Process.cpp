#include "Common.hpp"
#include <Lumino/IO/Process.hpp>

//==============================================================================
//# Process
class Test_IO_Process : public ::testing::Test { };

//# simple run test
TEST_F(Test_IO_Process, SimpleRun)
{
	//* [ ] 
	{
		Process proc1;
		proc1.setProgram(_T("LuminoCore_Test"));
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
		Process proc1(_T("LuminoCore_Test"), { _T("proctest2") });
		StreamWriter* w1 = proc1.stdinWriter();
		StreamReader* r1 = proc1.stdoutReader();
		StreamReader* r2 = proc1.stderrReader();
		proc1.start();
		w1->writeLine("test");	// 改行が必要
		String sr1 = r1->readToEnd();
		String sr2 = r2->readToEnd();
		ASSERT_EQ(_LT("[test]"), sr1);
		ASSERT_EQ(_LT("err:test"), sr2);
	}
}
