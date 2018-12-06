#include "Common.hpp"
#include "TestEnv.hpp"

String TestEnv::LuminoCLI;
Path TestEnv::LuminoTestProjectDir;

void TestEnv::setup()
{
#ifdef LN_OS_WIN32
#ifdef LN_DEBUG
	LuminoCLI = Path::combine(Path(ln::Environment::executablePath()).parent().parent().parent(), u"Debug", u"lumino-cli.exe");
#else
	LuminoCLI = Path::combine(Path(ln::Environment::executablePath()).parent().parent().parent(), u"Release", u"lumino-cli.exe");
#endif
#else
#endif

	LuminoTestProjectDir = Path(u"LuminoTestProj").canonicalize();

	if (FileSystem::existsDirectory(LuminoTestProjectDir)) {
		FileSystem::removeDirectory(LuminoTestProjectDir, true);
	}

	Process::execute(TestEnv::LuminoCLI, { u"init", u"LuminoTestProj" });
}

void TestEnv::teardown()
{
    detail::EngineDomain::release();
}
