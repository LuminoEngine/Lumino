#include "Common.hpp"
#include <LuminoCore/Base/DateTime.hpp>
#include <LuminoCore/Threading/Thread.hpp>
#include <LuminoCore/IO/FileInfo.hpp>
#include <LuminoCore/IO/FileSystem.hpp>

//==============================================================================
//# FileSystem
class Test_IO_FileSystem : public ::testing::Test {};

//## simple reading and writing test
TEST_F(Test_IO_FileSystem, SimpleReadWrite)
{
	//* [ ] can read/write all bytes.
	{
		byte_t data1[4] = { 'a', 'b', 'c', 'd' };
		FileSystem::writeAllBytes(LN_TEMPFILE("file1"), data1, 4);
		auto data2 = FileSystem::readAllBytes(LN_TEMPFILE("file1"));
		ASSERT_EQ(4, data2.size());
		ASSERT_EQ(0, memcmp(data2.data(), data1, 4));
		LN_EMSCRIPTEN_LAYZY_FLASH;
	}
	//* [ ] can read/write all text.
	{
		String str1 = _TT("efgh");
		FileSystem::writeAllText(LN_TEMPFILE("file2"), str1);
		String str2 = FileSystem::readAllText(LN_TEMPFILE("file2"));
		ASSERT_EQ(true, str1 == str2);
		LN_EMSCRIPTEN_LAYZY_FLASH;
	}
}

#if !defined(LN_EMSCRIPTEN)
//## file copy and delete
TEST_F(Test_IO_FileSystem, FileCopyDelete)
{
	String src1 = LN_TEMPFILE("Test_IO_FileSystem_FileCopyDelete1");
	String src2 = LN_TEMPFILE("Test_IO_FileSystem_FileCopyDelete2");
	String dest = LN_TEMPFILE("Test_IO_FileSystem_FileCopyDelete3");

	// create source
	FileSystem::writeAllText(src1.c_str(), _TT("a"));
	FileSystem::writeAllText(src2.c_str(), _TT("ab"));

	// 最初はコピー先ファイルを消しておく
	FileSystem::removeFile(dest);
	ASSERT_FALSE(FileSystem::existsFile(dest));

	//* [ ] コピー、コピーしたファイル(サイズ)が同じ
	FileSystem::copyFile(src1, dest, FileCopyOption::None);
	ASSERT_EQ(1, FileSystem::getFileSize(dest));

	//* [ ] 上書きしようとするが、変化なし
	//FileSystem::copyFile(src2, dest, false);
	//ASSERT_EQ(src1Size, FileSystem::getFileSize(dest));

	//* [ ] 上書き許可でコピー、コピーしたファイル(サイズ)が同じ
	FileSystem::copyFile(src2, dest, FileCopyOption::Overwrite);
	ASSERT_EQ(2, FileSystem::getFileSize(dest));

	//* [ ] ファイル削除
	FileSystem::removeFile(dest);
	ASSERT_FALSE(FileSystem::existsFile(dest));
}

