#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/Base/Enumerable.h>

class Test_IO_FileSystem : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, exists)
{
	// Windows 別ユーザーフォルダは false
	//ASSERT_FALSE(FileSystem::Exists("C:/Users/user2/Documents/Visual Studio 2013/Settings/CurrentSettings.vssettings"));

	ASSERT_TRUE(FileSystem::existsFile(LN_LOCALFILE("TestData/test1.txt")));
	// ディレクトリは false
	ASSERT_FALSE(FileSystem::existsFile(LN_LOCALFILE("TestData")));
	// 空文字列は false
	ASSERT_FALSE(FileSystem::existsFile(_TT("")));
	// NULL は false
	ASSERT_FALSE(FileSystem::existsFile((const Char*)NULL));
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, getAttribute)
{
	FileAttribute attr;

	// <Test> ファイルを読み取り専用にする
	// <Test> ファイルの読み取り専用を確認する
	{
		attr = FileSystem::getAttribute(LN_LOCALFILE("TestData/readonly.txt"));
		attr |= FileAttribute::ReadOnly;
		FileSystem::setAttribute(LN_LOCALFILE("TestData/readonly.txt"), attr);
	}

	// <Test> ファイルの読み取り専用を確認する
	{
		attr = FileSystem::getAttribute(LN_LOCALFILE("TestData/readonly.txt"));
		ASSERT_EQ(FileAttribute::Normal | FileAttribute::ReadOnly, attr);
	}

	// <Test> ファイルの読み取り専用を解除する
	{
		attr = FileSystem::getAttribute(LN_LOCALFILE("TestData/readonly.txt"));
		attr &=~ FileAttribute::ReadOnly;
		FileSystem::setAttribute(LN_LOCALFILE("TestData/readonly.txt"), attr);

		// 確認
		attr = FileSystem::getAttribute(LN_LOCALFILE("TestData/readonly.txt"));
		ASSERT_EQ(FileAttribute::Normal, attr);
	}

	// <Test> ディレクトリ属性を確認する
	{
		attr = FileSystem::getAttribute(LN_LOCALFILE("TestData"));
		ASSERT_EQ(FileAttribute::Directory, attr);
	}

#ifdef LN_OS_WIN32
#else
	// 隠しファイル
	attr = FileSystem::GetAttribute(LN_LOCALFILE("TestData/.test"));
	ASSERT_EQ(FileAttribute::Hidden, attr);
#endif

	//try
	//{
	//	// 非常に長いパス
	//	String longPath(1000, _LT('a'));
	//	FileSystem::GetAttribute(longPath);
	//}
	//catch (Exception& e)
	//{
	//	e;
	//}
}


//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, Copy_Delete)
{
	String src1 = LN_LOCALFILE("TestData/test1.txt");
	String src2 = LN_LOCALFILE("TestData/test2.txt");
	String dest = TEMPFILE("test_copy.txt");
	uint64_t src1Size = FileSystem::getFileSize(src1.c_str());
	uint64_t src2Size = FileSystem::getFileSize(src2.c_str());

	// 最初はコピー先ファイルが無いこと。
	ASSERT_FALSE(FileSystem::existsFile(dest.c_str()));

	// コピー
	FileSystem::copyFile(src1.c_str(), dest.c_str(), false);

	// コピーしたファイル(サイズ)が同じ
	ASSERT_EQ(src1Size, FileSystem::getFileSize(dest.c_str()));

	// 上書きしようとすると IOException
	ASSERT_THROW(
		FileSystem::copyFile(src2.c_str(), dest.c_str(), false),
		IOException);

	// 上書き許可でコピー
	FileSystem::copyFile(src2.c_str(), dest.c_str(), true);

	// コピーしたファイル(サイズ)が同じ
	ASSERT_EQ(src2Size, FileSystem::getFileSize(dest.c_str()));

	// ファイル削除
	FileSystem::deleteFile(dest.c_str());

	// 消えている
	ASSERT_FALSE(FileSystem::existsFile(dest.c_str()));
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, createDirectory)
{
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/CreateDirectory"));
	FileSystem::writeAllText(TEMPFILE("Test_IO_FileSystem/CreateDirectory/file"), _LT("test"));
	FileSystem::deleteDirectory(TEMPFILE("Test_IO_FileSystem"), true);
	ASSERT_FALSE(FileSystem::existsDirectory(TEMPFILE("Test_IO_FileSystem")));	// 消えている
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, copyDirectory)
{
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/CopyDirectory"));
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/CopyDirectory/dir1"));
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/CopyDirectory/dir2"));
	FileSystem::writeAllText(TEMPFILE("Test_IO_FileSystem/CopyDirectory/file1.txt"), _LT("test"));
	FileSystem::writeAllText(TEMPFILE("Test_IO_FileSystem/CopyDirectory/dir2/file2.txt"), _LT("test"));

	FileSystem::copyDirectory(TEMPFILE("Test_IO_FileSystem/CopyDirectory"), TEMPFILE("Test_IO_FileSystem/CopyDirectory2"), true, true);

	ASSERT_EQ(true, FileSystem::existsDirectory(TEMPFILE("Test_IO_FileSystem/CopyDirectory2/dir1")));
	ASSERT_EQ(true, FileSystem::existsDirectory(TEMPFILE("Test_IO_FileSystem/CopyDirectory2/dir2")));
	ASSERT_EQ(true, FileSystem::existsFile(TEMPFILE("Test_IO_FileSystem/CopyDirectory2/file1.txt")));
	ASSERT_EQ(true, FileSystem::existsFile(TEMPFILE("Test_IO_FileSystem/CopyDirectory2/dir2/file2.txt")));
}

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, matchPath)
{
	ASSERT_EQ(true, FileSystem::matchPath(_TT("dir/file.txt"), _TT("*.txt")));
	ASSERT_EQ(false, FileSystem::matchPath(_TT("dir/file.txt"), _TT("*.dat")));
}


