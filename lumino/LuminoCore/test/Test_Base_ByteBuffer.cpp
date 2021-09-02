#include "Common.hpp"
#include <LuminoCore/Base/Buffer.hpp>

class Test_Base_ByteBuffer : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Constructor)
{
	//* [ ] default constructor
	{
		ByteBuffer buf1;
		ASSERT_EQ(true, buf1.data() != nullptr);	// valid ptr (size == 0)
		ASSERT_EQ(0, buf1.size());
	}

	//* [ ] data copy
	{
		const char* data1 = "abc";
		ByteBuffer buf1(data1, 3);
		ASSERT_EQ(3, buf1.size());
		ASSERT_EQ('a', buf1[0]);
		ASSERT_EQ('b', buf1[1]);
		ASSERT_EQ('c', buf1[2]);
	}

	//* [ ] alloc empty memory
	{
		ByteBuffer buf1(2);
		ASSERT_EQ(2, buf1.size());
		ASSERT_EQ(0, buf1[0]);
		ASSERT_EQ(0, buf1[1]);
	}

	//* [ ] alloc 0byte
	{
		ByteBuffer buf1(0);
		ASSERT_EQ(0, buf1.size());
	}

	//* [ ] copy constructor
	{
		char buf[] = "abc";
		ByteBuffer buf1(buf, 3);
		ByteBuffer buf2(buf1);
		ASSERT_EQ(3, buf2.size());
		ASSERT_EQ('a', buf2.data()[0]);
		ASSERT_EQ('b', buf2.data()[1]);
		ASSERT_EQ('c', buf2.data()[2]);
		ASSERT_EQ(true, buf1.data() != buf2.data());
	}

	//* [ ] move constructor
	{
		char buf[] = "abc";
		ByteBuffer buf1(buf, 3);
		const byte_t* ptr = buf1.data();
		ByteBuffer buf2(std::move(buf1));
		ASSERT_EQ(ptr, buf2.data());
		ASSERT_EQ(true, buf1.isEmpty());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Assign)
{
	//* [ ] copy
	{
		char buf[] = "abc";
		ByteBuffer buf1(buf, 3);
		ByteBuffer buf2;
		buf2 = buf1;
		ASSERT_EQ(3, buf2.size());
		ASSERT_EQ('a', buf2.data()[0]);
		ASSERT_EQ('b', buf2.data()[1]);
		ASSERT_EQ('c', buf2.data()[2]);
		ASSERT_EQ(true, buf1.data() != buf2.data());

		char buf3[] = "d";
		ByteBuffer buf4(buf, 1);
		buf4 = buf1;
		ASSERT_EQ(3, buf4.size());
		ASSERT_EQ('a', buf4.data()[0]);
		ASSERT_EQ('b', buf4.data()[1]);
		ASSERT_EQ('c', buf4.data()[2]);
		ASSERT_EQ(true, buf1.data() != buf4.data());
	}

	//* [ ] self assign
	{
		char buf[] = "a";
		ByteBuffer buf1(buf, 1);
		buf1 = buf1;
		ASSERT_EQ(1, buf1.size());
		ASSERT_EQ('a', buf1.data()[0]);
	}

	//* [ ] move
	{
		char buf[] = "abc";
		ByteBuffer buf1(buf, 3);
		const byte_t* ptr = buf1.data();
		ByteBuffer buf2(buf, 2);
		buf2 = std::move(buf1);

		ASSERT_EQ(ptr, buf2.data());
		ASSERT_EQ(true, buf1.isEmpty());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, alloc)
{
	//* [ ] copy data
	const char* data1 = "ab";
	ByteBuffer buf1;
	buf1.assign(data1, 2);
	ASSERT_EQ(2, buf1.size());
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', buf1[1]);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, resize)
{
	//* [ ] shrinking
	{
		char data1[] = "ab";
		ByteBuffer buf1(data1, 2);
		const byte_t* ptr = buf1.data();

		buf1.resize(1);
		ASSERT_EQ(1, buf1.size());
		ASSERT_EQ('a', buf1[0]);
		ASSERT_EQ('b', (buf1.data())[1]);	// 再確保しないので元のが残っている

		// 元のサイズに戻す
		buf1.resize(2);
		ASSERT_EQ(2, buf1.size());
		ASSERT_EQ('a', buf1[0]);
		ASSERT_EQ('b', buf1[1]);			// 再確保しないので元のが残っている

		ASSERT_EQ(ptr, buf1.data());		// 再確保しない
	}

	//* [ ] swelling
	{
		char data1[] = "ab";
		ByteBuffer buf1(data1, 2);

		buf1.resize(3);
		ASSERT_EQ(3, buf1.size());
		ASSERT_EQ('a', buf1[0]);
		ASSERT_EQ('b', buf1[1]);			// 元のがコピーされている
		ASSERT_EQ(0, buf1[2]);				// 末尾は 0 クリアされている
	}

	//* [ ] shrinking -> swelling
	{
		char data1[] = "ab";
		ByteBuffer buf1(data1, 2);

		buf1.resize(1);
		buf1.resize(4);
		ASSERT_EQ('a', buf1[0]);	// 最初のは残っている
		ASSERT_EQ(0, buf1[1]);		// 末尾は 0 クリアされている
		ASSERT_EQ(0, buf1[2]);		// 末尾は 0 クリアされている
		ASSERT_EQ(0, buf1[3]);		// 末尾は 0 クリアされている
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Fill)
{
	ByteBuffer buf1(2);

	buf1.fill(1);
	ASSERT_EQ(1, buf1[0]);
	ASSERT_EQ(1, buf1[1]);

	buf1.fill(0);
	ASSERT_EQ(0, buf1[0]);
	ASSERT_EQ(0, buf1[1]);
}