//## directory create, copy and delete
TEST_F(Test_IO_FileSystem, DirectoryCreateCopyDelete)
{
	//* [ ] create directory
	FileSystem::createDirectory(LN_TEMPFILE("dir1"));
	ASSERT_EQ(true, FileSystem::existsDirectory(LN_TEMPFILE("dir1")));

	//* [ ] delete directory
	FileSystem::removeDirectory(LN_TEMPFILE("dir1"));
	ASSERT_EQ(false, FileSystem::existsDirectory(LN_TEMPFILE("dir1")));

	//* [ ] create directory (recursive)
	FileSystem::createDirectory(LN_TEMPFILE("dir1/dir2"));
	ASSERT_EQ(true, FileSystem::existsDirectory(LN_TEMPFILE("dir1/dir2")));

	//* [ ] delete directory (recursive)
	FileSystem::removeDirectory(LN_TEMPFILE("dir1"), true);
	ASSERT_EQ(false, FileSystem::existsDirectory(LN_TEMPFILE("dir1")));

	//* [ ] copy directory (recursive)
	{
		FileSystem::createDirectory(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory"));
		FileSystem::createDirectory(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory/dir1"));
		FileSystem::createDirectory(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory/dir2"));
		FileSystem::writeAllText(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory/file1.txt"), _LT("test"));
		FileSystem::writeAllText(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory/dir2/file2.txt"), _LT("test"));

		FileSystem::copyDirectory(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory"), LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory2"), true, true);

		ASSERT_EQ(true, FileSystem::existsDirectory(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory2/dir1")));
		ASSERT_EQ(true, FileSystem::existsDirectory(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory2/dir2")));
		ASSERT_EQ(true, FileSystem::existsFile(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory2/file1.txt")));
		ASSERT_EQ(true, FileSystem::existsFile(LN_TEMPFILE("Test_IO_FileSystem/CopyDirectory2/dir2/file2.txt")));

		FileSystem::removeDirectory(LN_TEMPFILE("Test_IO_FileSystem"), true);
		ASSERT_EQ(false, FileSystem::existsDirectory(LN_TEMPFILE("Test_IO_FileSystem")));
	}
}

//## check match file path by wildcard
TEST_F(Test_IO_FileSystem, matchPath)
{
	ASSERT_EQ(true, FileSystem::matchPath(_TT("file.txt"), _TT("*.txt")));
	ASSERT_EQ(false, FileSystem::matchPath(_TT("file.txt"), _TT("*.dat")));
	ASSERT_EQ(true, FileSystem::matchPath(_TT("dir/file.txt"), _TT("*.txt")));
	ASSERT_EQ(false, FileSystem::matchPath(_TT("dir/file.txt"), _TT("*.dat")));
}

//## get file list in directory
TEST_F(Test_IO_FileSystem, GetFilesOrDirectory)
{
	FileSystem::createDirectory(LN_TEMPFILE("GetFilesTest1"));
	FileSystem::createDirectory(LN_TEMPFILE("GetFilesTest1/dir2"));
	FileSystem::createDirectory(LN_TEMPFILE("GetFilesTest1/dir3"));
	FileSystem::createDirectory(LN_TEMPFILE("GetFilesTest1/dir3/dir4"));
	FileSystem::writeAllText(LN_TEMPFILE("GetFilesTest1/file1.txt"), _LT("test"));
	FileSystem::writeAllText(LN_TEMPFILE("GetFilesTest1/dir2/file2.md"), _LT("test"));
	FileSystem::writeAllText(LN_TEMPFILE("GetFilesTest1/dir3/file3.txt"), _LT("test"));
	FileSystem::writeAllText(LN_TEMPFILE("GetFilesTest1/dir3/file4.dat"), _LT("test"));

	//* [ ] file enumeration
	{
		auto pathes = FileSystem::getFiles(LN_TEMPFILE("GetFilesTest1/dir3"), StringRef(), SearchOption::TopDirectoryOnly);
		List<Path> list;
		for (auto& path : pathes) list.add(path);
		ASSERT_EQ(2, list.size());
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file3.txt")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file4.dat")); }));
	}

	//* [ ] file enumeration (pattern)
	{
		auto pathes = FileSystem::getFiles(LN_TEMPFILE("GetFilesTest1/dir3"), _LT("*.txt"), SearchOption::TopDirectoryOnly);
		List<Path> list;
		for (auto& path : pathes) list.add(path);
		ASSERT_EQ(1, list.size());
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file3.txt")); }));
	}

	//* [ ] file enumeration (recursive)
	{
		auto pathes = FileSystem::getFiles(LN_TEMPFILE("GetFilesTest1"), StringRef(), SearchOption::Recursive);
		List<Path> list;
		for (auto& path : pathes) list.add(path);
		ASSERT_EQ(4, list.size());
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file1.txt")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file2.md")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file3.txt")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file4.dat")); }));
	}

	//* [ ] file enumeration (pattern, recursive)
	{
		auto files = FileSystem::getFiles(LN_TEMPFILE("GetFilesTest1"), _LT("*.txt"), SearchOption::Recursive);
		List<Path> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(2, list.size());
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file1.txt")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("file3.txt")); }));
	}

	//* [ ] check empty directory (recursive)
	{
		auto files = FileSystem::getFiles(LN_TEMPFILE("GetFilesTest1/dir3/dir4"), StringRef(), SearchOption::Recursive);
		List<Path> list;
		for (auto& path : files) list.add(path);
		ASSERT_EQ(0, list.size());
	}

	//* [ ] directory enumeration
	{
		auto pathes = FileSystem::getDirectories(LN_TEMPFILE("GetFilesTest1"), StringRef(), SearchOption::TopDirectoryOnly);
		List<Path> list;
		for (auto& path : pathes) list.add(path);
		ASSERT_EQ(2, list.size());
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("dir2")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("dir3")); }));
	}

	//* [ ] directory enumeration (recursive)
	{
		auto pathes = FileSystem::getDirectories(LN_TEMPFILE("GetFilesTest1"), StringRef(), SearchOption::Recursive);
		List<Path> list;
		for (auto& path : pathes) list.add(path);
		ASSERT_EQ(3, list.size());
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("dir2")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("dir3")); }));
		ASSERT_EQ(true, list.containsIf([](const Path& path) { return path.str().contains(_TT("dir4")); }));
	}

	FileSystem::removeDirectory(LN_TEMPFILE("GetFilesTest1"), true);
}

//## check file update.
TEST_F(Test_IO_FileSystem, LastModifiedTime)
{
	FileSystem::writeAllText(LN_TEMPFILE("test"), u"test1");
	DateTime t1 = FileInfo(LN_TEMPFILE("test")).lastModifiedTime();

	Thread::sleep(2000);

	DateTime t2 = FileInfo(LN_TEMPFILE("test")).lastModifiedTime();

	Thread::sleep(2000);

	FileSystem::writeAllText(LN_TEMPFILE("test"), u"test2");
	DateTime t3 = FileInfo(LN_TEMPFILE("test")).lastModifiedTime();

	ASSERT_EQ(true, t1 == t2);
	ASSERT_EQ(true, t1 < t3);
}

#endif

