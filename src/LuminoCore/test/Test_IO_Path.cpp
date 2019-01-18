#include "Common.hpp"
#include <LuminoCore/Base/Environment.hpp>
#include <LuminoCore/IO/Path.hpp>
#include "../src/IO/PathHelper.hpp"

//==============================================================================
//# Path
class Test_IO_Path : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//## Construct
TEST_F(Test_IO_Path, Construct)
{
	//* [ ] can convert from String
	{
		Path path = String("dir");
		ASSERT_EQ(_TT("dir"), path.str());
	}
	/*
	// <Test> 
	{
		Path base = _TT("dir1/dir2");
		Path path(base, _TT("../file1.txt"));
#ifdef LN_OS_WIN32
		ASSERT_EQ(_TT("dir1/dir2\\../file1.txt"), path.str());
#else
		ASSERT_EQ(_TT("dir1/dir2/../file1.txt"), path.str());
#endif
	}
	// <Test> 
	{
		Path path(_TT("a/"), _TT("b"));
		ASSERT_EQ(_TT("a/b"), path.str());

	}
	*/
}

//## concat
TEST_F(Test_IO_Path, Concat)
{
	//* [ ] construct with basepath
	{
		Path base = _TT("dir1/dir2");
		Path path(base, _TT("../file1.txt"));
#ifdef LN_OS_WIN32
		ASSERT_EQ(_TT("dir1/dir2\\../file1.txt"), path.str());
#else
		ASSERT_EQ(_TT("dir1/dir2/../file1.txt"), path.str());
#endif
	}
	//* [ ] with empty path
	{
		ASSERT_EQ(u"a/b.txt", Path(Path(), u"a/b.txt").str());
		ASSERT_EQ(u"a", Path(Path(u"a"), u"").str());
		ASSERT_EQ(u"", Path(Path(), u"").str());
	}

	//* [ ] combine
	{
		auto path = Path::combine(_T("dir1"), _T("dir2"), _T("file.txt"));
#ifdef LN_OS_WIN32
		ASSERT_EQ(_TT("dir1\\dir2\\file.txt"), path.str());
#else
		ASSERT_EQ(_TT("dir1/dir2/file.txt"), path.str());
#endif
	}
}

//## Queries
TEST_F(Test_IO_Path, Queries)
{
	//* [ ] check root path
	{
#ifdef LN_OS_WIN32
		ASSERT_EQ(true, Path(_TT("C:")).isRoot());
		ASSERT_EQ(true, Path(_TT("C:/")).isRoot());
		ASSERT_EQ(true, Path(_TT("C:\\")).isRoot());
		ASSERT_EQ(false, Path(_TT("file")).isRoot());
		ASSERT_EQ(false, Path(_TT("")).isRoot());
#else
		ASSERT_EQ(true, Path(_TT("/")).isRoot());
		ASSERT_EQ(false, Path(_TT("file")).isRoot());
		ASSERT_EQ(false, Path(_TT("")).isRoot());
#endif
	}
	//* [ ] check absolute path
	{
#ifdef LN_OS_WIN32
		ASSERT_EQ(true, Path(_TT("C:")).isAbsolute());
		ASSERT_EQ(true, Path(_TT("C:/")).isAbsolute());
		ASSERT_EQ(true, Path(_TT("C:\\")).isAbsolute());
		ASSERT_EQ(true, Path(_TT("C:/file")).isAbsolute());
		ASSERT_EQ(true, Path(_TT("C:\\file/file")).isAbsolute());
		ASSERT_EQ(false, Path(_TT("file")).isAbsolute());
		ASSERT_EQ(false, Path(_TT("")).isAbsolute());
#else
		ASSERT_EQ(true, Path(_TT("/")).isAbsolute());
		ASSERT_EQ(true, Path(_TT("/dir/file")).isAbsolute());
		ASSERT_EQ(false, Path(_TT("file")).isAbsolute());
		ASSERT_EQ(false, Path(_TT("dir/file")).isAbsolute());
		ASSERT_EQ(false, Path(_TT("")).isAbsolute());
#endif
	}
	//* [ ] check relative path
	{
#ifdef LN_OS_WIN32
		ASSERT_EQ(true, Path(_TT("dir\\file/file")).isRelative());
		ASSERT_EQ(false, Path(_TT("C:\\file/file")).isRelative());
#else
		ASSERT_EQ(true, Path(_TT("dir/file")).isRelative());
		ASSERT_EQ(false, Path(_TT("/dir/file")).isRelative());
#endif
	}
}

