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

}
