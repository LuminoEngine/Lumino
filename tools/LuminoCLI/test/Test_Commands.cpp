#include "Common.hpp"

class Test_Commands : public ::testing::Test
{
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
};

//------------------------------------------------------------------------------
TEST_F(Test_Commands, Basic)
{
	int result;

	result = Process::execute(TestEnv::LuminoCLI, { u"init", u"LuminoTest" });
	ASSERT_EQ(0, result);

	Process proc2;
	proc2.setProgram(TestEnv::LuminoCLI);
	proc2.setArguments({ u"build" });
	proc2.setWorkingDirectory(u"LuminoTest");
	proc2.start();
	proc2.wait();
	ASSERT_EQ(0, proc2.exitCode());
}

//------------------------------------------------------------------------------
TEST_F(Test_Commands, Fxc)
{
	Process proc2;
	proc2.setProgram(TestEnv::LuminoCLI);
	proc2.setArguments({ u"fxc", u"Assets/LineWave.fx", u"Assets/LineWave.lcfx" });
	proc2.setWorkingDirectory(TestEnv::LuminoTestProjectDir);
	proc2.start();
	proc2.wait();
	ASSERT_EQ(0, proc2.exitCode());
}
