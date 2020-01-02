#include "Common.hpp"
#include "../src/Asset/AssetManager.hpp"
#include "../src/Asset/AssetArchive.hpp"

class Test_Asset_AssetArchive : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetArchive, CryptedAssetArchive)
{
	byte_t data1[1];
	byte_t data2[128];
	byte_t data3[129];
	byte_t data4[256];
	byte_t data5[384];
	for (int i = 0; i < LN_ARRAY_SIZE_OF(data1); i++) data1[i] = i;
	for (int i = 0; i < LN_ARRAY_SIZE_OF(data2); i++) data2[i] = i;
	for (int i = 0; i < LN_ARRAY_SIZE_OF(data3); i++) data3[i] = i;
	for (int i = 0; i < LN_ARRAY_SIZE_OF(data4); i++) data4[i] = i;
	for (int i = 0; i < LN_ARRAY_SIZE_OF(data5); i++) data5[i] = i;

	FileSystem::writeAllBytes(LN_TEMPFILE("data0"), data1, 0);
	FileSystem::writeAllBytes(LN_TEMPFILE("data1"), data1, LN_ARRAY_SIZE_OF(data1));
	FileSystem::writeAllBytes(LN_TEMPFILE("data2"), data2, LN_ARRAY_SIZE_OF(data2));
	FileSystem::writeAllBytes(LN_TEMPFILE("data3"), data3, LN_ARRAY_SIZE_OF(data3));
	FileSystem::writeAllBytes(LN_TEMPFILE("data4"), data4, LN_ARRAY_SIZE_OF(data4));
	FileSystem::writeAllBytes(LN_TEMPFILE("data5"), data5, LN_ARRAY_SIZE_OF(data5));

	detail::CryptedAssetArchiveWriter aw;
	aw.open(LN_TEMPFILE("test.lca"), u"pass");
	aw.addFile(LN_TEMPFILE("data0"), u"data0");
	aw.addFile(LN_TEMPFILE("data1"), u"data1");
	aw.addFile(LN_TEMPFILE("data2"), u"data2");
	aw.addFile(LN_TEMPFILE("data3"), u"data3");
	aw.addFile(LN_TEMPFILE("data4"), u"data4");
	aw.addFile(LN_TEMPFILE("data5"), u"data5");
	aw.close();

	{
		detail::CryptedAssetArchiveReader ar;
		ASSERT_EQ(true, ar.open(LN_TEMPFILE("test.lca"), u"pass", false));
		
		byte_t buf[512];

		// * [ ] 0 byte file
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data0"));
			ASSERT_EQ(0, s1->length());
			ASSERT_EQ(0, s1->read(buf, 1));
		}

		// * [ ] 1 byte file
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data1"));
			ASSERT_EQ(1, s1->length());
			ASSERT_EQ(1, s1->read(buf, 1));
			ASSERT_EQ(0, memcmp(buf, data1, 1));
		}

		// * [ ] 128 byte file
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data2"));
			ASSERT_EQ(128, s1->length());
			ASSERT_EQ(128, s1->read(buf, 128));
			ASSERT_EQ(0, memcmp(buf, data2, 128));
		}

		// * [ ] 129 byte file
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data3"));
			ASSERT_EQ(129, s1->length());
			ASSERT_EQ(129, s1->read(buf, 129));
			ASSERT_EQ(0, memcmp(buf, data3, 129));
		}

		// * [ ] 129 byte file (128 + 1)
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data3"));
			ASSERT_EQ(129, s1->length());
			ASSERT_EQ(128, s1->read(buf, 128));
			ASSERT_EQ(0, memcmp(buf, data3, 128));
			ASSERT_EQ(1, s1->read(buf, 2));
			ASSERT_EQ(data3[128], buf[0]);
		}

		// * [ ] 256 byte file
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data4"));
			ASSERT_EQ(256, s1->length());
			ASSERT_EQ(256, s1->read(buf, 256));
			ASSERT_EQ(0, memcmp(buf, data4, 256));
		}

		// * [ ] 256 byte file (cross boundary)
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data4"));
			ASSERT_EQ(256, s1->length());
			ASSERT_EQ(64, s1->read(buf, 64));
			ASSERT_EQ(128, s1->read(buf, 128));
			ASSERT_EQ(0, memcmp(buf, data4 + 64, 128));
		}

		// * [ ] 384 byte file (cross boundary)
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data5"));
			ASSERT_EQ(384, s1->length());
			ASSERT_EQ(64, s1->read(buf, 64));
			ASSERT_EQ(256, s1->read(buf, 256));
			ASSERT_EQ(0, memcmp(buf, data5 + 64, 256));
		}

		// * [ ] 384 byte file (by 1024)
		{
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data5"));
			ASSERT_EQ(384, s1->length());
			ASSERT_EQ(384, s1->read(buf, 1024));
			ASSERT_EQ(0, memcmp(buf, data5, 384));
		}

		// * [ ] odd and sequential
		{
			byte_t tbuf[384];
			auto s1 = ar.openFileStream(LN_TEMPFILE("test/data5"));
			size_t ofsset = 0;
			while (size_t size = s1->read(tbuf + ofsset, 55))
			{
				ofsset += size;
			}
			ASSERT_EQ(384, ofsset);
			ASSERT_EQ(0, memcmp(tbuf, data5, 384));
		}
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Asset_AssetArchive, AddAssetArchive)
{
#if 0   // TODO: 
    detail::EngineDomain::assetManager()->addAssetArchive(LN_ASSETFILE("test.lca"), u"pass");
	ASSERT_EQ(false, Assets::existsFile(u"dataX"));
	ASSERT_EQ(true, Assets::existsFile(u"data1"));

	auto buf = Assets::readAllBytes(u"data1");
	ASSERT_EQ(1, buf->size());
	ASSERT_EQ(0, buf->data()[0]);
#endif
}
