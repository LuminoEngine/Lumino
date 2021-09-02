#include "Common.hpp"
#include <LuminoCore/IO/FileSystem.hpp>
#include <LuminoCore/IO/CommandLineParser.hpp>

//==============================================================================
//# CommandLineParser
class Test_IO_CommandLineParser : public ::testing::Test { };

//# Requested defaults
TEST_F(Test_IO_CommandLineParser, RequestedDefaults)
{
	//# CommandLineOption is Optional
	{
		ln::CommandLineParser parser;
		auto* forceOption = parser.addFlagOption(u"f", u"force", u"force operation.");
		auto* sizeOption = parser.addValueOption(u"", u"size", u"buffer size.");
		ASSERT_EQ(true, testFlag(forceOption->flags(), CommandLineOptionFlags::Optional));
		ASSERT_EQ(true, testFlag(sizeOption->flags(), CommandLineOptionFlags::Optional));
	}
}

//# simple example
TEST_F(Test_IO_CommandLineParser, SimpleOptions)
{
	ln::CommandLineParser parser;
	auto* forceOption = parser.addFlagOption(u"f", u"force", u"force operation.");
	auto* sizeOption = parser.addValueOption(u"", u"size", u"buffer size.");
	auto* depthOption = parser.addValueOption(u"d", u"depth", u"depth level.", u"0");
	auto* langOption = parser.addNamedValueOption(u"", u"lang", u"language.", {u"cpp", u"cs", u"py"}, u"cpp");

	char* argv[] =
	{
		"<program>",
		"-f",
		"--size=100",
		"-d",
		"--lang=cs",
	};
	int argc = sizeof(argv) / sizeof(char*);
	
	ASSERT_EQ(true, parser.process(argc, argv));
	ASSERT_EQ(true, parser.has(forceOption));
	ASSERT_EQ(true, parser.has(sizeOption));
	ASSERT_EQ(true, parser.has(depthOption));
	ASSERT_EQ(true, parser.has(langOption));
}

