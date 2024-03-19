
#include <stdio.h>
#include <LuminoPlatform/PlatformSupport.hpp>
#include "Common.hpp"
#include "TestEnv.hpp"

class TestEnvironment : public ::testing::Environment {
public:
    TestEnvironment() {
    }

    virtual ~TestEnvironment() {
    }

    void SetUp() override {

        Logger::setLevel(LogLevel::Trace);
        Logger::addStdErrAdapter();

        auto tempPath = Path(ln::Environment::specialFolderPath(SpecialFolder::Temporary), _TT("Lumino"));

        TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
        TestHelper::setTempDirPath(tempPath);
        TestEnv::initialize();

        if (TestEnv::isCI) {
            TestEnv::setup(GraphicsAPI::Default);
        }
        else {
            TestEnv::setup(GraphicsAPI::Vulkan);//GraphicsAPI::DirectX12);//
        }
    }

    void TearDown() override {
        TestEnv::teardown();
    }

//private:
//    GraphicsAPI m_graphicsAPI;
};

int main(int argc, char** argv) {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    setlocale(LC_ALL, "");

    if (argc == 1) {
        char* testArgs[] = {
            argv[0],
            "--gtest_break_on_failure",
            //"--gtest_filter=Test_Animation_Mixer.*"
        };
        argc = sizeof(testArgs) / sizeof(char*);
        argv = testArgs;
    }

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnvironment());
	
    return RUN_ALL_TESTS();
}
