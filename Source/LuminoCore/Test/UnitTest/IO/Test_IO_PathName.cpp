#include <TestConfig.h>
#include <Lumino/IO/FileSystem.h>

class Test_IO_Path : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(Test_IO_Path, Constructor)
{
	// <Test> String から変換できること
	{
		Path path = String("dir");
	}
	// <Test> 
	{
		Path base = _TT("dir1/dir2");
		Path path(base, _TT("../file1.txt"));
#ifdef LN_OS_WIN32
		ASSERT_EQ(_TT("dir1/dir2\\../file1.txt"), path.getString());
#else
		ASSERT_EQ(_TT("dir1/dir2/../file1.txt"), path.getString());
#endif
	}
	// <Test> 
	{
		Path path(_TT("a/"), _TT("b"));
		ASSERT_EQ(_TT("a/b"), path.getString());

	}
	// <Test> 空パスとの結合を確認する
	{
		Path path(Path(), _TT("a/b.txt"));
		ASSERT_EQ(_TT("a/b.txt"), path.getString());
	}
}

TEST_F(Test_IO_Path, getFileName)
{
	Path path1(_TT("dir/file.txt"));
	ASSERT_EQ(_TT("file.txt"), path1.getFileName());

	Path path2(_TT("file.txt"));
	ASSERT_EQ(_TT("file.txt"), path2.getFileName());
}

TEST_F(Test_IO_Path, getWithoutExtension)
{
	Path path;

	path = _TT("C:/dir/file.txt");
	ASSERT_EQ(_TT("C:/dir/file"), path.getWithoutExtension().getString());

	path = _TT("file.txt");
	ASSERT_EQ(_TT("file"), path.getWithoutExtension().getString());

	path = _TT("file");
	ASSERT_EQ(_TT("file"), path.getWithoutExtension().getString());

	path = _TT("");
	ASSERT_EQ(_TT(""), path.getWithoutExtension().getString());

	path = _TT("C:/dir.sub/file");
	ASSERT_EQ(_TT("C:/dir.sub/file"), path.getWithoutExtension().getString());

	path = _TT("dir/.git");
	ASSERT_EQ(_TT("dir/"), path.getWithoutExtension().getString());

	path = _TT(".git");
	ASSERT_EQ(_TT(""), path.getWithoutExtension().getString());
}

TEST_F(Test_IO_Path, getExtension)
{
	ASSERT_EQ(_TT(".txt"), Path(_TT("file.txt")).getExtension());
	ASSERT_EQ(_TT("txt"), Path(_TT("file.txt")).getExtension(false));
	ASSERT_EQ(_TT(".txt"), Path(_TT("file.tmp.txt")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("file")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT(".")).getExtension());

	ASSERT_EQ(_TT(""), Path(_TT("dir.a/file")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("file.")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("..")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("a/")).getExtension());
	ASSERT_EQ(_TT(""), Path(_TT("/")).getExtension());
}

TEST_F(Test_IO_Path, getFileNameWithoutExtension)
{
	ASSERT_EQ(_TT("file"), Path(_TT("dir/file.txt")).getFileNameWithoutExtension());
}

TEST_F(Test_IO_Path, compare)
{
#ifdef LN_OS_WIN32
	Path path1(_LT("C:/dir/file.txt"));
	Path path2(_LT("C:\\dir\\file.txt"));
	ASSERT_TRUE(path1.equals(path2));
	ASSERT_TRUE(path1 == path2);
#endif
}

TEST_F(Test_IO_Path, canonicalizePath)
{
	// いろいろなケースをテストしやすいようにマクロ化
#define TEST_CASE(result, src) \
	{ \
		Char path1[LN_MAX_PATH] = _LT(src); \
		Char path2[LN_MAX_PATH]; \
		PathTraits::canonicalizePath(path1, StringTraits::tcslen(path1), path2); \
		ASSERT_STREQ(_LT(result), path2); \
	}

	TEST_CASE("A/C", "A/B/../C");
	TEST_CASE("", "");
	TEST_CASE("B.txt", "A1/../A2/A3/../A4/../../B.txt");
	TEST_CASE("A2/B.txt", "A1/../A2/A3/../A4/../A5/../B.txt");
	TEST_CASE("A1/", "A1/");


	TEST_CASE("/", "./");
	TEST_CASE("/", "/.");
	TEST_CASE("", "A/..");
	TEST_CASE("/", "A/../");
	TEST_CASE("", "..");

	TEST_CASE("A/", "A//");		// .NET の動作。
	TEST_CASE("A/", "A////");	// .NET の動作。[A/][/][/][/] とみなし、[/] は破棄
	TEST_CASE("A/B", "A//B");

	TEST_CASE("A", "./A");
	TEST_CASE("A", "././A");

#ifdef LN_OS_WIN32
	TEST_CASE("C:/Projects/bin/Debug/", "C:/Projects/bin/Debug/");
	TEST_CASE("C:/Projects/Debug", "C:/../Projects/Debug");
	TEST_CASE("C:/Debug", "C:/Projects/../Debug");
	TEST_CASE("C:/Debug", "C:/../../Debug");		// .NET の動作。ルートの外側に出ても、.. が消えるだけでフォルダ名は残る
	TEST_CASE("C:/", "C:/");
#endif

#undef TEST_CASE
	//char path[LN_MAX_PATH] = "C:/../dir/file.txt";
	//size_t len = strlen(path);
	//bool r = CanonicalizePath(path, &len);


#ifdef LN_OS_WIN32
	Path path1(Path::getCurrentDirectory(), _LT("dir\\Dir"));
	Path path12 = _LT("dir/bin/../Dir");
	path12 = path12.canonicalizePath();
	ASSERT_EQ(path1.getString(), path12.getString());

	Path path2(_LT("C:\\file.txt"));
	Path path22 = _LT("C:\\dir/..\\file.txt");
	path22 = path22.canonicalizePath();
	ASSERT_EQ(path2.getString(), path22.getString());
#else
	Path path1(Path::getCurrentDirectory(), _LT("dir/Dir"));
	Path path12 = _LT("dir/bin/../Dir");
	path12 = path12.canonicalizePath();
	ASSERT_EQ(path1.getString(), path12.getString());

	Path path2(_LT("/file.txt"));
	Path path22 = _LT("/dir/../file.txt");
	path22 = path22.canonicalizePath();
	ASSERT_EQ(path2.getString(), path22.getString());
#endif
}

