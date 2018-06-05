#include "Common.hpp"
#include <Lumino/Base/Buffer.hpp>

class Test_Base_ByteBuffer : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Constructor)
{
	//* [ ] default constructor
	{
		ByteBuffer2 buf1;
		ASSERT_EQ(true, buf1.data() != nullptr);	// valid ptr (size == 0)
		ASSERT_EQ(0, buf1.size());
	}

	//* [ ] data copy
	{
		const char* data1 = "abc";
		ByteBuffer2 buf1(data1, 3);
		ASSERT_EQ(3, buf1.size());
		ASSERT_EQ('a', buf1[0]);
		ASSERT_EQ('b', buf1[1]);
		ASSERT_EQ('c', buf1[2]);
	}

	//* [ ] alloc empty memory
	{
		ByteBuffer2 buf1(2);
		ASSERT_EQ(2, buf1.size());
		ASSERT_EQ(0, buf1[0]);
		ASSERT_EQ(0, buf1[1]);
	}

	//* [ ] alloc 0byte
	{
		ByteBuffer2 buf1(0);
		ASSERT_EQ(0, buf1.size());
	}

	//* [ ] copy constructor
	{
		char buf[] = "abc";
		ByteBuffer2 buf1(buf, 3);
		ByteBuffer2 buf2(buf1);
		ASSERT_EQ(3, buf2.size());
		ASSERT_EQ('a', buf2.data()[0]);
		ASSERT_EQ('b', buf2.data()[1]);
		ASSERT_EQ('c', buf2.data()[2]);
		ASSERT_EQ(true, buf1.data() != buf2.data());
	}

	//* [ ] move constructor
	{
		char buf[] = "abc";
		ByteBuffer2 buf1(buf, 3);
		const byte_t* ptr = buf1.data();
		ByteBuffer2 buf2(std::move(buf1));
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
		ByteBuffer2 buf1(buf, 3);
		ByteBuffer2 buf2;
		buf2 = buf1;
		ASSERT_EQ(3, buf2.size());
		ASSERT_EQ('a', buf2.data()[0]);
		ASSERT_EQ('b', buf2.data()[1]);
		ASSERT_EQ('c', buf2.data()[2]);
		ASSERT_EQ(true, buf1.data() != buf2.data());

		char buf3[] = "d";
		ByteBuffer2 buf4(buf, 1);
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
		ByteBuffer2 buf1(buf, 1);
		buf1 = buf1;
		ASSERT_EQ(1, buf1.size());
		ASSERT_EQ('a', buf1.data()[0]);
	}