//# 
TEST_F(Test_IO_CommandLineParser, Commands)
{
	//* [ ] コマンドが has で定義確認できることのテスト
	{
		ln::CommandLineParser parser;
		auto initCommand = parser.addCommand(_T("init"), _T("init description."));
		auto forceOption1 = initCommand->addFlagOption(_T("f"), _T("force"), _T("force description."));

		auto addCommand = parser.addCommand(_T("add"), _T("add description."));
		auto forceOption2 = addCommand->addFlagOption(_T("f"), _T("force"), _T("force description."));

		char* argv[] =
		{
			"<program>",
			"init",
			"-f",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(true, parser.has(initCommand));
		ASSERT_EQ(true, parser.has(forceOption1));
		ASSERT_EQ(false, parser.has(addCommand));
		ASSERT_EQ(false, parser.has(forceOption2));
	}
}

//# check options
TEST_F(Test_IO_CommandLineParser, Options)
{
	//* [ ] without default values (short names)
	{
		ln::CommandLineParser parser;
		auto* flagOption = parser.addFlagOption(u"f", u"flag", u"description.");
		auto* valueOption = parser.addValueOption(u"v", u"value", u"description.");
		auto* namedValueOption = parser.addNamedValueOption(u"n", u"named-value", u"description.", { u"abc", u"def", u"ghi" });

		char* argv[] =
		{
			"<program>",
			"-f",
			"-v", "100",
			"-n", "ghi",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(true, parser.has(flagOption));
		ASSERT_EQ(true, parser.has(valueOption));
		ASSERT_EQ(true, parser.has(namedValueOption));
		ASSERT_EQ(u"100", valueOption->value());
		ASSERT_EQ(u"ghi", namedValueOption->value());
	}
	//* [ ] without default values (long names)
	{
		ln::CommandLineParser parser;
		auto* flagOption = parser.addFlagOption(u"f", u"flag", u"description.");
		auto* valueOption = parser.addValueOption(u"v", u"value", u"description.");
		auto* namedValueOption = parser.addNamedValueOption(u"n", u"named-value", u"description.", { u"abc", u"def", u"ghi" });

		char* argv[] =
		{
			"<program>",
			"--flag",
			"--value=100",
			"--named-value=abc",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(true, parser.has(flagOption));
		ASSERT_EQ(true, parser.has(valueOption));
		ASSERT_EQ(true, parser.has(namedValueOption));
		ASSERT_EQ(_T("100"), valueOption->value());
		ASSERT_EQ(_T("abc"), namedValueOption->value());
	}
	//* [ ] with default values
	{
		ln::CommandLineParser parser;
		auto* flagOption = parser.addFlagOption("f", "flag", "description.");
		auto* valueOption = parser.addValueOption("v", "value", "description.", "5");
		auto* namedValueOption = parser.addNamedValueOption("n", "named-value", "description.", { "abc", "def", "ghi" }, "def");

		char* argv[] =
		{
			"<program>",
			"-v",
			"-n",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(false, parser.has(flagOption));
		ASSERT_EQ(true, parser.has(valueOption));
		ASSERT_EQ(true, parser.has(namedValueOption));
		ASSERT_EQ(_T("5"), valueOption->value());
		ASSERT_EQ(_T("def"), namedValueOption->value());
	}
}

TEST_F(Test_IO_CommandLineParser, PositionalArgument)
{
	//* [ ] get value from PositionalArgument
	{
		ln::CommandLineParser parser;
		auto* srcArg = parser.addPositionalArgument("source", "description.");
		auto* dstArg = parser.addPositionalArgument("destination", "description.");

		char* argv[] =
		{
			"<program>",
			"file1",
			"file2",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(_T("file1"), srcArg->value());
		ASSERT_EQ(_T("file2"), dstArg->value());
	}

	//* [ ] PositionalArgument list
	{
		ln::CommandLineParser parser;
		auto* inputsArg = parser.addListPositionalArgument("inputs", "description.");

		char* argv[] =
		{
			"<program>",
			"file1",
			"file2",
			"file3",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(3, inputsArg->values().size());
		ASSERT_EQ(_T("file1"), inputsArg->values()[0]);
		ASSERT_EQ(_T("file2"), inputsArg->values()[1]);
		ASSERT_EQ(_T("file3"), inputsArg->values()[2]);
	}

	//* [ ] mixied PositionalArgument list
	{
		ln::CommandLineParser parser;
		auto* analyzeCommand = parser.addCommand("analyze", "description.");
		auto* settingsArg = parser.addPositionalArgument("settings", "description.");
		auto* inputsArg = parser.addListPositionalArgument("inputs", "description.");

		char* argv[] =
		{
			"<program>",
			"settingfile",
			"file1",
			"file2",
		};
		int argc = sizeof(argv) / sizeof(char*);

		ASSERT_EQ(true, parser.process(argc, argv));
		ASSERT_EQ(_T("settingfile"), settingsArg->value());
		ASSERT_EQ(2, inputsArg->values().size());
		ASSERT_EQ(_T("file1"), inputsArg->values()[0]);
		ASSERT_EQ(_T("file2"), inputsArg->values()[1]);
	}
}

//# 
TEST_F(Test_IO_CommandLineParser, Help)
{

	/*
		app [options] args <command> [options] args
		~~~~~~~~~~~~~~~~~~
		                  ~~~~~~~~~~~~~~~~~~~~~~~~~
	*/

	ln::CommandLineParser parser;
	auto* forceOption = parser.addFlagOption("f", "force", "force operation.");
	auto* sizeOption = parser.addValueOption("", "size", "buffer size.");
	auto* depthOption = parser.addValueOption("d", "depth", "depth level.", "0");
	auto* langOption = parser.addNamedValueOption("", "lang", "language.", { "cpp", "cs", "py" }, "cpp");
	auto* settingArg = parser.addPositionalArgument("setting-file", "setting file path.");

	auto* initCommand = parser.addCommand(_T("init"), _T("create new database."));
	auto* sharedOption = initCommand->addFlagOption("s", "shared", "add shared attribute.");
	auto* filesOption = initCommand->addListPositionalArgument("files", "input file list.");

	auto* analyzeCommand = parser.addCommand(_T("analyze"), _T("analyze database."));
	auto* modeOption = analyzeCommand->addNamedValueOption("", "mode", "analyze mode.", { "normal", "fast" });
	auto* reportOption = analyzeCommand->addListPositionalArgument("report", "report list.", CommandLinePositionalArgumentFlags::Optional);

	char* argv[] =
	{
		"<program>",
		//"-f",
		//"--size=100",
		//"-d",
		//"--lang=cs",
		"help",
		//"init",
	};
	int argc = sizeof(argv) / sizeof(char*);

	parser.process(argc, argv);
}

TEST_F(Test_IO_CommandLineParser, DocExample)
{
	char* argv[] =
	{
		"<program>",
		"--force",
		"file1.txt",
		"file2.txt",
	};
	int argc = sizeof(argv) / sizeof(char*);


	ln::CommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption(_T("1.0.0"));
	parser.setApplicationDescription(_T("This is open-source software under MIT License."));

	auto* forceOpt = parser.addFlagOption("f", "force", "Overwrite file.");
	auto* sourceArg = parser.addPositionalArgument("SOURCE", "source filename.");
	auto* destArg = parser.addPositionalArgument("DEST", "destination filename.");

	ASSERT_EQ(true, parser.process(argc, argv));
	ASSERT_EQ(true, parser.has(forceOpt));
	ASSERT_EQ(_T("file1.txt"), sourceArg->value());
	ASSERT_EQ(_T("file2.txt"), destArg->value());
}

#if 0
//# writing test
TEST_F(Test_IO_CommandLineParser, Basic)
{
	ln::CommandLineParser parser;

	auto* initCommand = parser.addCommand(
		_T("init"),
		_T("create new database."));

	auto* analyzeCommand = parser.addCommand(
		_T("analyze"),
		_T("analyze database."));

	// 値を持たないフラグオプション
	auto* forceOption = analyzeCommand->addFlagOption(
		_T("f"), _T("force"),
		_T("force operation."));

	// 値を持つオプション (引数省略不可)
	auto* dirOption = analyzeCommand->addValueOption(
		_T("d"), _T("dir"),
		_T("directory path."));

	char* args[] =
	{
		"<program>",
		"analyze",
		"-f",
		"--dir=dir1",
	};
	int argc = sizeof(args) / sizeof(char*);

	parser.printHelp();

	ASSERT_EQ(true, parser.process(argc, args));
	ASSERT_EQ(false, parser.has(initCommand));
	ASSERT_EQ(true, parser.has(analyzeCommand));
	ASSERT_EQ(true, forceOption->isSet());
	ASSERT_EQ(_T("dir1"), dirOption->value());


	//sl::AnalyzeContext c;
	//c.setupEnv(_T("."));
	//c.addInputFile(fileOption->value());
	//c.analyzeAllFiles();
}
#endif

