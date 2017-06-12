#include <TestConfig.h>
#include <Lumino/IO/MemoryStream.h>

class Test_IO_MemoryStream : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};
//------------------------------------------------------------------------------
TEST_F(Test_IO_MemoryStream, Variable)
{
	MemoryStreamPtr s = MemoryStream::create();
	s->write("test", 4);
	byte_t* buf = (byte_t*)s->GetBuffer();
	ASSERT_EQ('t', buf[0]);
	ASSERT_EQ('e', buf[1]);
	ASSERT_EQ('s', buf[2]);
	ASSERT_EQ('t', buf[3]);
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_MemoryStream, Fixed)
{
	byte_t buf[256];
	MemoryStreamPtr s = MemoryStream::create(buf, 256);
	//s.create(buf, 256);
	s->write("test", 4);
	ASSERT_EQ('t', buf[0]);
	ASSERT_EQ('e', buf[1]);
	ASSERT_EQ('s', buf[2]);
	ASSERT_EQ('t', buf[3]);
}

