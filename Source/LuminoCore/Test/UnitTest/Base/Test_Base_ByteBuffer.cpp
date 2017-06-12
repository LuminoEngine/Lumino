#include <TestConfig.h>

class Test_Base_ByteBuffer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};


//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Constructor)
{
	// デフォルト
	ByteBuffer buf1;
	ASSERT_TRUE(buf1.getData() != NULL);
	ASSERT_EQ(0, buf1.getSize());

	// サイズ指定 + 0 クリア
	ByteBuffer buf2(10);
	ASSERT_EQ(0, *((byte_t*)buf2.getData()));
	ASSERT_EQ(10, buf2.getSize());

	// サイズ指定
	ByteBuffer buf3(1, false);
	ASSERT_EQ(1, buf3.getSize());

	// データコピー
	const char* data1 = "test";
	ByteBuffer buf4(data1, 4);
	ASSERT_EQ(4, buf4.getSize());
	ASSERT_EQ('t', buf4[0]);
	ASSERT_EQ('e', buf4[1]);
	ASSERT_EQ('s', buf4[2]);
	ASSERT_EQ('t', buf4[3]);

	// メモリバッファ参照
	char data2[2];
	ByteBuffer buf5(data2, 2, true);
	buf5.copy("ab", 2);
	ASSERT_EQ(2, buf5.getSize());
	ASSERT_TRUE(TestHelper::CheckArrays(data2, buf5.getData(), 2));
	ASSERT_EQ('a', buf5[0]);
	ASSERT_EQ('b', buf5[1]);
	ASSERT_EQ('a', data2[0]);	// 元バッファにも書き込まれている
	ASSERT_EQ('b', data2[1]);	// 元バッファにも書き込まれている

	// データコピー
	char data6[5] = "test";
	ByteBuffer buf6(data6, 4, false);
	ASSERT_EQ(4, buf6.getSize());
	ASSERT_EQ('t', buf6[0]);
	ASSERT_EQ('e', buf6[1]);
	ASSERT_EQ('s', buf6[2]);
	ASSERT_EQ('t', buf6[3]);

	// <Test> 0byte 確保
	{
		ByteBuffer buf((size_t)0);
		ASSERT_TRUE(buf1.getData() != NULL);
		ASSERT_EQ(0, buf1.getSize());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, CopyInstance)
{
	char buf[] = "abc";

	// <Test> デフォルトの空バッファをコピーコンストラクタで共有
	{
		ByteBuffer buf1;
		ByteBuffer buf2 = buf1;
		ASSERT_EQ(buf1.getData(), buf2.getData());
	}
	// <Test> デフォルトの空バッファをoperator = で共有
	{
		ByteBuffer buf1, buf2;
		buf2 = buf1;
		ASSERT_EQ(buf1.getData(), buf2.getData());
	}
	// <Test> 非 const GetData() の呼び出しで再確保される。
	{
		ByteBuffer buf1(buf, 3);
		ByteBuffer buf2 = buf1;
		buf1.getData();
		ASSERT_NE(buf1.getData(), buf2.getData());	// 再確保が走ってしまうがやむなし。
	}
	// <Test> const GetData() の暗黙的呼び出しで再確保されない。
	{
		const ByteBuffer buf1(buf, 3);
		const ByteBuffer buf2 = buf1;
		ASSERT_EQ(buf1.getData(), buf2.getData());	// 再確保は走らない。
	}
	// <Test> バッファをコピーコンストラクタで共有
	{
		const ByteBuffer buf1(buf, 3);
		const ByteBuffer buf2 = buf1;
		ASSERT_EQ(buf1.getData(), buf2.getData());
		ASSERT_EQ(3, buf1.getSize());
		ASSERT_EQ(3, buf2.getSize());
		ASSERT_TRUE(memcmp(buf2.getData(), buf, 3) == 0);
	}
	// <Test> バッファをoperator = で共有
	{
		const ByteBuffer buf1(buf, 3);
		ByteBuffer buf2;
		buf2 = buf1;
		ASSERT_EQ(buf1.getConstData(), buf2.getConstData());
		ASSERT_EQ(3, buf1.getSize());
		ASSERT_EQ(3, buf2.getSize());
		ASSERT_TRUE(memcmp(buf2.getData(), buf, 3) == 0);
	}
	// <Test> 共有後、Resize するとメモリは再配置されるが内容は維持される。
	{
		ByteBuffer buf1(buf, 3);
		ByteBuffer buf2 = buf1;
		const byte_t* ptr = buf2.getConstData();
		buf2.resize(100);
		ASSERT_EQ(100, buf2.getSize());
		ASSERT_NE(ptr, buf2.getConstData());
		ASSERT_TRUE(memcmp(buf2.getConstData(), buf, 3) == 0);
	}
	// <Test> 自己代入できる。
	{
		ByteBuffer buf1(buf, 3);
		buf1 = buf1;
		ASSERT_EQ(3, buf1.getSize());
		ASSERT_TRUE(memcmp(buf1.getConstData(), buf, 3) == 0);
	}
	// <Test> 共有されていなければ GetData() を読んでも再配置は行われない。
	{
		ByteBuffer buf1(buf, 3);
		const byte_t* ptr = buf1.getConstData();
		buf1.getData();
		ASSERT_EQ(ptr, buf1.getConstData());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, alloc)
{
	// Alloc + 0クリア
	ByteBuffer buf1;
	buf1.alloc(5);
	ASSERT_EQ(0, buf1[0]);
	ASSERT_EQ(5, buf1.getSize());

	// 再確保
	buf1.alloc(10, false);
	ASSERT_EQ(10, buf1.getSize());

	// データコピー
	const char* data1 = "ab";
	buf1.alloc(data1, 2);
	ASSERT_EQ(2, buf1.getSize());
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', buf1[1]);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, resize)
{
	// 縮退
	char data1[] = "ab";
	ByteBuffer buf1(data1, 2);
	buf1.resize(1);
	ASSERT_EQ(1, buf1.getSize());
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', ((byte_t*)buf1.getData())[1]);	// 再確保しないので元のが残っているはず

	// 元のサイズに戻す
	buf1.resize(2);
	ASSERT_EQ(2, buf1.getSize());
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', buf1[1]);	// 再確保しないので元のが残っているはず

	// 伸長
	buf1.resize(3);
	ASSERT_EQ(3, buf1.getSize());
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', buf1[1]);	// 元のがコピーされている
	ASSERT_EQ(0, buf1[2]);		// 末尾は 0 クリアされている

	// 縮退 → 伸長
	buf1.resize(1);
	buf1.resize(4);
	ASSERT_EQ('a', buf1[0]);	// 最初のは残っている
	ASSERT_EQ(0, buf1[1]);		// 末尾は 0 クリアされている
	ASSERT_EQ(0, buf1[2]);		// 末尾は 0 クリアされている
	ASSERT_EQ(0, buf1[3]);		// 末尾は 0 クリアされている
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, attach)
{
	char data2[2];
	ByteBuffer buf1;
	buf1.attach(data2, 2);
	buf1.copy("ab", 2);
	ASSERT_EQ(2, buf1.getSize());
	ASSERT_TRUE(TestHelper::CheckArrays(data2, buf1.getData(), 2));
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', buf1[1]);
	ASSERT_EQ('a', data2[0]);	// 元バッファにも書き込まれている
	ASSERT_EQ('b', data2[1]);	// 元バッファにも書き込まれている
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ByteBuffer, Copy_Clear)
{
	// 通常
	ByteBuffer buf1(2);
	buf1.copy("ab", 2);
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('b', buf1[1]);

	// オフセット指定
	buf1.copy(1, "c", 1);
	ASSERT_EQ('a', buf1[0]);
	ASSERT_EQ('c', buf1[1]);

	// 0 クリア
	buf1.clear();
	ASSERT_EQ(0, buf1[0]);
	ASSERT_EQ(0, buf1[1]);
}