//## decompose components
TEST_F(Test_IO_Path, DecomposeComponents)
{
	//* [ ] get filename
	{
		Path path1(_TT("dir/file.txt"));
		ASSERT_EQ(0, Path::compare(_TT("file.txt"), path1.fileName()));

		Path path2(_TT("file.txt"));
		ASSERT_EQ(0, Path::compare(_TT("file.txt"), path2.fileName()));

		Path path3;
		ASSERT_EQ(true, path3.fileName().isEmpty());

		// http://en.cppreference.com/w/cpp/filesystem/path/filename
		ASSERT_EQ(_TT("bar.txt"), Path(_TT("/foo/bar.txt")).fileName().str());
		ASSERT_EQ(_TT(".bar"), Path(_TT("/foo/.bar")).fileName().str());
		ASSERT_EQ(_TT(""), Path(_TT("/foo/bar/")).fileName().str());	// different from std::filesystem::path
		ASSERT_EQ(_TT("."), Path(_TT("/foo/.")).fileName().str());
		ASSERT_EQ(_TT(".."), Path(_TT("/foo/..")).fileName().str());
		ASSERT_EQ(_TT("."), Path(_TT(".")).fileName().str());
		ASSERT_EQ(_TT(".."), Path(_TT("..")).fileName().str());
		ASSERT_EQ(_TT(""), Path(_TT("/")).fileName().str());	// different from std::filesystem::path
	}
	//* [ ] get WithoutExtension
	{
		Path path;

		path = _TT("C:/dir/file.txt");
		ASSERT_EQ(_TT("C:/dir/file"), path.withoutExtension().str());

		path = _TT("file.txt");
		ASSERT_EQ(_TT("file"), path.withoutExtension().str());

		path = _TT("file");
		ASSERT_EQ(_TT("file"), path.withoutExtension().str());

		path = _TT("");
		ASSERT_EQ(_TT(""), path.withoutExtension().str());

		path = _TT("C:/dir.sub/file");
		ASSERT_EQ(_TT("C:/dir.sub/file"), path.withoutExtension().str());

		path = _TT("dir/.git");
		ASSERT_EQ(_TT("dir/"), path.withoutExtension().str());

		path = _TT(".git");
		ASSERT_EQ(_TT(""), path.withoutExtension().str());
	}
	//* [ ] get extension
	{
		ASSERT_EQ(_TT(".txt"), Path(_TT("file.txt")).extension());
		ASSERT_EQ(_TT("txt"), Path(_TT("file.txt")).extension(false));
		ASSERT_EQ(_TT(".txt"), Path(_TT("file.tmp.txt")).extension());
		ASSERT_EQ(_TT(""), Path(_TT("file")).extension());
		ASSERT_EQ(_TT(""), Path(_TT("")).extension());
		ASSERT_EQ(_TT(""), Path(_TT(".")).extension());

		ASSERT_EQ(_TT(""), Path(_TT("dir.a/file")).extension());
		ASSERT_EQ(_TT(""), Path(_TT("file.")).extension());
		ASSERT_EQ(_TT(""), Path(_TT("..")).extension());
		ASSERT_EQ(_TT(""), Path(_TT("a/")).extension());
		ASSERT_EQ(_TT(""), Path(_TT("/")).extension());
	}
	//* [ ] get FileName Without Extension
	{
		ASSERT_EQ(_TT("file"), Path(_TT("file.txt")).fileNameWithoutExtension());
		ASSERT_EQ(_TT("file"), Path(_TT("dir/file.txt")).fileNameWithoutExtension());
		ASSERT_EQ(_TT("file"), Path(_TT("file")).fileNameWithoutExtension());
		ASSERT_EQ(_TT(""), Path(_TT(".file")).fileNameWithoutExtension());
		ASSERT_EQ(_TT(""), Path(_TT("dir/.file")).fileNameWithoutExtension());
		ASSERT_EQ(true, Path().fileNameWithoutExtension().isEmpty());
	}
	//* [ ] can get parent directory path
	{
#ifdef LN_OS_WINDOWS
		ASSERT_EQ(_TT("C:/dir1"), Path(_TT("C:/dir1/file1.txt")).parent().str());
		ASSERT_EQ(_TT("C:"), Path(_TT("C:/dir1")).parent().str());
		ASSERT_EQ(_TT("C:"), Path(_TT("C:/")).parent().str());
		ASSERT_EQ(_TT("C:"), Path(_TT("C:")).parent().str());
#endif
		ASSERT_EQ(_TT("/"), Path(_TT("/")).parent().str());
		ASSERT_EQ(_TT("dir"), Path(_TT("dir/")).parent().str());
		ASSERT_EQ(_TT(""), Path(_TT("file1.txt")).parent().str());
		ASSERT_EQ(_TT(".."), Path(_TT("")).parent().str());
		ASSERT_EQ(0, Path::compare(_TT("../.."), Path(_TT("..")).parent().str()));
		ASSERT_EQ(_TT(""), Path(_TT(".")).parent().str());
	}
	//* [ ] can check has extentions
	{
		ASSERT_EQ(true, Path(_TT("file.txt")).hasExtension());
		ASSERT_EQ(false, Path(_TT("file")).hasExtension());
		ASSERT_EQ(true, Path(_TT("file.txt")).hasExtension(_TT(".txt")));
		ASSERT_EQ(true, Path(_TT("file.txt")).hasExtension(_TT("txt")));
		ASSERT_EQ(false, Path(_TT("file.txt")).hasExtension(_TT(".t")));
		ASSERT_EQ(false, Path(_TT("file.txt")).hasExtension(_TT("t")));
		ASSERT_EQ(false, Path(_TT("file.txt")).hasExtension(_TT("txtt")));
		ASSERT_EQ(false, Path(_TT("")).hasExtension(_TT("txt")));
	}
	//* [ ] can replace extention
	{
		ASSERT_EQ(_TT("file.md"), Path(_TT("file.txt")).replaceExtension(_TT(".md")).str());
		ASSERT_EQ(_TT("dir/file.md"), Path(_TT("dir/file.txt")).replaceExtension(_TT("md")).str());
		ASSERT_EQ(_TT("/dir/file"), Path(_TT("/dir/file.txt")).replaceExtension(_TT("")).str());
		ASSERT_EQ(_TT("file.md"), Path(_TT("file")).replaceExtension(_TT("md")).str());
	}
}