#ifdef LN_USTRING
#else
//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, forEachFilesInDirectory)
{
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/ForEachFilesInDirectory"));
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/ForEachFilesInDirectory/dir1"));
	FileSystem::createDirectory(TEMPFILE("Test_IO_FileSystem/ForEachFilesInDirectory/dir2"));
	FileSystem::writeAllText(TEMPFILE("Test_IO_FileSystem/ForEachFilesInDirectory/file1"), _LT("test"));
	FileSystem::writeAllText(TEMPFILE("Test_IO_FileSystem/ForEachFilesInDirectory/file2"), _LT("test"));

	List<PathName> list;
	FileSystem::forEachFilesInDirectory<Char>(TEMPFILE("Test_IO_FileSystem/ForEachFilesInDirectory"), [&list](const PathName& path) { list.add(path); });

	ASSERT_EQ(4, list.getCount());
	ASSERT_EQ(true, list[0].isAbsolute());
	ASSERT_EQ(_LT("dir1"), list[0].getFileName());
	ASSERT_EQ(true, list[1].isAbsolute());
	ASSERT_EQ(_LT("dir2"), list[1].getFileName());
	ASSERT_EQ(true, list[2].isAbsolute());
	ASSERT_EQ(_LT("file1"), list[2].getFileName());
	ASSERT_EQ(true, list[3].isAbsolute());
	ASSERT_EQ(_LT("file2"), list[3].getFileName());
}
#endif

//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, writeAllText)
{
	String text = _LT("abc");
	FileSystem::writeAllText(TEMPFILE("Test_IO_FileSystem/WriteAllText1"), text);
	ASSERT_EQ(true, TestHelper::equalFiles(TEMPFILE("Test_IO_FileSystem/WriteAllText1"), LN_LOCALFILE("TestData/WriteAllText1.txt")));
}




//
//namespace ln {
//namespace detail {
//
//
//} // namespace detail
//}


//------------------------------------------------------------------------------
TEST_F(Test_IO_FileSystem, getFiles)
{
	// <Test> ファイル列挙
	{
		auto files = FileSystem::getFiles(LN_LOCALFILE("TestData/Files2"));
		List<PathName> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(3, list.getCount());
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f1.a"); }));
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f2.b"); }));
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f3.a"); }));
	}
	// <Test> ファイル列挙(パターン指定)
	{
		auto files = FileSystem::getFiles(LN_LOCALFILE("TestData/Files2"), _LT("*.a"));
		List<PathName> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(2, list.getCount());
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f1.a"); }));
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f3.a"); }));
	}
	// <Test> ファイル列挙(パターン指定)
	{
		auto files = FileSystem::getFiles(LN_LOCALFILE("TestData/Files2"), _LT("*.b"));
		List<PathName> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(1, list.getCount());
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f2.b"); }));
	}
	// <Test> ファイル列挙(パターン指定)
	{
		auto files = FileSystem::getFiles(LN_LOCALFILE("TestData/Files2"), _LT("*.c"));
		List<PathName> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(0, list.getCount());
	}
	// <Test> 空フォルダの確認
	{
		PathName dir = TEMPFILE("Test_IO_FileSystem/getFiles");
		FileSystem::createDirectory(dir.c_str());
		auto files = FileSystem::getFiles(dir);
		List<PathName> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(0, list.getCount());
	}
	// <Test> ファイルだけ列挙されることの確認
	{
		auto files = FileSystem::getFiles(LN_LOCALFILE("TestData/Files3"));
		List<PathName> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(1, list.getCount());
		ASSERT_EQ(true, list.containsIf([](const PathName& path) { return path.getFileName() == _LT("f1.a"); }));
	}
	// <Test> . lead
	//{TODO:
	//	auto files = FileSystem::getFiles(LN_LOCALFILE("TestData/Files2"), _LT("*.test"));
	//	List<PathName> list;
	//	for (auto& path : files) list.add(path);
	//	ASSERT_EQ(1, list.getCount());
	//	ASSERT_EQ(true, list.contains([](const PathName& path) { return path.getFileName() == _LT(".test"); }));
	//}

	//detail::DirectoryIterator itr(LN_LOCALFILE("TestData"), _LT("*.dat"));
	//detail::DirectoryIterator end;
	//auto list = tr::MakeEnumerator::from(itr, end);
	////for (; itr != end; ++itr)
	////for (auto& path : list)
	////for (auto i2 = list.begin(); i2 != list.end(); ++i2)
	////{
	////	auto& path = *i2;
	////	printf("%s\n", itr->getFileName().c_str());
	////}
	//for (auto& path : list)
	//{
	//	printf("%s\n", path.getFileName().c_str());
	//}
}