	//* [ ] move
	{
		char buf[] = "abc";
		ByteBuffer2 buf1(buf, 3);
		const byte_t* ptr = buf1.data();
		ByteBuffer2 buf2(buf, 2);
		buf2 = std::move(buf1);

		ASSERT_EQ(ptr, buf2.data());
		ASSERT_EQ(true, buf1.isEmpty());
	}
}
//
////------------------------------------------------------------------------------
//TEST_F(Test_Base_ByteBuffer, CopyInstance)
//{
//	char buf[] = "abc";
//
//	// <Test> デフォルトの空バッファをコピーコンストラクタで共有
//	{
//		ByteBuffer buf1;
//		ByteBuffer buf2 = buf1;
//		ASSERT_EQ(buf1.getData(), buf2.getData());
//	}
//	// <Test> デフォルトの空バッファをoperator = で共有
//	{
//		ByteBuffer buf1, buf2;
//		buf2 = buf1;
//		ASSERT_EQ(buf1.getData(), buf2.getData());
//	}
//	// <Test> 非 const GetData() の呼び出しで再確保される。
//	{
//		ByteBuffer buf1(buf, 3);
//		ByteBuffer buf2 = buf1;
//		buf1.getData();
//		ASSERT_NE(buf1.getData(), buf2.getData());	// 再確保が走ってしまうがやむなし。
//	}
//	// <Test> const GetData() の暗黙的呼び出しで再確保されない。
//	{
//		const ByteBuffer buf1(buf, 3);
//		const ByteBuffer buf2 = buf1;
//		ASSERT_EQ(buf1.getData(), buf2.getData());	// 再確保は走らない。
//	}
//	// <Test> バッファをコピーコンストラクタで共有
//	{
//		const ByteBuffer buf1(buf, 3);
//		const ByteBuffer buf2 = buf1;
//		ASSERT_EQ(buf1.getData(), buf2.getData());
//		ASSERT_EQ(3, buf1.getSize());
//		ASSERT_EQ(3, buf2.getSize());
//		ASSERT_TRUE(memcmp(buf2.getData(), buf, 3) == 0);
//	}
//	// <Test> バッファをoperator = で共有
//	{
//		const ByteBuffer buf1(buf, 3);
//		ByteBuffer buf2;
//		buf2 = buf1;
//		ASSERT_EQ(buf1.getConstData(), buf2.getConstData());
//		ASSERT_EQ(3, buf1.getSize());
//		ASSERT_EQ(3, buf2.getSize());
//		ASSERT_TRUE(memcmp(buf2.getData(), buf, 3) == 0);
//	}
//	// <Test> 共有後、Resize するとメモリは再配置されるが内容は維持される。
//	{
//		ByteBuffer buf1(buf, 3);
//		ByteBuffer buf2 = buf1;
//		const byte_t* ptr = buf2.getConstData();
//		buf2.resize(100);
//		ASSERT_EQ(100, buf2.getSize());
//		ASSERT_NE(ptr, buf2.getConstData());
//		ASSERT_TRUE(memcmp(buf2.getConstData(), buf, 3) == 0);
//	}
//	// <Test> 自己代入できる。
//	{
//		ByteBuffer buf1(buf, 3);
//		buf1 = buf1;
//		ASSERT_EQ(3, buf1.getSize());
//		ASSERT_TRUE(memcmp(buf1.getConstData(), buf, 3) == 0);
//	}
//	// <Test> 共有されていなければ GetData() を読んでも再配置は行われない。
//	{
//		ByteBuffer buf1(buf, 3);
//		const byte_t* ptr = buf1.getConstData();
//		buf1.getData();
//		ASSERT_EQ(ptr, buf1.getConstData());
//	}
//}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, alloc)
{
	//// Alloc + 0クリア
	//ByteBuffer buf1;
	//buf1.alloc(5);
	//ASSERT_EQ(0, buf1[0]);
	//ASSERT_EQ(5, buf1.getSize());

	//// 再確保
	//buf1.alloc(10, false);
	//ASSERT_EQ(10, buf1.getSize());

	//* [ ] copy data
	const char* data1 = "ab";
	ByteBuffer2 buf1;
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
		ByteBuffer2 buf1(data1, 2);
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
		ByteBuffer2 buf1(data1, 2);

		buf1.resize(3);
		ASSERT_EQ(3, buf1.size());
		ASSERT_EQ('a', buf1[0]);
		ASSERT_EQ('b', buf1[1]);			// 元のがコピーされている
		ASSERT_EQ(0, buf1[2]);				// 末尾は 0 クリアされている
	}

	//* [ ] shrinking -> swelling
	{
		char data1[] = "ab";
		ByteBuffer2 buf1(data1, 2);

		buf1.resize(1);
		buf1.resize(4);
		ASSERT_EQ('a', buf1[0]);	// 最初のは残っている
		ASSERT_EQ(0, buf1[1]);		// 末尾は 0 クリアされている
		ASSERT_EQ(0, buf1[2]);		// 末尾は 0 クリアされている
		ASSERT_EQ(0, buf1[3]);		// 末尾は 0 クリアされている
	}
}

////------------------------------------------------------------------------------
//TEST_F(Test_Base_ByteBuffer, attach)
//{
//	char data2[2];
//	ByteBuffer buf1;
//	buf1.attach(data2, 2);
//	buf1.copy("ab", 2);
//	ASSERT_EQ(2, buf1.getSize());
//	ASSERT_TRUE(TestHelper::checkArrays(data2, buf1.getData(), 2));
//	ASSERT_EQ('a', buf1[0]);
//	ASSERT_EQ('b', buf1[1]);
//	ASSERT_EQ('a', data2[0]);	// 元バッファにも書き込まれている
//	ASSERT_EQ('b', data2[1]);	// 元バッファにも書き込まれている
//}
//
//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Fill)
{
	ByteBuffer2 buf1(2);

	buf1.fill(1);
	ASSERT_EQ(1, buf1[0]);
	ASSERT_EQ(1, buf1[1]);

	buf1.fill(0);
	ASSERT_EQ(0, buf1[0]);
	ASSERT_EQ(0, buf1[1]);
}