TEST_F(Test_IO_Path, getSpecialFolderPath)
{
	// 何が取れるかはすごく環境依存なので、取ったパスの先がフォルダであるかだけを確認しておく。

	// <Test> アプリケーションデータフォルダ
	{
		Path path1 = Path::getSpecialFolderPath(SpecialFolder::ApplicationData);
		ASSERT_TRUE(FileSystem::existsDirectory(path1.c_str()));
	}
	// <Test> 一時ファイルフォルダ
	{
		Path path1 = Path::getSpecialFolderPath(SpecialFolder::Temporary);
		ASSERT_TRUE(FileSystem::existsDirectory(path1.c_str()));
	}
}

TEST_F(Test_IO_Path, Unit_MakeRelative)
{
	// <Test> パスが一致する場合は "." を返す
	{
		Path path1(_LT("d1/d2/d3"));
		Path path2(_LT("d1/d2/d3"));
		path1 = path1.canonicalizePath();
		path2 = path2.canonicalizePath();
		ASSERT_EQ(_LT("."), path1.makeRelative(path2).getString());

		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/d3")).canonicalizePath();
			ASSERT_EQ(_LT("."), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			ASSERT_EQ(_LT("."), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			ASSERT_EQ(_LT("."), path1.makeRelative(path2).getString());
		}
	}
	// <Test> パスの末尾は / があっても無くても良い
	{
		Path path1(_LT("d1/d2/d3"));
		Path path2(_LT("d1/d2/d3"));
		path1 = path1.canonicalizePath();
		path2 = path2.canonicalizePath();
		ASSERT_EQ(_LT("."), path1.makeRelative(path2).getString());
	}
	// <Test> 1つ上のディレクトリへ戻る場合は ".." を返す
	{
		Path path1(_LT("d1/d2/d3/"));
		Path path2(_LT("d1/d2/"));
		path1 = path1.canonicalizePath();
		path2 = path2.canonicalizePath();
		ASSERT_EQ(_LT(".."), path1.makeRelative(path2).getString());

		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/")).canonicalizePath();
			ASSERT_EQ(_LT(".."), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2")).canonicalizePath();
			ASSERT_EQ(_LT(".."), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2")).canonicalizePath();
			ASSERT_EQ(_LT(".."), path1.makeRelative(path2).getString());
		}
	}
	// <Test> 2つ上のディレクトリへ戻る場合は "../.." を返す
	{
		Path path1 = Path(_LT("d1/d2/d3/")).canonicalizePath();
		Path path2 = Path(_LT("d1/")).canonicalizePath();
		ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).getString());
		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalizePath();
			Path path2 = Path(_LT("d1/")).canonicalizePath();
			ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			Path path2 = Path(_LT("d1")).canonicalizePath();
			ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalizePath();
			Path path2 = Path(_LT("d1")).canonicalizePath();
			ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).getString());
		}
	}
	// <Test> 1つ上のディレクトリへ戻る場合は ".." を返す
	{
		Path path1 = Path(_LT("d1/d2/")).canonicalizePath();
		Path path2 = Path(_LT("d1/d2/d3/")).canonicalizePath();
		ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).getString());
		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/d3/")).canonicalizePath();
			ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2/")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/d3")).canonicalizePath();
			ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).getString());
		}
		{
			Path path1 = Path(_LT("d1/d2")).canonicalizePath();
			Path path2 = Path(_LT("d1/d2/d3")).canonicalizePath();
			ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).getString());
		}
	}
	// <Test> 2つ上のディレクトリへ戻る場合は "../.." を返す
	{
		Path path1 = Path(_LT("d1/")).canonicalizePath();
		Path path2 = Path(_LT("d1/d2/d3")).canonicalizePath();
		ASSERT_EQ(_LT("d2/d3"), path1.makeRelative(path2).getString().replace(_LT("\\"), _LT("/")));
	}
}
