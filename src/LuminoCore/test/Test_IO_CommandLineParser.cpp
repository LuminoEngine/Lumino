#include "Common.hpp"
#include <Lumino/IO/CommandLineParser.hpp>

//==============================================================================
//# CommandLineParser
class Test_IO_CommandLineParser : public ::testing::Test { };

//#  
TEST_F(Test_IO_CommandLineParser, SimpleOptions)
{
	ln::CommandLineParser parser;
	auto* forceOption = parser.addFlagOption("f", "force", "force operation.");
	auto* sizeOption = parser.addValueOption("", "size", "buffer size.");
	auto* depthOption = parser.addValueOption("d", "depth", "depth level.", "0");
	auto* langOption = parser.addNamedValueOption("", "lang", "language.", {"cpp", "cs", "py"}, "cpp");

	char* args[] =
	{
		"<program>",
		"-f",
		"--size=100",
		"-d",
		"--lang=cs",
	};
	int argc = sizeof(args) / sizeof(char*);

	parser.printHelp();
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
	auto settingArg = parser.addPositionalArgument("setting-file", "setting file path.");


	auto* initCommand = parser.addCommand(_T("init"), _T("create new database."));
	auto* sharedOption = initCommand->addFlagOption("s", "shared", "add shared attribute.");

	auto* analyzeCommand = parser.addCommand(_T("analyze"), _T("analyze database."));
	auto* modeOption = analyzeCommand->addNamedValueOption("", "mode", "analyze mode.", { "normal", "fast" });


	char* args[] =
	{
		"<program>",
		"-f",
		"--size=100",
		"-d",
		"--lang=cs",
	};
	int argc = sizeof(args) / sizeof(char*);

	parser.printHelp();

	std::cout << initCommand->buildHelpText();
}

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
	ASSERT_EQ(false, parser.hasCommand(initCommand));
	ASSERT_EQ(true, parser.hasCommand(analyzeCommand));
	ASSERT_EQ(true, forceOption->isSet());
	ASSERT_EQ(_T("dir1"), dirOption->value());


	//sl::AnalyzeContext c;
	//c.setupEnv(_T("."));
	//c.addInputFile(fileOption->value());
	//c.analyzeAllFiles();
}