//## convert to native path
TEST_F(Test_IO_Path, Native)
{
	//* [ ] can '/' to '\\' (windows)
	{
#ifdef LN_OS_WIN32
		Path path = _TT("dir/file");
		ASSERT_EQ(_TT("dir\\file"), path.native().str());
#endif
	}
}

//## compare
TEST_F(Test_IO_Path, Compare)
{
#ifdef LN_OS_WIN32
	Path path1(_LT("C:/dir/file.txt"));
	Path path2(_LT("C:\\dir\\file.txt"));
	ASSERT_EQ(0, Path::compare(path1, path2));
	//ASSERT_EQ(true, path1 == path2);
#else
	Path path1(_LT("/dir/file.txt"));
	Path path2(_LT("/dir/file.txt"));
	ASSERT_EQ(0, Path::compare(path1, path2));
	//ASSERT_EQ(true, path1 == path2);
#endif
}

//## append
TEST_F(Test_IO_Path, Append)
{
	//* [ ] can append
	{
		Path path = _TT("dir");
		path.append(_TT("file"));
		ASSERT_EQ(0, Path::compare(_TT("dir/file"), path));
	}
}

//## canonicalize
TEST_F(Test_IO_Path, canonicalizePath)
{
	//* [ ] can canonicalize path
	{
#define TEST_CASE(result, src) \
	{ \
		Char path1[256] = _LT(src); \
		Char path2[256]; \
		detail::PathTraits::canonicalizePath(path1, StringHelper::strlen(path1), path2, 256); \
		ASSERT_EQ(0,detail:: PathTraits::comparePathString(_LT(result), StringHelper::strlen(_LT(result)), path2, StringHelper::strlen(path2))); \
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
		Path path1(Environment::currentDirectory(), _LT("dir\\Dir"));
		Path path12 = _LT("dir/bin/../Dir");
		path12 = path12.canonicalize();
		ASSERT_EQ(path1.str(), path12.str());

		Path path2(_LT("C:\\file.txt"));
		Path path22 = _LT("C:\\dir/..\\file.txt");
		path22 = path22.canonicalize();
		ASSERT_EQ(path2.str(), path22.str());
#else
		Path path1(Environment::currentDirectory(), _LT("dir/Dir"));
		Path path12 = _LT("dir/bin/../Dir");
		path12 = path12.canonicalize();
		ASSERT_EQ(path1.str(), path12.str());

		Path path2(_LT("/file.txt"));
		Path path22 = _LT("/dir/../file.txt");
		path22 = path22.canonicalize();
		ASSERT_EQ(path2.str(), path22.str());
#endif

		// check overrun fixed
		Path(_T("file")).canonicalize();
	}
}

//## make relative path
TEST_F(Test_IO_Path, MakeRelative)
{
	//* [ ] パスが一致する場合は "." を返す
	{
		Path path1(_LT("d1/d2/d3"));
		Path path2(_LT("d1/d2/d3"));
		path1 = path1.canonicalize();
		path2 = path2.canonicalize();
		ASSERT_EQ(_LT("."), path1.makeRelative(path2).str());

		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalize();
			Path path2 = Path(_LT("d1/d2/d3")).canonicalize();
			ASSERT_EQ(_LT("."), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalize();
			Path path2 = Path(_LT("d1/d2/d3/")).canonicalize();
			ASSERT_EQ(_LT("."), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalize();
			Path path2 = Path(_LT("d1/d2/d3/")).canonicalize();
			ASSERT_EQ(_LT("."), path1.makeRelative(path2).str());
		}
	}
	//* [ ]  パスの末尾は / があっても無くても良い
	{
		Path path1(_LT("d1/d2/d3"));
		Path path2(_LT("d1/d2/d3"));
		path1 = path1.canonicalize();
		path2 = path2.canonicalize();
		ASSERT_EQ(_LT("."), path1.makeRelative(path2).str());
	}
	//* [ ]  1つ上のディレクトリへ戻る場合は ".." を返す
	{
		Path path1(_LT("d1/d2/d3/"));
		Path path2(_LT("d1/d2/"));
		path1 = path1.canonicalize();
		path2 = path2.canonicalize();
		ASSERT_EQ(_LT(".."), path1.makeRelative(path2).str());

		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalize();
			Path path2 = Path(_LT("d1/d2/")).canonicalize();
			ASSERT_EQ(_LT(".."), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalize();
			Path path2 = Path(_LT("d1/d2")).canonicalize();
			ASSERT_EQ(_LT(".."), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalize();
			Path path2 = Path(_LT("d1/d2")).canonicalize();
			ASSERT_EQ(_LT(".."), path1.makeRelative(path2).str());
		}
	}
	//* [ ]  2つ上のディレクトリへ戻る場合は "../.." を返す
	{
		Path path1 = Path(_LT("d1/d2/d3/")).canonicalize();
		Path path2 = Path(_LT("d1/")).canonicalize();
		ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).str());
		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalize();
			Path path2 = Path(_LT("d1/")).canonicalize();
			ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3/")).canonicalize();
			Path path2 = Path(_LT("d1")).canonicalize();
			ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/d3")).canonicalize();
			Path path2 = Path(_LT("d1")).canonicalize();
			ASSERT_EQ(_LT("../.."), path1.makeRelative(path2).str());
		}
	}
	//* [ ]  1つ上のディレクトリへ戻る場合は ".." を返す
	{
		Path path1 = Path(_LT("d1/d2/")).canonicalize();
		Path path2 = Path(_LT("d1/d2/d3/")).canonicalize();
		ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).str());
		// 末尾がセパレータのパターンを見る
		{
			Path path1 = Path(_LT("d1/d2")).canonicalize();
			Path path2 = Path(_LT("d1/d2/d3/")).canonicalize();
			ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2/")).canonicalize();
			Path path2 = Path(_LT("d1/d2/d3")).canonicalize();
			ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).str());
		}
		{
			Path path1 = Path(_LT("d1/d2")).canonicalize();
			Path path2 = Path(_LT("d1/d2/d3")).canonicalize();
			ASSERT_EQ(_LT("d3"), path1.makeRelative(path2).str());
		}
	}
	//* [ ]  2つ上のディレクトリへ戻る場合は "../.." を返す
	{
		Path path1 = Path(_LT("d1/")).canonicalize();
		Path path2 = Path(_LT("d1/d2/d3")).canonicalize();
		ASSERT_EQ(_LT("d2/d3"), path1.makeRelative(path2).str().replace(_LT("\\"), _LT("/")));
	}
	//* [ ] Issues
	{
		Path path1 = Path(_LT("Proj/Result/Proj1")).canonicalize();
		Path path2 = Path(_LT("Proj/Data/Source1.cpp")).canonicalize();
		auto t = path1.makeRelative(path2);
		ASSERT_EQ(_LT("../../Data/Source1.cpp"), path1.makeRelative(path2).str().replace(_LT("\\"), _LT("/")));
	}
}
